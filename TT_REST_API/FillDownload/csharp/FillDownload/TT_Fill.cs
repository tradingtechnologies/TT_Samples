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
using Newtonsoft.Json.Linq;

namespace FillDownload
{
    public class TT_Fill
    {
        JToken m_jsonData = null;

        public TT_Fill(JToken json_data)
        {
            m_jsonData = json_data;
        }

        public string JsonData()
        {
            return m_jsonData.ToString();
        }


        override public String ToString()
        {
            return String.Join(",",
                UtcDateString,
                UtcTimeString,
                ExchangeName,
                ContractName,
                TradeSide,
                FillQty,
                Price,
                FullPartial,
                OrdType,
                Modifier,
                Route,
                PositionEffect,
                Broker,
                Account,
                AccountType,
                GiveUp,
                TextA,
                TextB,
                TextC,
                TextTT,
                Originator,
                CurrentUser,
                ClientOrderID, TTOrderID,
                ParentOrderID,
                OmaOrderID,
                ExchangeOrderID,
                ExchangeTransactionID,
                ExchangeAccount,
                ExchangeDate,
                ExchangeTime,
                ManualFill,
                Symbol,
                ProductType,
                FillType,
                ExecQty,
                WorkQty,
                AggressorFlag,
                ConnectionId,
                PutCall,
                Strike,
                OrderOrigination,
                TradingCapacity,
                LiquidityProvision,
                CommodityDerivativeIndicator,
                InvestDec,
                ExecDec,
                ClientID);
        }

        public bool IsSyntheticMarket()
        {
            string exchangeName = ExchangeName;
            return ExchangeName == "ASE" || ExchangeName == "AGREGATOR";
        }

        public bool IsAlgoOrder()
        {
            string algoType = AlgoType;
            return algoType == "ALGO_TYPE_ADL" ||
                    algoType == "ALGO_TYPE_SSE" ||
                    algoType == "ALGO_TYPE_CUSTOM_LIBRARY" ||
                    algoType == "ALGO_TYPE_VENDOR";
        }

        public bool IsSyntheticOrder()
        {
            return IsSyntheticMarket() || IsAlgoOrder();
        }

        public bool IsIceberg()
        {
            return int.Parse(GetDataOrDefault("displayQty", "-1")) > 0;
        }

        public bool IsNativeIceberg()
        {
            return IsIceberg() && IsSyntheticOrder();
        }

        private bool JsonContains(string data)
        {
            return m_jsonData.AsJEnumerable()[data] != null;
        }

        private string GetData(string data)
        {
            return m_jsonData.AsJEnumerable()[data].ToString();
        }

        private string GetDataOrDefault(string data, string defaultString)
        {
            object try_get = m_jsonData.AsJEnumerable()[data];
            return try_get != null ? try_get.ToString() : defaultString;
        }

        private string GetAttribWithValue(string value)
        {
            var attributes = m_jsonData.AsJEnumerable()["orderAttributes"];
            if (attributes != null)
            {
                foreach (JToken attr in attributes)
                {
                    if (attr["order_attribute_type"].ToString() == value)
                    {
                        return attr["order_attribute_type"].ToString();
                    }
                }
            }
            return "";
        }

        private string GetPartyWithRole(string role)
        {
            var parties = m_jsonData.AsJEnumerable()["parties"];
            if (parties != null)
            {
                foreach (JToken party in parties)
                {
                    if (party["role"].ToString() == role)
                    {
                        return party["id"].ToString();
                    }
                }
            }
            return "";
        }

        private string GetPartyWithRoleOrIdSource(string role, string idSource)
        {
            var parties = m_jsonData.AsJEnumerable()["parties"];
            if (parties != null)
            {
                foreach (JToken party in parties)
                {
                    if (party["role"].ToString() == role)
                    {
                        return party["id"].ToString();
                    }
                    else if (party["idSource"] != null && party["idSource"].ToString() == idSource)
                    {
                        return party["id"].ToString();
                    }
                }
            }
            return "";
        }

        private static string GetEnumString(string enumName, string enumValue)
        {
            int val = 0;
            bool parsed = int.TryParse(enumValue, out val);
            return parsed ? CommonEnums.GetEnum(enumName, val) : "";
        }

        private static DateTime FromRestTimestamp(string TT_TimeStamp)
        {
            long ticks = long.Parse(TT_TimeStamp);
            return TT_Info.FromRestTimestamp(ticks);
        }

        private static DateTime GetDate(string TT_TimeStamp)
        {
            DateTime date = FromRestTimestamp(TT_TimeStamp);
            return date.ToUniversalTime().Date;
        }

        private static DateTime GetTime(string TT_TimeStamp)
        {
            DateTime date = FromRestTimestamp(TT_TimeStamp);
            return date.ToUniversalTime();
        }

        public string RecordID
        {
            get
            {
                return GetDataOrDefault("recordId", "");
            }
        }

        public string UtcDateString
        {
            get
            {
                string time_stamp = GetData("timeStamp");
                return GetDate(time_stamp).ToString("yyyy/MM/dd");
            }
        }

