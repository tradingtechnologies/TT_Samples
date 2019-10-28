using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace FillDownload
{
    class RequestLog
    {
        private static readonly object m_lock = new object();
        private static RequestLog instance = null;
        private StreamWriter m_logFile = null;

        public static void Log(String request)
        {
            RequestLog log = privInstance;
            request += Environment.NewLine;
            log.m_logFile.Write(DateTime.Now.ToString() + " - " + request);
        }

        private RequestLog()
        {
            string log_name = GetFileName();
            FileStream fs = File.Create(log_name);
            fs.Close();
            m_logFile = new StreamWriter(log_name, true, Encoding.ASCII);
            m_logFile.AutoFlush = true;
        }

        private static RequestLog privInstance
        {
            get
            {
                lock (m_lock)
                {
                    if (instance == null)
                    {
                        instance = new RequestLog();
                    }
                    return instance;
                }
            }
        }

        private static string GetFileName()
        {
            return "requests_" + DateTime.Now.ToString("yyyy_MM_dd") + ".log";
        }
    }
}
