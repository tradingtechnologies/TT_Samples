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
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using tt_net_sdk;

namespace TTNETAPI_Sample_Console_Autospread
{
    public class TTNetApiFunctions
    {
        // Declare the API objects
        private TTAPI m_api = null;
        private InstrumentCatalogSubscription m_instCatSubscription = null;
        private tt_net_sdk.WorkerDispatcher m_disp = null;
        private object m_Lock = new object();
        private bool m_isDisposed = false;

        // Instrument Information 
        private readonly MarketId m_marketId = MarketId.CME;
        private readonly string m_product = "GE";
        private readonly string m_alias1 = "GE Jun20";
        private readonly string m_alias2 = "GE Sep20";
        private static bool subscriptionReady = false;

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

            WorkerDispatcher dispatcher = new WorkerDispatcher("InstCat");
            dispatcher.Run();

            InstrumentCatalogSubscription asSpreads = new InstrumentCatalogSubscription(Product.Autospreader,dispatcher);
            asSpreads.OnData += OnSpreadDefinitionNotification;
            asSpreads.Start();


        }

        Instrument FindInstrument(string product, string alias)
        {
            InstrumentLookup instLookup = new InstrumentLookup(tt_net_sdk.Dispatcher.Current,m_marketId, ProductType.Future,product,alias);
            ProductDataEvent einst = instLookup.Get();
            if(einst != ProductDataEvent.Found)
            {
                Console.WriteLine("ERROR: can't fetch leg instrument");
                return null;
            }
            return instLookup.Instrument;
        }

        // When a spread is created, updated or deleted.
        private void OnSpreadDefinitionNotification(object sender,InstrumentCatalogEventArgs e)
        {
            Console.WriteLine("=== OnSpreadDefinitionNotification " + e?.Event + " " + e?.Message);
            switch(e.Event)
            {
                case ProductDataEvent.Found:
                    Console.WriteLine("   Here are all the current spread definitions");
                    foreach(var sp in e.InstrumentCatalog.InstrumentList)
                    {
                        Console.WriteLine("   Spread " + sp.Name);
                    }

                    if(!subscriptionReady)
                    {       
                        subscriptionReady = true;
                        var newInst = CreateSpread();
                        var updatedInst = UpdateSpread(newInst);
                        DeleteSpread(updatedInst);
                    }
                    break;
                case ProductDataEvent.InstrumentDeleted:
                    foreach(var sp in e.Deleted)
                    {
                        Console.WriteLine("   Spread " + sp.Name + " was deleted.");
                    }
                    break;
                case ProductDataEvent.InstrumentUpdated:
                    foreach(var sp in e.Updated)
                    {   
                        Console.WriteLine("   Spread " + sp.Name + " was updated.");
                    }
                    break;
                case ProductDataEvent.InstrumentCreated:
                    foreach(var sp in e.Added)
                    {
                        Console.WriteLine("   Spread " + sp.Name + " was created.");
                    }
                    break;
            }
        }

        Instrument CreateSpread()
        {
            string aseName = "ASE.Test." + DateTime.Now.ToString("yyyyMMddhhmmss");
            Console.WriteLine("Create a new SPREAD " + aseName);
            var spreadReq = new SpreadDetails(aseName)
            {
                Color = Color.GreenYellow,
                PricingModel = PricingModel.NetChange
            };

            // first leg
            var inst1 = FindInstrument(m_product,m_alias1);
            var leg1 = new SpreadLegDetails(inst1,5,8.5M)
            {
                ActiveQuoting = true,
                IsLeanIndicative = false,
                MinLeanQty = "ThisLeg.DisclosedRemainingQuantity"
            };
            spreadReq.AppendLeg(leg1);

            // second leg
            var inst2 = FindInstrument(m_product,m_alias2);
            var leg2 = new SpreadLegDetails(inst2,-1,-2)
            {
                ActiveQuoting = true,
                IsLeanIndicative = true,
                MinLeanQty = "ThisLeg.DisclosedRemainingQuantity"
            };
            spreadReq.AppendLeg(leg2);

            // Add a spread to the spread manager and Save the ASE instrument in TT system.
            //  Parameter spreadReq: SpreadDetails object representing the spread to add
            //  Parameter rtnCode: ASReturnCodes enum indicating the status of the request
            ASReturnCodes rtnCode;
            Instrument newInst = AutospreaderManager.AddSpreadDetails(spreadReq,out rtnCode);
            System.Diagnostics.Debug.Assert(rtnCode == ASReturnCodes.Success);
            Console.WriteLine($"   New ASE instrument is created {newInst.InstrumentDetails.Name}: {newInst.InstrumentDetails.Id}/{newInst.InstrumentDetails.Version}");
            return newInst;
        }

        Instrument UpdateSpread(Instrument instrument)
        {
            // update
            ASReturnCodes rtnCode;
            Console.WriteLine("Update the new created SPREAD");
            SpreadDetails spreadReq = instrument.GetSpreadDetails();
            spreadReq.UserDefinedDenominator = 20;
            spreadReq.UserDefinedNumerator = 1;
            var legChange = spreadReq.GetLeg(1);
            legChange.PriceMultiplier = -3;
            spreadReq.Updateleg(1,legChange);

            // Update the spread to the spread manager and Save the ASE instrument in TT system.
            //  Parameter spreadReq: SpreadDetails object representing the spread to update
            //  Parameter rtnCode: ASReturnCodes enum indicating the status of the request
            var updatedInst = AutospreaderManager.UpdateSpreadDetails(spreadReq,out rtnCode);
            System.Diagnostics.Debug.Assert(rtnCode == ASReturnCodes.Success);

            return updatedInst;
        }

        void DeleteSpread(Instrument instrument)
        {
            // delete
            ASReturnCodes rtnCode;
            Console.WriteLine("Delete the SPREAD");
            SpreadDetails spread = instrument.GetSpreadDetails();

            // Delete the spread from TT system.
            //  Parameter spreadReq: SpreadDetails object representing the spread to delete
            //  Return rtnCode: ASReturnCodes enum indicating the status of the request
            rtnCode = AutospreaderManager.DeleteSpreadDetails(spread);
            System.Diagnostics.Debug.Assert(rtnCode == ASReturnCodes.Success);

        }

        public void Dispose()
        {
            lock(m_Lock)
            {
                if(!m_isDisposed)
                {
                    if(m_instCatSubscription != null)
                    {
                        m_instCatSubscription.OnData -= OnSpreadDefinitionNotification;
                        m_instCatSubscription.Dispose();
                        m_instCatSubscription = null;
                    }

                    m_isDisposed = true;
                }


                TTAPI.Shutdown();
            }
        }

        public void TTAPI_ShutdownCompleted(object sender,EventArgs e)
        {
            // Dispose of any other objects / resources
            Console.WriteLine("TTAPI shutdown completed");
        }
    }
}

