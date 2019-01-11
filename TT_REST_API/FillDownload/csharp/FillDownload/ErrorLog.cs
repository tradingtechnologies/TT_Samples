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
        private TextWriter m_errorFile = null;

        public static void Write(String error_msg)
        {
            ErrorLog log = privInstance;
            error_msg += Environment.NewLine;
            log.m_errorFile.Write(error_msg);
        }

        private ErrorLog()
        {
            FileStream fs = File.Create("error_log.txt");
            fs.Close();
            m_errorFile = TextWriter.Synchronized(File.AppendText("error_log.txt"));
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
