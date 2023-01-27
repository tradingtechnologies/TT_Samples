namespace TTNETAPI_Sample_GUI_Market_Explorer
{
    partial class MarketExplorer
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
            this.listViewMarketList = new System.Windows.Forms.ListView();
            this.listViewProductTypeList = new System.Windows.Forms.ListView();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.treeViewProductList = new System.Windows.Forms.TreeView();
            this.statusLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // listViewMarketList
            // 
            this.listViewMarketList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.listViewMarketList.Location = new System.Drawing.Point(3, 12);
            this.listViewMarketList.Name = "listViewMarketList";
            this.listViewMarketList.Size = new System.Drawing.Size(127, 229);
            this.listViewMarketList.TabIndex = 0;
            this.listViewMarketList.UseCompatibleStateImageBehavior = false;
            this.listViewMarketList.View = System.Windows.Forms.View.Details;
            this.listViewMarketList.SelectedIndexChanged += new System.EventHandler(this.listViewMarketList_SelectedIndexChanged);
            // 
            // listViewProductTypeList
            // 
            this.listViewProductTypeList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.listViewProductTypeList.Location = new System.Drawing.Point(3, 247);
            this.listViewProductTypeList.Name = "listViewProductTypeList";
            this.listViewProductTypeList.Size = new System.Drawing.Size(127, 229);
            this.listViewProductTypeList.TabIndex = 1;
            this.listViewProductTypeList.UseCompatibleStateImageBehavior = false;
            this.listViewProductTypeList.View = System.Windows.Forms.View.Details;
            this.listViewProductTypeList.SelectedIndexChanged += new System.EventHandler(this.listViewProductTypeList_SelectedIndexChanged);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 486);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(395, 22);
            this.statusStrip1.TabIndex = 3;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // treeViewProductList
            // 
            this.treeViewProductList.Location = new System.Drawing.Point(136, 12);
            this.treeViewProductList.Name = "treeViewProductList";
            this.treeViewProductList.ShowNodeToolTips = true;
            this.treeViewProductList.Size = new System.Drawing.Size(259, 464);
            this.treeViewProductList.TabIndex = 5;
            this.treeViewProductList.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeViewProductList_BeforeExpand);
            // 
            // statusLabel
            // 
            this.statusLabel.AutoSize = true;
            this.statusLabel.Location = new System.Drawing.Point(4, 492);
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(0, 13);
            this.statusLabel.TabIndex = 6;
            // 
            // MarketExplorer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(395, 508);
            this.Controls.Add(this.statusLabel);
            this.Controls.Add(this.treeViewProductList);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.listViewProductTypeList);
            this.Controls.Add(this.listViewMarketList);
            this.MaximumSize = new System.Drawing.Size(411, 547);
            this.MinimumSize = new System.Drawing.Size(411, 547);
            this.Name = "MarketExplorer";
            this.Text = "Market Explorer";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listViewMarketList;
        private System.Windows.Forms.ListView listViewProductTypeList;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.TreeView treeViewProductList;
        private System.Windows.Forms.Label statusLabel;
    }
}

