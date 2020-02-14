// **********************************************************************************************************************
//
//	Copyright © 2005-2020 Trading Technologies International, Inc.
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
using System.Threading;
using tt_net_sdk;

namespace TTNETAPI_Sample_Console_AlgoOrderRouting
{
    public class TTNetApiFunctions
    {
        // Declare the API objects
        private TTAPI m_api = null;
        private InstrumentLookup m_instrLookupRequest = null;
        private PriceSubscription m_priceSubscription = null;
        private tt_net_sdk.WorkerDispatcher m_disp = null;
        private AlgoTradeSubscription m_algoTradeSubscription = null;
        private AlgoLookupSubscription m_algoLookupSubscription = null;
        private IReadOnlyCollection<Account> m_accounts = null;
        private Instrument m_instrument = null;
        private Algo m_algo = null;
        private object m_Lock = new object();
        private bool m_isDisposed = false;
        private Price m_price = Price.Empty;

        // Instrument Information 
        private readonly string m_market = "CME";
        private readonly string m_product = "GE";
        private readonly string m_prodType = "Future";
        private readonly string m_alias = "GE Sep21";


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

            //For Algo Orders
            apiConfig.AlgoUserDisconnectAction = UserDisconnectAction.Cancel;
            TTAPI.CreateTTAPI(tt_net_sdk.Dispatcher.Current,apiConfig,apiInitializeHandler);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for status of API initialization. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void ttNetApiInitHandler(TTAPI api,ApiCreationException ex)
        {
            if(ex == null)
            {
                Console.WriteLine("TT.NET SDK INITIALIZED");

                // Authenticate your credentials
                m_api = api;
                m_api.TTAPIStatusUpdate += new EventHandler<TTAPIStatusUpdateEventArgs>(m_api_TTAPIStatusUpdate);
                m_api.Start();
            }
            else if(ex.IsRecoverable)
            {
                // Initialization failed but retry is in progress...
            }
            else
            {
                Console.WriteLine("TT.NET SDK Initialization Failed: {0}",ex.Message);
                Dispose();
            }
        }

