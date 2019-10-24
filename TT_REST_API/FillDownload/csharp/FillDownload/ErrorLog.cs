using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace FillDownload
{
    class ErrorLog
    {
        private static readonly object m_lock = new object();
        private static ErrorLog instance = null;
        private StreamWriter m_errorFile = null;

        public static void Write(String error_msg)
        {
            ErrorLog log = privInstance;
            error_msg += Environment.NewLine;
            log.m_errorFile.Write(DateTime.Now.ToString() + " " + error_msg);
        }

        private ErrorLog()
        {
            string log_name = "error_log.txt";
            FileStream fs = File.Create(log_name);
            fs.Close();
            m_errorFile = new StreamWriter(log_name, false, Encoding.ASCII);
            m_errorFile.AutoFlush = true;
        }

        private static ErrorLog privInstance
        {
            get
            {
                lock (m_lock)
                {
                    if (instance == null)
                    {
                        instance = new ErrorLog();
                    }
                    return instance;
                }
            }
        }
    }
}
