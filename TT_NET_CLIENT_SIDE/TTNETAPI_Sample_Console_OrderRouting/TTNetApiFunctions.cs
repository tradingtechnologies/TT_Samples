// **********************************************************************************************************************
//
//	Copyright © 2005-2019 Trading Technologies International, Inc.
//	All Rights Reserved Worldwide
//
// 	* * * S T R I C T L Y   P R O P R I E T A R Y * * *
//
//  WARNING: This file and all related programs (including any computer programs, example programs, and all source code) 
//  are the exclusive property of Trading Technologies International, Inc. (“TT”), are protected by copyright law and 
//  international treaties, and are for use only by those with the express written permission from TT.  Unauthorized 
//  possession, reproduction, distribution, use or disclosure of this file and any related program (or document) derived 
//  from it is prohibited by State and Federal law, and by local law outside of the U.S. and may result in severe civil 
//  and criminal penalties.
//
// ************************************************************************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using tt_net_sdk;

namespace TTNETAPI_Sample_Console_OrderRouting
{
    public class TTNetApiFunctions
    {
        // Declare the API objects
        private TTAPI m_api = null;
        private InstrumentLookup m_instrLookupRequest = null;
        private PriceSubscription m_priceSubscription = null;
        private tt_net_sdk.WorkerDispatcher m_disp = null;
        private InstrumentTradeSubscription m_instrumentTradeSubscription = null;
        private IReadOnlyCollection<Account> m_accounts = null;
        private bool m_isOrderBookDownloaded = false;
        private string m_orderKey = "";
        private object m_Lock = new object();
        private bool m_isDisposed = false;

