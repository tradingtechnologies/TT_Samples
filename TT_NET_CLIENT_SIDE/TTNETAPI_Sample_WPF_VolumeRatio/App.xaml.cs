using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using tt_net_sdk;

namespace TTNETAPI_Sample_WPF_VolumeRatio
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public tt_net_sdk.Dispatcher SDKDispatcher { get; set; }
    }

    public class EntryPoint
    {
        [STAThread]
        public static void Main(string[] args)
        {
            using (var disp = Dispatcher.AttachUIDispatcher())
            {
                TTNETAPI_Sample_WPF_VolumeRatio.App app = new TTNETAPI_Sample_WPF_VolumeRatio.App();
                app.SDKDispatcher = disp;
                app.InitializeComponent();
                app.Run();
            } 
        }
    }
}


