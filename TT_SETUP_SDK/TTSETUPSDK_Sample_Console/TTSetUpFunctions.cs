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
using tt_setup_sdk;
using Newtonsoft.Json;

namespace TTSETUPSDK_Sample_Console
{
    class TTSetUpFunctions
    {
        private WorkerDispatcher m_disp = null;
        private TTSetupSDK m_sdk = null;

        public void Start(tt_setup_sdk.TTSetupSDKOptions apiConfig)
        {
            m_disp = tt_setup_sdk.Dispatcher.AttachWorkerDispatcher();
            m_disp.DispatchAction(() =>
            {
                Init(apiConfig);
            });

            m_disp.Run();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Initialize the API </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void Init(tt_setup_sdk.TTSetupSDKOptions apiConfig)
        {
            TTSetupSDKInitializeHandler apiInitializeHandler = new TTSetupSDKInitializeHandler(TTSetupSdkInitHandler);
            TTSetupSDK.ShutdownCompleted += TTSetupSDK_ShutdownCompleted;
            TTSetupSDK.CreateTTSetupSDK(m_disp, apiConfig, apiInitializeHandler);
        }

        private void M_sdk_TTSetupSDKStatusUpdate(object sender, TTSetupStatusUpdateEventArgs e)
        {
            Console.WriteLine("TTSetupSDKStatusUpdate: {0}", e);

            if (e.IsReady == false)
            {
                // TODO: Do any clean up processing here
                return;
            }

            // Status is up
            Console.WriteLine("TTSETUP SDK Authenticated");

            m_sdk.SubscribeResource(TTSetupResource.User);
            m_sdk.SubscribeResource(TTSetupResource.Account);
            m_sdk.SubscribeResource(TTSetupResource.OrderTagDefault);
            m_sdk.SubscribeResource(TTSetupResource.UserGroup);
            m_sdk.SubscribeResource(TTSetupResource.RiskGroup);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Event notification for status of API initialization. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void TTSetupSdkInitHandler(TTSetupSDK sdk, ApiCreationException ex)
        {
            if (ex == null)
            {
                Console.WriteLine("TT SETUP SDK Initialization Complete");

                // Authenticate your credentials
                m_sdk = sdk;
                m_sdk.TTSetupSDKStatusUpdate += M_sdk_TTSetupSDKStatusUpdate;
                m_sdk.TTSetupSubscriptionStatusUpdate += M_sdk_TTSetupSubscriptionStatusUpdate;

                m_sdk.OnAccountChanged += M_sdk_OnAccountChanged;
                m_sdk.OnAccountAlgoSettingsChanged += M_sdk_OnAccountAlgoSettingsChanged;
                m_sdk.OnAccountSettingsChanged += M_sdk_OnAccountSettingsChanged;
                m_sdk.OnAccountExchangeOrderRoutingChanged += M_sdk_OnAccountExchangeOrderRoutingChanged;
                m_sdk.OnAccountCompanyChanged += M_sdk_OnAccountCompanyChanged;
                m_sdk.OnAccountRestrictionsChanged += M_sdk_OnAccountRestrictionsChanged;
                m_sdk.OnAccountRiskLimitsChanged += M_sdk_OnAccountRiskLimitsChanged;
                m_sdk.OnAccountRiskSettingsChanged += M_sdk_OnAccountRiskSettingsChanged;
                m_sdk.OnAccountUserChanged += M_sdk_OnAccountUserChanged;

                m_sdk.OnUserChanged += M_sdk_OnUserChanged;
                m_sdk.OnUserSettingsChanged += M_sdk_OnUserSettingsChanged;
                m_sdk.OnUserMarketDataChanged += M_sdk_OnUserMarketDataChanged;
                m_sdk.OnUserRiskLimitsChanged += M_sdk_OnUserRiskLimitsChanged;
                m_sdk.OnUserCustomFieldsChanged += M_sdk_OnUserCustomFieldsChanged;
                m_sdk.OnUserAgreementsChanged += M_sdk_OnUserAgreementsChanged;
                m_sdk.OnUserExchangeOrderRoutingChanged += M_sdk_OnUserExchangeOrderRoutingChanged;
                m_sdk.OnUserCompanyChanged += M_sdk_OnUserCompanyChanged;
                m_sdk.OnUserProfileChanged += M_sdk_OnUserProfileChanged;

                m_sdk.OnOrderTagDefaultsChanged += M_sdk_OnOrderTagDefaults;
                m_sdk.OnCompanyOrderTagDefaultsChanged += M_sdk_OnCompanyOrderTagDefaultsChanged;

                m_sdk.OnUserGroupChanged += M_sdk_OnUserGroupChanged;
                m_sdk.OnUserGroupConnectivityChanged += M_sdk_OnUserGroupConnectivityChanged;
                m_sdk.OnUserGroupUserChanged += M_sdk_OnUserGroupUserChanged;
                m_sdk.OnUserGroupSettingsChanged += M_sdk_OnUserGroupSettingsChanged;
                
                m_sdk.OnRiskGroupChanged += M_sdk_OnRiskGroupChanged;
                m_sdk.OnRiskGroupAccountChanged += M_sdk_OnRiskGroupAccountChanged;
                m_sdk.OnRiskGroupRiskLimitsChanged += M_sdk_OnRiskGroupRiskLimitsChanged;
                m_sdk.OnRiskGroupRiskSettingsChanged += M_sdk_OnRiskGroupRiskSettingsChanged;
                m_sdk.OnRiskGroupSettingsChanged += M_sdk_OnRiskGroupSettingsChanged;

                m_sdk.OnRecoveryFailed += M_sdk_OnRecoveryFailed;
                m_sdk.OnRefreshNeeded += M_sdk_OnRefreshNeeded;
                m_sdk.UnhandledManagedException += M_sdk_UnhandledManagedException;
                m_sdk.Start();
            }
            else if (ex.IsRecoverable)
            {
                // Initialization failed but retry is in progress...
            }
            else
            {
                Console.WriteLine("TT SETUP SDK Initialization Failed: {0}", ex.Message);
                Dispose();
            }
        }

        private void M_sdk_OnRefreshNeeded(object sender, RefreshNeededEventArgs e)
        {
            Console.WriteLine("TT SETUP OnRefreshNeeded : " + e.Message.ToString());
        }

        private void M_sdk_OnRecoveryFailed(object sender, RecoveryFailedEventArgs e)
        {
            Console.WriteLine("TT SETUP SDK OnRecoveryFailed : " + e.Message.ToString());
        }

        public void GetAllAccounts()
        {
            // Example to show how to call the GET methods

            int numberOfAccounts = 0;
            string accts = DataFetcher.Get_Accounts();

            try
            {
                dynamic json = JsonConvert.DeserializeObject(accts);

                if (json["lastPage"] != null)
                {
                    while (json["lastPage"] == false)
                    {
                        string accounts = DataFetcher.Get_Accounts(json["nextPageKey"].ToString());
                        json = JsonConvert.DeserializeObject(accounts);
                        numberOfAccounts += json["accounts"].Count;

                        //Code to process the accounts
                    }

                    Console.WriteLine("The number of accounts for the user " + m_sdk.UserName + " are : " + numberOfAccounts);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error parsing the json" + ex.Message.ToString());
            }


        }

        private void M_sdk_UnhandledManagedException(object sender, UnhandledManagedExceptionEventArgs e)
        {
            Console.WriteLine("TT SETUP SDK Unmanaged Exception : " + e.Exception.Message.ToString());
        }


        private void M_sdk_OnAccountRiskSettingsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnAccountRiskSettingsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnAccountRiskSettingsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnAccountRiskLimitsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n _OnAccountRiskLimitsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n _OnAccountRiskLimitsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnAccountRestrictionsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnAccountRestrictionsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnAccountRestrictionsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnAccountSettingsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnAccountSettingsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnAccountSettingsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnAccountAlgoSettingsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnAccountAlgoSettingsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnAccountAlgoSettingsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnAccountChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnAccountChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnAccountChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnAccountExchangeOrderRoutingChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnAccountExchangeOrderRoutingChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnAccountExchangeOrderRoutingChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnAccountCompanyChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnAccountCompanyChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnAccountCompanyChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnAccountUserChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnAccountUserChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnAccountUserChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        
        private void M_sdk_OnUserSettingsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserSettingsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserSettingsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserMarketDataChanged(object sender,UpdateEventArgs e)
        {
            if(e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserMarketDataChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserMarketDataChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserCompanyChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserCompanyChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserCompanyChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserRiskLimitsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserRiskLimitsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserRiskLimitsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserProfileChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserProfileChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserProfileChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserCustomFieldsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserCustomFieldsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserCustomFieldsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserAgreementsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserAgreementsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserAgreementsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserExchangeOrderRoutingChanged(object sender,UpdateEventArgs e)
        {
            if(e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserExchangeOrderRoutingChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserExchangeOrderRoutingChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }


        private void M_sdk_OnOrderTagDefaults(object sender,UpdateEventArgs e)
        {
            if(e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnOrderTagDefaults DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnOrderTagDefaults  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnCompanyOrderTagDefaultsChanged(object sender,UpdateEventArgs e)
        {
            if(e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnCompanyOrderTagDefaultsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnCompanyOrderTagDefaultsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }


        private void M_sdk_OnUserGroupChanged(object sender,UpdateEventArgs e)
        {
            if(e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserGroupChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserGroupChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserGroupSettingsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserGroupSettingsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserGroupSettingsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserGroupConnectivityChanged(object sender,UpdateEventArgs e)
        {
            if(e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserGroupConnectivityChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserGroupConnectivityChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnUserGroupUserChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnUserGroupUserChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnUserGroupUserChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }


        private void M_sdk_OnRiskGroupChanged(object sender,UpdateEventArgs e)
        {
            if(e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnRiskGroupChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnRiskGroupChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnRiskGroupAccountChanged(object sender,UpdateEventArgs e)
        {
            if(e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnRiskGroupAccountChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnRiskGroupAccountChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnRiskGroupSettingsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnRiskGroupSettingsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnRiskGroupSettingsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnRiskGroupRiskSettingsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnRiskGroupRiskSettingsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnRiskGroupRiskSettingsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        private void M_sdk_OnRiskGroupRiskLimitsChanged(object sender, UpdateEventArgs e)
        {
            if (e.TTAction == UpdateAction.Delete)
                Console.WriteLine("\n OnRiskGroupRiskLimitsChanged DELETE " + e.Id);
            else
            {
                Console.WriteLine("\n OnRiskGroupRiskLimitsChanged  " + e.TTAction.ToString());
                Console.WriteLine("Details :  " + e.TTSetupRecord.ToString());
            }
        }

        
        private void M_sdk_OnServerVersionChanged(object sender, EventArgs e)
        {
            Console.WriteLine("\n OnServerVersionChanged ");
            Console.WriteLine("\n User needs to refresh resources to sync with TTUS server. ");
        }

        

        public void M_sdk_TTSetupSubscriptionStatusUpdate(object sender,TTSetupSubscriptionStatusEventArgs e)
        {
            Console.WriteLine("TTSetupSubscriptionStatusUpdate: {0}",e);
            if(e.IsSubscribed)
                Console.WriteLine($"Subscription {e.TTSetupResource} is subscribed.");
            else
                Console.WriteLine($"Subscription {e.TTSetupResource} is unsubscribed.");
        }



        private void TTSetupSDK_ShutdownCompleted(object sender, EventArgs e)
        {
            Console.WriteLine("TT SETUP SDK Shutdown completed");
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Shuts down the API  </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        public void Dispose()
        {
            TTSetupSDK.Shutdown();
        }
    }
}
