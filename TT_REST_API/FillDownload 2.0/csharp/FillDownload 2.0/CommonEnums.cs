/*
    Copyright © 2018-2019 Trading Technologies International, Inc. All Rights Reserved Worldwide

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    * Redistributions of source or binary code must be free of charge.

    * Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
            var result = RestManager.GetRequest("ttpds", "markets");
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
            var result = RestManager.GetRequest("ttpds", "instrumentdata");
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
            var result = RestManager.GetRequest("ttpds", "productdata");
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
            var result = RestManager.GetRequest("ttledger", "orderdata");
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
            string out_val;
            bool success = privInstance.dicts[enumName].TryGetValue(enumVal, out out_val);
            return success ? out_val : enumVal.ToString();
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
