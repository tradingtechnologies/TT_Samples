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
using System.Text;
using System.Windows.Forms;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using System.IO;

namespace FillDownload
{
    public partial class FrmFillDownload : Form
    {
        FillDownloadThread m_fillThread = null;
        FillFile m_outputFile = null;

        List<FillColumn> m_TradePaneColumns = null;

        public FrmFillDownload()
        {
            InitializeComponent();

            InitializeColumnList();

            clbColumns.Items.Clear();
            foreach (FillColumn column in m_TradePaneColumns)
            {
                clbColumns.Items.Add(column, true);
            }
             
            this.FormClosing += FrmFillDownload_FormClosing;

            cbFileMode.DataSource = Enum.GetValues(typeof(FileMode));

            LoadSettings();

            this.DragOver += FrmFillDownload_DragOver;
            this.DragDrop += Debug_DragDrop;
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            // Validate our inputs

            int interval = 0;
            if (!int.TryParse(txtFrequency.Text, out interval))
            {
                MessageBox.Show("Error parsing frequency \"" + txtFrequency.Text + "\".");
                return;
            }

            if(interval <= 0)
            {
                MessageBox.Show("Frequency must be greater than zero.");
                return;
            }

            TimeSpan start_time = dtpStartTime.Value.TimeOfDay;
            TimeSpan end_time = dtpEndTime.Value.TimeOfDay;
            if(start_time > end_time)
            {
                MessageBox.Show("Error: Start time must come before end time");
                return;
            }


            // Try to log in to the REST API
            string app_key = txtSecret.Text.Split(':')[0];
            string app_secret = txtSecret.Text;
            RestManager.Init(app_key, app_secret, txtEnvironment.Text);
            if (!RestManager.IsAuthorized())
            {
                MessageBox.Show("Rest API was not able to log in with provided App Key and Secret");
                return;
            }
            else
            {
                FDLog.LogMessage("Successfully logged in with app key and secret");
                RestManager.OnTokenError += RestManager_OnTokenError;
            }

            DateTime start_date = default(DateTime);
            if(dtpStartDate.CustomFormat != " ")
            {
                // TimeStamp correction so it properly reflects midnight of 
                // this day in local time
                start_date = dtpStartDate.Value.Date - TimeZoneInfo.Local.BaseUtcOffset;
            }
            else
            {
                start_date = DateTime.Today - TimeZoneInfo.Local.BaseUtcOffset;
            }

            if (!chkSunday.Checked && !chkMonday.Checked && !chkTuesday.Checked && !chkWednesday.Checked && !chkThursday.Checked && !chkFriday.Checked && !chkSaturday.Checked)
            {
                MessageBox.Show("Must select at least one day to run downloader.");
                return;
            }

            bool[] days_to_run = new bool[7];
            days_to_run[0] = chkSunday.Checked;
            days_to_run[1] = chkMonday.Checked;
            days_to_run[2] = chkTuesday.Checked;
            days_to_run[3] = chkWednesday.Checked;
            days_to_run[4] = chkThursday.Checked;
            days_to_run[5] = chkFriday.Checked;
            days_to_run[6] = chkSaturday.Checked;

            try
            {
                FileMode mode = (FileMode)cbFileMode.SelectedItem;
                m_outputFile = FillFile.GetFillFile(mode, txtOutput.Text, GetReportItems());
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error creating output file: " + ex.Message);
                return;
            }

            clbColumns.Enabled = false;
            btnStart.Text = "Stop Downloading";
            btnStart.Click -= btnStart_Click;
            btnStart.Click += btnStart_Close;
            btnBrowse.Enabled = false;

            FDLog.LogMessage("Beginning downloads...");


            m_fillThread = new FillDownloadThread(start_time, end_time, new TimeSpan(0, interval, 0), days_to_run, start_date);
            m_fillThread.FillDownload += fillThread_OnFillDownload;
            m_fillThread.OnError += OnError;
            m_fillThread.Start();
        }

        private void RestManager_OnTokenError(object sender, string error_message)
        {
            this.OnError(this, error_message);
        }

        private void btnStart_Close(object sender, EventArgs e)
        {
            this.Close();
        }

        private void fillThread_OnFillDownload(object sender, List<TT_Fill> fills)
        {
            bool errors = m_outputFile.ProcessFills(fills);

            if (errors)
            {
                m_fillThread.StopDownloading();
                this.OnError(this, "Errors parsing fills. Closing down.");
            }
        }

        private void OnError(object sender, string error_message)
        {
            FDLog.LogError(error_message + Environment.NewLine + "--------------------------------------------------" + Environment.NewLine);
            MessageBox.Show(error_message);

            if (this.InvokeRequired)
                this.BeginInvoke(new Action(this.Close));
            else
                this.Close();
        }

