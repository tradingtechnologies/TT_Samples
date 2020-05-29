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
using System.Threading;
using System.Threading.Tasks;


namespace TTAPI_Sample_Console_PriceDepthSubscription
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                // Add your app secret Key here . The app_key looks like : 00000000-0000-0000-0000-000000000000:00000000-0000-0000-0000-000000000000
                string appSecretKey = "Add your app secret key here";

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

                // set any other SDK options you need configured
                apiConfig.ProfitLossCalculationType = tt_net_sdk.ProfitLossCalculationType.RiskWaterfall;

                // Start the TT API on the same thread
                TTNetApiFunctions tf = new TTNetApiFunctions();

                Thread workerThread = new Thread( ()=>tf.Start(apiConfig) );
                workerThread.Name = "TT NET SDK Thread";
                workerThread.Start();

                while (true)
                {
                    string input  = System.Console.ReadLine();
                    if( input == "q" )
                        break;
                }
                tf.Dispose();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message + "\n" + e.StackTrace);
            }

        }
    }
}
