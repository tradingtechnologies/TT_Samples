using System;
using System.IO;
using System.Threading;
using System.Collections.Generic;
using System.Linq;
using tt_net_sdk;

namespace TTNETAPI_Sample_Console_TTUncovered_MarketCreation
{
    public class TTNetApiFunctions
    {
        private WorkerDispatcher dispatcher = null;
        private readonly object lockObject = new();
        private TTAPI api = null;
        InstrumentCatalogSubscription instrumentCatalogSubscription = null;
        private readonly List<PriceSubscription> priceSubscriptions = new();
        private readonly Dictionary<UInt64, InstrumentTradeSubscription> tradeSubscriptions = new();
        private readonly ManualResetEvent manualResetPriceSubscriptionEvent = new(false);
        private int ps_counter;
        private readonly string market;
        private readonly string product;
        List<string> syntheticOrders = new();
        private readonly Dictionary<Instrument, Price> instrumentBidPrices = new();
        private readonly Dictionary<Instrument, Price> instrumentAskPrices = new();
        private readonly List<Instrument> allAvailableInstruments = new();
        Int32 numberOfPriceSubscription = 0;
        private FileSystemWatcher watcher = null;

        public struct Instruments 
        {
            public Instrument option;
            public Instrument hedge;
            public List<Instrument> uds;
        };

        public TTNetApiFunctions()
        {
            ReadSyntheticOrders();
            WatchSpreadDefinitionFile();

            Console.WriteLine("Market: CME");
            market = "CME";
            Console.WriteLine("Enter product name(e.g.GE):");
            product = Console.ReadLine();
        }
 
        private void ReadSyntheticOrders()
        {
            syntheticOrders.Clear();
            string cacheFile = GetCacheFilePath();

            if (System.IO.File.Exists(cacheFile))
            {
                var fileStream = System.IO.File.OpenText(cacheFile);
                string persisted = fileStream.ReadLine();
                while (!string.IsNullOrEmpty(persisted))
                {
                    syntheticOrders.Add(persisted);
                    persisted = fileStream.ReadLine();
                }

                fileStream.Close();
            }
        }

        private static string GetCacheFilePath()
        {
            return System.Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) +
                           "\\TradingTechnologies\\logs\\TTUncovered\\option_spread_definitions.txt";
        }


        private void WatchSpreadDefinitionFile()
        {
            // Create a new FileSystemWatcher and set its properties.
            watcher = new()
            {
                Path = Path.GetDirectoryName(GetCacheFilePath()),
                NotifyFilter = NotifyFilters.LastWrite,
                Filter = "option_spread_definitions.txt"
            };

            // Add event handlers.
            watcher.Changed += new FileSystemEventHandler(OnFileChanged);
            //watcher.Created += new FileSystemEventHandler(OnFileChanged);

            // Begin watching.
            watcher.EnableRaisingEvents = true;
        }

        private void OnFileChanged(object sender, FileSystemEventArgs e)
        {
            if (e.ChangeType != WatcherChangeTypes.Changed)
            {
                return;
            }

            dispatcher.DispatchAction(() =>
            {
                ReadSyntheticOrders();
                HandleInstrumentUpdate(allAvailableInstruments);
            });
           
            //Console.WriteLine($"Changed: {e.FullPath}");
        }

        public void Start(TTAPIOptions config)
        {
            dispatcher = Dispatcher.AttachWorkerDispatcher();
            dispatcher.DispatchAction(() => { Init(config); });
            dispatcher.Run();
        }
        private void Init(TTAPIOptions apiOptions)
        {
            Console.WriteLine("Intializing TT SDK...");
            ApiInitializeHandler initializeHandler = new ApiInitializeHandler(InitializationHandler);
            TTAPI.ShutdownCompleted += OnShutdownCompleted;
            TTAPI.CreateTTAPI(Dispatcher.Current, apiOptions, initializeHandler);
        }
        public void InitializationHandler(TTAPI ttAPI, ApiCreationException ex)
        {
            if (ex == null)
            {
                Console.WriteLine("TT.NET SDK INITIALIZED");
                api = ttAPI;
                api.TTAPIStatusUpdate += new EventHandler<TTAPIStatusUpdateEventArgs>(OnStatusUpdate);
                api.Start();
            }
            else if (ex.IsRecoverable)
            {
                Console.WriteLine("TT.NET SDK Initialization Message: {0}", ex.Message);
            }
            else
            {
                Console.WriteLine("TT.NET SDK Initialization Failed: {0}", ex.Message);
                Dispose();
            }
        }

        public void OnShutdownCompleted(object sender, EventArgs e)
        {
            Console.WriteLine("TTAPI shutdown completed");
        }

