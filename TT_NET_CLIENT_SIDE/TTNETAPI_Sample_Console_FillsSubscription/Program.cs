using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace TTNETAPI_Sample_Console_FillsSubscription
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                // Add your app secret Key here. It looks like: 00000000-0000-0000-0000-000000000000:00000000-0000-0000-0000-000000000000
                string appSecretKey = "Add your app secret Key here";

                // Add the host machine instance id assigned to you by TT only needed for Server mode. If set in
                // Client mode, it will simply be ignored
                string instanceId = "Add your host machine instance id here";

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
                // set the instance id and any other options needed
                apiConfig.ServerInstanceId = instanceId;

                // Start the TT API on the same thread
                TTNetApiFunctions tf = new TTNetApiFunctions();

                Thread workerThread = new Thread(() => tf.Start(apiConfig));
                workerThread.Name = "TT NET SDK Thread";
                workerThread.Start();

                while (true)
                {
                    string input = System.Console.ReadLine();
                    if (input == "q")
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