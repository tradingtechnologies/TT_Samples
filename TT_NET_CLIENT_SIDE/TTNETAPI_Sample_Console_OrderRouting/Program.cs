using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace TTNETAPI_Sample_Console_OrderRouting
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                //Add your app secret Key here . The app_key looks like : 00000000-0000-0000-0000-000000000000:00000000-0000-0000-0000-000000000000
                string appSecretKey = "110fca09-f999-00c1-d0ab-329397c69c7d:31941f4f-0d96-6798-e152-ce26cc22ef66";

                //Set the environment the app needs to run in here
                tt_net_sdk.ServiceEnvironment environment = tt_net_sdk.ServiceEnvironment.DevCert;

                tt_net_sdk.TTAPIOptions apiConfig = new tt_net_sdk.TTAPIOptions(
                        environment,
                        appSecretKey,
                        5000);

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