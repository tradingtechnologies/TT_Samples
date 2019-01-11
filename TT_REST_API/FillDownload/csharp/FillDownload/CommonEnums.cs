using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace FillDownload
{
    class CommonEnums
    {
        private static CommonEnums instance = null;
        private Dictionary<string, Dictionary<int, string>> dicts { get; set; }

        public static void BuildEnums()
        {
            GetMarkets();
            GetOrderData();
            GetInstrumentData();
            GetProductData();
        }

        private static void GetMarkets()
        {
            var result = RestManager.GetRequest("pds", "markets");
            TTMarkets markets = JsonConvert.DeserializeObject<TTMarkets>(result.Content);
            if (markets.Markets != null)
            {
                privInstance.dicts.Add("markets", CreateDictionary(markets.Markets));
            }
            else
            {
                throw new Exception("GET Market request failed" + Environment.NewLine + result.Content.ToString());
            }
        }

        private static void GetInstrumentData()
        {
            var result = RestManager.GetRequest("pds", "instrumentdata");
            InstrumentData instrument_data = JsonConvert.DeserializeObject<InstrumentData>(result.Content);
            if(instrument_data.optionCodes != null && instrument_data.optionSchemes != null && instrument_data.seriesTerms != null && instrument_data.comboTypes != null)
            {
                privInstance.dicts.Add("optionCodes", CreateDictionary(instrument_data.optionCodes));
                privInstance.dicts.Add("optionSchemes", CreateDictionary(instrument_data.optionSchemes));
                privInstance.dicts.Add("seriesTerms", CreateDictionary(instrument_data.seriesTerms));
                privInstance.dicts.Add("comboTypes", CreateDictionary(instrument_data.comboTypes));
            }
            else
            {
                throw new Exception("GET InstrumentData request failed" + Environment.NewLine + result.Content.ToString());
            }
        }

        private static void GetProductData()
        {
            var result = RestManager.GetRequest("pds", "productdata");
            ProductData product_data = JsonConvert.DeserializeObject<ProductData>(result.Content);

            if (product_data.productTypes != null)
            {
                privInstance.dicts.Add("productTypes", CreateDictionary(product_data.productTypes));
            }
            else
            {
                throw new Exception("GET ProductData request failed" + Environment.NewLine + result.Content.ToString());
            }
        }

        private static void GetOrderData()
        {
            var result = RestManager.GetRequest("ledger", "orderdata");
            OrderData order_data = JsonConvert.DeserializeObject<OrderData>(result.Content);

            if(order_data.data != null)
            {
                foreach (var entry in order_data.data)
                {
                    privInstance.dicts.Add(entry.Key, entry.Value);
                }
            }
            else
            {
                throw new Exception("GET OrderData request failed" + Environment.NewLine + result.Content.ToString());
            }
        }

        CommonEnums()
        {
            dicts = new Dictionary<string, Dictionary<int, string>>();
        }

        private static CommonEnums privInstance
        {
            get
            {
                if (instance == null)
                {
                    instance = new CommonEnums();
                }
                return instance;
            }
        }

        public static string GetEnum(string enumName, int enumVal)
        {
            return privInstance.dicts[enumName][enumVal];
        }

        private static Dictionary<int, string> CreateDictionary(List<TT_Enum> enumList)
        {
            var ret_val = new Dictionary<int, string>();

            foreach(TT_Enum ttEnum in enumList)
            {
                ret_val.Add(ttEnum.id, ttEnum.name);
            }

            return ret_val;
        }
    }

    class TTMarkets
    {
        public string Status { get; set; }
        public List<TT_Enum> Markets { get; set; }
    }

    class InstrumentData
    {
        public string Status { get; set; }
        public List<TT_Enum> comboTypes { get; set; }
        public List<TT_Enum> optionCodes { get; set; }
        public List<TT_Enum> optionSchemes { get; set; }
        public List<TT_Enum> seriesTerms { get; set; }
    }
    class ProductData
    {
        public string Status { get; set; }
        public List<TT_Enum> productTypes { get; set; }
    }

    class OrderData
    {
        public string Status { get; set; }
        [JsonProperty("orderData")]
        public Dictionary<string, Dictionary<int, string>> data { get; set; }
    }

    class FillData
    {
        public string Status { get; set; }
        [JsonProperty("fillData")]
        public Dictionary<string, Dictionary<int, string>> data { get; set; }
    }

    class TT_Enum
    {
        public int id { get; set; }
        public string name { get; set; }
    }

}
