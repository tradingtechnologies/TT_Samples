// **********************************************************************************************************************
//
//	Copyright © 2005-2019 Trading Technologies International, Inc.
//	All Rights Reserved Worldwide
//
// 	* * * S T R I C T L Y   P R O P R I E T A R Y * * *
//
//  WARNING: This file and all related programs (including any computer programs, example programs, and all source code) 
//  are the exclusive property of Trading Technologies International, Inc. (“TT”), are protected by copyright law and 
//  international treaties, and are for use only by those with the express written permission from TT.  Unauthorized 
//  possession, reproduction, distribution, use or disclosure of this file and any related program (or document) derived 
//  from it is prohibited by State and Federal law, and by local law outside of the U.S. and may result in severe civil 
//  and criminal penalties.
//
// ************************************************************************************************************************

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Windows;
using System.Windows.Data;
using System.Runtime.CompilerServices;
using tt_net_sdk;
using System.Text.RegularExpressions;

namespace TTNETAPI_Sample_WPF_VolumeRatio
{
    #region TreeRow Implementation
    public interface ITreeRow : INotifyPropertyChanged
    {
        string MarketName { get; }
        string ProductName { get; }
        string ContractAlias { get; }
        decimal Volume { get; }
        long NumNewOrders { get; }
        long NumModifications { get; }
        long NumCancellations { get; }
        int Score { get; }
        decimal Ratio { get; }
        bool IsExpanded { get; set; }
    }

    public class MarketTreeRow : ITreeRow
    {
        public MarketTreeRow(Instrument instr)
        {
            m_market = instr.Product.Market;
            IsExpanded = false;
            CreateOrGetExistingChild(instr);
        }

        #region Tree Column Properties
        public string MarketName => m_market.Name;
        public string ProductName => string.Empty;
        public string ContractAlias => string.Empty;
        public long NumNewOrders => this.Children.Sum(x => x.NumNewOrders);
        public long NumModifications => this.Children.Sum(x => x.NumModifications);
        public long NumCancellations => this.Children.Sum(x => x.NumCancellations);
        public decimal Volume => this.Children.Sum(x => x.Volume);
        public int Score => this.Children.Sum(x => x.Score);
        public decimal Ratio => (this.Volume == 0) ? 0 : this.Score / this.Volume;
        #endregion

        #region TreeViewItem Methods/Properties
        public MarketId MarketId => m_market.MarketId;
        public ObservableCollection<ITreeRow> Children { get; } = new ObservableCollection<ITreeRow>();