        public void Dispose()
        {
            lock (lockObject)
            {
                TTAPI.Shutdown();
            }
        }

        public void OnStatusUpdate(object sender, TTAPIStatusUpdateEventArgs e)
        {
            Console.WriteLine("TTAPIStatusUpdate: {0}", e);
            if (e.IsReady == false)
            {
                // TODO: Do any connection lost processing here
                return;
            }
            else
            {
                Console.WriteLine("TT Login for user " + api.UserName + " Succeeded: {0}", e);
            }

            if (object.ReferenceEquals(instrumentCatalogSubscription, null) == false)
            {
                return;
            }

            string prodType = "OptionStrategy";
            
            MarketId marketId = Market.GetMarketIdFromName(market);
            ProductType productType = Product.GetProductTypeFromName(prodType);
            Console.WriteLine("Market ID: " + marketId.ToString());
            Console.WriteLine("Product Type: " + productType.ToString());

            instrumentCatalogSubscription = new InstrumentCatalogSubscription(marketId, productType, product, Dispatcher.Current);
            instrumentCatalogSubscription.OnData += InstrumentsUpdated;
            instrumentCatalogSubscription.Start();

            // Get the accounts
            //accounts = api.Accounts;
        }

        private Dictionary<string, Instruments> GetExistingInstruments(IEnumerable<Instrument> allInstruments)
        {
            var myDictionary = new Dictionary<string, Instruments>();
            //Dictionary<string, List<Instrument>> instruments = new();

            foreach (var v in syntheticOrders)
            {
                foreach (var instrument in allInstruments)
                {
                    LegList legs = instrument.GetLegs();
                    if (legs.Count == 2)
                    {
                        Leg optionLeg = legs.ToList().Find(l => l.Instrument.Product.Type == ProductType.Option);
                        Leg hedgeLeg = legs.ToList().Find(l => l.Instrument.Product.Type == ProductType.Future);
                        if (optionLeg == null || hedgeLeg == null)
                        {
                            continue;
                        }

                        string[] tags = v.Split(',');
                        UInt64 optionInstrumentId = Convert.ToUInt64(tags[1]);
                        UInt64 hedgeInstrumentId = Convert.ToUInt64(tags[2]);

                        if (optionLeg.Instrument.InstrumentDetails.Id == optionInstrumentId &&
                            hedgeLeg.Instrument.InstrumentDetails.Id == hedgeInstrumentId)
                        {
                            string key = tags[1] + "," + tags[2];

                            if (!myDictionary.ContainsKey(key))
                            {
                                Instruments instr = new();
                                instr.uds = new();
                                instr.option = optionLeg.Instrument;
                                instr.hedge = hedgeLeg.Instrument;
                                myDictionary.Add(key, instr);
                            }
                                
                            myDictionary[key].uds.Add(instrument);

                        }
                    }
                }
            }

            return myDictionary;
        }

        private void InstrumentsUpdated(object sender, InstrumentCatalogEventArgs e)
        {
            if (e.Event == ProductDataEvent.Found)
            {
                allAvailableInstruments.AddRange(e.Added);
                HandleInstrumentUpdate(allAvailableInstruments);
            }
            else if (e.Event == ProductDataEvent.InstrumentCreated)
            {
                ReadSyntheticOrders();
                allAvailableInstruments.AddRange(e.Added);
                HandleInstrumentUpdate(allAvailableInstruments);
            }
        }

        private void HandleInstrumentUpdate(IEnumerable<Instrument> allInstruments)
        {
            Console.WriteLine("Updating instruments...");
            Dictionary<string, Instruments> instrumentsFound = GetExistingInstruments(allInstruments);
            instrumentAskPrices.Clear();
            instrumentBidPrices.Clear();

            foreach (var dictElement in instrumentsFound)
            {
                Instrument option = dictElement.Value.option;
                Instrument hedge = dictElement.Value.hedge;

                if (!instrumentBidPrices.ContainsKey(option))
                {
                    instrumentBidPrices.Add(option, new());
                }

                if (!instrumentAskPrices.ContainsKey(option))
                {
                    instrumentAskPrices.Add(option, new());
                }

                if (!instrumentBidPrices.ContainsKey(hedge))
                {
                    instrumentBidPrices.Add(hedge, new());
                }

                if (!instrumentAskPrices.ContainsKey(hedge))
                {
                    instrumentAskPrices.Add(hedge, new());
                }


                CreatePriceSubscription(option);
                CreatePriceSubscription(hedge);

                CreateTradeSubscription(option);
                CreateTradeSubscription(hedge);


                foreach (var uds in dictElement.Value.uds)
                {
                    if (!instrumentBidPrices.ContainsKey(uds))
                    {
                        instrumentBidPrices.Add(uds, new());
                    }

                    if (!instrumentAskPrices.ContainsKey(uds))
                    {
                        instrumentAskPrices.Add(uds, new());
                    }

                    CreatePriceSubscription(uds);
                    CreateTradeSubscription(uds);
                }

                numberOfPriceSubscription = 2 + dictElement.Value.uds.Count;
            }

            Console.WriteLine("Instruments Updated");

            if (instrumentsFound.Count == 0)
            {
                Console.WriteLine("\n\nCould not find strategies, press q to exit or create strategies...");
            }
            else
            {
                PrintUserChoice();
            }
        }