        // Instrument Information 
        private readonly string m_market = "CME";
        private readonly string m_product = "GE";
        private readonly string m_prodType = "Future";
        private readonly string m_alias = "GE Sep20";


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>  Attach the worker Dispatcher </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void Start(tt_net_sdk.TTAPIOptions apiConfig)
        {
            m_disp = tt_net_sdk.Dispatcher.AttachWorkerDispatcher();
            m_disp.DispatchAction(() =>
            {
                Init(apiConfig);
            });

            m_disp.Run();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Initialize the API </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void Init(tt_net_sdk.TTAPIOptions apiConfig)
        {
            ApiInitializeHandler apiInitializeHandler = new ApiInitializeHandler(ttNetApiInitHandler);
            TTAPI.ShutdownCompleted += TTAPI_ShutdownCompleted;
            TTAPI.CreateTTAPI(tt_net_sdk.Dispatcher.Current, apiConfig, apiInitializeHandler);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for status of API initialization. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void ttNetApiInitHandler(TTAPI api, ApiCreationException ex)
        {
            if (ex == null)
            {
                Console.WriteLine("TT.NET SDK INITIALIZED");

                // Authenticate your credentials
                m_api = api;
                m_api.TTAPIStatusUpdate += new EventHandler<TTAPIStatusUpdateEventArgs>(m_api_TTAPIStatusUpdate);
                m_api.Start();
            }
            else if (ex.IsRecoverable)
            {
                // Initialization failed but retry is in progress...
            }
            else
            {
                Console.WriteLine("TT.NET SDK Initialization Failed: {0}", ex.Message);
                Dispose();
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for status of authentication. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void m_api_TTAPIStatusUpdate(object sender, TTAPIStatusUpdateEventArgs e)
        {
            if (e.IsReady)
            {
                // connection to TT is established
                Console.WriteLine("TT.NET SDK Authenticated");

                MarketId  marketKey   = Market.GetMarketIdFromName( m_market );
                ProductType productType = Product.GetProductTypeFromName(m_prodType);

                // lookup an instrument
                m_instrLookupRequest = new InstrumentLookup(tt_net_sdk.Dispatcher.Current,
                            marketKey, productType, m_product, m_alias);

                m_instrLookupRequest.OnData += m_instrLookupRequest_OnData;
                m_instrLookupRequest.GetAsync();

                // Get the accounts
                m_accounts = m_api.Accounts;

            }
            else
            {
                Console.WriteLine("TT.NET SDK Status: {0}", e);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for instrument lookup. </summary>
        /// <param name="sender">   Source of the event. </param>
        /// <param name="e">        Instrument lookup subscription event information. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_instrLookupRequest_OnData(object sender, InstrumentLookupEventArgs e)
        {
            if (e.Event == ProductDataEvent.Found)
            {
                // Instrument was found
                Instrument instrument = e.InstrumentLookup.Instrument;
                Console.WriteLine("Found: {0}", instrument);

                // Subscribe for market Data
                m_priceSubscription = new PriceSubscription(instrument, tt_net_sdk.Dispatcher.Current);
                m_priceSubscription.Settings = new PriceSubscriptionSettings(PriceSubscriptionType.MarketDepth);
                m_priceSubscription.FieldsUpdated += m_priceSubscription_FieldsUpdated;
                m_priceSubscription.Start();


                // Create a TradeSubscription to listen for order / fill events only for orders submitted through it
                m_instrumentTradeSubscription = new InstrumentTradeSubscription(tt_net_sdk.Dispatcher.Current, instrument);

                m_instrumentTradeSubscription.OrderUpdated += new EventHandler<OrderUpdatedEventArgs>(m_instrumentTradeSubscription_OrderUpdated);
                m_instrumentTradeSubscription.OrderAdded += new EventHandler<OrderAddedEventArgs>(m_instrumentTradeSubscription_OrderAdded);
                m_instrumentTradeSubscription.OrderDeleted += new EventHandler<OrderDeletedEventArgs>(m_instrumentTradeSubscription_OrderDeleted);
                m_instrumentTradeSubscription.OrderFilled += new EventHandler<OrderFilledEventArgs>(m_instrumentTradeSubscription_OrderFilled);
                m_instrumentTradeSubscription.OrderRejected += new EventHandler<OrderRejectedEventArgs>(m_instrumentTradeSubscription_OrderRejected);
                m_instrumentTradeSubscription.OrderBookDownload += new EventHandler<OrderBookDownloadEventArgs>(m_instrumentTradeSubscription_OrderBookDownload);
                m_instrumentTradeSubscription.Start();
            }
            else if (e.Event == ProductDataEvent.NotAllowed)
            {
                Console.WriteLine("Not Allowed : Please check your Token access");
            }
            else
            {
                // Instrument was not found and TT API has given up looking for it
                Console.WriteLine("Cannot find instrument: {0}", e.Message);
                Dispose();
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for price update. </summary>
        /// <param name="sender">   Source of the event. </param>
        /// <param name="e">        Fields updated event information. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_priceSubscription_FieldsUpdated(object sender, FieldsUpdatedEventArgs e)
        {
            if (e.Error == null)
            {
                // Make sure that there is a valid bid
                if (e.Fields.GetBestBidPriceField().Value != null && m_isOrderBookDownloaded)
                {
                    if (m_orderKey == "")
                    {
                        // If there is no order working, submit one .
                        OrderProfile op = new OrderProfile(e.Fields.Instrument);
                        op.BuySell = BuySell.Buy;
                        op.Account = m_accounts.ElementAt(0);
                        op.OrderQuantity = Quantity.FromDecimal(e.Fields.Instrument, 10);
                        op.OrderType = OrderType.Limit;
                        op.LimitPrice = e.Fields.GetBestBidPriceField().Value - 1;

                        if (!m_instrumentTradeSubscription.SendOrder(op))
                        {
                            Console.WriteLine("Send new order Failed.!!");
                            Dispose();
                        }
                        else
                        {
                            m_orderKey = op.SiteOrderKey;
                            Console.WriteLine("\nSent new order: " + e.Fields.Instrument.Name + " " + op.BuySell + " " + op.OrderQuantity.ToString() + "@" + op.LimitPrice.ToString() + " SOK=" + op.SiteOrderKey);
                        }
                    }
                    else if (m_instrumentTradeSubscription.Orders.ContainsKey(m_orderKey) &&
                        m_instrumentTradeSubscription.Orders[m_orderKey].LimitPrice != (e.Fields.GetBestBidPriceField().Value - 1))
                    {
                        // If there is a working order, reprice it
                        OrderProfile op = m_instrumentTradeSubscription.Orders[m_orderKey].GetOrderProfile();
                        op.LimitPrice = e.Fields.GetBestBidPriceField().Value - 1;
                        op.Action = OrderAction.Change;

                        Console.WriteLine("Change price from {0} to {1}", m_instrumentTradeSubscription.Orders[m_orderKey].LimitPrice, op.LimitPrice);

                        if (!m_instrumentTradeSubscription.SendOrder(op))
                        {
                            Console.WriteLine("Sent order update: " + e.Fields.Instrument.Name + " " + op.OrderQuantity.ToString() + "@" + op.LimitPrice.ToString() + " SOK=" + op.SiteOrderKey);
                        }
                        else
                        {
                            Console.WriteLine("Send change order succeeded.");
                        }
                    }
                }
            }
            else
            {
                if (e.Error != null)
                {
                    Console.WriteLine("Unrecoverable price subscription error: {0}", e.Error.Message);
                    Dispose();
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order book download complete. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_instrumentTradeSubscription_OrderBookDownload(object sender, OrderBookDownloadEventArgs e)
        {
            Console.WriteLine("Orderbook downloaded...");
            m_isOrderBookDownloaded = true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order rejection. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_instrumentTradeSubscription_OrderRejected(object sender, OrderRejectedEventArgs e)
        {
            Console.WriteLine("\nOrderRejected [{0}]", e.Order.SiteOrderKey);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order fills. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_instrumentTradeSubscription_OrderFilled(object sender, OrderFilledEventArgs e)
        {
            if (e.FillType == tt_net_sdk.FillType.Full)
            {
                Console.WriteLine("\nOrderFullyFilled [{0}]: {1}@{2}", e.Fill.SiteOrderKey, e.Fill.Quantity, e.Fill.MatchPrice);
            }
            else
            {
                Console.WriteLine("\nOrderPartiallyFilled [{0}]: {1}@{2}", e.Fill.SiteOrderKey, e.Fill.Quantity, e.Fill.MatchPrice);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order deletion. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_instrumentTradeSubscription_OrderDeleted(object sender, OrderDeletedEventArgs e)
        {
            Console.WriteLine("\nOrderDeleted [{0}]", e.OldOrder.SiteOrderKey);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order addition. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_instrumentTradeSubscription_OrderAdded(object sender, OrderAddedEventArgs e)
        {
            Console.WriteLine("\nOrderAdded [{0}] {1}: {2}", e.Order.SiteOrderKey, e.Order.BuySell, e.Order.ToString());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order update. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_instrumentTradeSubscription_OrderUpdated(object sender, OrderUpdatedEventArgs e)
        {
            Console.WriteLine("\nOrderUpdated [{0}] with price={1}", e.NewOrder.SiteOrderKey, e.NewOrder.LimitPrice);
        }

        public void Dispose()
        {
            lock (m_Lock)
            {
                if (!m_isDisposed)
                {
                    // Unattached callbacks and dispose of all subscriptions
                    if (m_instrLookupRequest != null)
                    {
                        m_instrLookupRequest.OnData -= m_instrLookupRequest_OnData;
                        m_instrLookupRequest.Dispose();
                        m_instrLookupRequest = null;
                    }
                    if (m_priceSubscription != null)
                    {
                        m_priceSubscription.FieldsUpdated -= m_priceSubscription_FieldsUpdated;
                        m_priceSubscription.Dispose();
                        m_priceSubscription = null;
                    }
                    if (m_instrumentTradeSubscription != null)
                    {
                        m_instrumentTradeSubscription.OrderUpdated -= m_instrumentTradeSubscription_OrderUpdated;
                        m_instrumentTradeSubscription.OrderAdded -= m_instrumentTradeSubscription_OrderAdded;
                        m_instrumentTradeSubscription.OrderDeleted -= m_instrumentTradeSubscription_OrderDeleted;
                        m_instrumentTradeSubscription.OrderFilled -= m_instrumentTradeSubscription_OrderFilled;
                        m_instrumentTradeSubscription.OrderRejected -= m_instrumentTradeSubscription_OrderRejected;
                        m_instrumentTradeSubscription.Dispose();
                        m_instrumentTradeSubscription = null;
                    }

                    m_isDisposed = true;
                }

                
                TTAPI.Shutdown();
            }
        }

        public void TTAPI_ShutdownCompleted(object sender, EventArgs e)
        {
            // Dispose of any other objects / resources
            Console.WriteLine("TTAPI shutdown completed");
        }
    }
}
