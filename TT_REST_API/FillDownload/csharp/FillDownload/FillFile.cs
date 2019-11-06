using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.IO;
using System.Diagnostics;

namespace FillDownload
{
    public enum FileMode
    {
        PerDay,
        PerDownload
    }

    public abstract class FillFile
    {
        protected StreamWriter m_outputFile = null;
        string m_path;
        List<FillColumn> m_columns;

        public static FillFile GetFillFile(FileMode mode, string path_to_file, List<FillColumn> columns)
        {
            if(mode == FileMode.PerDay)
            {
                return new DailyFillFile(path_to_file, columns);

            }
            else if(mode == FileMode.PerDownload)
            {
                return new DownloadFillFile(path_to_file, columns);
            }
            else
            {
                Debug.Assert(false, "Invalid fill file type");
                return null;
            }
        }

        public abstract bool ProcessFills(List<TT_Fill> fills);

        protected FillFile(string path_to_file, List<FillColumn> columns)
        {
            m_path = path_to_file;
            m_columns = columns;
        }

        protected void CreateFile(string file_name)
        {
            string outFileName = m_path + "\\" + file_name;
            FileStream fs = File.Create(outFileName);
            fs.Close();
            m_outputFile = new StreamWriter(outFileName, false, Encoding.ASCII);
            m_outputFile.AutoFlush = true;
            m_outputFile.Write(GetCSVHeader());
        }

        protected String GetCSVHeader()
        {
            String header = "";
            foreach (FillColumn column in m_columns)
            {
                header += column.ToString() + ",";
            }

            header += Environment.NewLine;
            return header;
        }

        protected bool WriteFillsToFile(List<TT_Fill> fills)
        {
            bool errors = false;
            foreach (TT_Fill fill in fills)
            {
                String row = "";
                foreach (FillColumn column in m_columns)
                {
                    try
                    {
                        row += column.DisplayField(fill) + ",";
                    }
                    catch (Exception ex)
                    {
                        row += ",";
                        ErrorLog.Write("Error: Error parsing fill column " + column.ColumnName + " for fill " + fill.RecordID + Environment.NewLine + ex.Message);
                        errors = true;
                    }
                }
                row += Environment.NewLine;
                m_outputFile.Write(row.ToString());
            }
            return errors;
        }

        public void Close()
        {
            m_outputFile.Close();
        }
    }

    public class DailyFillFile : FillFile
    {
        DateTime m_lastCreated;

        public DailyFillFile(string path_to_file, List<FillColumn> columns)
            :base(path_to_file, columns)
        {
            m_lastCreated = DateTime.Now;
            CreateFile(GetFileName(m_lastCreated));
        }

        public override bool ProcessFills(List<TT_Fill> fills)
        {
            if(m_lastCreated.Date < DateTime.Now.Date)
            {
                m_lastCreated = DateTime.Now;
                CreateFile(GetFileName(m_lastCreated));
            }
            return WriteFillsToFile(fills);
        }

        // export_yyyy_mm_dd.csv
        private string GetFileName(DateTime time)
        {
            return "export_" + time.ToString("yyyy_MM_dd") + ".csv";
        }
    }

    public class DownloadFillFile : FillFile
    {
        public DownloadFillFile(string path_to_file, List<FillColumn> columns)
            : base(path_to_file, columns)
        {

        }

        public override bool ProcessFills(List<TT_Fill> fills)
        {
            bool errors = false;
            if(fills.Count > 0)
            {
                DateTime firstFillTime = fills[0].UtcTimeStamp.ToLocalTime();
                CreateFile(GetFileName(firstFillTime));
                errors = WriteFillsToFile(fills);
            }
            return errors;
        }

        // export_yyyy_mm_dd_hh_mm.csv
        private string GetFileName(DateTime time)
        {
            return "export_" + time.ToString("yyyy_MM_dd_HH_mm") + ".csv";
        }
    }
}
