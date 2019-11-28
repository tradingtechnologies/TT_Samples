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
using System.IO;

namespace FillDownload
{
    class FDLog
    {
        private static readonly object m_lock = new object();
        private static FDLog instance = null;
        private StreamWriter m_logFile = null;


        public static void LogMessage(String message)
        {
            FDLog.privLog("MSG | " + message + Environment.NewLine);
        }

        public static void LogRequest(String request)
        {
            FDLog.privLog("REQ | " + request + Environment.NewLine);
        }

        public static void LogError(String error_msg)
        {
            FDLog.privLog("ERR | " + error_msg + Environment.NewLine);
        }

        private static void privLog(String msg)
        {
            FDLog log = privInstance;
            log.m_logFile.Write(DateTime.UtcNow.ToString() + " | " + msg);
        }

        private FDLog()
        {
            string log_name = GetFileName();
            FileStream fs = File.Create(log_name);
            fs.Close();
            m_logFile = new StreamWriter(log_name, true, Encoding.ASCII);
            m_logFile.AutoFlush = true;
        }

        private static FDLog privInstance
        {
            get
            {
                lock (m_lock)
                {
                    if (instance == null)
                    {
                        instance = new FDLog();
                    }
                    return instance;
                }
            }
        }

        private static string GetFileName()
        {
            return "fd_" + DateTime.Now.ToString("yyyy_MM_dd") + ".log";
        }
    }
}
