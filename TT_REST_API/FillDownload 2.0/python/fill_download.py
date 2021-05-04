# coding=utf-8
"""
    Copyright © 2018-2019 Trading Technologies International, Inc. All Rights Reserved Worldwide

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    * Redistributions of source or binary code must be free of charge.

    * Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

import os
import re
import sys
import time
import signal
import logging
import platform
import argparse
from uuid import uuid4
from getpass import getpass
from datetime import datetime, timedelta
from threading import Thread, Lock, Event

import requests
from simplejson import JSONDecodeError


###############################
# MODULE LEVEL ATTRIBUTES
###############################

TT_URL_BASE = 'https://apigateway.trade.tt'

VALID_TT_ENVIRONMENTS = (
    'ext_uat_cert',
    'ext_prod_live'
)

# Updated in main below
REQUEST_ID_BASE = None


class GLOBALS(object):
    enums = []
    api_http_header = {}
    tt_environment = ''
    max_narrowing_retries = 32


common = GLOBALS()
stop_running = Event()
log = logging.getLogger()
log.setLevel(logging.INFO)

###############################
# COMMON UTILITIES
###############################


def api_request(url, headers, data=None, http_method='get', request_timeout=False):
    log.debug('{} {}'.format(http_method.upper(), url))
    req_id = '{}--{}'.format(REQUEST_ID_BASE, uuid4())
    params = {'requestId': req_id}
    response = \
        getattr(requests, http_method)(url=url, headers=headers, data=data, params=params)
    if request_timeout and response.status_code == 408:
        return response
    if response.status_code != 200:
        raise AssertionError(
            "Error on API request --> http code: {} message: {}".
            format(response.status_code, response.text))

    try:
        response = response.json()

        if 'status' in response and response['status'] != 'Ok':
            error_message = \
                response['message'] if 'message' in response else response['status_message']
            raise AssertionError('Unable to retrieve data: {}'.format(error_message))
    except JSONDecodeError:
        stop_running.set()
        raise AssertionError('Error decoding the response for {}'.format(url))

    return response


def pause(total_pause, quit_event, thread_name):
    log.info('Pausing {} thread for {} seconds'.format(thread_name, total_pause))
    interval_counter = 0
    while interval_counter < total_pause and not quit_event.is_set():
        # sleeping in one second intervals to handle an interrupt in
        # a timely manner
        time.sleep(1)
        interval_counter += 1


def get_time_until(timestamp, day_shift=0):
    dt = datetime.strptime(timestamp, "%H:%M")
    dt_now = datetime.now()
    dt = dt.replace(year=dt_now.year, month=dt_now.month, day=dt_now.day)
    dt_shift = timedelta(days=day_shift)
    dt += dt_shift
    unix_time = time.mktime(dt.timetuple())
    return unix_time - time.time()


def initialize_logger(log_dir=None, stdout=False):
    log_format = "%(asctime)s | %(levelname)s | %(process)d | %(message)s"
    formatter = logging.Formatter(fmt=log_format)

    if log_dir:
        today = datetime.now()
        file_name = 'tt_fill_downloader_{}.log'.format(today.strftime("%Y_%m_%d"))
        full_file_path = os.path.join(log_dir, file_name)
        handler = logging.FileHandler(full_file_path)
        handler.setFormatter(formatter)
        handler.setLevel(logging.INFO)
        log.addHandler(handler)

    if stdout:
        handler = logging.StreamHandler(sys.stdout)
        handler.setFormatter(formatter)
        handler.setLevel(logging.INFO)
        log.addHandler(handler)


def memoize(func):
    cache = dict()

    def memoized_func(*args):
        if args in cache:
            return cache[args]
        result = func(*args)
        cache[args] = result
        return result
    return memoized_func


@memoize
def get_date(datetimestamp):
    return time.strftime('%Y-%m-%d', time.gmtime(int(datetimestamp) / (1000 * 1000 * 1000))) if datetimestamp else None


@memoize
def get_time(datetimestamp):
    if datetimestamp:
        s, ms = divmod(int(datetimestamp) / (1000 * 1000), 1000)
        return '%s.%03d' % (time.strftime('%H:%M:%S', time.gmtime(s)), ms)


###############################
# FILL DOWNLOAD LOGIC
###############################

@memoize
def get_instrument_info(instrument_id):
    instrument_download_url = \
        '{}/ttpds/{}/instrument/{}'.format(TT_URL_BASE, common.tt_environment, instrument_id)
    try:
        instrument_info = api_request(instrument_download_url, common.api_http_header)
        return instrument_info['instrument'][0]
    except AssertionError:
        return {'alias': '',
                'productSymbol': '',
                'strike': '',
                'instrumentId': instrument_id}


@memoize
def get_account_info(account_id):
    account_download_url = \
        '{}/ttaccount/{}/account/{}'.format(TT_URL_BASE, common.tt_environment, account_id)
    account_info = api_request(account_download_url, common.api_http_header)
    return account_info['account'][0]


@memoize
def get_user_info(user_id):
    user_download_url = '{}/ttuser/{}/user/{}'.format(TT_URL_BASE, common.tt_environment, user_id)
    try:
        user_info = api_request(user_download_url, common.api_http_header)
        return user_info['user'][0]
    except AssertionError:
        return {'id': user_id,
                'alias': "user_id:" + str(user_id),
                'company': {'name': "user_id:" + str(user_id), 'id': -1, 'abbrevName': "user_id:" + str(user_id)}
                }


def retrieve_token(environment, key, secret, common, lock, stop_running):
    ttid_headers = {
        'Content-Type': 'application/x-www-form-urlencoded',
        'content-type': 'application/json',
        'accept': 'application/json',
        'x-api-key': key}

    ttid_data = {
        'grant_type': 'user_app',
        'app_key': secret}

    token_url = '{}/ttid/{}/token'.format(TT_URL_BASE, environment)

    while not stop_running.is_set():
        try:
            login_info = api_request(url=token_url, headers=ttid_headers, data=ttid_data, http_method='post')
        except AssertionError:
            stop_running.set()
            raise

        token = '{} {}'.format(login_info['token_type'].capitalize(),
                               login_info['access_token'])

        token_expiry = time.time() + login_info['seconds_until_expiry']

        with lock:
            common.api_http_header = {'x-api-key': key, 'Authorization': token}

        while time.time() < token_expiry-10 and not stop_running.is_set():
            time.sleep(1)


def build_enums(environment, headers):
    # Order Data Enums
    order_data_download_url = '{}/ttledger/{}/orderdata'.format(TT_URL_BASE, environment)
    fill_data = api_request(order_data_download_url, headers)
    enums = fill_data['orderData']

    # Market Name Enums
    markets_download_url = '{}/ttpds/{}/markets'.format(TT_URL_BASE, environment)
    response = api_request(markets_download_url, headers)
    enums['markets'] = {info['id']: info['name'] for info in response['markets']}

    # Market Code Enums
    mics_download_url = '{}/ttpds/{}/mics'.format(TT_URL_BASE, environment)
    response = api_request(mics_download_url, headers)
    enums['mics'] = {}
    for mic in response['markets']:
        if mic['desc']:
            enums['mics'][mic['id']] = mic['desc']
        elif mic['marketId'] in enums['markets']:
            enums['mics'][int(mic['id'])] = \
                '{}_All'.format(enums['markets'][mic['marketId']])

    # Product Data Enums
    prod_data_download_url = '{}/ttpds/{}/productdata'.format(TT_URL_BASE, environment)
    prod_data = api_request(prod_data_download_url, headers)
    for enum in prod_data:
        if enum in ('status', 'message', 'lastPage', 'nextPageKey'):
            continue
        enum_dict = {}
        for info in prod_data[enum]:
            if 'desc' in info:
                enum_dict[int(info['id'])] = info['desc']
            else:
                enum_dict[int(info['id'])] = info['name']

        enums[enum] = enum_dict

    # Instrument Date Enums
    inst_data_download_url = '{}/ttpds/{}/instrumentdata'.format(TT_URL_BASE, environment)
    inst_data = api_request(inst_data_download_url, headers)
    for enum in inst_data:
        if enum in ('status', 'message', 'lastPage', 'nextPageKey'):
            continue
        enum_dict = {}
        for info in inst_data[enum]:
            if 'desc' in info:
                enum_dict[int(info['id'])] = info['desc']
            else:
                enum_dict[int(info['id'])] = info['name']

        enums[enum] = enum_dict

    return enums


def retrieve_fills(environment, headers, min_time_stamp=None, max_time_stamp=None):
    fill_download_url = '{}/ttledger/{}/fills'.format(TT_URL_BASE, environment)
    if min_time_stamp:
        fill_download_url += '?minTimestamp={}'.format(min_time_stamp)
    if max_time_stamp:
        fill_download_url += '?maxTimestamp={}'.format(max_time_stamp)
    fills = api_request(fill_download_url, headers, request_timeout=True)
    if type(fills) is not dict and fills.status_code == 408:
        max_time = time.time()
        fill_download_url = '{}/ttledger/{}/fills'.format(TT_URL_BASE, environment)
        for _ in range(common.max_narrowing_retries):
            max_time = min_time_stamp + ((max_time - min_time_stamp) / 2)
            log.warning(
                "Fill request timed out. Retrying with mintime {} and maxtime {}....".format(min_time_stamp, max_time))
            fills = api_request(
                fill_download_url + '?minTimestamp={}'.format(min_time_stamp) + '?maxTimestamp={}'.format(
                    max_time), headers, request_timeout=True)
            if fills.status_code == 408:
                continue
            break
        else:
            raise AssertionError("Request for fills unsuccessful. Max Retries exceeded.")
    return fills['fills']


def output_fill_data_to_file(fills, output_file):
    csv_header = r''\
        'Date (UTC),'\
        'Time (UTC),'\
        'Exchange,'\
        'Contract,'\
        'B/S,'\
        'FillQty,'\
        'Price,'\
        'P/F,'\
        'Type,'\
        'Modifier,'\
        'Route,'\
        'O/C,'\
        'Broker,'\
        'Account,'\
        'AcctType,'\
        'GiveUp,'\
        'TextA,'\
        'TextB,'\
        'TextC,'\
        'TextTT,'\
        'Originator,'\
        'CurrentUser,'\
        'ClOrderID,'\
        'TTOrderID,'\
        'ParentID,'\
        'OMAOrderID,'\
        'ExchOrderID,'\
        'ExchTransID,'\
        'ExchAcct,'\
        'ExchDate (UTC),'\
        'ExchTime (UTC),'\
        'ManualFill,'\
        'Product,'\
        'Prod Type,'\
        'Fill Type,'\
        'ExeQty,'\
        'WorkQty,'\
        'P/A,'\
        'ConnectionID,'\
        'P/C,'\
        'Strike,'\
        'D.E.A.,'\
        'TrdgCap,'\
        'LiqProv,'\
        'C.D.I.,'\
        'InvestDec,'\
        'ExecDec,'\
        'Client'

    write_header = True if not os.path.exists(output_file) else False

    with open(output_file, 'a+') as output:
        if write_header:
            # this is the first time the file is being written too,
            # therefore write the header
            output.write('{}\n'.format(csv_header))
        for fill in fills:
            output.write('{}\n'.format(fill))


class FillData(object):
    def __init__(self, json_data):
        self.json_data = json_data

    @property
    def utc_date(self):
        return get_date(self.json_data['timeStamp'])

    @property
    def utc_time(self):
        return get_time(self.json_data['timeStamp'])

    @property
    def exchange_name(self):
        return common.enums['markets'].get(str(self.json_data['marketId']), '')

    @property
    def contract_name(self):
        return get_instrument_info(self.json_data['instrumentId'])['alias']

    @property
    def trade_side(self):
        return common.enums['side'].get(str(self.json_data['side']), '')

    @property
    def fill_qty(self):
        return str(self.json_data.get('lastQty', ''))

    @property
    def price(self):
        return str(self.json_data.get('lastPx', ''))

    @property
    def full_partial(self):
        status = self.json_data.get('ordStatus', '')
        return 'P' if status == '2' else 'F' if status == '3' else ''

    @property
    def algo_type(self):
        return common.enums['algoType'].get(str(self.json_data['algoType']), '')

    @property
    def ord_type(self):
        return common.enums['orderType'].get(str(self.json_data.get('ordType', -1)), '').capitalize()

    @property
    def synthetic_type(self):
        return common.enums['syntheticType'].get(str(self.json_data.get('syntheticType', -1)), '')

    @property
    def trade_type(self):
        if 'tradeType' in self.json_data:
            return common.enums['tradeType'].get(str(self.json_data.get('tradeType', -1)), '')
        elif 'trdType' in self.json_data:
            return common.enums['tradeType'].get(str(self.json_data.get('trdType', -1)), '')
        else:
            return ''

    @property
    def exec_inst(self):
        return common.enums['execInst'].get(str(self.json_data['execInst']), '')

    @property
    def contingency_type(self):
        return common.enums['contingencyType'].get(str(self.json_data.get('contingencyType', -1)), '')

    @property
    def modifier(self):
        if self.is_native_iceberg():
            return 'Iceberg'
        elif self.ord_type == 'LIMIT' and self.exec_inst == 'EXEC_INST_PARTICIPATE_DO_NOT_INITIATE':
            return 'BOC'
        elif self.ord_type == 'LIMIT' and self.contingency_type == 'CONTINGENCY_TYPE_ONE_CANCELS_THE_OTHER':
            return 'OCO'
        elif self.json_data.get('crossId', ''):
            return 'Cross'
        elif self.json_data.get('minQty', -1) > 0:
            return 'MinVol'
        elif self.trade_type or self.json_data.get('tradeReportId', ''):
            return 'Block'
        else:
            return ''

    @property
    def order_cross_prevention_type(self):
        return common.enums['orderCrossPreventionType'].get(str(self.json_data.get('orderCrossPreventionType', -1)), '')

    @property
    def route(self):
        if self.order_cross_prevention_type in \
                ('POSITION_TRANSFER_FILL', 'ORDER_REDUCED_PARTIAL_POSITION_TRANSFER_FILL'):
            return 'Internal'
        else:
            return 'Direct'

    @property
    def position_effect(self):
        return common.enums['positionEffect'].get(str(self.json_data.get('positionEffect', -1)), '')

    @property
    def broker(self):
        if 'userId' in self.json_data and self.json_data['userId'] != 0:
            return get_user_info(self.json_data['userId'])['user'][0]['company']['name']
        else:
            return ''

    @property
    def account(self):
        if 'accountId' in self.json_data:
            return get_account_info(self.json_data['accountId'])['name']
        else:
            return ''

    @property
    def account_type(self):
        for party in self.json_data.get('parties', []):
            if party.get('role', -1) == 85:  # PARTY_ROLE_ACCOUNT_CODE
                return party['id']
        else:
            return ''

    @property
    def give_up(self):
        for party in self.json_data.get('parties', []):
            if party.get('role', -1) == 6:  # PARTY_ROLE_GIVEUP_CLEARING_FIRM
                return party['id']
        else:
            return ''

    @property
    def text_a(self):
        return self.json_data.get('textA', '')

    @property
    def text_b(self):
        return self.json_data.get('textB', '')

    @property
    def text_c(self):
        return self.json_data.get('textC', '')

    @property
    def text_tt(self):
        return self.json_data.get('textTT', '')

    @property
    def originator(self):
        if 'userId' in self.json_data and self.json_data['userId'] != 0:
            return get_user_info(self.json_data['userId'])['alias']
        else:
            return ''

    @property
    def current_user(self):
        if 'currUserId' in self.json_data and self.json_data['currUserId'] != 0:
            return get_user_info(self.json_data['currUserId'])['alias']
        else:
            return ''

    @property
    def client_order_id(self):
        return self.json_data.get('secondaryClOrderId', '')

    @property
    def tt_order_id(self):
        return self.json_data.get('orderId', '')

    @property
    def parent_order_id(self):
        return self.json_data.get('parentOrderId', '')

    @property
    def oma_order_id(self):
        return self.json_data.get('omaParentOrderId', '')

    @property
    def exchange_order_id(self):
        return self.json_data.get('secondaryOrderId', '')

    @property
    def exchange_transaction_id(self):
        return self.json_data.get('execID', '')

    @property
    def exchange_account(self):
        return self.json_data.get('account', '')

    @property
    def exchange_data(self):
        return get_date(self.json_data['transactTime'])

    @property
    def exchange_time(self):
        return get_time(self.json_data['transactTime'])

    @property
    def manual_fill(self):
        return str(self.json_data.get('manualFill', ''))

    @property
    def symbol(self):
        return get_instrument_info(self.json_data['instrumentId'])['productSymbol']

    @property
    def product_type(self):
        return common.enums['productTypes'].get(get_instrument_info(self.json_data['instrumentId']).get
                                                ('productTypeId', -1), '')

    @property
    def fill_type(self):
        return common.enums['fillType'].get(str(self.json_data.get('multiLegReportingType', -1)), '')

    @property
    def exec_qty(self):
        return str(self.json_data.get('lastQty', ''))

    @property
    def work_qty(self):
        return str(self.json_data.get('exchLeavesQty', ''))

    @property
    def aggressor_flag(self):
        if 'aggressorIndicator' in self.json_data:
            return 'A' if self.json_data['aggressorIndicator'] == 'true' else 'P'
        else:
            return ''

    @property
    def connection_id(self):
        if 'connectionId' in self.json_data:
            return self.json_data['connectionId']
        else:
            return self.json_data.get('exchOrderAssoc', '')

    @property
    def put_call(self):
        return common.enums['optionCodes'].get(get_instrument_info(self.json_data['instrumentId']).get
                                               ('optionCodeId', -1), '')

    @property
    def strike(self):
        return str(get_instrument_info(self.json_data['instrumentId']).get('strike', ''))

    @property
    def order_origination(self):
        if 'orderOrigination' not in self.json_data:
            return ''
        elif common.enums['orderOrigination'].get(self.json_data['orderOrigination'], '') == \
                'ORDER_ORIGINATION_FROM_DIRECT_ACCESS_OR_SPONSORED_ACCESS_CUSTOMER':
            return 'Y'
        else:
            return 'N'

    @property
    def order_capacity(self):
        return common.enums['orderCapacity'].get(str(self.json_data.get('orderCapacity', -1)), '')

    @property
    def trading_capacity(self):
        if 'orderCapacity' not in self.json_data:
            return ''
        elif self.order_capacity == 'ORDER_CAPACITY_AGENCY':
            return 'Other'
        elif self.order_capacity == 'ORDER_CAPACITY_PRINCIPAL':
            return 'Deal'
        elif self.order_capacity == 'ORDER_CAPACITY_RISKLESS_PRINCIPAL':
            return 'Match'
        else:
            return ''

    @property
    def liquidity_provision(self):
        for attr in self.json_data.get('orderAttributes', []):
            if attr.get('order_attribute_type', -1) == 2:  # ORDER_ATTRIBUTE_TYPE_LIQUIDITY_PROVISION_ACTIVITY_ORDER
                return attr['order_attribute_type']
        else:
            return ''

    @property
    def commodity_derivative_indicator(self):
        for attr in self.json_data.get('orderAttributes', []):
            if attr.get('order_attribute_type', -1) == 3:  # ORDER_ATTRIBUTE_TYPE_RISK_REDUCTION_ORDER
                return attr['order_attribute_type']
        else:
            return ''

    @property
    def invest_dec(self):
        for party in self.json_data.get('parties', []):
            if party.get('role', -1) == 98:  # PARTY_ROLE_INVESTMENT_DECISION_MAKER
                return party['id']
        else:
            return ''

    @property
    def exec_dec(self):
        for party in self.json_data.get('parties', []):
            if party.get('role', -1) == 4:  # PARTY_ROLE_EXECUTING_TRADER
                return party['id']
        else:
            return ''

    @property
    def client_id(self):
        for party in self.json_data.get('parties', []):
            if party.get('role', -1) == 22:  # PARTY_ROLE_CLIENT_ID
                return party['id']
            elif party.get('idSource', -1) == 19:  # PARTY_ID_SOURCE_SHORT_CODE_IDENTIFIER
                return party['id']
        else:
            return ''

    def is_synthetic_market(self):
        return self.exchange_name in ('ASE', 'AGGREGATOR')

    def is_algo_order(self):
        return self.algo_type in (
            'ALGO_TYPE_ADL',
            'ALGO_TYPE_SSE',
            'ALGO_TYPE_CUSTOM_LIBRARY',
            'ALGO_TYPE_VENDOR')

    def is_synthetic_order(self):
        return self.is_synthetic_market() or self.is_algo_order()

    def is_iceberg_order(self):
        return self.json_data.get('displayQty', -1) > 0

    def is_native_iceberg(self):
        return self.is_iceberg_order() and not self.is_synthetic_order()

    def __str__(self):
        return ','.join((
            self.utc_date,
            self.utc_time,
            self.exchange_name,
            self.contract_name,
            self.trade_side,
            self.fill_qty,
            self.price,
            self.full_partial,
            self.ord_type,
            self.modifier,
            self.route,
            self.position_effect,
            self.broker,
            self.account,
            self.account_type,
            self.give_up,
            self.text_a,
            self.text_b,
            self.text_c,
            self.text_tt,
            self.originator,
            self.current_user,
            self.client_order_id,
            self.tt_order_id,
            self.parent_order_id,
            self.oma_order_id,
            self.exchange_order_id,
            self.exchange_transaction_id,
            self.exchange_account,
            self.exchange_data,
            self.exchange_time,
            self.manual_fill,
            self.symbol,
            self.product_type,
            self.fill_type,
            self.exec_qty,
            self.work_qty,
            self.aggressor_flag,
            self.connection_id,
            self.put_call,
            self.strike,
            self.order_origination,
            self.trading_capacity,
            self.liquidity_provision,
            self.commodity_derivative_indicator,
            self.invest_dec,
            self.exec_dec,
            self.client_id))


def fill_downloader(app_key, app_secret, stop_running, end_time, interval, output):
    # Create and start token handling thread
    common_lock = Lock()
    token_handler = Thread(name='token_handler', target=retrieve_token,
                           args=(common.tt_environment, app_key, app_secret, common, common_lock, stop_running))
    token_handler.start()

    # wait until the first token requests completes before moving on
    while not stop_running.is_set() and not common.api_http_header:
        time.sleep(1)

    if not stop_running.is_set() and not common.enums:
        with common_lock:
            log.info('building enums')
            common.enums = build_enums(common.tt_environment, common.api_http_header)

    # use midnight of the current day as the min time stamp for the first request
    dt_now = datetime.now()
    midnight_today = dt_now.replace(hour=0, minute=0, second=0, microsecond=0)
    min_time_stamp = int(time.mktime(midnight_today.timetuple())) * 10**9

    while not stop_running.is_set() and time.time() < end_time:
        log.info('retrieving fills')
        fills = retrieve_fills(common.tt_environment, common.api_http_header, min_time_stamp=min_time_stamp)

        while not stop_running.is_set() and fills and len(fills) % 500 == 0:
            # received the maximum number of fills allowed per request,
            # therefore grab the remaining for the time span being requested
            min_time_stamp = int(fills[-1]['timeStamp']) + 1
            new_fills = retrieve_fills(common.tt_environment, common.api_http_header,
                                       min_time_stamp=min_time_stamp)
            if new_fills:
                fills += new_fills
            else:
                break

        if fills:
            # sort fills by when they occurred on the TT system, oldest first
            fills = sorted([FillData(fill) for fill in fills],
                           lambda f1, f2: f1.json_data['timeStamp'] > f2.json_data['timeStamp'])

            min_time_stamp = int(fills[-1].json_data['timeStamp']) + 1
            output_fill_data_to_file(fills, output)

        else:
            min_time_stamp = min_time_stamp + (interval * 60)

        pause(interval*60, stop_running, 'fill downloader')

    token_handler.join()

    with common_lock:
        # reset the header so that when this runs on the next session the while loop above executes
        common.api_http_header = {}

    log.info('Exiting execution handler thread')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='',
        formatter_class=argparse.RawTextHelpFormatter,
        epilog="Example:\n\tpython fill_download.py -e ext_uat_cert -c TradingTech -st 08:00 -et 14:30 -r 1 -o -l /tmp"
    )

    parser.add_argument('-e', '--env', dest='tt_env', action='store', required=False, default='ext_prod_live',
                        help='The TT order environment to interact with')
    parser.add_argument('-c', '--company', dest='company', action='store',
                        required=True, help='Company name of the user')
    parser.add_argument('-st', '--start-time', dest='start_time', action='store', required=False, default='00:00',
                        help='HH:MM formatted input based on a 24 hour clock, determining the '
                             'daily start time of the fill capturing')
    parser.add_argument('-et', '--end-time', dest='end_time', action='store', required=False, default='23:50',
                        help='HH:MM formatted input based on a 24 hour clock, determining the '
                             'daily start time of the fill capturing')
    parser.add_argument('-r-', '--rate', dest='rate', action='store', type=int, required=False, default=60,
                        help='Time minutes on how often to perform a fill download')
    parser.add_argument('-p', '--output', dest='output', required=False,
                        help='Full file path of where to write the fill data too')
    parser.add_argument('-l', '--log-dir', dest='log_dir', action='store',
                        help='If given, will log to a file in the given directory')
    parser.add_argument('-o', '--stdout', dest='stdout', action='store_true', help='If supplied, will log to console')

    args = parser.parse_args()

    REQUEST_ID_BASE = 'PyFillDownloader-{}'.format(args.company)

    if args.log_dir or args.stdout:
        # setup logging
        initialize_logger(args.log_dir, args.stdout)

    # validate proper TT environment was given
    if args.tt_env not in VALID_TT_ENVIRONMENTS:
        raise ValueError('Given TT environment value must be one of {}'.format(VALID_TT_ENVIRONMENTS))

    common.tt_environment = args.tt_env

    # setup quitting logic if an interrupt signal is received
    stop_running = Event()

    def exit_handler(signal_type, _):
        log.info('Quitting due to signal {}'.format(signal_type))
        stop_running.set()

    signal.signal(signal.SIGTERM, exit_handler)
    signal.signal(signal.SIGINT, exit_handler)
    if platform.system() != 'Windows':
        signal.signal(signal.SIGHUP, exit_handler)

    # Determine the output file name
    today = datetime.now()
    output = None
    if not args.output:
        # assuming Linux if not Windows
        temp_folder = 'temp' if platform.system() == 'Windows' else 'tmp'
        output = os.path.join(os.sep, temp_folder, 'fills_data')
        output = '{}_{}.csv'.format(output, today.strftime("%Y_%m_%d"))
    else:
        if '.' in args.output:
            file_name, file_ext = os.path.splitext(output)
        else:
            file_name = args.output
            file_ext = 'csv'
        output = '{}_{}.{}'.format(file_name, today.strftime("%Y_%m_%d"), file_ext)
        log.info('Outputting fills to {}'.format(output))

    # validate proper start and end times
    time_arg_regex = re.compile('^([0-1]?[0-9]|2[0-3]):[0-5][0-9]$')
    if not time_arg_regex.match(args.start_time):
        raise ValueError('Invalid value for start_time, must be in HH:MM format')
    if not time_arg_regex.match(args.end_time):
        raise ValueError('Invalid value for end_time, must be in HH:MM format')

    seconds_until_start = get_time_until(args.start_time)
    seconds_until_end = get_time_until(args.end_time)

    if seconds_until_start >= seconds_until_end:
        raise ValueError('End time is less than start time, please adjust')

    # best to store the secrets in an encrypted data store such as AWS S3,
    # however for this sample app, using getpass instead
    app_secret = getpass('Please enter your app_key: ')
    app_key = app_secret.split(':')[0]

    if seconds_until_start > 0:
        pause(seconds_until_start, stop_running, 'main')

    execution_handler = None
    while not stop_running.is_set():
        # this condition only comes into play on the first iteration if starting
        # after the given end time argument
        if seconds_until_end > 0:
            # Run the main logic thread
            execution_handler = Thread(target=fill_downloader,
                args=(app_key, app_secret, stop_running, time.time() + seconds_until_end, args.rate, output))

            try:
                log.info('Running main logic for {} seconds'.format(seconds_until_end))
                execution_handler.start()
            except Exception:
                stop_running.set()
                raise

        # don't run on weekends, but not taking into account holidays
        day_shift = 3 if datetime.now().weekday() == 4 else 1

        seconds_until_start = get_time_until(args.start_time, day_shift=day_shift)
        seconds_until_end = get_time_until(args.end_time, day_shift=day_shift)

        pause(seconds_until_start, stop_running, 'main')

    if execution_handler:
        log.info('Main thread waiting for the execution handler thread to stop')
        execution_handler.join()