        public string UtcTimeString
        {
            get
            {
                string time_stamp = GetData("timeStamp");
                return GetTime(time_stamp).ToString("HH:mm:ss.ffff");
            }
        }

        public DateTime UtcTimeStamp
        {
            get
            {
                string time_stamp = GetData("timeStamp");
                return GetTime(time_stamp);
            }
        }

        public string ExchangeName
        {
            get
            {
                int market_id = int.Parse(GetData("marketId"));
                return CommonEnums.GetEnum("markets", market_id);
            }
        }

        public string ContractName
        {
            get
            {
                string instrument_id = GetData("instrumentId");
                return TT_Info.GetInstrument(instrument_id).alias;
            }
        }

        public string TradeSide
        {
            get
            {
                string side = GetDataOrDefault("side", "");
                return GetEnumString("side", side);
            }
        }

        public string FillQty
        {
            get
            {
                return GetData("lastQty");
            }
        }

        public string Price
        {
            get
            {
                return GetData("lastPx");
            }
        }

        public string FullPartial
        {
            get
            {
                string status = GetData("ordStatus");
                string return_val = "";
                if (status == "2")
                    return_val = "P";
                else if (status == "3")
                    return_val = "F";
                return return_val;
            }
        }

        public string AlgoType
        {
            get
            {
                return GetEnumString("algoType", GetDataOrDefault("algoType", ""));
            }
        }

        public string OrdType
        {
            get
            {
                return GetEnumString("orderType", GetDataOrDefault("orderType", ""));
            }
        }

        public string SyntheticType
        {
            get
            {
                return GetEnumString("syntheticType", GetDataOrDefault("syntheticType", ""));
            }
        }

        public string TradeType
        {
            get
            {
                string trade_type = "";

                if (JsonContains("tradeType"))
                    trade_type = GetEnumString("tradeType", GetData("tradeType"));
                else if (JsonContains("trdType"))
                    trade_type = GetEnumString("tradeType", GetData("trdType"));

                return trade_type;
            }
        }

        public string ExecInst
        {
            get
            {
                return GetEnumString("execInst", GetDataOrDefault("execInst", ""));
            }
        }

        public string ContingencyType
        {
            get
            {
                return GetEnumString("contingencyType", GetDataOrDefault("contingencyType", ""));
            }
        }

        public string Modifier
        {
            get
            {
                if (IsNativeIceberg())
                    return "Iceberg";
                else if (OrdType == "LIMIT" && ExecInst == "EXEC_INST_PARTICIPATE_DO_NOT_INITIATE")
                    return "BOC";
                else if (OrdType == "LIMIT" && ContingencyType == "CONTINGENCY_TYPE_ONE_CANCELS_THE_OTHER")
                    return "OCO";
                else if (JsonContains("crossId"))
                    return "Cross";
                else if (int.Parse(GetDataOrDefault("minQty", "-1")) > 0)
                    return "MinVol";
                else if (TradeType != "" || JsonContains("tradeReportId"))
                    return "Block";
                else
                    return "";
            }
        }

        public string OrderCrossPreventionType
        {
            get
            {
                return GetEnumString("orderCrossPreventionType", GetDataOrDefault("orderCrossPreventionType", ""));
            }
        }

        public string Route
        {
            get
            {
                string ocpt = OrderCrossPreventionType;
                if (ocpt == "POSITION_TRANSFER_FILL" || ocpt == "ORDER_REDUCED_PARTIAL_POSITION_TRANSFER_FILL")
                    return "Internal";
                else
                    return "Direct";
            }
        }

        public string PositionEffect
        {
            get
            {
                return GetEnumString("positionEffect", GetDataOrDefault("positionEffect", ""));
            }
        }

        public string Broker
        {
            get
            {
                string broker = "";
                if(JsonContains("currUserId") && GetData("currUserId") != "0")
                    broker = TT_Info.GetUser(GetData("currUserId")).company.name;
                return broker;
            }
        }

        public string Account
        {
            get
            {
                string accountId = GetData("accountId");
                return TT_Info.GetAccount(accountId).name;
            }
        }

        public string AccountType
        {
            get
            {
                return GetPartyWithRole("85");
            }
        }

        public string GiveUp
        {
            get
            {
                return GetPartyWithRole("6");
            }
        }

        public string TextA
        {
            get
            {
                return GetDataOrDefault("textA", "");
            }
        }

        public string TextB
        {
            get
            {
                return GetDataOrDefault("textB", "");
            }
        }

        public string TextC
        {
            get
            {
                return GetDataOrDefault("textC", "");
            }
        }

        public string TextTT
        {
            get
            {
                return GetDataOrDefault("textTT", "");
            }
        }

        public string Originator
        {
            get
            {
                string originator = "";
                if(JsonContains("currUserId") && GetData("currUserId") != "0")
                    originator = TT_Info.GetUser(GetData("currUserId")).alias;
                return originator;
            }
        }

