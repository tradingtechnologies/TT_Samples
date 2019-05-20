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

namespace TTNETAPI_Sample_Console_SSEOrderRouting
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

            //For Algo Orders
            apiConfig.AlgoUserDisconnectAction = UserDisconnectAction.Cancel;
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
            Console.WriteLine("TTAPIStatusUpdate: {0}", e);
            if (e.IsReady == false)
            {
                // TODO: Do any connection lost processing here
                return;
            }
            // TODO: Do any connection up processing here
            //       note: can happen multiple times with your application life cycle

            // can get status multiple times - do not create subscription if it exists
            //
            if (object.ReferenceEquals(m_instrLookupRequest, null) == false)
                return;

            MarketId marketKey = Market.GetMarketIdFromName(m_market);
            ProductType productType = Product.GetProductTypeFromName(m_prodType);

            // lookup an instrument
            m_instrLookupRequest = new InstrumentLookup(tt_net_sdk.Dispatcher.Current,
                        marketKey, productType, m_product, m_alias);

            m_instrLookupRequest.OnData += m_instrLookupRequest_OnData;
            m_instrLookupRequest.GetAsync();

            // Get the accounts
            m_accounts = m_api.Accounts;
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

                AlgoLookupSubscription algoLookupSubscription = new AlgoLookupSubscription(tt_net_sdk.Dispatcher.Current, "TT Iceberg");
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
                m_price = e.Fields.GetBestBidPriceField().Value - 1;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order book download complete. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_OrderBookDownload(object sender, OrderBookDownloadEventArgs e)
        {
            Console.WriteLine("Orderbook downloaded...");
            
            //To retrieve the list of parameters valid for the Algo you can call
            //algo.AlgoParameters;

            //For an Iceberg Synthetic order here is the list of parameters you can set.
            /***************************************************************************
                Strategy: TT_Iceberg
                ***************************************************************************
            ----------------------------------------------------------------------------------------------
            ORDER PROFILE PROPERTIES
            Name Type                                       Required                Updateable
            ----------------------------------------------------------------------------------------------
            OrderInstrumentID                                 true                     false
            OrderQty                                          true                     true
            OrderSide                                         true                     false
            OrderAccount                                      true                     false
            OrderType                                         true                     false
            LimitPrice                                        true                     true


            ----------------------------------------------------------------------------------------------------------------------
            USER PARAMETER PROPERTIES
            Name                        Type                Required         Updateable   Algo Specific Enum
            -----------------------------------------------------------------------------------------------------------------------
            ChildTIF                    Int_t               true            false
            ParentTIF                   Int_t               true            false         tt_net_sdk.tt_iceberg.ParentTIF
            DiscVal                     Qty_t               true            true         
            DiscValType                 Int_t               true            true          tt_net_sdk.tt_iceberg.DiscValType
            Variance                    Int_t               false           false          
            LimitTicksAway              Int_t               false           true        
            LimitPriceType              Int_t               false           false         tt_net_sdk.tt_iceberg.LimitPriceType
            TriggerType                 Int_t               false           false         tt_net_sdk.tt_iceberg.TriggerType
            TriggerPriceType            Int_t               false           false         tt_net_sdk.tt_iceberg.TriggerPriceType
            IsTrlTrg                    Boolean_t           false           false         
            TriggerTicksAway            Int_t               false           true
            WithATickType               Int_t               false           false         tt_net_sdk.tt_iceberg.WithATickType
            WithATick                   Qty_t               false           true         
            STime                       UTCTimestamp_t      false           true
            ETime                       UTCTimestamp_t      false           true
            ETimeAct                    Int_t               false           false         tt_net_sdk.tt_iceberg.ETimeAct
            AutoResubExpiredGTD         Boolean_t           false           false
            ----------------------------------------------------------------------------------------------------------------------- */

            //Construct a dictionary of the parameters and the values to send out 
            Dictionary<string, object> iceberg_userparams = new Dictionary<string, object>
                {
                    {"DiscVal",     5 },
                    {"DiscValType", tt_net_sdk.tt_iceberg.DiscValType.Qty},
                    {"ChildTIF" ,   tt_net_sdk.TimeInForce.Day  },
                    {"ParentTIF" ,  tt_net_sdk.tt_stop.ParentTIF.Day },
                };

            OrderProfile iceberg_op = m_algo.GetOrderProfile(m_instrument);
            iceberg_op.LimitPrice = m_price;
            iceberg_op.Account = m_accounts.ElementAt(2);
            iceberg_op.Side = OrderSide.Buy;
            iceberg_op.OrderType = OrderType.Limit;
            iceberg_op.OrderQuantity = Quantity.FromDecimal(m_instrument, 10);
            iceberg_op.TimeInForce = TimeInForce.Day;

            iceberg_op.UserParameters = iceberg_userparams;
            iceberg_op.UserTag = "IcebergAlgoUsertag";
            iceberg_op.OrderTag = "IcebergAlgoOrderTag";
            m_algoTradeSubscription.SendOrder(iceberg_op);
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
                Console.WriteLine("\n PARENT Algo OrderAdded [{0}] for Algo : {1} with Synthetic Status : {2} ", e.Order.SiteOrderKey, e.Order.Algo.Alias, e.Order.SyntheticStatus.ToString());
            else
                Console.WriteLine("\nCHILD OrderAdded [{0}] {1}: {2}", e.Order.SiteOrderKey, e.Order.BuySell, e.Order.ToString());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for order update. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_algoTradeSubscription_OrderUpdated(object sender, OrderUpdatedEventArgs e)
        {
            if(e.NewOrder.ExecutionType == ExecType.Restated)
                Console.WriteLine("\n PARENT Algo Order Restated [{0}] for Algo : {1} with Synthetic Status : {2} ", e.NewOrder.SiteOrderKey, e.NewOrder.Algo.Alias, e.NewOrder.SyntheticStatus.ToString());
            else
                Console.WriteLine("\nOrderUpdated [{0}] {1}: {2}", e.NewOrder.SiteOrderKey, e.NewOrder.BuySell, e.NewOrder.ToString());
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

