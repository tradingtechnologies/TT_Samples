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

        public static void Init (string app_key, string secret_key, string api_env, string api_url = "https://apigateway.trade.tt/")
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
                catch (Exception)
                {
                    // Login errors can be ignored during initialization
                    // to allow the user to attempt to log in again
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
                        ErrorLog.Write(err_msg);
                        throw new Exception(err_msg);
                    }
                }
                else
                {
                    string err_msg = "Error, POST request for token failed: " + response.ErrorMessage;
                    ErrorLog.Write(err_msg);
                    throw new Exception(err_msg);
                }
            }
        }

        public static IRestResponse GetRequest(string target, string call, params Parameter [] parameters)
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

                var response = rest_man.Client.Execute(request);
                LogRequest(request, response);
                return response;
            }
        }

        private static void LogRequest(RestRequest request, RestSharp.IRestResponse result)
        {
            RestManager rest_man = privInstance;
            string log_message = String.Format("{0} - {1} {2}", result.StatusCode.ToString(), request.Method.ToString(), rest_man.Client.BuildUri(request));
            RequestLog.Log(log_message);
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