        private void FrmFillDownload_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_fillThread != null)
                m_fillThread.StopThread();

            if (m_outputFile != null)
                m_outputFile.Close();
        }

        private void Form_Load(object sender, EventArgs e)
        {

        }

        private void InitializeColumnList()
        {
            m_TradePaneColumns = new List<FillColumn>();

            m_TradePaneColumns.Add(new FillColumn("UtcDate", delegate (TT_Fill fill) { return fill.UtcDateString; }));
            m_TradePaneColumns.Add(new FillColumn("UtcTime", delegate (TT_Fill fill) { return fill.UtcTimeString; }));
            m_TradePaneColumns.Add(new FillColumn("ExchangeName", delegate (TT_Fill fill) { return fill.ExchangeName; }));
            m_TradePaneColumns.Add(new FillColumn("ContractName", delegate (TT_Fill fill) { return fill.ContractName; }));
            m_TradePaneColumns.Add(new FillColumn("TradeSide", delegate (TT_Fill fill) { return fill.TradeSide; }));
            m_TradePaneColumns.Add(new FillColumn("FillQty", delegate (TT_Fill fill) { return fill.FillQty; }));
            m_TradePaneColumns.Add(new FillColumn("Price", delegate (TT_Fill fill) { return fill.Price; }));
            m_TradePaneColumns.Add(new FillColumn("FullPartial", delegate (TT_Fill fill) { return fill.FullPartial; }));
            m_TradePaneColumns.Add(new FillColumn("OrdType", delegate (TT_Fill fill) { return fill.OrdType; }));
            m_TradePaneColumns.Add(new FillColumn("Modifier", delegate (TT_Fill fill) { return fill.Modifier; }));
            m_TradePaneColumns.Add(new FillColumn("Route", delegate (TT_Fill fill) { return fill.Route; }));
            m_TradePaneColumns.Add(new FillColumn("PositionEffect", delegate (TT_Fill fill) { return fill.PositionEffect; }));
            m_TradePaneColumns.Add(new FillColumn("Broker", delegate (TT_Fill fill) { return fill.Broker; }));
            m_TradePaneColumns.Add(new FillColumn("Account", delegate (TT_Fill fill) { return fill.Account; }));
            m_TradePaneColumns.Add(new FillColumn("AccountType", delegate (TT_Fill fill) { return fill.AccountType; }));
            m_TradePaneColumns.Add(new FillColumn("GiveUp", delegate (TT_Fill fill) { return fill.GiveUp; }));
            m_TradePaneColumns.Add(new FillColumn("TextA", delegate (TT_Fill fill) { return fill.TextA; }));
            m_TradePaneColumns.Add(new FillColumn("TextB", delegate (TT_Fill fill) { return fill.TextB; }));
            m_TradePaneColumns.Add(new FillColumn("TextC", delegate (TT_Fill fill) { return fill.TextC; }));
            m_TradePaneColumns.Add(new FillColumn("TextTT", delegate (TT_Fill fill) { return fill.TextTT; }));
            m_TradePaneColumns.Add(new FillColumn("Originator", delegate (TT_Fill fill) { return fill.Originator; }));
            m_TradePaneColumns.Add(new FillColumn("CurrentUser", delegate (TT_Fill fill) { return fill.CurrentUser; }));
            m_TradePaneColumns.Add(new FillColumn("ClientOrderID", delegate (TT_Fill fill) { return fill.ClientOrderID; }));
            m_TradePaneColumns.Add(new FillColumn("ParentOrderID", delegate (TT_Fill fill) { return fill.ParentOrderID; }));
            m_TradePaneColumns.Add(new FillColumn("OmaOrderID", delegate (TT_Fill fill) { return fill.OmaOrderID; }));
            m_TradePaneColumns.Add(new FillColumn("ExchangeOrderID", delegate (TT_Fill fill) { return fill.ExchangeOrderID; }));
            m_TradePaneColumns.Add(new FillColumn("ExchangeTransactionID", delegate (TT_Fill fill) { return fill.ExchangeTransactionID; }));
            m_TradePaneColumns.Add(new FillColumn("ExchangeAccount", delegate (TT_Fill fill) { return fill.ExchangeAccount; }));
            m_TradePaneColumns.Add(new FillColumn("ExchangeDate", delegate (TT_Fill fill) { return fill.ExchangeDate; }));
            m_TradePaneColumns.Add(new FillColumn("ExchangeTime", delegate (TT_Fill fill) { return fill.ExchangeTime; }));
            m_TradePaneColumns.Add(new FillColumn("ManualFill", delegate (TT_Fill fill) { return fill.ManualFill; }));
            m_TradePaneColumns.Add(new FillColumn("Symbol", delegate (TT_Fill fill) { return fill.Symbol; }));
            m_TradePaneColumns.Add(new FillColumn("ProductType", delegate (TT_Fill fill) { return fill.ProductType; }));
            m_TradePaneColumns.Add(new FillColumn("FillType", delegate (TT_Fill fill) { return fill.FillType; }));
            m_TradePaneColumns.Add(new FillColumn("ExecQty", delegate (TT_Fill fill) { return fill.ExecQty; }));
            m_TradePaneColumns.Add(new FillColumn("WorkQty", delegate (TT_Fill fill) { return fill.WorkQty; }));
            m_TradePaneColumns.Add(new FillColumn("AggressorFlag", delegate (TT_Fill fill) { return fill.AggressorFlag; }));
            m_TradePaneColumns.Add(new FillColumn("ConnectionId", delegate (TT_Fill fill) { return fill.ConnectionId; }));
            m_TradePaneColumns.Add(new FillColumn("PutCall", delegate (TT_Fill fill) { return fill.PutCall; }));
            m_TradePaneColumns.Add(new FillColumn("Strike", delegate (TT_Fill fill) { return fill.Strike; }));
            m_TradePaneColumns.Add(new FillColumn("OrderOrigination", delegate (TT_Fill fill) { return fill.OrderOrigination; }));
            m_TradePaneColumns.Add(new FillColumn("TradingCapacity", delegate (TT_Fill fill) { return fill.TradingCapacity; }));
            m_TradePaneColumns.Add(new FillColumn("LiquidityProvision", delegate (TT_Fill fill) { return fill.LiquidityProvision; }));
            m_TradePaneColumns.Add(new FillColumn("CommodityDerivativeIndicator", delegate (TT_Fill fill) { return fill.CommodityDerivativeIndicator; }));
            m_TradePaneColumns.Add(new FillColumn("InvestDec", delegate (TT_Fill fill) { return fill.InvestDec; }));
            m_TradePaneColumns.Add(new FillColumn("ExecDec", delegate (TT_Fill fill) { return fill.ExecDec; }));
            m_TradePaneColumns.Add(new FillColumn("ClientID", delegate (TT_Fill fill) { return fill.ClientID; }));
        }


        private List<FillColumn> GetReportItems()
        {
            List<FillColumn> list = new List<FillColumn>();
            var selected_columns = clbColumns.SelectedItems;
            var selectEnumerator = selected_columns.GetEnumerator();

            for (int i = 0; i < clbColumns.CheckedItems.Count; i++)
            {
                list.Add((FillColumn)clbColumns.CheckedItems[i]);
            }

            return list;
        }

        private String GetCSVHeader()
        {
            String header = "";
            var selected_columns = clbColumns.SelectedItems;

            var selectEnumerator = selected_columns.GetEnumerator();
            for(int i = 0; i < clbColumns.CheckedItems.Count; i++)
            {
                header += clbColumns.CheckedItems[i].ToString() + ",";
            }

            header += Environment.NewLine;
            return header;
        }

        private void dtpStartDate_ValueChanged(object sender, EventArgs e)
        {
            // DateTimePickers do not allow a null value so we signal whether 
            // the date time picker is being used or not by displaying the date
            // or making it blank.
            if (dtpStartDate.Checked)
                dtpStartDate.CustomFormat = "MM/dd/yyyy";
            else
                dtpStartDate.CustomFormat = " ";
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            //DialogResult result = fdOutFile.ShowDialog();
            DialogResult result = fbdOutFolder.ShowDialog();
            if (result == DialogResult.OK)
            {
                string file = fbdOutFolder.SelectedPath;
                try
                {
                    txtOutput.Text = file;
                }
                catch (IOException)
                {
                }
            }
        }

        private void LoadSettings()
        {
            txtURL.Text = Properties.filldownload.Default.ApiURL;
            txtSecret.Text = Properties.filldownload.Default.Secret;
            txtEnvironment.Text = Properties.filldownload.Default.Env;

            txtFrequency.Text = Properties.filldownload.Default.Frequency;

            txtOutput.Text = Properties.filldownload.Default.OutputFolder;

            dtpEndTime.Value = dtpEndTime.Value.Date + Properties.filldownload.Default.EndTime;
            dtpStartTime.Value = dtpEndTime.Value.Date + Properties.filldownload.Default.StartTime;

            if (Properties.filldownload.Default.StartDate != default(DateTime))
            {
                dtpStartDate.Checked = true;
                dtpStartDate.Value = Properties.filldownload.Default.StartDate;
            }

            chkSunday.Checked = Properties.filldownload.Default.RunSunday;
            chkMonday.Checked = Properties.filldownload.Default.RunMonday;
            chkTuesday.Checked = Properties.filldownload.Default.RunTuesday;
            chkWednesday.Checked = Properties.filldownload.Default.RunWednesday;
            chkThursday.Checked = Properties.filldownload.Default.RunThurday;
            chkFriday.Checked = Properties.filldownload.Default.RunFriday;
            chkSaturday.Checked = Properties.filldownload.Default.RunSaturday;

            cbFileMode.SelectedItem = Properties.filldownload.Default.FileMode;

            var columns = Properties.filldownload.Default.Columns;
            if (columns != null)
            {
                for (int i = 0; i < clbColumns.Items.Count; ++i)
                {
                    clbColumns.SetItemChecked(i, columns.Contains(clbColumns.Items[i].ToString()));
                }
            }
        }

        private void btnSaveSettings_Click(object sender, EventArgs e)
        {
            Properties.filldownload.Default.ApiURL = txtURL.Text;
            Properties.filldownload.Default.Secret = txtSecret.Text;
            Properties.filldownload.Default.Env = txtEnvironment.Text;

            Properties.filldownload.Default.Frequency = txtFrequency.Text;

            Properties.filldownload.Default.OutputFolder = txtOutput.Text;

            Properties.filldownload.Default.StartTime = dtpStartTime.Value.TimeOfDay;
            Properties.filldownload.Default.EndTime = dtpEndTime.Value.TimeOfDay;

            if (dtpStartDate.Checked)
                Properties.filldownload.Default.StartDate = dtpStartDate.Value;
            else
                Properties.filldownload.Default.StartDate = default(DateTime);

            Properties.filldownload.Default.RunSunday = chkSunday.Checked;
            Properties.filldownload.Default.RunMonday = chkMonday.Checked;
            Properties.filldownload.Default.RunTuesday = chkTuesday.Checked;
            Properties.filldownload.Default.RunWednesday = chkWednesday.Checked;
            Properties.filldownload.Default.RunThurday = chkThursday.Checked;
            Properties.filldownload.Default.RunFriday = chkFriday.Checked;
            Properties.filldownload.Default.RunSaturday = chkSaturday.Checked;

            Properties.filldownload.Default.FileMode = (FileMode)cbFileMode.SelectedItem;

            var columns = new System.Collections.Specialized.StringCollection();
            foreach(var col in clbColumns.CheckedItems)
            {
                columns.Add(col.ToString());
            }
            Properties.filldownload.Default.Columns = columns;

            Properties.filldownload.Default.Save();
        }

        private void FrmFillDownload_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.All;
            else
                e.Effect = DragDropEffects.None;
        }

        private void Debug_DragDrop(object sender, DragEventArgs e)
        {
            if (!RestManager.IsAuthorized())
            {
                string app_key = txtSecret.Text.Split(':')[0];
                string app_secret = txtSecret.Text;
                RestManager.Init(app_key, app_secret, txtEnvironment.Text);
                if (!RestManager.IsAuthorized())
                {
                    MessageBox.Show("Rest API was not able to log in with provided App Key and Secret");
                    return;
                }
                else
                {
                    FDLog.LogMessage("Successfully logged in with app key and secret");
                    RestManager.OnTokenError += RestManager_OnTokenError;
                }
            }
            string[] s = (string[])e.Data.GetData(DataFormats.FileDrop, false);

            Action debug = () => { DebugFillFiles(e); };
            this.BeginInvoke(debug);
        }

        private void DebugFillFiles(DragEventArgs e)
        {
            string[] s = (string[])e.Data.GetData(DataFormats.FileDrop, false);
            int i;
            var fields = GetReportItems();
            for (i = 0; i < s.Length; i++)
            {
                Console.WriteLine();

                using (StreamReader file = File.OpenText(s[i]))
                using (JsonTextReader reader = new JsonTextReader(file))
                {
                    TT_Fill fill = new TT_Fill(JToken.ReadFrom(reader));
                    FDLog.LogMessage(String.Format("Parsing fill from file {0}", s[i]));
                    string row = "";
                    foreach (var field in fields)
                    {
                        try
                        {
                            row += field.DisplayField(fill) + ",";
                        }
                        catch (Exception ex)
                        {
                            row += ",";
                            FDLog.LogError("Error parsing fill column " + field.ColumnName + " for fill " + fill.RecordID + Environment.NewLine + ex.Message);
                        }
                    }
                    FDLog.LogMessage(row);
                }
            }
        }
    }

    public delegate String ColumnDisplay(TT_Fill display);

    public class FillColumn
    {
        public String ColumnName;
        public String DisplayField(TT_Fill fill)
        {
            return fieldDisplay(fill);
        }
        ColumnDisplay fieldDisplay;

        public FillColumn(String name, ColumnDisplay display)
        {
            ColumnName = name;
            fieldDisplay = display;
        }

        public override string ToString()
        {
            return ColumnName;
        }
    }
}