        private void CreatePriceSubscription(Instrument instrument)
        {
            var priceSub = new PriceSubscription(instrument, Dispatcher.Current);
            priceSub.Settings = new PriceSubscriptionSettings(PriceSubscriptionType.MarketDepth);
            priceSub.FieldsUpdated += OnPriceFieldsUpdated;
            priceSub.Start();
            manualResetPriceSubscriptionEvent.Set();
            priceSubscriptions.Add(priceSub);
        }

        private void CreateTradeSubscription(Instrument instrument)
        {
            var tradeSub = new InstrumentTradeSubscription(tt_net_sdk.Dispatcher.Current, instrument);
            //tradeSub.OrderAdded += new EventHandler<OrderAddedEventArgs>(OnOrderAdded);
            tradeSub.Start();
            tradeSubscriptions.Add(instrument.InstrumentDetails.Id, tradeSub);
        }

        void OnPriceFieldsUpdated(object sender, FieldsUpdatedEventArgs e)
        {
            PriceSubscription ps = (PriceSubscription)sender;

            //Console.WriteLine("The current thread: " + Thread.CurrentThread.Name);
            //Console.WriteLine("\n[{0}] ================= {1}", ps_counter++, ps.Fields.Instrument.InstrumentDetails.Alias);
            ps_counter++;
            if (e.Error == null)
            {
                if (e.UpdateType == UpdateType.Snapshot)
                {
                    //Console.WriteLine("\nSnapshot Updates");
                    //foreach (FieldId f in e.Fields.GetChangedFieldIds())
                    //{
                    //    Console.WriteLine("    {0} : {1}", f.ToString(), e.Fields[f].FormattedValue);
                    //}

                    PriceField lastTradedPrice = e.Fields.GetLastTradedPriceField();
                    //Console.WriteLine("Last traded price: {0}", lastTradedPrice.FormattedValue);

                    PriceField bestAskPrice = e.Fields.GetBestAskPriceField();
                    //Console.WriteLine("Best Ask price: {0}", bestAskPrice.FormattedValue);

                    PriceField bestBidPrice = e.Fields.GetBestBidPriceField();
                    //Console.WriteLine("Best Bid price: {0}", bestBidPrice.FormattedValue);

                    //Console.WriteLine("Best XXX price: {0}", e.Fields.GetOpenPriceField().FormattedValue);

                    PriceField settlementPriceField = e.Fields.GetSettlementPriceField();
                    Price settlementBidPrice = settlementPriceField.Value;
                    Price settlementAskPrice = settlementPriceField.Value;
                    //Console.WriteLine("settlementPrice: {0}", settlementBidPrice);
                                        
                    if (bestBidPrice.HasValidValue)
                    {
                        settlementBidPrice = bestBidPrice.Value;
                    }
                    else if (lastTradedPrice.HasValidValue)
                    {
                        settlementBidPrice = lastTradedPrice.Value;
                    }
                    
                    if (bestAskPrice.HasValidValue)
                    {
                        settlementAskPrice = bestAskPrice.Value;
                    }
                    else if (lastTradedPrice.HasValidValue)
                    {
                        settlementAskPrice = lastTradedPrice.Value.GetTickPrice(1);
                    }


                    Price bidPrice = new();
                    Price askPrice = new();

                    if (settlementBidPrice.IsValid)
                    {
                        bidPrice = settlementBidPrice;
                        //settlementBidPrice.Offset(1, Rounding.Nearest);
                        //for (int i = 1; i < numberOfPrices; ++i)
                        //{
                        //    Price bidPrice = settlementBidPrice.GetTickPrice(i * ladderInterval);
                        //    bidPrices.Add(bidPrice);
                        //}
                    }
              
                    if (settlementAskPrice.IsValid)
                    {
                        askPrice = settlementAskPrice;
                        if (askPrice == bidPrice)
                        {
                            askPrice = askPrice.GetTickPrice(1);
                        }
                        //for (int i = 1; i < numberOfPrices; ++i)
                        //{
                        //    Price askPrice = settlementAskPrice.GetTickPrice(i * ladderInterval);
                        //    askPrices.Add(askPrice);
                        //}
                    }

                    if (e.Fields.Instrument.Product.Type == ProductType.OptionStrategy && (!bidPrice.IsValid || !askPrice.IsValid))
                    {
                        LegList legs = e.Fields.Instrument.GetLegs();
                        if (legs.Count == 2)
                        {
                            Instrument optionLeg = null;

                            foreach (var leg in legs)
                            {
                                if (leg.Instrument.Product.Type == ProductType.Option)
                                {
                                    optionLeg = leg.Instrument;
                                    break;
                                }
                            }

                            if (optionLeg != null)
                            {
                                askPrice = instrumentAskPrices[optionLeg];
                                bidPrice = instrumentBidPrices[optionLeg];
                            }
                        }
                    }
                    //else if (instrumentAskPrices.ContainsKey(e.Fields.Instrument))
                    //{
                    //    askPrices = instrumentAskPrices[e.Fields.Instrument];
                    //    bidPrices = instrumentBidPrices[e.Fields.Instrument];
                    //}

                    if (askPrice.IsValid)
                    {
                        if (instrumentAskPrices.ContainsKey(e.Fields.Instrument))
                        {
                            instrumentAskPrices[e.Fields.Instrument] = askPrice;
                        }

                        //Console.WriteLine("Ask Prices: ");
                        //foreach (var price in askPrices)
                        //{
                        //    Console.WriteLine("\t{0}", price);
                        //}
                    }
                    else
                    {
                        Console.WriteLine("{0} : Can't set ask price, please set manually.", e.Fields.Instrument);
                    }

                    if (bidPrice.IsValid)
                    {
                        if (instrumentBidPrices.ContainsKey(e.Fields.Instrument))
                        {
                            instrumentBidPrices[e.Fields.Instrument] = bidPrice;
                        }

                        //Console.WriteLine("Bid Prices: ");
                        //foreach (var price in bidPrices)
                        //{
                        //    Console.WriteLine("\t{0}", price);
                        //}
                    }
                    else
                    {
                        Console.WriteLine("{0} : Can't set bid price, please set manually.", e.Fields.Instrument);
                    }

                    //foreach (var price in askPrices)
                    //{
                    //    SendOrder(ps.Fields.Instrument, price, BuySell.Sell);
                    //    break;
                    //}

                    //foreach (var price in bidPrices)
                    //{
                    //    SendOrder(ps.Fields.Instrument, price, BuySell.Buy);
                    //    break;
                    //}
                }
                
                if (ps_counter >= numberOfPriceSubscription)
                {
                    ps_counter = 0;
                    numberOfPriceSubscription = 0;
                }
            }
            else
            {
                Console.WriteLine(">>> Price subscription error: {0}", e.Error.Message);
                if (e.Error != null)
                {
                    Console.WriteLine("Unrecoverable price subscription error: {0}", e.Error.Message);
                    ps.Dispose();
                    ps = null;
                }
            }
        }