        public bool IsExpanded
        {
            get { return m_isExpanded; }
            set
            {
                m_isExpanded = value;
                NotifyPropertyChanged();
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        #region Public Methods
        public ProductTreeRow CreateOrGetExistingChild(Instrument instr)
        {
            var child = this.Children.FirstOrDefault(x => x.ProductName == instr.Product.Key.Name);
            if (child == null)
            {
                this.Children.Add(new ProductTreeRow(this, instr));
                child = this.Children.Last();

                child.PropertyChanged += (o, args) =>
                {
                    NotifyPropertyChanged(args.PropertyName);
                    NotifyPropertyChanged("Score");
                    NotifyPropertyChanged("Ratio");
                };
            }

            return child as ProductTreeRow;
        }

        public void ResetWeights(int nos_weight, int chg_weight, int cxl_weight)
        {
            foreach (var child in Children)
            {
                var contractRow = child as ProductTreeRow;
                contractRow.ResetWeights(nos_weight, chg_weight, cxl_weight);
            }
        }
        #endregion

        #region Private Members
        private readonly Market m_market;
        private bool m_isExpanded = false;
        #endregion
    }

    public class ProductTreeRow : ITreeRow
    {
        public ProductTreeRow(MarketTreeRow parent, Instrument instr)
        {
            m_parent = parent;
            m_productKey = instr.Product.Key;
            CreateOrGetExistingChild(instr);
        }

        #region Tree Column Properties
        public string MarketName => string.Empty;
        public string ProductName => m_productKey.Name;
        public string ContractAlias => string.Empty;
        public decimal Volume => this.Children.Sum(x => x.Volume);
        public long NumNewOrders => this.Children.Sum(x => x.NumNewOrders);
        public long NumModifications => this.Children.Sum(x => x.NumModifications);
        public long NumCancellations => this.Children.Sum(x => x.NumCancellations);
        public int Score => this.Children.Sum(x => x.Score);
        public decimal Ratio => (this.Volume == 0) ? 0 : this.Score / this.Volume;
        #endregion

        #region TreeViewItem Methods/Properties
        public ulong ProductId => m_productKey.ProductId;
        public ObservableCollection<ITreeRow> Children { get; } = new ObservableCollection<ITreeRow>();

        public bool IsExpanded
        {
            get { return m_isExpanded; }
            set
            {
                m_isExpanded = value;
                this.NotifyPropertyChanged("IsExpanded");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        #region Public Methods
        public ContractTreeRow CreateOrGetExistingChild(Instrument instr)
        {
            var child = this.Children.FirstOrDefault(x => (x as ContractTreeRow).ContractId == instr.InstrumentDetails.Id);
            if (child == null)
            {
                this.Children.Add(new ContractTreeRow(this, instr));
                child = this.Children.Last();

                child.PropertyChanged += (o, args) =>
                {
                    NotifyPropertyChanged(args.PropertyName);
                    NotifyPropertyChanged("Score");
                    NotifyPropertyChanged("Ratio");
                };
            }

            return child as ContractTreeRow;
        }

        public void ResetWeights(int nos_weight, int chg_weight, int cxl_weight)
        {
            foreach(var child in Children)
            {
                var contractRow = child as ContractTreeRow;
                contractRow.ResetWeights(nos_weight, chg_weight, cxl_weight);
            }
        }
        #endregion

        #region Private Members
        private readonly MarketTreeRow m_parent = null;
        private readonly ProductKey m_productKey = ProductKey.Empty;
        private bool m_isExpanded = false;
        #endregion
    }

    public class ContractTreeRow : ITreeRow
    {
        public ContractTreeRow(ProductTreeRow parent, Instrument instr)
        {
            m_parent = parent;
            m_instrKey = instr.Key;
            IsExpanded = false;
        }

        #region Tree Column Properties
        public string MarketName => string.Empty;
        public string ProductName => string.Empty;
        public string ContractAlias => m_instrKey.Alias;

        public decimal Volume
        {
            get { return m_volume; }
            set
            {
                m_volume = value;
                NotifyRatioChanged();
            }
        }

        public long NumNewOrders
        {
            get { return m_numNewOrders; }
            set
            {
                m_numNewOrders = value;
                NotifyRatioChanged();
            }
        }

        public long NumModifications
        {
            get { return m_numModifications; }
            set
            {
                m_numModifications = value;
                NotifyRatioChanged();
            }
        }

        public long NumCancellations
        {
            get { return m_numCancellations; }
            set
            {
                m_numCancellations = value;
                NotifyRatioChanged();
            }
        }

        public int Score => (int)(m_numNewOrders * m_nosWeight) +
                            (int)(m_numModifications * m_chgWeight) +
                            (int)(m_numCancellations * m_cxlWeight);
        public decimal Ratio => (this.Volume == 0) ? 0 : this.Score / this.Volume;
        #endregion

        #region TreeViewItem Methods/Properties
        public ProductTreeRow Parent => m_parent;
        public ulong ContractId => m_instrKey.InstrumentId;

        public bool IsExpanded
        {
            get { return m_isExpanded; }
            set
            {
                m_isExpanded = value;
                NotifyPropertyChanged();
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void NotifyRatioChanged([CallerMemberName] String baseProperty = "")
        {
            if (baseProperty != "")
            {
                NotifyPropertyChanged(baseProperty);
            }
            
            NotifyPropertyChanged("Score");
            NotifyPropertyChanged("Ratio");
        }

        #endregion

        #region Public Methods
        public void ProcessMessage(OrderAddedEventArgs e)
        {
            NumNewOrders++;
        }

        public void ProcessMessage(OrderUpdatedEventArgs e)
        {
            NumModifications++;
        }

        public void ProcessMessage(OrderDeletedEventArgs e)
        {
            NumCancellations++;
        }

        public void ProcessMessage(OrderFilledEventArgs e)
        {
            Volume += e.Fill.Quantity.Value;
        }

        public void ResetWeights(int nos_weight, int chg_weight, int cxl_weight)
        {
            m_nosWeight = nos_weight;
            m_chgWeight = chg_weight;
            m_cxlWeight = cxl_weight;
            NotifyRatioChanged();
        }
        #endregion

        #region Private Members
        private int m_nosWeight = 0;
        private int m_chgWeight = 1;
        private int m_cxlWeight = 3;

        private decimal m_volume = 0;
        private long m_numNewOrders = 0;
        private long m_numModifications = 0;
        private long m_numCancellations = 0;

        private readonly InstrumentKey m_instrKey = InstrumentKey.Empty;
        private readonly ProductTreeRow m_parent = null;
        private bool m_isExpanded = false;
        #endregion
    }

    public class MarketList : ObservableCollection<MarketTreeRow>
    {
        public MarketList() : base() { }
    }
    #endregion

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private TTAPI m_api = null;
        private tt_net_sdk.Dispatcher m_dispatcher = null;
        private TradeSubscription m_ts = null;
        private bool m_shutdownComplete = false;

        private MarketList MarketListData = new MarketList();
        public int NOS_Weight { get; set; } = 0;
        public int CHG_Weight { get; set; } = 1;
        public int CXL_Weight { get; set; } = 3;

        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = this;
            treeviewList.ItemsSource = MarketListData;

            var app = App.Current as TTNETAPI_Sample_WPF_VolumeRatio.App;
            m_dispatcher = app.SDKDispatcher;

            var app_key = "app_key";
            var env = ServiceEnvironment.ProdSim;
            var mode = TTAPIOptions.SDKMode.Client;
            var options = new TTAPIOptions(mode, env, app_key, 5000);
            TTAPI.CreateTTAPI(m_dispatcher, options, new ApiInitializeHandler(OnSDKInitialized));

            lblStatus.Text = "Initializing...";
        }

        #region SDK Events
        private void OnSDKInitialized(TTAPI api, ApiCreationException ex)
        {
            if (ex == null)
            {
                lblStatus.Text = "Initialized.  Authenticating...";
                m_api = api;
                m_api.TTAPIStatusUpdate += OnSDKStatusUpdate;
                TTAPI.ShutdownCompleted += OnSDKShutdownComplete;
                m_api.Start();
            }
            else if (!ex.IsRecoverable)
            {
                MessageBox.Show($"API Initialization Failed: {ex.Message}");
            }
        }

        private void OnSDKStatusUpdate(object sender, TTAPIStatusUpdateEventArgs e)
        {
            if (e.IsReady && m_ts == null)
            {
                lblStatus.Text = "Authenticated.  Launching subscriptions...";
                m_ts = new TradeSubscription(m_dispatcher, false);
                m_ts.OrderBookDownload += OnOrderBookDownload;
                m_ts.OrderAdded += OnOrderAdded;
                m_ts.OrderDeleted += OnOrderDeleted;
                m_ts.OrderFilled += OnOrderFilled;
                m_ts.OrderUpdated += OnOrderUpdated;
                m_ts.OrderPendingAction += OnOrderPendingAction;
                m_ts.OrderRejected += OnOrderRejected;
                m_ts.Start();
            }
            else if (e.IsDown)
            {
                lblStatus.Text = $"SDK is down: {e.StatusMessage}";
            }
        }

        private void OnSDKShutdownComplete(object sender, EventArgs e)
        {
            m_shutdownComplete = true;
            this.Close();
        }
        #endregion

        #region TradeSubscription Events
        private void OnOrderBookDownload(object sender, OrderBookDownloadEventArgs e)
        {
            lblStatus.Text = "Running";
        }

        private void OnOrderAdded(object sender, OrderAddedEventArgs e)
        {
            var row = GetContractRow(e.Order.Instrument);
            if (row != null)
            {
                row.ProcessMessage(e);
            }
        }

        private void OnOrderUpdated(object sender, OrderUpdatedEventArgs e)
        {
            var row = GetContractRow(e.NewOrder.Instrument);
            if (row != null)
            {
                row.ProcessMessage(e);
            }
        }

        private void OnOrderDeleted(object sender, OrderDeletedEventArgs e)
        {
            var row = GetContractRow(e.DeletedUpdate.Instrument);
            if (row != null)
            {
                row.ProcessMessage(e);
            }
        }

        private void OnOrderFilled(object sender, OrderFilledEventArgs e)
        {
            var row = GetContractRow(e.Fill.Instrument);
            if (row != null)
            {
                row.ProcessMessage(e);
            }
        }

        private void OnOrderRejected(object sender, OrderRejectedEventArgs e)
        {
            // Not Implemented 
        }

        private void OnOrderPendingAction(object sender, OrderPendingActionEventArgs e)
        {
            // Not Implemented 
        }
        #endregion

        #region Form Events
        private void OnWindowClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (!m_shutdownComplete)
            {
                e.Cancel = true;
                Shutdown();
            }
        }

        private static readonly Regex s_numericOnly = new Regex("[^0-9.-]+");
        private void TextBox_PreviewTextInput(object sender, System.Windows.Input.TextCompositionEventArgs e)
        {
            e.Handled = s_numericOnly.IsMatch(e.Text);
        }

        private void TextBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        {
            try
            {
                if (string.IsNullOrWhiteSpace(txtNOSWeight.Text))
                    return;
                if (string.IsNullOrWhiteSpace(txtCHGWeight.Text))
                    return;
                if (string.IsNullOrWhiteSpace(txtCXLWeight.Text))
                    return;

                var nos_weight = Int32.Parse(txtNOSWeight.Text);
                var chg_weight = Int32.Parse(txtCHGWeight.Text);
                var cxl_weight = Int32.Parse(txtCXLWeight.Text);
                foreach(var parent in MarketListData)
                {
                    var parentRow = parent as MarketTreeRow;
                    parentRow.ResetWeights(nos_weight, chg_weight, cxl_weight);
                }
            }
            catch { }
        }
        #endregion

        #region Private Methods
        private ContractTreeRow GetContractRow(Instrument instr)
        {
            var parent = this.MarketListData.FirstOrDefault(x => x.MarketId == instr.Product.Market.MarketId);
            if (parent == null)
            {
                this.MarketListData.Add(new MarketTreeRow(instr));
                parent = this.MarketListData.Last();
            }

            var productRow = parent.CreateOrGetExistingChild(instr);
            var contractRow = productRow.CreateOrGetExistingChild(instr);
            return contractRow;
        }

        private void Shutdown()
        {
            if (m_ts != null)
            {
                m_ts.OrderBookDownload -= OnOrderBookDownload;
                m_ts.OrderAdded -= OnOrderAdded;
                m_ts.OrderDeleted -= OnOrderDeleted;
                m_ts.OrderFilled -= OnOrderFilled;
                m_ts.OrderUpdated -= OnOrderUpdated;
                m_ts.OrderPendingAction -= OnOrderPendingAction;
                m_ts.OrderRejected -= OnOrderRejected;
                m_ts.Dispose();
                m_ts = null;
            }

            TTAPI.Shutdown();
        }
        #endregion
    }
}
