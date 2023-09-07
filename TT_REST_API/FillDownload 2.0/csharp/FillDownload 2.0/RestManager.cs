﻿/*
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
using System.IO;
using System.Threading;

using RestSharp;

namespace FillDownload
{
    class RestManager
    {
        private string ApiURL  = null;
        private string ApiEnv = null;
        private string AppKey = null;
        private string SecretKey = null;
        private string AccessToken = null;
        private RestClient Client = null;
        private Timer TokenTimer;
        private static readonly object s_lock = new object();
        private static RestManager s_instance = null;
        private DateTime m_nextRequest = default(DateTime);
        private static readonly TimeSpan MinSpace = new TimeSpan(0, 0, 0, 0, 333);

        public static void Init (string app_key, string secret_key, string api_env, string api_url = "https://ttrestapi.trade.tt/")
        {
            RestManager rest_man = privInstance;
            lock(rest_man)
            {
                rest_man.AppKey = app_key;
                rest_man.SecretKey = secret_key;
                rest_man.ApiEnv = api_env;
                rest_man.ApiURL = api_url;

                rest_man.Client = new RestClient(rest_man.ApiURL);

                try
                {
                    TimeSpan renewal_time = RefreshToken() - new TimeSpan(1, 0, 0);
                    renewal_time = new TimeSpan(0, 0, 0, 10);
                    rest_man.TokenTimer = new Timer(TokenTimerHandler, rest_man, renewal_time, renewal_time);
                }
                catch (Exception e)
                {
                    // Login errors can be ignored during initialization
                    // to allow the user to attempt to log in again
                    FDLog.LogError(String.Format("Error Authenticating: {0}", e.Message));
                }
            }
        }

        private static void TokenTimerHandler(Object state_info)
        {
            RestManager rest_man = (RestManager)state_info;
            try
            {
                TimeSpan renewal_time = RefreshToken() - new TimeSpan(1, 0, 0);
                rest_man.TokenTimer.Change(renewal_time, renewal_time);
            }
            catch(Exception e)
            {
                rest_man.TokenTimer.Change(Timeout.Infinite, Timeout.Infinite);
                RaiseTokenError(e.Message);
            }
        }

        public static bool IsAuthorized()
        {
            return privInstance.AccessToken != null;
        }

        public static TimeSpan RefreshToken()
        {
            RestManager rest_man = privInstance;
            lock (rest_man)
            {
                string endpoint = string.Format("{0}/{1}/{2}", "ttid", rest_man.ApiEnv, "token");
                RestRequest request = new RestRequest(endpoint, Method.POST);
                request.RequestFormat = DataFormat.Json;

                request.AddHeader("Content-Type", "application/x-www-form-urlencoded");
                request.AddHeader("x-api-key", rest_man.AppKey);
                request.AddHeader("Accept", "application/json");

                request.AddParameter("grant_type", "user_app");
                request.AddParameter("app_key", rest_man.SecretKey);
                request.AddParameter(GetRequestId());

                var response = rest_man.Client.Execute(request);
                var content = response.Content;

                LogRequest(request, response);

                if (response.StatusCode == System.Net.HttpStatusCode.OK)
                {
                    RestSharp.Deserializers.JsonDeserializer deserial = new RestSharp.Deserializers.JsonDeserializer();
                    var dict_response = deserial.Deserialize<Dictionary<string, string>>(response);

                    if (dict_response["status"] == "Ok")
                    {
                        rest_man.AccessToken = "Bearer " + dict_response["access_token"];
                        return new TimeSpan(0, 0, int.Parse(dict_response["seconds_until_expiry"]));
                    }
                    else
                    {
                        string err_msg = "Error, POST request for token failed: " + dict_response["status_message"];
                        FDLog.LogError(err_msg);
                        throw new Exception(err_msg);
                    }
                }
                else
                {
                    string err_msg = "Error, POST request for token failed: " + response.ErrorMessage;
                    FDLog.LogError(err_msg);
                    throw new Exception(err_msg);
                }
            }
        }

        public static IRestResponse GetRequest(string target, string call, params Parameter[] parameters)
        {
            return RestManager.GetRequest(target, call, 1, parameters);
        }

            public static IRestResponse GetRequest(string target, string call, int max_retries, params Parameter [] parameters)
        {
            RestManager rest_man = privInstance;
            lock (rest_man)
            {
                string endpoint = string.Format("{0}/{1}/{2}", target, rest_man.ApiEnv, call);

                RestRequest request = new RestRequest(endpoint, Method.GET);
                request.RequestFormat = DataFormat.Json;

                request.Parameters.AddRange(parameters);
                request.Parameters.Add(GetRequestId());

                request.AddHeader("Authorization", rest_man.AccessToken);
                request.AddHeader("x-api-key", rest_man.AppKey);

                IRestResponse response = null;

                for (int i = 0; i < max_retries; ++i)
                {
                    if (privInstance.m_nextRequest > DateTime.Now)
                    {
                        Thread.Sleep(privInstance.m_nextRequest - DateTime.Now);
                    }

                    response = rest_man.Client.Execute(request);
                    privInstance.m_nextRequest = DateTime.Now + RestManager.MinSpace;
                    LogRequest(request, response);

                    if (response.IsSuccessful || (response.StatusCode != System.Net.HttpStatusCode.InternalServerError && response.StatusCode != System.Net.HttpStatusCode.BadRequest && response.ResponseStatus != ResponseStatus.TimedOut))
                        break;
                }

                return response;
            }
        }

        private static void LogRequest(RestRequest request, RestSharp.IRestResponse result)
        {
            RestManager rest_man = privInstance;
            string log_message = String.Format("{0} - {1} {2}", result.StatusCode.ToString(), request.Method.ToString(), rest_man.Client.BuildUri(request));
            FDLog.LogRequest(log_message);
        }

        private static Parameter GetRequestId()
        {
            string request_id = "REST-FILLDOWNLOAD--" + Guid.NewGuid();
            Parameter parameter = new Parameter("requestId", request_id, RestSharp.ParameterType.QueryString);
            return parameter;
        }

        private static RestManager privInstance
        {
            get
            {
                lock (s_lock)
                {
                    if (s_instance == null)
                    {
                        s_instance = new RestManager();
                    }
                    return s_instance;
                }
            }
        }

        public delegate void TokenErrorHandler(object sender, string error_message);
        public static event TokenErrorHandler OnTokenError;
        private static void RaiseTokenError(string error_message)
        {
            if (OnTokenError != null)
                OnTokenError(null, error_message);
        }
    }
}
