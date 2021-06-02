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

namespace TTNETAPI_Sample_Console_FillsSubscription
{
    public class TTNetApiFunctions
    {
        private object m_lock     = new object();
        private bool   m_disposed = false;

        // Declare the API objects
        private TTAPI             m_apiInstance = null;
        private WorkerDispatcher  m_disp = null;
        private FillsSubscription m_fs = null;


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
                m_apiInstance = api;
                m_apiInstance.TTAPIStatusUpdate += new EventHandler<TTAPIStatusUpdateEventArgs>(m_api_TTAPIStatusUpdate);
                m_apiInstance.Start();
            }
            else if (ex.IsRecoverable)
            {
                // this is in informational update from the SDK
                Console.WriteLine("TT.NET SDK Initialization Message: {0}", ex.Message);
                if (ex.Code == ApiCreationException.ApiCreationError.NewAPIVersionAvailable)
                {
                    // a newer version of the SDK is available - notify someone to upgrade
                }
            }
            else
            {
                Console.WriteLine("TT.NET SDK Initialization Failed: {0}", ex.Message);
                if (ex.Code == ApiCreationException.ApiCreationError.NewAPIVersionRequired)
                {
                    // do something to upgrade the SDK package since it will not start until it is upgraded 
                    // to the minimum version noted in the exception message
                }
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
            if( object.ReferenceEquals(m_fs, null) == false )
                return;

            // Status is up and we have not started a subscription yet

                // start the fill processing
            m_apiInstance.StartFillFeed();

            m_fs = new FillsSubscription(m_disp);
            m_fs.FillAdded        +=  m_fs_FillAdded;
            m_fs.FillConfirmed    +=  m_fs_FillConfirmed;
            m_fs.FillBookDownload += m_fs_FillBookDownload;
                    
            m_fs.FillListEnd   += m_fs_FillListEnd;
            m_fs.FillListStart += m_fs_FillListStart;
            m_fs.Start();
        }


        /// <summary>
        /// Event notification for fill download beginning for a given gateway
        /// </summary>
        void m_fs_FillListStart(object sender, FillListEventArgs e)
        {
            Console.WriteLine("Begin adding fills from {0}", e.FeedConnectionKey.ToString());
        }

        /// <summary>
        /// Event notification for fill download completion for a given gateway
        /// </summary>
        void m_fs_FillListEnd(object sender, FillListEventArgs e)
        {
            Console.WriteLine("Finished adding fills from {0} count={1}", e.FeedConnectionKey.ToString(), myFills.Count);
        }

        /// <summary>
        /// Event notification for fills being downloaded
        /// </summary>
        void m_fs_FillBookDownload(object sender, FillBookDownloadEventArgs e)
        {
            foreach (Fill f in e.Fills)
            {
                processFill("Fill from download:", f);
            }
        }

        /// <summary>
        /// Event notification for fills being confirmed
        /// </summary>
        void m_fs_FillConfirmed(object sender, FillConfirmedEventArgs e)
        {
            processFill("Fill Confirmed:", e.Fill);
        }

        /// <summary>
        /// Event notification for a new fill
        /// </summary>
        void m_fs_FillAdded(object sender, FillAddedEventArgs e)
        {
            processFill("Fill Added:", e.Fill);
        }

        LinkedList<tt_net_sdk.Fill> myFills = new LinkedList<Fill>();
        void processFill(string heading, Fill fill)
        {
            Console.WriteLine(heading + " FillKey={0}, InstrKey={1}, Qty={2}, MatchPrice={3}", fill.FillKey, fill.InstrumentKey, fill.Quantity, fill.MatchPrice);
        }

        /// <summary>
        /// Shuts down the TT API
        /// </summary>
        public void Dispose()
        {
            lock(m_lock)
            {
                if (!m_disposed)
                {
                    // Unattached callbacks and dispose of all subscriptions
                    if (m_fs != null)
                    {
                        m_fs.FillAdded         -= m_fs_FillAdded;
                        m_fs.FillConfirmed     -= m_fs_FillConfirmed;
                        m_fs.FillBookDownload  -= m_fs_FillBookDownload;
                        m_fs.FillListEnd       -= m_fs_FillListEnd;
                        m_fs.FillListStart     -= m_fs_FillListStart;

                        m_fs.Dispose();
                        m_fs = null;
                    }

                    // Begin shutdown the TT API
                    TTAPI.ShutdownCompleted += new EventHandler(TTAPI_ShutdownCompleted);
                    TTAPI.Shutdown();

                    m_disposed = true;
                }
            }
        }
 
        /// <summary>
        /// Event notification for completion of TT API shutdown
        /// </summary>
        public void TTAPI_ShutdownCompleted(object sender, EventArgs e)
        {
            // Shutdown the Dispatcher
            if (m_disp != null)
            {
                m_disp.BeginInvokeShutdown();
                m_disp = null;
            }

            // Dispose of any other objects / resources
        }
    }
}