        private void SendOrder(Instrument instrument, Price price, BuySell side)
        {
            OrderProfile op = new OrderProfile(instrument)
            {
                BuySell = side,

                Account = api.Accounts.ElementAt(0),
                OrderQuantity = Quantity.FromDecimal(instrument, 100),
                OrderType = OrderType.Limit,
                LimitPrice = price
            };

            if (tradeSubscriptions.TryGetValue(instrument.InstrumentDetails.Id, out InstrumentTradeSubscription tradeSub))
            {
                if (!tradeSub.SendOrder(op))
                {
                    Console.WriteLine("Send new order Failed.!!");
                    //TTAPI.Shutdown();
                }
                else
                {
                    Console.WriteLine("\nSent new order: " + instrument + " " + op.BuySell + " " + op.OrderQuantity.ToString() + "@" + op.LimitPrice.ToString() + " SOK=" + op.SiteOrderKey);
                }
            }
            else
            {
                Console.WriteLine("Please set market manually for {0}", instrument);
            }
        }

        public void SendOrders(ProductType productType = ProductType.NotSet)
        {
            foreach (var instrumentElement in instrumentAskPrices)
            {
                if (productType == ProductType.NotSet || instrumentElement.Key.Product.Type == productType)
                {
                    Instrument instrument = instrumentElement.Key;
                    Price askPrice = instrumentElement.Value;
                    SendOrder(instrument, askPrice, BuySell.Sell);
                }
            }

            foreach (var instrumentElement in instrumentBidPrices)
            {
                if (productType == ProductType.NotSet || instrumentElement.Key.Product.Type == productType)
                {
                    Instrument instrument = instrumentElement.Key;
                    Price bidPrice = instrumentElement.Value;
                    SendOrder(instrument, bidPrice, BuySell.Buy);
                }
            }
        }

