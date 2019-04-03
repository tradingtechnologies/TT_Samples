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
using System.Reflection;
using tt_net_sdk;

namespace TTAPI_Sample_Console_PriceDepthSubscription
{
    public class TTNetApiFunctions
    {
        // Declare the API objects
        private TTAPI m_api = null;
        private InstrumentLookup m_instrLookupRequest = null;
        private PriceSubscription m_priceSubsciption = null;
        private tt_net_sdk.WorkerDispatcher m_disp = null;

        // Other private member variables
        private int m_ps_counter = 0;

        // Instrument Information 
        private readonly string m_market = "CME";
        private readonly string m_product = "GE";
        private readonly string m_prodType = "Future";
        private readonly string m_alias = "GE Sep19";


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>  Attach the worker Dispatcher </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void Start(tt_net_sdk.TTAPIOptions apiConfig)
        {
            m_disp = tt_net_sdk.Dispatcher.AttachWorkerDispatcher();
            m_disp.DispatchAction( ()=>
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
                Console.WriteLine("TT.NET SDK Initialization Complete");

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
            if( object.ReferenceEquals(m_instrLookupRequest, null) == false )
                return;

            // Status is up and we have not started a subscription yet

            // connection to TT is established
            Console.WriteLine("TT.NET SDK Authenticated");

            MarketId  marketKey   = Market.GetMarketIdFromName( m_market );
            ProductType productType = Product.GetProductTypeFromName(m_prodType);

            // lookup an instrument
            m_instrLookupRequest = new InstrumentLookup(tt_net_sdk.Dispatcher.Current,
                        marketKey, productType, m_product, m_alias);

            m_instrLookupRequest.OnData += m_instrLookupRequest_OnData;
            m_instrLookupRequest.GetAsync();
      
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
                Instrument  instrument  = e.InstrumentLookup.Instrument;
                Console.WriteLine("Found: {0}", instrument);

                // Subscribe for market Data
                m_priceSubsciption = new PriceSubscription(instrument, tt_net_sdk.Dispatcher.Current);
                m_priceSubsciption.Settings  = new PriceSubscriptionSettings(PriceSubscriptionType.MarketDepth);
                m_priceSubsciption.FieldsUpdated += m_priceSubscription_FieldsUpdated;
                m_priceSubsciption.Start();
            }
            else if (e.Event == ProductDataEvent.NotAllowed)
            {
                Console.WriteLine("Not Allowed : Please check your Token access");
            }
            else
            {
                // Instrument was not found and TT API has given up looking for it
                Console.WriteLine("Cannot find instrument: {0}", e.Message);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for price update. </summary>
        /// <param name="sender">   Source of the event. </param>
        /// <param name="e">        Fields updated event information. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        void m_priceSubscription_FieldsUpdated(object sender, FieldsUpdatedEventArgs e)
        {
            Console.WriteLine("\n================= {0}", m_ps_counter++);
            if (e.Error != null)
            {
                // Error has occured - the subscription is no longer active

                tt_net_sdk.PriceSubscription ps = (tt_net_sdk.PriceSubscription)sender;
                Console.WriteLine("Unrecoverable price subscription error: {0}", e.Error.Message);
                ps.Dispose();
                return;
            }

            if (e.UpdateType == UpdateType.Snapshot)
            {
                // Received a market data snapshot

                // TODO: initialize your data here.
                //       the snap shot event can come multiple times
                Console.WriteLine("\nSnapshot Update");

                // You can now apply the changed/valid value fields.
            }
            else
            {
                Console.WriteLine("\nIncremental Update");
            }
            Console.WriteLine("\nTop and level 0 field(s):");
            foreach (FieldId f in e.Fields.GetChangedFieldIds())
                    Console.WriteLine("    {0} : {1}", f.ToString(), e.Fields[f].FormattedValue);

            Console.WriteLine("\nDepth field(s):");
            int depthLevels = e.Fields.GetMaxDepthLevel();
            for (int i = 0; i < depthLevels; i++)
            {
                if (e.Fields.GetChangedFieldIds(i).Length > 0)
                {
                    Console.WriteLine("Level={0}", i);
                    foreach (FieldId id in e.Fields.GetChangedFieldIds(i))
                    {
                        Console.WriteLine("    {0}: {1}", id.ToString(), e.Fields[id, i].FormattedValue);
                    }
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Shuts down the TT API and disposes the subscriptions </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void Dispose()
        {
            if (object.ReferenceEquals(m_instrLookupRequest, null) == false)
                m_instrLookupRequest.Dispose();

            if ( object.ReferenceEquals(m_priceSubsciption, null) == false )
                m_priceSubsciption.Dispose();
            
            TTAPI.Shutdown();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for completion of TT API shutdown. </summary>
        /// <param name="sender">   Source of the event. </param>
        /// <param name="e">        Event information. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void TTAPI_ShutdownCompleted(object sender, EventArgs e)
        {
            Console.WriteLine("TTAPI Shutdown completed");
        }
    }

}