        public string CurrentUser
        {
            get
            {
                string current_user = "";
                if (JsonContains("currUserId") && GetData("currUserId") != "0")
                    current_user = TT_Info.GetUser(GetData("currUserId")).alias;
                return current_user;
            }
        }

        public string ClientOrderID
        {
            get
            {
                return GetDataOrDefault("secondaryClOrderId", "");
            }
        }

        public string TTOrderID
        {
            get
            {
                return GetDataOrDefault("orderId", "");
            }
        }

        public string ParentOrderID
        {
            get
            {
                return GetDataOrDefault("parentOrderId", "");
            }
        }

        public string OmaOrderID
        {
            get
            {
                return GetDataOrDefault("omaParentOrderId", "");
            }
        }

        public string ExchangeOrderID
        {
            get
            {
                return GetDataOrDefault("secondaryOrderId", "");
            }
        }

        public string ExchangeTransactionID
        {
            get
            {
                return GetDataOrDefault("execId", "");
            }
        }

        public string ExchangeAccount
        {
            get
            {
                return GetDataOrDefault("account", "");
            }
        }

        public string ExchangeDate
        {
            get
            {
                return GetDate(GetData("transactTime")).ToString("yyyy/MM/dd");
            }
        }

        public string ExchangeTime
        {
            get
            {
                return GetTime(GetDataOrDefault("transactTime", "")).ToString("HH:mm:ss.ffff");
            }
        }

        public string ManualFill
        {
            get
            {
                return GetDataOrDefault("manualFill", "").ToUpper();
            }
        }

        public string Symbol
        {
            get
            {
                return TT_Info.GetInstrument(GetData("instrumentId")).productSymbol;
            }
        }

        public string ProductType
        {
            get
            {
                return CommonEnums.GetEnum("productTypes" ,TT_Info.GetInstrument(GetData("instrumentId")).productTypeId);
            }
        }

        public string FillType
        {
            get
            {
                string retVal = "";
                string multiLegReportingType = GetDataOrDefault("multiLegReportingType", "");
                if(multiLegReportingType != "")
                {
                    retVal = CommonEnums.GetEnum("fillType", int.Parse(multiLegReportingType));
                }
                return retVal;
            }
        }

        public string ExecQty
        {
            get
            {
                return GetDataOrDefault("lastQty", "");
            }
        }

        public string WorkQty
        {
            get
            {
                return GetDataOrDefault("exchLeavesQty", "");
            }
        }

        public string AggressorFlag
        {
            get
            {
                string aggressorIndicator = GetDataOrDefault("aggressorIndicator", "");
                if (aggressorIndicator != "")
                    aggressorIndicator = aggressorIndicator == "true" ? "A" : "P";
                return aggressorIndicator;
            }
        }

        public string ConnectionId
        {
            get
            {
                if (JsonContains("connectionId"))
                    return GetData("connectionId");
                else
                    return GetDataOrDefault("exchOrderAssoc","");
            }
        }

        public string PutCall
        {
            get
            {
                if(TT_Info.GetInstrument(GetData("instrumentId")).productTypeId == 51)
                {
                    TT_Instrument instr = TT_Info.GetInstrument(GetData("instrumentId"));
                    return CommonEnums.GetEnum("optionCodes", instr.optionCodeId);
                }
                else
                {
                    return "";
                }
            }
        }

        public string Strike
        {
            get
            {
                return GetDataOrDefault("strike", "");
            }
        }

        public string OrderOrigination
        {
            get
            {
                if (!JsonContains("orderOrigination"))
                    return "";
                else if (CommonEnums.GetEnum("orderOrigination",int.Parse(GetData("orderOrigination"))) == "ORDER_ORIGINATION_FROM_DIRECT_ACCESS_OR_SPONSORED_ACCESS_CUSTOMER")
                    return "Y";
                else
                    return "N";
            }
        }

        public string OrderCapacity
        {
            get
            {
                if (JsonContains("orderCapacity"))
                    return CommonEnums.GetEnum("orderCapacity", int.Parse(GetData("orderCapacity")));
                else
                    return "";
            }
        }

        public string TradingCapacity
        {
            get
            {
                string orderCapacity = GetDataOrDefault("orderCapacity", "");

                if (orderCapacity == "ORDER_CAPACITY_AGENCY")
                    return "Other";
                else if (orderCapacity == "ORDER_CAPACITY_PRINCIPAL")
                    return "Deal";
                else if (orderCapacity == "ORDER_CAPACITY_RISKLESS_PRINCIPAL")
                    return "Match";
                else
                    return "";
            }
        }

        public string LiquidityProvision
        {
            get
            {
                return GetAttribWithValue("2");
            }
        }

        public string CommodityDerivativeIndicator
        {
            get
            {
                return GetAttribWithValue("3");
            }
        }

        public string InvestDec
        {
            get
            {
                return GetPartyWithRole("98");
            }
        }

        public string ExecDec
        {
            get
            {
                return GetPartyWithRole("4");
            }
        }

        public string ClientID
        {
            get
            {
                return GetPartyWithRoleOrIdSource("22","19");
            }
        }
    }
}