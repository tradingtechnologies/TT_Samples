# coding=utf-8
"""
    Copyright © 2022 Trading Technologies International, Inc. All Rights Reserved Worldwide

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

# This is a sample to show how to add a backtest.
# It does basic error handling but does not use exception handling.
# It adds backtest for the specific algo (scalper-7). User can change this algo and other related parameters.
# It adds backtest by using user parameters provided as ALGO_USER_PARAMETERS, and periodically checks the status
#  of the backtest, and prints the current status info(progress). Once the test finishes, it prints URL for downloading
#  the result if it is successful. On failure, it prints error status.

import argparse
import requests
import socket
import json
from datetime import *
from uuid import uuid1
import time
import sys

ALGO_NAME = 'scalper-7'

# below is the dictionary/map of user parameters displayName vs corresponding value
ALGO_USER_PARAMETERS = {
    'Order Instrument': '11632489858396816857',
    'Order Instrument Account': 32192,
    'Order Qty': 1,
    'ProfitTicks': 4,
    'RandomBoolean': True,
    'Ignore Market State': False,
    'Ladder Display': 0,
    'Order Side': 1
}

MARKET_DATA_DATE_TIME = '06-07-2022 13:00:00'  # DD-MM-YY HH:MM:SS
MARKET_DATA_DURATION_IN_SECOND = 60  # 60 seconds
REPLAY_SPEED = 1.0


class Backtest:
    def __init__(self, environment, api_key, access_token):
        self.environment = environment
        self.api_key = api_key
        self.access_token = access_token
        self.local_host = socket.gethostname()

        self.backtest_url = 'https://apigateway.trade.tt/ttbacktest/' + environment

        self.pds_url = 'https://apigateway.trade.tt/ttpds/' + environment

    def _get_header(self):
        header = {'x-api-key': self.api_key,
                  'Authorization': self.access_token,
                  'Content-Type': 'application/json',
                  'Connection': 'keep-alive'}
        return header

    def _get(self, base_url=None, endpoint=None):
        request_id = 'RESTTEST-{}--{}'.format(self.local_host, uuid1())

        if endpoint.__contains__("?"):
            request_id_separator = '&'
        else:
            request_id_separator = '?'

        if base_url:
            url = base_url + endpoint + request_id_separator + 'requestId={}'.format(request_id)
        else:
            url = self.backtest_url + endpoint + request_id_separator + 'requestId={}'.format(request_id)

        # print(url)
        response = requests.get(url=url, headers=self._get_header())
        return response.json()

    # region ***** backtest APIs *****
    def add_backtest(self, data):
        request_id = 'RESTTEST-{}--{}'.format(self.local_host, uuid1())
        url = self.backtest_url + "/algobacktest?{0}".format(request_id)
        response = requests.post(url=url, data=json.dumps(data), headers=self._get_header())
        return response.json()

    def delete_backtest(self, backtest_id):
        request_id = 'RESTTEST-{}--{}'.format(self.local_host, uuid1())
        url = self.backtest_url + "/algobacktest/{0}?{1}".format(backtest_id, request_id)
        response = requests.delete(url=url, headers=self._get_header())
        return response.json()

    def stop_backtest(self, backtest_id):
        request_id = 'RESTTEST-{}--{}'.format(self.local_host, uuid1())
        url = self.backtest_url + "/algobacktest/{0}/stop?{1}".format(backtest_id, request_id)
        response = requests.post(url=url, headers=self._get_header())
        return response.json()

    def get_all_backtests(self):
        return self._get(endpoint='/algobacktest')

    def get_backtest(self, backtest_id):
        return self._get(endpoint='/algobacktest/{0}'.format(backtest_id))

    def get_algobacktest_result(self, backtest_id):
        return self._get(endpoint="/algobacktest/{0}/result".format(backtest_id))
    # endregion

    # region ***** PDS APIs *****
    def get_algos(self):
        return self._get(base_url=self.pds_url, endpoint='/algos')

    def get_algo_userparams(self, algo_id):
        return self._get(base_url=self.pds_url, endpoint='/algos/{0}/userparameters'.format(algo_id))

    def get_market_id_for_instrument(self, instrument_id):
        r = self._get(base_url=self.pds_url, endpoint='/instrument/{0}'.format(instrument_id))
        if r['status'] == 'Ok':
            return r['instrument'][0]['marketId']
        return None
    # endregion


def get_token(environment, key, secret):
    url = 'https://apigateway.trade.tt/ttid/' + environment

    request_id = 'RESTTEST-GetToken--{}'.format(uuid1())
    url += '/token?requestId=' + request_id

    headers = {
        'Content-Type': 'application/x-www-form-urlencoded',
        'x-api-key': key}

    data = {
        'grant_type': 'user_app',
        'app_key': secret}

    login = requests.post(url=url, data=data, headers=headers)

    if login.status_code == 200:
        content = login.json()
        token = '{} {}'.format(content['token_type'].capitalize(),
                               content['access_token'])
        return token
    else:
        return None


if __name__ == '__main__':
    # region ***** Initialize parser *****
    parser = argparse.ArgumentParser(formatter_class=argparse.RawTextHelpFormatter)

    # This is just to show other environment can be added into choices
    parser.add_argument('-e', '--env', required=True,
                        dest='env', action='store',
                        choices=['ext_prod_sim'],
                        default='ext_prod_sim',
                        help='TT environment')

    parser.add_argument('-k', '--key', required=True,
                        dest='app_key', action='store',
                        help='TT RestAPI AppKey.')

    args = parser.parse_args()

    app_secret = args.app_key
    app_key = app_secret.split(':')[0]

    # endregion

    print('Status: getting authorization token')
    token = get_token(args.env, app_key, app_secret)

    if token:
        backtest_api = Backtest(args.env, app_key, token)

        # region ***** Get algo list and pick one *****
        print('Status: getting list of algos available for backtest')
        available_algos = backtest_api.get_algos()

        if available_algos['status'] == 'Ok' and 'algos' in available_algos:
            print('Available Algos:')
            for number, algo in enumerate(available_algos['algos']):
                print(number, "-", algo['name'])
        else:
            print('Status: {0}'.format(json.dumps(available_algos)))
            exit(0)

        algo_to_use = next((item for item in available_algos["algos"] if item["name"] == ALGO_NAME), None)

        if algo_to_use is None:
            print(ALGO_NAME + ' is not present in the list, choose something from above list')
            exit(0)
        # endregion

        # region ***** user parameters for the seleted algo *****
        print('\nStatus: getting user parameters for {}'.format(ALGO_NAME))
        userparams_response = backtest_api.get_algo_userparams(algo_to_use['id'])

        user_parameters = []
        if userparams_response['status'] == 'Ok':
            print('Status: setting user parameters')
            for param in userparams_response['userParameters']:
                if param['displayName'] in ALGO_USER_PARAMETERS:
                    user_parameter = {
                        'displayName': param['displayName'],
                        'name': param['name'],
                        'type': param['type'],
                        param['value']: ALGO_USER_PARAMETERS[param['displayName']]
                    }
                    # print(user_parameter)
                    user_parameters.append(user_parameter)
                else:
                    print('ALGO_USER_PARAMETERS is incorrect.')
                    exit(0)

            # Need to add algo version in user parameters
            algo_version = {
                "displayName": "Algo Version",
                "name": "_algo_version",
                "type": 7,
                "stringValue": algo_to_use['version']
            }

            user_parameters.append(algo_version)
        else:
            print('Status: {0}'.format(json.dumps(userparams_response)))
            exit(0)
        # endregion

        market_data_datetime = datetime.strptime(MARKET_DATA_DATE_TIME, '%d-%m-%Y %H:%M:%S')

        # region calculate the epoch time
        start_time = int((market_data_datetime - datetime(1970, 1, 1)).total_seconds())
        end_time = start_time + MARKET_DATA_DURATION_IN_SECOND
        # endregion

        # region ***** create payload and add backtest using it *****
        instrument_id = ALGO_USER_PARAMETERS['Order Instrument']
        market_id = backtest_api.get_market_id_for_instrument(instrument_id)
        account_id = ALGO_USER_PARAMETERS['Order Instrument Account']
        data = {
            'accountId': account_id,
            'algoId': algo_to_use['id'],
            'startTime': start_time,
            'stopTime': end_time,
            'replaySpeed': REPLAY_SPEED,
            'marketId': market_id,
            'replayInstruments': [],
            'userParameters': []
        }

        data['replayInstruments'].append(instrument_id)
        data['userParameters'].append(user_parameters)

        print("Status: adding new backtest")
        response = backtest_api.add_backtest(data)
        # endregion

        backtest_id = None
        if response['status'] == 'Ok':
            backtest_id = response['backtest']['id']
            print('added new backtest, backtest id: {0}'.format(backtest_id))

            # region ****** status and progress *******
            current_status = None
            new_status = None
            backtest_info = backtest_api.get_backtest(backtest_id)

            while backtest_info['status'] == 'Ok' and new_status not in ['FINISHED', 'SUCCESS', 'STOPPED', 'FAIL']:
                new_status = backtest_info['backtest']['backtestStatus']
                if current_status != new_status:
                    current_status = new_status
                    sys.stdout.write('\nStatus: {0}'.format(current_status))
                else:
                    sys.stdout.write('-')

                sys.stdout.flush()
                time.sleep(5)
                backtest_info = backtest_api.get_backtest(backtest_id)
            # endregion

            # region ***** Get result of the backtest *****
            if backtest_info['status'] == 'Ok':
                if backtest_id and new_status in ['FINISHED', 'SUCCESS']:
                    print('\nStatus: Downloading result URL')
                    response = backtest_api.get_algobacktest_result(backtest_id)
                    if response['status'] == 'Ok':
                        print('URL: {0}'.format(response['url']))
                    else:
                        print('Status: {0}'.format(json.dumps(response)))
                else:
                    print("Status: {0}".format(backtest_info))
            else:
                print('Status: {0}'.format(json.dumps(backtest_info)))
            # endregion

            print('\nStatus: Done')
    else:
        print("Unable to authorize")
