using System;
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
        private readonly Dictionary<Instrument, List<Price>> instrumentBidPrices = new();
        private readonly Dictionary<Instrument, List<Price>> instrumentAskPrices = new();
        private readonly List<UInt64> optionInstruments = new();

        public struct Instruments 
        {
            public Instrument option;
            public Instrument hedge;
            public List<Instrument> uds;
        };

        public TTNetApiFunctions()
        {
            ReadSyntheticOrders();

            Console.WriteLine("Enter market name(e.g.CME):");
            market = Console.ReadLine();
            Console.WriteLine("Enter product name(e.g.GE):");
            product = Console.ReadLine();
        }

        private void ReadSyntheticOrders()
        {
            string cacheFile = System.Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) +
                           "\\TradingTechnologies\\logs\\TTUncovered\\option_spread_definitions.txt";

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

        public void Start(TTAPIOptions config)
        {
            dispatcher = Dispatcher.AttachWorkerDispatcher();
            dispatcher.DispatchAction(() => { Init(config); });
            dispatcher.Run();
        }
        private void Init(TTAPIOptions apiOptions)
        {
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
            instrumentCatalogSubscription.OnData += instrumentsUpdated;
            instrumentCatalogSubscription.Start();

            // Get the accounts
            //accounts = api.Accounts;
        }

        private Dictionary<string, Instruments> GetExistingInstruments(IEnumerable<Instrument> allInstruments)
        {
            var myDictionary = new Dictionary<string, Instruments>();
            foreach (var v in syntheticOrders)
            {
                string[] tags = v.Split(',');
                UInt64 optionInstrumentId = Convert.ToUInt64(tags[1]);
                UInt64 hedgeInstrumentId = Convert.ToUInt64(tags[2]);

                foreach (var instrument in allInstruments)
                {
                    LegList legs = instrument.GetLegs();
                    if (legs.Count == 2)
                    {
                        var firstInstrumentId = legs[0].Instrument.InstrumentDetails.Id;
                        var secondInstrumentId = legs[1].Instrument.InstrumentDetails.Id;
                        string key = tags[1] + "," + tags[2];

                        if (optionInstrumentId == firstInstrumentId && hedgeInstrumentId == secondInstrumentId)
                        {
                            if (myDictionary.ContainsKey(key))
                            {
                                myDictionary[key].uds.Add(instrument);
                            }
                            else
                            {
                                Instruments instr = new();
                                instr.uds = new();
                                instr.option = legs[0].Instrument;
                                instr.hedge = legs[1].Instrument;
                                instr.uds.Add(instrument);
                                myDictionary.Add(key, instr);
                            }
                        }
                        else if (optionInstrumentId == secondInstrumentId && hedgeInstrumentId == firstInstrumentId)
                        {
                            if (myDictionary.ContainsKey(key))
                            {
                                myDictionary[key].uds.Add(instrument);
                            }
                            else
                            {
                                Instruments instr = new();
                                instr.uds = new();
                                instr.option = legs[0].Instrument;
                                instr.hedge = legs[1].Instrument;
                                instr.uds.Add(instrument);
                                myDictionary.Add(key, instr);
                            }
                        }
                    }
                }
            }

            return myDictionary;
        }

        private void instrumentsUpdated(object sender, InstrumentCatalogEventArgs e)
        {
            if (e.Event == ProductDataEvent.Found)
            {
                HandleInstrumentUpdate(e.Added);
            }
            else if (e.Event == ProductDataEvent.InstrumentCreated)
            {
                ReadSyntheticOrders();
                HandleInstrumentUpdate(e.Added);
            }
        }

        private void HandleInstrumentUpdate(IEnumerable<Instrument> allInstruments)
        {
            Dictionary<string, Instruments> instrumentsFound = GetExistingInstruments(allInstruments);

            foreach (var dictElement in instrumentsFound)
            {
                Instrument option = dictElement.Value.option;
                Instrument hedge = dictElement.Value.hedge;
                optionInstruments.Add(option.InstrumentDetails.Id);

                instrumentBidPrices.Add(key: option, new());
                instrumentAskPrices.Add(key: option, new());

                instrumentBidPrices.Add(hedge, new());
                instrumentAskPrices.Add(hedge, new());

                CreatePriceSubscription(option);
                CreatePriceSubscription(hedge);

                CreateTradeSubscription(option);
                CreateTradeSubscription(hedge);


                foreach (var uds in dictElement.Value.uds)
                {
                    //Console.WriteLine($"PriceSubscription started for {uds} | {option} | {hedge}");
                    instrumentBidPrices.Add(uds, new());
                    instrumentAskPrices.Add(uds, new());
                    CreatePriceSubscription(uds);
                    CreateTradeSubscription(uds);
                }
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
            Console.WriteLine("\n[{0}] ================= {1}", ps_counter++, ps.Fields.Instrument.InstrumentDetails.Alias);

            if (e.Error == null)
            {
                if (e.UpdateType == UpdateType.Snapshot)
                {
                    //trade_subscription();  // start InstrumentTradeSubscription for m_instr

                    //m_ps_counter = 0;
                    Console.WriteLine("\nSnapshot Updates");
                    foreach (FieldId f in e.Fields.GetChangedFieldIds())
                    {
                        Console.WriteLine("    {0} : {1}", f.ToString(), e.Fields[f].FormattedValue);
                    }

                    PriceField lastTradedPrice = e.Fields.GetLastTradedPriceField();
                    Console.WriteLine("Last traded price: {0}", lastTradedPrice.FormattedValue);

                    PriceField bestAskPrice = e.Fields.GetBestAskPriceField();
                    Console.WriteLine("Best Ask price: {0}", bestAskPrice.FormattedValue);

                    PriceField bestBidPrice = e.Fields.GetBestBidPriceField();
                    Console.WriteLine("Best Bid price: {0}", bestBidPrice.FormattedValue);

                    Console.WriteLine("Best XXX price: {0}", e.Fields.GetOpenPriceField().FormattedValue);

                    PriceField settlementPriceField = e.Fields.GetSettlementPriceField();
                    Price settlementBidPrice = settlementPriceField.Value;
                    Price settlementAskPrice = settlementPriceField.Value;
                    Console.WriteLine("settlementPrice: {0}", settlementBidPrice);
                                        
                    if (bestBidPrice.HasValidValue)
                    {
                        settlementBidPrice = bestBidPrice.Value;
                    }

                    if (bestAskPrice.HasValidValue)
                    {
                        settlementAskPrice = bestAskPrice.Value;
                    }

                    int ladderInterval = 3;
                    int numberOfPrices = 4;

                    List<Price> bidPrices = new();
                    List<Price> askPrices = new();

                    if (settlementBidPrice.IsValid)
                    {
                        for (int i = 1; i < numberOfPrices; ++i)
                        {
                            Price bidPrice = settlementBidPrice.GetTickPrice(i * ladderInterval);
                            bidPrices.Add(bidPrice);

                            Price askPrice = settlementBidPrice.GetTickPrice((i + numberOfPrices) * ladderInterval);
                            askPrices.Add(askPrice);
                        }
                    }
                    else if (instrumentAskPrices.ContainsKey(e.Fields.Instrument) && instrumentAskPrices[e.Fields.Instrument].Count == 0)
                    {
                        LegList legs = e.Fields.Instrument.GetLegs();
                        if (legs.Count == 2)
                        {
                            Instrument optionLeg = null;
                            if (optionInstruments.Contains(legs[0].Instrument.InstrumentDetails.Id))
                            {
                                optionLeg = legs[0].Instrument;
                            }
                            else if (optionInstruments.Contains(legs[1].Instrument.InstrumentDetails.Id))
                            {
                                optionLeg = legs[1].Instrument;
                            }

                            if (optionLeg != null)
                            {
                                askPrices = instrumentAskPrices[optionLeg];
                                bidPrices = instrumentBidPrices[optionLeg];
                            }
                        }
                    }
                    else if (instrumentAskPrices.ContainsKey(e.Fields.Instrument))
                    {
                        askPrices = instrumentAskPrices[e.Fields.Instrument];
                        bidPrices = instrumentBidPrices[e.Fields.Instrument];
                    }

                    if (askPrices.Count > 0)
                    {
                        if (instrumentAskPrices.ContainsKey(e.Fields.Instrument))
                        {
                            instrumentAskPrices[e.Fields.Instrument] = askPrices;
                        }

                        Console.WriteLine("Ask Prices: ");
                        foreach (var price in askPrices)
                        {
                            Console.WriteLine("\t{0}", price);
                        }
                    }
                    else
                    {
                        Console.WriteLine("{0} : Can't set ask price, please set manually.", e.Fields.instrumentID);
                    }

                    if (bidPrices.Count > 0)
                    {
                        if (instrumentBidPrices.ContainsKey(e.Fields.Instrument))
                        {
                            instrumentBidPrices[e.Fields.Instrument] = bidPrices;
                        }

                        Console.WriteLine("Bid Prices: ");
                        foreach (var price in bidPrices)
                        {
                            Console.WriteLine("\t{0}", price);
                        }
                    }
                    else
                    {
                        Console.WriteLine("{0} : Can't set bid price, please set manually.", e.Fields.instrumentID);
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
                OrderQuantity = Quantity.FromDecimal(instrument, 20),
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

        public void SendOrders()
        {
            foreach (var instrumentElement in instrumentAskPrices)
            {
                foreach (var price in instrumentElement.Value)
                {
                    //Console.WriteLine(instrumentElement.Key);
                    //string q = Console.ReadLine();
                    //if (q == "q")
                    //    break;
                    SendOrder(instrumentElement.Key, price, BuySell.Sell);
                }
            }

            foreach (var instrumentElement in instrumentBidPrices)
            {
                foreach (var price in instrumentElement.Value)
                {
                    //Console.WriteLine(instrumentElement.Key);
                    //string q = Console.ReadLine();
                    //if (q == "q")
                    //    break;
                    SendOrder(instrumentElement.Key, price, BuySell.Buy);
                }
            }
        }

    }

    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                //string appSecretKey = "f8710ecf-f581-9bbb-ff33-e71f294f09d4:aec599a1-c784-585f-af28-318047ced955";
                string appSecretKey = "834ac713-e67f-49b0-845f-a22c6f61d0c4:22b22302-edf8-365f-2541-aebc28715def";
                ServiceEnvironment environment = ServiceEnvironment.DevCert;
                TTAPIOptions.SDKMode sdkMode = TTAPIOptions.SDKMode.Client;
                
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
                        break;
                    if (input == "o")
                    {
                        apiFunctions.SendOrders();
                    }
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