        public void PrintUserChoice()
        {
            Console.WriteLine("\n\n\nPress one of the following key.");
            Console.WriteLine("\t'a'. To send all orders");
            Console.WriteLine("\t'f'. To send future orders");
            Console.WriteLine("\t'o'. To send option orders");
            Console.WriteLine("\t's'. To send option strategy orders");
            Console.WriteLine("\t'd'. To display all instruments names and price");
            Console.WriteLine("\t'q'. To Quit");
        }

        public void ShowInstruments()
        {
            int legCounter = 0;
            foreach (var instrumentElement in instrumentAskPrices)
            {
                Instrument instrument = instrumentElement.Key;
                Price askPrice = instrumentElement.Value;
                Price bidPrice = instrumentBidPrices[instrument];

                Console.WriteLine("[{0}] [{1}] {2}, bid: {3} ask: {4}", 
                    legCounter++, instrument.Product.Type, instrument, bidPrice, askPrice); ;
            }

            if (instrumentAskPrices.Count == 0)
            {
                Console.WriteLine("No instrument");
            }
        }

    }

   

    class Program
    {
        static string GetDescription<EnumType>(EnumType value)
        {
            var type = typeof(EnumType);
            var memInfo = type.GetMember(value.ToString());
            var attributes = memInfo[0].GetCustomAttributes(typeof(System.ComponentModel.DescriptionAttribute), false);
            var description = ((System.ComponentModel.DescriptionAttribute)attributes[0]).Description;
            return description;
        }
        static void Main(string[] args)
        {
            try
            {
                //string appSecretKey = "f8710ecf-f581-9bbb-ff33-e71f294f09d4:aec599a1-c784-585f-af28-318047ced955";
                string appSecretKey = "834ac713-e67f-49b0-845f-a22c6f61d0c4:22b22302-edf8-365f-2541-aebc28715def";
                ServiceEnvironment environment = ServiceEnvironment.DevCert;
                TTAPIOptions.SDKMode sdkMode = TTAPIOptions.SDKMode.Client;

                Console.Write("Enter environment[options: ");

                Dictionary<string, ServiceEnvironment> environmentsMap = new();
                foreach (ServiceEnvironment environment1 in (ServiceEnvironment[])Enum.GetValues(typeof(ServiceEnvironment)))
                {
                    if (environment1 == ServiceEnvironment.NotSet) continue;

                    string name = GetDescription<ServiceEnvironment>(environment1);
                    environmentsMap.Add(name, environment1);

                    Console.Write(name);
                    if (environment1 != ServiceEnvironment.DevPerf)
                    {
                        Console.Write(", ");
                    }
                    else
                    {
                        Console.WriteLine("]");
                    }

                }

                string env = Console.ReadLine();

                if (environmentsMap.ContainsKey(env))
                {
                    Console.WriteLine("Enter secret key for {0}", env);
                    appSecretKey = Console.ReadLine();

                    environment = environmentsMap[env];
                }
                else
                {
                    Console.WriteLine("Invalid environment");
                    return;
                }

                TTAPIOptions apiConfig = new TTAPIOptions(
                        sdkMode,
                        environment,
                        appSecretKey,
                        5000);

                // Start the TT API on the same thread
                TTNetApiFunctions apiFunctions = new TTNetApiFunctions();

                Thread workerThread = new Thread(() => apiFunctions.Start(apiConfig));
                workerThread.Name = "TT NET SDK Thread";
                workerThread.Start();

                while (true)
                {
                    string input = System.Console.ReadLine();
                    if (input == "q")
                    {
                        break;
                    }
                    else if (input == "a")
                    {
                        apiFunctions.SendOrders();
                    }
                    else if (input == "f")
                    {
                        apiFunctions.SendOrders(ProductType.Future);
                    }
                    else if (input =="o")
                    {
                        apiFunctions.SendOrders(ProductType.Option);
                    }
                    else if (input =="s")
                    {
                        apiFunctions.SendOrders(ProductType.OptionStrategy);
                    }
                    else if (input == "d")
                    {
                        apiFunctions.ShowInstruments();
                    }

                    apiFunctions.PrintUserChoice();
                }

                apiFunctions.Dispose();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message + "\n" + e.StackTrace);
            }
        }
    }
}