        void StartAlgo()
        {
            while (! m_price.IsValid || m_algo ==null)
            {
                Console.WriteLine("Waiting ...");
                Thread.Sleep(1000);
            }

            // To retrieve the list of parameters valid for the Algo you can call algo.AlgoParameters;
            // Construct a dictionary of the parameters and the values to send out 
            Dictionary<string,object> algo_userparams = new Dictionary<string,object>
                {
                    {"Ignore Market State",     true},
                };

            var lines = algo_userparams.Select(kvp => kvp.Key + ": " + kvp.Value.ToString());
            Console.WriteLine(string.Join(Environment.NewLine,lines));

            OrderProfile algo_op = m_algo.GetOrderProfile(m_instrument);
            algo_op.LimitPrice = m_price;
            algo_op.OrderQuantity = Quantity.FromDecimal(m_instrument,5); ;
            algo_op.Side = OrderSide.Buy;
            algo_op.OrderType = OrderType.Limit;
            algo_op.Account = m_accounts.ElementAt(0);
            algo_op.UserParameters = algo_userparams;
            m_algoTradeSubscription.SendOrder(algo_op);
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for status of authentication. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void m_api_TTAPIStatusUpdate(object sender,TTAPIStatusUpdateEventArgs e)
        {
            Console.WriteLine("TTAPIStatusUpdate: {0}",e);
            if(e.IsReady == false)
            {
                // TODO: Do any connection lost processing here
                return;
            }
            // TODO: Do any connection up processing here
            //       note: can happen multiple times with your application life cycle

            // can get status multiple times - do not create subscription if it exists
            //
            if(object.ReferenceEquals(m_instrLookupRequest,null) == false)
                return;

            MarketId marketKey = Market.GetMarketIdFromName(m_market);
            ProductType productType = Product.GetProductTypeFromName(m_prodType);

            // lookup an instrument
            m_instrLookupRequest = new InstrumentLookup(tt_net_sdk.Dispatcher.Current,
                        marketKey,productType,m_product,m_alias);

            m_instrLookupRequest.OnData += m_instrLookupRequest_OnData;
            m_instrLookupRequest.GetAsync();

            // Get the accounts
            m_accounts = m_api.Accounts;

            // Start Algo Trading thread.
            Thread algoThread = new Thread(() => this.StartAlgo());
            algoThread.Name = "Algo Trading Thread";
            algoThread.Start();
        }

        private void AlgoLookupSubscription_OnData(object sender, AlgoLookupEventArgs e)
        {
            if (e.Event == ProductDataEvent.Found)
            {   
                Console.WriteLine("Algo Instrument Found: {0}", e.AlgoLookup.Algo.Alias);
                m_algo = e.AlgoLookup.Algo;

                // Create an Algo TradeSubscription to listen for order / fill events only for orders submitted through it
                m_algoTradeSubscription = new AlgoTradeSubscription(tt_net_sdk.Dispatcher.Current, m_algo);

                m_algoTradeSubscription.OrderUpdated += new EventHandler<OrderUpdatedEventArgs>(m_algoTradeSubscription_OrderUpdated);
                m_algoTradeSubscription.OrderAdded += new EventHandler<OrderAddedEventArgs>(m_algoTradeSubscription_OrderAdded);
                m_algoTradeSubscription.OrderDeleted += new EventHandler<OrderDeletedEventArgs>(m_algoTradeSubscription_OrderDeleted);
                m_algoTradeSubscription.OrderFilled += new EventHandler<OrderFilledEventArgs>(m_algoTradeSubscription_OrderFilled);
                m_algoTradeSubscription.OrderRejected += new EventHandler<OrderRejectedEventArgs>(m_algoTradeSubscription_OrderRejected);
                m_algoTradeSubscription.OrderBookDownload += new EventHandler<OrderBookDownloadEventArgs>(m_algoTradeSubscription_OrderBookDownload);
                m_algoTradeSubscription.ExportValuesUpdated += new EventHandler<ExportValuesUpdatedEventArgs>(m_algoTradeSubscription_ExportValuesUpdated);
                m_algoTradeSubscription.AlertsFired += new EventHandler<AlertsFiredEventArgs>(m_algoTradeSubscription_AlertsUpdated);
                m_algoTradeSubscription.Start();
            }
            else if (e.Event == ProductDataEvent.NotAllowed)
            {
                Console.WriteLine("Not Allowed : Please check your Token access");
            }
            else
            {
                // Algo Instrument was not found and TT API has given up looking for it
                Console.WriteLine("Cannot find Algo instrument: {0}", e.Message);
                Dispose();
            }
        }

        void m_instrLookupRequest_OnData(object sender, InstrumentLookupEventArgs e)
        {
            if (e.Event == ProductDataEvent.Found)
            {
                // Instrument was found
                m_instrument = e.InstrumentLookup.Instrument;
                Console.WriteLine("Found: {0}", m_instrument);

                AlgoLookupSubscription algoLookupSubscription = new AlgoLookupSubscription(tt_net_sdk.Dispatcher.Current, "test-algo");
                algoLookupSubscription.OnData += AlgoLookupSubscription_OnData;
                algoLookupSubscription.GetAsync(); 
                
                // Subscribe for market Data
                m_priceSubscription = new PriceSubscription(m_instrument, tt_net_sdk.Dispatcher.Current);
                m_priceSubscription.Settings = new PriceSubscriptionSettings(PriceSubscriptionType.MarketDepth);
                m_priceSubscription.FieldsUpdated += m_priceSubscription_FieldsUpdated;
                m_priceSubscription.Start();   
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
            if (e.Error != null)
            {
                Console.WriteLine("Unrecoverable price subscription error: {0}", e.Error.Message);
                Dispose();
            }
            else if (e.Fields.GetBestBidPriceField().Value != null)
                m_price = e.Fields.GetBestBidPriceField().Value;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order book download complete. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_OrderBookDownload(object sender, OrderBookDownloadEventArgs e)
        {
            Console.WriteLine("Orderbook downloaded...");
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order rejection. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_OrderRejected(object sender, OrderRejectedEventArgs e)
        {
            Console.WriteLine("\nOrderRejected for : [{0}]", e.Order.SiteOrderKey);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order fills. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_OrderFilled(object sender, OrderFilledEventArgs e)
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
        void m_algoTradeSubscription_OrderDeleted(object sender, OrderDeletedEventArgs e)
        {
            Console.WriteLine("\nOrderDeleted [{0}] , Message : {1}", e.OldOrder.SiteOrderKey, e.Message);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order addition. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_OrderAdded(object sender, OrderAddedEventArgs e)
        {   
            if (e.Order.IsSynthetic)
                Console.WriteLine("\nPARENT Algo OrderAdded [{0}] for Algo : {1} with Synthetic Status : {2} ", e.Order.SiteOrderKey, e.Order.Algo.Alias, e.Order.SyntheticStatus.ToString());
            else
                Console.WriteLine("\nCHILD OrderAdded [{0}] {1}: {2}", e.Order.SiteOrderKey, e.Order.BuySell, e.Order.ToString());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order update. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_OrderUpdated(object sender, OrderUpdatedEventArgs e)
        {
            if(e.NewOrder.ExecutionType == ExecType.Restated)
                Console.WriteLine("\nAlgo Order Restated [{0}] for Algo : {1} with Synthetic Status : {2} ", e.NewOrder.SiteOrderKey, e.NewOrder.Algo.Alias, e.NewOrder.SyntheticStatus.ToString());
            else
                Console.WriteLine("\nOrderUpdated [{0}] {1}: {2}", e.NewOrder.SiteOrderKey, e.NewOrder.BuySell, e.NewOrder.ToString());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for Algo ExportedValue update. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_ExportValuesUpdated(object sender, ExportValuesUpdatedEventArgs e)
        {
            foreach(string key in e.ExportValues.Keys)
            {
                Console.WriteLine("Algo EVU: Parameter Name = {0} and Parameter Value = {1}",key,e.ExportValues[key]);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for Algo Alert update. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_AlertsUpdated(object sender, AlertsFiredEventArgs e)
        {
            foreach(string key in e.Alerts.Keys)
            {
                Console.WriteLine("Algo ALERTs Fired: Name = {0} and Alert Value = {1}",key,e.Alerts[key]);
            }
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

                    if (m_algoLookupSubscription != null)
                    {
                        m_algoLookupSubscription.OnData -= AlgoLookupSubscription_OnData;
                        m_algoLookupSubscription.Dispose();
                        m_algoLookupSubscription = null;
                    }

                    if (m_priceSubscription != null)
                    {
                        m_priceSubscription.FieldsUpdated -= m_priceSubscription_FieldsUpdated;
                        m_priceSubscription.Dispose();
                        m_priceSubscription = null;
                    }

                    if (m_algoTradeSubscription != null)
                    {
                        m_algoTradeSubscription.OrderUpdated -= m_algoTradeSubscription_OrderUpdated;
                        m_algoTradeSubscription.OrderAdded -= m_algoTradeSubscription_OrderAdded;
                        m_algoTradeSubscription.OrderDeleted -= m_algoTradeSubscription_OrderDeleted;
                        m_algoTradeSubscription.OrderFilled -= m_algoTradeSubscription_OrderFilled;
                        m_algoTradeSubscription.OrderRejected -= m_algoTradeSubscription_OrderRejected;
                        m_algoTradeSubscription.ExportValuesUpdated -= m_algoTradeSubscription_ExportValuesUpdated;
                        m_algoTradeSubscription.AlertsFired -= m_algoTradeSubscription_AlertsUpdated;
                        m_algoTradeSubscription.Dispose();
                        m_algoTradeSubscription = null;
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

