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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Threading;
using System.IO;
using tt_net_sdk;

namespace TTNETAPI_Sample_GUI_Market_Explorer
{
    public partial class MarketExplorer : Form
    {
        // Declare the API objects
        private TTAPI m_api = null;
        private ProductCatalog m_prodCat = null;
        private MarketCatalog m_marketCatalog = null;
        private bool m_isShutdown = false, m_shutdownInProcess = false;

        // Other private member variables
        Dictionary<Product, InstrumentCatalog> m_instrumentCatalogList = null;
        List<MarketListViewItem> m_marketList = null;


        public MarketExplorer()
        {
            InitializeComponent();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for status of API initialization. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void ttNetApiInitHandler(TTAPI api, ApiCreationException ex)
        {
            if (ex == null)
            {
                statusLabel.Text = "TT NET SDK INITIALIZED";
                m_api = api;
                m_api.TTAPIStatusUpdate += new EventHandler<TTAPIStatusUpdateEventArgs>(m_api_TTAPIStatusUpdate);
                m_api.Start();
            }
            else if (ex.IsRecoverable)
            {
                // Initialization failed but retry is in progress...
            }
            else
            {
                MessageBox.Show("API Initialization Failed: " + ex.Message);
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for status of authentication. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        private void m_api_TTAPIStatusUpdate(object sender, TTAPIStatusUpdateEventArgs e)
        {
            if (e.IsReady)
            {
                statusLabel.Text = "APP KEY AUTHENTICATED : LOGIN SUCCESSFUL";

                m_marketList = new List<MarketListViewItem>();

                m_instrumentCatalogList = new Dictionary<Product, InstrumentCatalog>();

                // Init the windows.
                initWindowViews();

                // Create the Market Catalog
                m_marketCatalog = m_api.MarketCatalog;

                listViewMarketList.Items.Clear();

                // Create a sorted list of markets objects.
                var marketList = m_api.MarketCatalog.Markets.Values.OrderBy(market => market.Name);

                foreach (Market market in marketList)
                {
                    // Add the item if not currently in the tree
                    if (m_marketList.Where(m => m.Name == market.Name).Count() == 0)
                    {
                        MarketListViewItem marketItem = new MarketListViewItem(listViewMarketList);
                        marketItem.Market = market;
                        m_marketList.Add(marketItem);
                        listViewMarketList.Items.Add(marketItem);
                    }
                }

                statusLabel.Text = "There are " + m_marketList.Count.ToString() + " Markets";

            }
            else
            {
                MessageBox.Show(String.Format("M_TTAPI_TTAPIStatusUpdate: {0}", e));
            }
        }

        public void shutdownTTAPI()
        {
            if (!m_shutdownInProcess)
            {

                if (m_prodCat != null)
                {
                    m_prodCat.OnData -= new EventHandler<ProductCatalogEventArgs>(productsUpdated);
                    m_prodCat.Dispose();
                    m_prodCat = null;
                }

                if (m_instrumentCatalogList?.Count > 0)
                {
                    foreach (InstrumentCatalog instrCat in m_instrumentCatalogList.Values)
                    {
                        instrCat.OnData -= new EventHandler<InstrumentCatalogEventArgs>(instrumentsUpdated);
                        instrCat.Dispose();
                    }
                    m_instrumentCatalogList.Clear();
                }

                if (m_marketList != null)
                {
                    foreach (MarketListViewItem market in m_marketList)
                    {
                        market.Dispose();
                    }
                    m_marketList.Clear();

                    foreach (MarketListViewItem market in listViewMarketList.Items)
                    {
                        market.Dispose();
                    }
                    listViewMarketList.Items.Clear();
                }

                TTAPI.ShutdownCompleted += new EventHandler(TTAPI_ShutdownCompleted);
                TTAPI.Shutdown();

                m_shutdownInProcess = true;
            }
        }

        public void TTAPI_ShutdownCompleted(object sender, EventArgs e)
        {
            m_isShutdown = true;
            Close();
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if (!m_isShutdown)
            {
                e.Cancel = true;
                shutdownTTAPI();
            }
            else
            {
                base.OnFormClosing(e);
            }
        }

        #region MarketCatalog Markets

        /// <summary>
        /// MarketsUpdated callback.
        /// NOTE: MarketsUpdated will be called on the dispatcher that TT API was initialized on.
        /// </summary>
        private void marketsUpdated(object sender, MarketCatalogUpdatedEventArgs e)
        {
            foreach (Market market in e.Added)
            {
                // Add the item if not currently in the tree
                if (m_marketList.Where(m => m.Name == market.Name).Count() == 0)
                {
                    MarketListViewItem marketItem = new MarketListViewItem(listViewMarketList);
                    marketItem.Market = market;
                    m_marketList.Add(marketItem);
                    listViewMarketList.Items.Add(marketItem);
                }
            }
        }

        #endregion
        
        #region ProductCatalog

        private void createProductSubscription(Market market)
        {
            m_prodCat = new ProductCatalog(market, Dispatcher.Current);
            m_prodCat.OnData += new EventHandler<ProductCatalogEventArgs>(productsUpdated);
            m_prodCat.GetAsync();
        }

        private void productsUpdated(object sender, ProductCatalogEventArgs e)
        {
            treeViewProductList.Nodes.Clear();

            foreach (Product product in e.Added)
            {
                //Add all the product types supported
                if (!listViewProductTypeList.Items.ContainsKey(product.Type.ToString()))
                {
                    ListViewItem item = new ListViewItem();
                    item.Name = product.Type.ToString();
                    item.Text = product.Type.ToString();
                    item.Tag = product.Type;
                    listViewProductTypeList.Items.Add(item);
                }
            }

            List<ProductType> selectedTypes = BuildSelectedTypes();

            if (selectedTypes.Count > 0)
            {
                IEnumerable<Product> products;
                products = (from Product product in e.Added
                            join type in selectedTypes on product.Type equals type
                            orderby product.Name
                            select product).OrderBy(product => product.Alias);

                // Add product nodes to the TreeView.
                // Set the default child node as "loading..." This node will be replaced when the node is expanded and
                // the instrument catalog is downloaded for the given product.
                foreach (Product product in products)
                {
                    TreeNode prodNode = treeViewProductList.Nodes.Add(product.Alias);
                    prodNode.Tag = (object)product;
                    TreeNode childNode = new TreeNode("Loading ...");
                    prodNode.Nodes.Add(childNode);
                }
                statusLabel.Text = "There are " + Convert.ToString(products.Count()) + " products";
            }

            // Clean up this ProductCatalog subscription.
            if (m_prodCat != null)
            {
                m_prodCat.OnData -= new EventHandler<ProductCatalogEventArgs>(productsUpdated);
                m_prodCat.Dispose();
                m_prodCat = null;
            }

            Cursor = Cursors.Default;
        }

        #endregion

        #region InstrumentCatalog

        /// <summary>
        /// Subscribe to the instrument catalog for a given product.
        /// </summary>
        /// <param name="product">Product to subscribe to.</param>
        private void subscribeToInstrumentCatalog(Product product)
        {
            if (!m_instrumentCatalogList.ContainsKey(product))
            {
                // Create and start an instrument catalog subscription.
                InstrumentCatalog instrumentCatalog = new InstrumentCatalog(product, Dispatcher.Current);
                instrumentCatalog.OnData += new EventHandler<InstrumentCatalogEventArgs>(instrumentsUpdated);
                instrumentCatalog.GetAsync();

                m_instrumentCatalogList.Add(product, instrumentCatalog);
            }
            else
            {
                updateInstrumentTreeViewNodes(m_instrumentCatalogList[product]);
            }
        }

        private void instrumentsUpdated(object sender, InstrumentCatalogEventArgs e)
        {
            InstrumentCatalog instrumentCatalog = sender as InstrumentCatalog;
            updateInstrumentTreeViewNodes(instrumentCatalog);
        }
        
        #endregion

        #region GUI

        private void initWindowViews()
        {
            ColumnHeader exchangeHeader = new ColumnHeader();
            exchangeHeader.Text = "MARKETS";
            exchangeHeader.Width = listViewMarketList.ClientRectangle.Width;
            listViewMarketList.Columns.Add(exchangeHeader);

            ColumnHeader productTypeHeader = new ColumnHeader();
            productTypeHeader.Text = "PRODUCT TYPES";
            productTypeHeader.Width = listViewProductTypeList.ClientRectangle.Width;
            listViewProductTypeList.Columns.Add(productTypeHeader);

            ColumnHeader productsHeader = new ColumnHeader();
            productsHeader.Text = "PRODUCTS";
            productsHeader.Width = treeViewProductList.ClientRectangle.Width;
        }

        private void listViewMarketList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewMarketList.SelectedItems.Count > 0)
            {
                treeViewProductList.Nodes.Clear();
                statusLabel.Text = "";
                listViewProductTypeList.Items.Clear();

                MarketListViewItem marketItem = listViewMarketList.SelectedItems[0] as MarketListViewItem;

                if (m_prodCat != null)
                {
                    m_prodCat.OnData -= productsUpdated;
                    m_prodCat.Dispose();
                    m_prodCat = null;
                }

                // Create the market subscription.
                createProductSubscription(marketItem.Market);
            }
        }

        private void listViewProductTypeList_SelectedIndexChanged(object sender, EventArgs e)
        {
            Cursor = Cursors.WaitCursor;

            if (listViewMarketList.SelectedItems.Count > 0)
            {
                Market market = listViewMarketList.SelectedItems[0].Tag as Market;

                if (m_prodCat != null)
                {
                    m_prodCat.OnData -= productsUpdated;
                    m_prodCat.Dispose();
                    m_prodCat = null;
                }

                // Create the Product subscription.
                createProductSubscription(market);
            }
        }

        private void updateInstrumentTreeViewNodes(InstrumentCatalog instrumentCatalog)
        {
            treeViewProductList.BeginUpdate();

            TreeNode updatedNode = null;

            // Find the product in the tree view for the selected product tree view node.
            foreach (TreeNode node in treeViewProductList.Nodes)
            {
                if (String.Equals((node.Tag as Product).Alias, instrumentCatalog.Product.Alias))
                {
                    updatedNode = node;
                    break;
                }
            }

            // updatedNode should never be null.
            if (updatedNode == null)
            {
                return;
            }

            updatedNode.Nodes.Clear();

            // Insert the instruments as child nodes within the product tree view node.
            foreach (Instrument instr in instrumentCatalog.InstrumentList)
            {
                TreeNode node = updatedNode.Nodes.Add(instr.GetFormattedName(InstrumentNameFormat.Normal));

                // Add tooltip text from the Definition
                node.ToolTipText = instr.Name + "\r\n"
                    + instr.InstrumentDetails.Currency + "\r\n" + instr.InstrumentDetails.TickSize.ToString()
                    + " / " + instr.InstrumentDetails.TickSize.ToString();

                node.Tag = instr;
            }

            Cursor = Cursors.Default;
            treeViewProductList.EndUpdate();
        }

        private void treeViewProductList_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            treeViewProductList.SelectedNode = e.Node;
            Product product = e.Node.Tag as Product;

            subscribeToInstrumentCatalog(product);
        }

