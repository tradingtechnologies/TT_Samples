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
using System.Windows.Forms;

using tt_net_sdk;

namespace TTNETAPI_Sample_GUI_Market_Explorer
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            using (Dispatcher disp = Dispatcher.AttachUIDispatcher())
            {
                Application.EnableVisualStyles();
                
                // Create an instance of the API
                MarketExplorer marketExplorer = new MarketExplorer();

                // Add your app secret Key here. It looks like: 00000000-0000-0000-0000-000000000000:00000000-0000-0000-0000-000000000000
                string appSecretKey = "Add your app secret Key here";

                // Set the environment the app needs to run in here
                tt_net_sdk.ServiceEnvironment environment = tt_net_sdk.ServiceEnvironment.UatCert;
                // Select the mode in which you wish to run -- Client (outside the TT datacenter)  
                //                                          or Server (on a dedicated machine inside TT datacenter)
                tt_net_sdk.TTAPIOptions.SDKMode sdkMode = tt_net_sdk.TTAPIOptions.SDKMode.Client;
                tt_net_sdk.TTAPIOptions apiConfig = new tt_net_sdk.TTAPIOptions(
                        sdkMode,
                        environment,
                        appSecretKey,
                        5000);

                ApiInitializeHandler handler = new ApiInitializeHandler(marketExplorer.ttNetApiInitHandler);
                TTAPI.CreateTTAPI(disp, apiConfig, handler);

                Application.Run(marketExplorer);
            }
        }

    }
}
