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
using Newtonsoft.Json;

namespace FillDownload
{
    class TT_Info
    {
        private static TT_Info s_instance = null;
        private Dictionary<string, TT_Instrument> instruments = new Dictionary<string, TT_Instrument>();
        private Dictionary<string, TT_Account> accounts = new Dictionary<string, TT_Account>();
        private Dictionary<string, TT_User> users = new Dictionary<string, TT_User>();
        private static long EPOCH_TICKS = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc).Ticks;
        public static uint MAX_RESPONSE_FILLS { get; } = 500;

        public static TT_Instrument GetInstrument(string instr_id)
        {
            if (!privInstance.instruments.ContainsKey(instr_id))
            {
                TT_Instrument req = RequestInstrument(instr_id);
                if(req != null)
                {
                    privInstance.instruments.Add(req.id, req);
                }
            }
            TT_Instrument instr = privInstance.instruments[instr_id];
            return instr;
        }

        private static TT_Instrument RequestInstrument(string instr_id)
        {
            var result = RestManager.GetRequest("pds", "instrument/" + instr_id);

            InstrumentResponse response = JsonConvert.DeserializeObject<InstrumentResponse>(result.Content);
            if(response.instrument != null)
            {
                return response.instrument[0];
            }
            else
            {
                TT_Instrument instr = new TT_Instrument();
                instr.id = instr_id;
                instr.alias = "Permission Denied";
                instr.productSymbol = "Permission Denied";
                instr.productTypeId = 34;
                instr.optionCodeId = 2;
                return instr;
            }
        }


        public static TT_Account GetAccount(string account_id)
        {
            if (!privInstance.accounts.ContainsKey(account_id))
            {
                TT_Account req = RequestAccount(account_id);
                if (req != null)
                {
                    privInstance.accounts.Add(req.id, req);
                }
            }
            TT_Account acct = privInstance.accounts[account_id];
            return acct;
        }

        private static TT_Account RequestAccount(string account_id)
        {
            var result = RestManager.GetRequest("risk", "account/" + account_id);
            try
            {
                AccountResponse acct = JsonConvert.DeserializeObject<AccountResponse>(result.Content);
                return acct.account[0];
            }
            catch(Exception)
            {
                TT_Account acct = new TT_Account();
                acct.id = account_id;
                acct.name = "PLACEHOLDER";
                return acct;
            }
        }

        public static TT_User GetUser(string user_id)
        {
            if (!privInstance.users.ContainsKey(user_id))
            {
                TT_User req = RequestUser(user_id);
                if (req != null)
                {
                    privInstance.users.Add(req.id, req);
                }
            }
            TT_User usr = privInstance.users[user_id];
            return usr;
        }

        private static TT_User RequestUser(string user_id)
        {
            var result = RestManager.GetRequest("risk", "user/" + user_id);
            UserResponse usr = JsonConvert.DeserializeObject<UserResponse>(result.Content);
            return usr.user[0];
        }


        // In these two functions, we must correct for two differences in ticking
        // 1. A TT Tick is a NanoSecond, a .NET tick is 100 nanoseconds
        // 2. The Unix Epoch is January 1, 1970; the .NET Epoch is January 1, 0001 AD

        public static DateTime FromRestTimestamp(long tt_ticks)
        {
            long ticks = (tt_ticks / 100) + EPOCH_TICKS;
            return new DateTime(ticks, DateTimeKind.Utc);
        }

        public static long ToRestTimestamp(DateTime time_stamp)
        {
            return (time_stamp.Ticks - EPOCH_TICKS) * 100;
        }

        private static TT_Info privInstance
        {
            get
            {
                if (s_instance == null)
                {
                    s_instance = new TT_Info();
                }
                return s_instance;
            }
        }
    }

    class InstrumentResponse
    {
        public string Status { get; set; }
        public List<TT_Instrument> instrument { get; set; }
    }

    class TT_Instrument
    {
        public string name { get; set; } 
        public string alias { get; set; } 
        public string productId { get; set; } 
        public string productFamilyId { get; set; }
        public int marketId { get; set; }
        public string expirationDate { get; set; }
        public int productTypeId { get; set; }
        public string productSymbol { get; set; }
        public int marketDepth { get; set; }
        public float tickValue { get; set; }
        public float tickSize { get; set; }
        public float pointValue { get; set; }
        public int optionCodeId { get; set; }
        public string optionSchemeId { get; set; }
        public string underlyingInstrumentId { get; set; }
        public float strike { get; set; }
        public string securityId { get; set; }
        public int seriesTermId { get; set; }
        public string term { get; set; }
        public float displayFactor { get; set; }
        public int roundLotQty { get; set; }
        public string id { get; set; }
    }

    class AccountResponse
    {
        public string Status { get; set; }
        public List<TT_Account> account { get; set; }
    }

    class TT_Account
    {
        public string id { get; set; }
        public string name { get; set; }
    }

    class UserResponse
    {
        public string Status { get; set; }
        public List<TT_User> user { get; set; }
    }

    class TT_User
    {
        public string id { get; set; }
        public string alias { get; set; }
        public TT_Company company { get; set; }
    }

    class TT_Company
    {
        public int id { get; set; }
        public string abbrevName { get; set; }
        public string name { get; set; }
    }

}
