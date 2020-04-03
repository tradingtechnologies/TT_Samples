namespace FillDownload
{
    partial class FrmFillDownload
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmFillDownload));
            this.btnStart = new System.Windows.Forms.Button();
            this.txtSecret = new System.Windows.Forms.TextBox();
            this.txtOutput = new System.Windows.Forms.TextBox();
            this.lblSecret = new System.Windows.Forms.Label();
            this.lblOutputFile = new System.Windows.Forms.Label();
            this.lblInterval = new System.Windows.Forms.Label();
            this.txtFrequency = new System.Windows.Forms.TextBox();
            this.lblStartTime = new System.Windows.Forms.Label();
            this.lblEndTime = new System.Windows.Forms.Label();
            this.lblEnvironment = new System.Windows.Forms.Label();
            this.txtEnvironment = new System.Windows.Forms.TextBox();
            this.dtpStartDate = new System.Windows.Forms.DateTimePicker();
            this.dtpStartTime = new System.Windows.Forms.DateTimePicker();
            this.label2 = new System.Windows.Forms.Label();
            this.dtpEndTime = new System.Windows.Forms.DateTimePicker();
            this.clbColumns = new System.Windows.Forms.CheckedListBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.txtURL = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.btnSaveSettings = new System.Windows.Forms.Button();
            this.chkMonday = new System.Windows.Forms.CheckBox();
            this.chkTuesday = new System.Windows.Forms.CheckBox();
            this.chkWednesday = new System.Windows.Forms.CheckBox();
            this.chkSunday = new System.Windows.Forms.CheckBox();
            this.chkFriday = new System.Windows.Forms.CheckBox();
            this.chkSaturday = new System.Windows.Forms.CheckBox();
            this.chkThursday = new System.Windows.Forms.CheckBox();
            this.label8 = new System.Windows.Forms.Label();
            this.fbdOutFolder = new System.Windows.Forms.FolderBrowserDialog();
            this.cbFileMode = new System.Windows.Forms.ComboBox();
            this.lblFileMode = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnStart
            // 
            this.btnStart.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(150)))), ((int)(((byte)(214)))));
            this.btnStart.FlatAppearance.BorderSize = 0;
            this.btnStart.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnStart.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnStart.ForeColor = System.Drawing.Color.White;
            this.btnStart.Location = new System.Drawing.Point(427, 503);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(305, 40);
            this.btnStart.TabIndex = 2;
            this.btnStart.Text = "Begin Downloading";
            this.btnStart.UseVisualStyleBackColor = false;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // txtSecret
            // 
            this.txtSecret.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtSecret.Location = new System.Drawing.Point(22, 215);
            this.txtSecret.Name = "txtSecret";
            this.txtSecret.Size = new System.Drawing.Size(210, 26);
            this.txtSecret.TabIndex = 4;
            // 
            // txtOutput
            // 
            this.txtOutput.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtOutput.Location = new System.Drawing.Point(522, 353);
            this.txtOutput.Name = "txtOutput";
            this.txtOutput.Size = new System.Drawing.Size(130, 26);
            this.txtOutput.TabIndex = 5;
            this.txtOutput.Text = ".\\";
            // 
            // lblSecret
            // 
            this.lblSecret.AutoSize = true;
            this.lblSecret.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSecret.Location = new System.Drawing.Point(19, 192);
            this.lblSecret.Name = "lblSecret";
            this.lblSecret.Size = new System.Drawing.Size(75, 18);
            this.lblSecret.TabIndex = 7;
            this.lblSecret.Text = "App Secret";
            // 
            // lblOutputFile
            // 
            this.lblOutputFile.AutoSize = true;
            this.lblOutputFile.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblOutputFile.Location = new System.Drawing.Point(519, 325);
            this.lblOutputFile.Name = "lblOutputFile";
            this.lblOutputFile.Size = new System.Drawing.Size(107, 18);
            this.lblOutputFile.TabIndex = 8;
            this.lblOutputFile.Text = "Output Location";
            // 
            // lblInterval
            // 
            this.lblInterval.AutoSize = true;
            this.lblInterval.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblInterval.Location = new System.Drawing.Point(268, 396);
            this.lblInterval.Name = "lblInterval";
            this.lblInterval.Size = new System.Drawing.Size(137, 18);
            this.lblInterval.TabIndex = 10;
            this.lblInterval.Text = "Frequency (minutes)";
            // 
            // txtFrequency
            // 
            this.txtFrequency.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtFrequency.Location = new System.Drawing.Point(271, 421);
            this.txtFrequency.Name = "txtFrequency";
            this.txtFrequency.Size = new System.Drawing.Size(210, 26);
            this.txtFrequency.TabIndex = 9;
            this.txtFrequency.Text = "1";
            // 
            // lblStartTime
            // 
            this.lblStartTime.AutoSize = true;
            this.lblStartTime.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblStartTime.Location = new System.Drawing.Point(268, 194);
            this.lblStartTime.Name = "lblStartTime";
            this.lblStartTime.Size = new System.Drawing.Size(132, 18);
            this.lblStartTime.TabIndex = 12;
            this.lblStartTime.Text = "Start Time (HH:MM)";
            // 
            // lblEndTime
            // 
            this.lblEndTime.AutoSize = true;
            this.lblEndTime.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblEndTime.Location = new System.Drawing.Point(268, 259);
            this.lblEndTime.Name = "lblEndTime";
            this.lblEndTime.Size = new System.Drawing.Size(126, 18);
            this.lblEndTime.TabIndex = 14;
            this.lblEndTime.Text = "End Time (HH:MM)";
            // 
            // lblEnvironment
            // 
            this.lblEnvironment.AutoSize = true;
            this.lblEnvironment.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblEnvironment.Location = new System.Drawing.Point(19, 259);
            this.lblEnvironment.Name = "lblEnvironment";
            this.lblEnvironment.Size = new System.Drawing.Size(88, 18);
            this.lblEnvironment.TabIndex = 16;
            this.lblEnvironment.Text = "Environment";
            // 
            // txtEnvironment
            // 
            this.txtEnvironment.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtEnvironment.Location = new System.Drawing.Point(22, 280);
            this.txtEnvironment.Name = "txtEnvironment";
            this.txtEnvironment.Size = new System.Drawing.Size(210, 26);
            this.txtEnvironment.TabIndex = 15;
            // 
            // dtpStartDate
            // 
            this.dtpStartDate.Checked = false;
            this.dtpStartDate.CustomFormat = " ";
            this.dtpStartDate.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dtpStartDate.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dtpStartDate.Location = new System.Drawing.Point(271, 353);
            this.dtpStartDate.Name = "dtpStartDate";
            this.dtpStartDate.ShowCheckBox = true;
            this.dtpStartDate.Size = new System.Drawing.Size(210, 26);
            this.dtpStartDate.TabIndex = 17;
            this.dtpStartDate.ValueChanged += new System.EventHandler(this.dtpStartDate_ValueChanged);
            // 
            // dtpStartTime
            // 
            this.dtpStartTime.CalendarFont = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dtpStartTime.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dtpStartTime.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dtpStartTime.Location = new System.Drawing.Point(271, 215);
            this.dtpStartTime.Name = "dtpStartTime";
            this.dtpStartTime.ShowUpDown = true;
            this.dtpStartTime.Size = new System.Drawing.Size(210, 26);
            this.dtpStartTime.TabIndex = 18;
            this.dtpStartTime.Value = new System.DateTime(2018, 11, 30, 9, 0, 0, 0);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(268, 332);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 18);
            this.label2.TabIndex = 19;
            this.label2.Text = "Start Date";
            // 
            // dtpEndTime
            // 
            this.dtpEndTime.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.dtpEndTime.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dtpEndTime.Location = new System.Drawing.Point(271, 280);
            this.dtpEndTime.Name = "dtpEndTime";
            this.dtpEndTime.ShowUpDown = true;
            this.dtpEndTime.Size = new System.Drawing.Size(210, 26);
            this.dtpEndTime.TabIndex = 24;
            this.dtpEndTime.Value = new System.DateTime(2018, 11, 30, 17, 0, 0, 0);
            // 
            // clbColumns
            // 
            this.clbColumns.CheckOnClick = true;
            this.clbColumns.Font = new System.Drawing.Font("Calibri", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.clbColumns.FormattingEnabled = true;
            this.clbColumns.Location = new System.Drawing.Point(522, 143);
            this.clbColumns.Name = "clbColumns";
            this.clbColumns.Size = new System.Drawing.Size(210, 166);
            this.clbColumns.TabIndex = 25;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(519, 119);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(101, 18);
            this.label4.TabIndex = 26;
            this.label4.Text = "Fill Parameters";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Calibri", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.SystemColors.InactiveCaptionText;
            this.label1.Location = new System.Drawing.Point(22, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(200, 36);
            this.label1.TabIndex = 27;
            this.label1.Text = "Fill Downloader";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(19, 122);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 18);
            this.label3.TabIndex = 29;
            this.label3.Text = "API URL";
            // 
            // txtURL
            // 
            this.txtURL.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtURL.Location = new System.Drawing.Point(22, 143);
            this.txtURL.Name = "txtURL";
            this.txtURL.Size = new System.Drawing.Size(210, 26);
            this.txtURL.TabIndex = 28;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(19, 463);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(715, 13);
            this.label5.TabIndex = 30;
            this.label5.Text = resources.GetString("label5.Text");
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Calibri", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(25, 66);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(542, 15);
            this.label6.TabIndex = 31;
            this.label6.Text = "Enter your API credentials below to get started. Please note that the higher the " +
    "frequency the more ";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Calibri", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(25, 91);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(550, 15);
            this.label7.TabIndex = 32;
            this.label7.Text = "API calls will be made and will count towards your daily quota based upon your su" +
    "bscription plan.";
            // 
            // btnBrowse
            // 
            this.btnBrowse.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnBrowse.Location = new System.Drawing.Point(652, 352);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(80, 28);
            this.btnBrowse.TabIndex = 33;
            this.btnBrowse.Text = "Browse...";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.btnBrowse_Click);
            // 
            // btnSaveSettings
            // 
            this.btnSaveSettings.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSaveSettings.Location = new System.Drawing.Point(22, 503);
            this.btnSaveSettings.Name = "btnSaveSettings";
            this.btnSaveSettings.Size = new System.Drawing.Size(125, 40);
            this.btnSaveSettings.TabIndex = 34;
            this.btnSaveSettings.Text = "Save Settings";
            this.btnSaveSettings.UseVisualStyleBackColor = true;
            this.btnSaveSettings.Click += new System.EventHandler(this.btnSaveSettings_Click);
            // 
            // chkMonday
            // 
            this.chkMonday.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkMonday.BackColor = System.Drawing.Color.White;
            this.chkMonday.Checked = true;
            this.chkMonday.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkMonday.FlatAppearance.BorderColor = System.Drawing.Color.DarkGray;
            this.chkMonday.FlatAppearance.CheckedBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(150)))), ((int)(((byte)(214)))));
            this.chkMonday.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.chkMonday.ForeColor = System.Drawing.Color.White;
            this.chkMonday.Location = new System.Drawing.Point(271, 140);
            this.chkMonday.Name = "chkMonday";
            this.chkMonday.Size = new System.Drawing.Size(30, 30);
            this.chkMonday.TabIndex = 35;
            this.chkMonday.Text = "M";
            this.chkMonday.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkMonday.UseVisualStyleBackColor = false;
            // 
            // chkTuesday
            // 
            this.chkTuesday.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkTuesday.BackColor = System.Drawing.Color.White;
            this.chkTuesday.Checked = true;
            this.chkTuesday.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkTuesday.FlatAppearance.BorderColor = System.Drawing.Color.DarkGray;
            this.chkTuesday.FlatAppearance.CheckedBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(150)))), ((int)(((byte)(214)))));
            this.chkTuesday.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.chkTuesday.ForeColor = System.Drawing.Color.White;
            this.chkTuesday.Location = new System.Drawing.Point(300, 140);
            this.chkTuesday.Name = "chkTuesday";
            this.chkTuesday.Size = new System.Drawing.Size(30, 30);
            this.chkTuesday.TabIndex = 36;
            this.chkTuesday.Text = "T";
            this.chkTuesday.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkTuesday.UseVisualStyleBackColor = false;
            // 
            // chkWednesday
            // 
            this.chkWednesday.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkWednesday.BackColor = System.Drawing.Color.White;
            this.chkWednesday.Checked = true;
            this.chkWednesday.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkWednesday.FlatAppearance.BorderColor = System.Drawing.Color.DarkGray;
            this.chkWednesday.FlatAppearance.CheckedBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(150)))), ((int)(((byte)(214)))));
            this.chkWednesday.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.chkWednesday.ForeColor = System.Drawing.Color.White;
            this.chkWednesday.Location = new System.Drawing.Point(329, 140);
            this.chkWednesday.Name = "chkWednesday";
            this.chkWednesday.Size = new System.Drawing.Size(30, 30);
            this.chkWednesday.TabIndex = 37;
            this.chkWednesday.Text = "W";
            this.chkWednesday.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkWednesday.UseVisualStyleBackColor = false;
            // 
            // chkSunday
            // 
            this.chkSunday.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkSunday.BackColor = System.Drawing.Color.White;
            this.chkSunday.FlatAppearance.BorderColor = System.Drawing.Color.DarkGray;
            this.chkSunday.FlatAppearance.CheckedBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(150)))), ((int)(((byte)(214)))));
            this.chkSunday.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.chkSunday.ForeColor = System.Drawing.Color.White;
            this.chkSunday.Location = new System.Drawing.Point(445, 140);
            this.chkSunday.Name = "chkSunday";
            this.chkSunday.Size = new System.Drawing.Size(30, 30);
            this.chkSunday.TabIndex = 38;
            this.chkSunday.Text = "U";
            this.chkSunday.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkSunday.UseVisualStyleBackColor = false;
            // 
            // chkFriday
            // 
            this.chkFriday.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkFriday.BackColor = System.Drawing.Color.White;
            this.chkFriday.Checked = true;
            this.chkFriday.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkFriday.FlatAppearance.BorderColor = System.Drawing.Color.DarkGray;
            this.chkFriday.FlatAppearance.CheckedBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(150)))), ((int)(((byte)(214)))));
            this.chkFriday.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.chkFriday.ForeColor = System.Drawing.Color.White;
            this.chkFriday.Location = new System.Drawing.Point(387, 140);
            this.chkFriday.Name = "chkFriday";
            this.chkFriday.Size = new System.Drawing.Size(30, 30);
            this.chkFriday.TabIndex = 39;
            this.chkFriday.Text = "F";
            this.chkFriday.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkFriday.UseVisualStyleBackColor = false;
            // 
            // chkSaturday
            // 
            this.chkSaturday.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkSaturday.BackColor = System.Drawing.Color.White;
            this.chkSaturday.FlatAppearance.BorderColor = System.Drawing.Color.DarkGray;
            this.chkSaturday.FlatAppearance.CheckedBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(150)))), ((int)(((byte)(214)))));
            this.chkSaturday.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.chkSaturday.ForeColor = System.Drawing.Color.White;
            this.chkSaturday.Location = new System.Drawing.Point(416, 140);
            this.chkSaturday.Name = "chkSaturday";
            this.chkSaturday.Size = new System.Drawing.Size(30, 30);
            this.chkSaturday.TabIndex = 40;
            this.chkSaturday.Text = "S";
            this.chkSaturday.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkSaturday.UseVisualStyleBackColor = false;
            // 
            // chkThursday
            // 
            this.chkThursday.Appearance = System.Windows.Forms.Appearance.Button;
            this.chkThursday.BackColor = System.Drawing.Color.White;
            this.chkThursday.Checked = true;
            this.chkThursday.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkThursday.FlatAppearance.BorderColor = System.Drawing.Color.DarkGray;
            this.chkThursday.FlatAppearance.CheckedBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(150)))), ((int)(((byte)(214)))));
            this.chkThursday.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.chkThursday.ForeColor = System.Drawing.Color.White;
            this.chkThursday.Location = new System.Drawing.Point(358, 140);
            this.chkThursday.Name = "chkThursday";
            this.chkThursday.Size = new System.Drawing.Size(30, 30);
            this.chkThursday.TabIndex = 41;
            this.chkThursday.Text = "R";
            this.chkThursday.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.chkThursday.UseVisualStyleBackColor = false;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(268, 119);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(116, 18);
            this.label8.TabIndex = 42;
            this.label8.Text = "Days of the Week";
            // 
            // fbdOutFolder
            // 
            this.fbdOutFolder.RootFolder = System.Environment.SpecialFolder.UserProfile;
            this.fbdOutFolder.SelectedPath = ".\\";
            // 
            // cbFileMode
            // 
            this.cbFileMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbFileMode.Font = new System.Drawing.Font("Calibri", 11.25F);
            this.cbFileMode.FormattingEnabled = true;
            this.cbFileMode.Location = new System.Drawing.Point(22, 352);
            this.cbFileMode.Name = "cbFileMode";
            this.cbFileMode.Size = new System.Drawing.Size(210, 26);
            this.cbFileMode.TabIndex = 43;
            // 
            // lblFileMode
            // 
            this.lblFileMode.AutoSize = true;
            this.lblFileMode.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblFileMode.Location = new System.Drawing.Point(19, 331);
            this.lblFileMode.Name = "lblFileMode";
            this.lblFileMode.Size = new System.Drawing.Size(71, 18);
            this.lblFileMode.TabIndex = 44;
            this.lblFileMode.Text = "File Mode";
            // 
            // FrmFillDownload
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(764, 571);
            this.Controls.Add(this.lblFileMode);
            this.Controls.Add(this.cbFileMode);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.chkThursday);
            this.Controls.Add(this.chkSaturday);
            this.Controls.Add(this.chkFriday);
            this.Controls.Add(this.chkSunday);
            this.Controls.Add(this.chkWednesday);
            this.Controls.Add(this.chkTuesday);
            this.Controls.Add(this.chkMonday);
            this.Controls.Add(this.btnSaveSettings);
            this.Controls.Add(this.btnBrowse);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtURL);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.clbColumns);
            this.Controls.Add(this.dtpEndTime);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.dtpStartTime);
            this.Controls.Add(this.dtpStartDate);
            this.Controls.Add(this.lblEnvironment);
            this.Controls.Add(this.txtEnvironment);
            this.Controls.Add(this.lblEndTime);
            this.Controls.Add(this.lblStartTime);
            this.Controls.Add(this.lblInterval);
            this.Controls.Add(this.txtFrequency);
            this.Controls.Add(this.lblOutputFile);
            this.Controls.Add(this.lblSecret);
            this.Controls.Add(this.txtOutput);
            this.Controls.Add(this.txtSecret);
            this.Controls.Add(this.btnStart);
            this.Cursor = System.Windows.Forms.Cursors.Default;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FrmFillDownload";
            this.Load += new System.EventHandler(this.Form_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        private void Chk_CheckedChanged(object sender, System.EventArgs e)
        {
            setBoxStateColors((System.Windows.Forms.CheckBox)sender);
        }

        private static void setBoxStateColors(System.Windows.Forms.CheckBox box)
        {
            if (box.Checked)
                box.ForeColor = System.Drawing.Color.White;
            else
                box.ForeColor = System.Drawing.Color.Black;
        }

        #endregion
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.TextBox txtSecret;
        private System.Windows.Forms.TextBox txtOutput;
        private System.Windows.Forms.Label lblSecret;
        private System.Windows.Forms.Label lblOutputFile;
        private System.Windows.Forms.Label lblInterval;
        private System.Windows.Forms.TextBox txtFrequency;
        private System.Windows.Forms.Label lblStartTime;
        private System.Windows.Forms.Label lblEndTime;
        private System.Windows.Forms.Label lblEnvironment;
        private System.Windows.Forms.TextBox txtEnvironment;
        private System.Windows.Forms.DateTimePicker dtpStartDate;
        private System.Windows.Forms.DateTimePicker dtpStartTime;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.DateTimePicker dtpEndTime;
        private System.Windows.Forms.CheckedListBox clbColumns;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtURL;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.Button btnSaveSettings;
        private System.Windows.Forms.CheckBox chkMonday;
        private System.Windows.Forms.CheckBox chkTuesday;
        private System.Windows.Forms.CheckBox chkWednesday;
        private System.Windows.Forms.CheckBox chkSunday;
        private System.Windows.Forms.CheckBox chkFriday;
        private System.Windows.Forms.CheckBox chkSaturday;
        private System.Windows.Forms.CheckBox chkThursday;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.FolderBrowserDialog fbdOutFolder;
        private System.Windows.Forms.ComboBox cbFileMode;
        private System.Windows.Forms.Label lblFileMode;
    }
}