        #endregion

        #region Helper Functions

        private List<ProductType> BuildSelectedTypes()
        {
            var selectedTypes = new List<ProductType>() { };

            ListView.SelectedListViewItemCollection col = listViewProductTypeList.SelectedItems;
            foreach (ListViewItem item in col)
            {
                // The ProductType is stored in the item Tag.
                selectedTypes.Add((ProductType)item.Tag);
            }

            return selectedTypes;
        }

        #endregion

        public class MarketListViewItem : ListViewItem
        {
            private ListView m_parentView = null;
            private Market m_market = null;
            private Dispatcher m_dispatcher = null;

            public MarketListViewItem(ListView parentView)
            {
                // Set the parent view, current dispatcher, and attach to the Dispatcher
                m_parentView = parentView;
                m_dispatcher = Dispatcher.Current;
            }

            public Market Market
            {
                get { return m_market; }
                set
                {
                    m_market = value;
                    Text = m_market.Name;   // this is what is displayed in the control
                    Name = m_market.Name;
                    Tag = m_market;
                }
            }

            #region Cleanup and Dispose
            public void Dispose()
            {
                // Dispose may be called on another thread due to garbage collection.
                // InvokeIfRequired is an extension method defined in Utility.cs within this project.
                // Please refer to Utility.cs for more information on what InvokeIfRequired is doing.
                if (m_dispatcher.InvokeRequired())
                {
                    m_dispatcher.DispatchAction(() =>
                    {
                        if (m_market != null)
                        {
                            m_market = null;
                        }
                    });

                    m_parentView = null;
                }
            }

            #endregion
        }
    }
}
