
from System import Action

import sys
import clr

clr.AddReferenceToFileAndPath(r"tt-net-api.dll")
from tt_net_sdk import *

m_api = None
m_instrument = None
m_ts = None
m_isOrderBookDownloaded = 0
m_orderKey = None


def m_ts_OrderBookDownload(sender, e):
    print("OrderBookDownload complete...")
    global m_isOrderBookDownloaded
    m_isOrderBookDownloaded = 1


def m_ts_OrderAdded(sender, e):
    print("Order added: " + e.ToString())


def m_ts_OrderDeleted(sender, e):
    print("Order deleted: " + e.ToString())
    sys.exit()


def m_ts_OrderFilled(sender, e):
    print("Order filled: " + e.ToString())


def m_ts_OrderRejected(sender, e):
    print("Order rejected: " + e.ToString())
    sys.exit()


def m_ts_OrderUpdated(sender, e):
    print("Order updated: " + e.ToString())


def m_ts_OrderStatusUnknown(sender, e):
    print("Order status unknown: " + e.ToString())
    sys.exit()


def m_ts_OrderTimeout(sender, e):
    print("Order timed out: " + e.ToString())
    sys.exit()


def m_priceSubscription_FieldsUpdated(sender, e):
    global m_orderKey
    global m_isOrderBookDownloaded
    global m_ts
    if e.Error is None:
        if e.Fields.GetBestBidPriceField().Value != None and m_isOrderBookDownloaded == 1:
            if m_orderKey == None:
                op = OrderProfile(m_instrument)
                op.Account = m_api.DefaultAccount
                op.BuySell = BuySell.Buy
                op.OrderType = OrderType.Limit
                op.OrderQuantity = Quantity.FromDecimal(m_instrument, 10)
                op.LimitPrice = e.Fields.GetBestBidPriceField().Value

                if m_ts.SendOrder(op) == False:
                    print("Send new order Failed!!")
                    sys.exit()
                else:
                    m_orderKey = op.SiteOrderKey
                    print("Sent new order key = " + op.SiteOrderKey)
            elif m_ts.Orders.ContainsKey(m_orderKey) and m_ts.Orders[
                m_orderKey].LimitPrice != e.Fields.GetBestBidPriceField().Value:
                op = m_ts.Orders[m_orderKey].GetOrderProfile()
                op.LimitPrice = e.Fields.GetBestBidPriceField().Value
                op.Action = OrderAction.Change

                if m_ts.SendOrder(op) == False:
                    print("Sent order update failed!!")
                else:
                    print("Send change order succeeded.")


def m_api_TTAPIStatusUpdate(sender, e):
    if e.IsReady:
        print("Ready...")
        print("Look up instrument...")
        m_instrLookupRequest = InstrumentLookup(Dispatcher.Current, MarketId.CME, ProductType.Future, "CL", "CL Dec19")
        e2 = m_instrLookupRequest.Get()

        if e2 == ProductDataEvent.Found:
            global m_instrument
            m_instrument = m_instrLookupRequest.Instrument
            print("Instrument found...")

            global m_ts
            m_ts = TradeSubscription(Dispatcher.Current)
            m_ts.OrderBookDownload += m_ts_OrderBookDownload
            m_ts.OrderAdded += m_ts_OrderAdded
            m_ts.OrderDeleted += m_ts_OrderDeleted
            m_ts.OrderFilled += m_ts_OrderFilled
            m_ts.OrderUpdated += m_ts_OrderUpdated
            m_ts.OrderUpdated += m_ts_OrderStatusUnknown
            m_ts.OrderUpdated += m_ts_OrderTimeout
            m_ts.Start()

            m_priceSubsciption = PriceSubscription(m_instrLookupRequest.Instrument, Dispatcher.Current)
            m_priceSubsciption.Settings = PriceSubscriptionSettings(PriceSubscriptionType.MarketDepth)
            m_priceSubsciption.FieldsUpdated += m_priceSubscription_FieldsUpdated
            m_priceSubsciption.Start()
        else:
            print("Cannot find instrument: " + e.ToString())


def ttNetApiInitHandler(api, ex):
    if ex is None:
        global m_api
        m_api = api
        api.TTAPIStatusUpdate += m_api_TTAPIStatusUpdate
        api.Start()
    else:
        print("TT.NET SDK Initialization Failed: " + ex.Message)


def Init():
    opt = TTAPIOptions(ServiceEnvironment.UatCert,
                       "7601b8a0-dd23-2a8a-ee52-3712c53841d4:9ad4a302-5577-45c3-11db-6138b2757cca", 5000)
    apiInitializeHandler = ApiInitializeHandler(ttNetApiInitHandler)
    TTAPI.CreateTTAPI(Dispatcher.Current, opt, apiInitializeHandler)


m_disp = Dispatcher.AttachWorkerDispatcher()
m_disp.DispatchAction(Action(Init))
m_disp.Run()
