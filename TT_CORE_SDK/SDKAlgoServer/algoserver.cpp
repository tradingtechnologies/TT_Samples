#include <iostream>
#include <iomanip>
#include <atomic>
#include <sstream>
#include <string.h>
#include <csignal>
#include <condition_variable>
#include "TTWrapper.h"
#include "SDKAlgoManager.h"


std::condition_variable gShutdown;
std::mutex gShutdownMtx;

void SignalTerminateCallback(int signal)
{
    gShutdown.notify_all();
}

inline void SetupSignalHandlers()
{
#ifndef WIN32
    // SIGQUIT is not defined on windows
    if (SIG_ERR == ::signal((int)SIGQUIT, SignalTerminateCallback))
        std::cout << "Could not setup Signal handler for signal=SIGQUIT" << std::endl;
#endif
    if (SIG_ERR == ::signal((int)SIGINT, SignalTerminateCallback))
        std::cout << "Could not setup Signal handler for signal=SIGINT" << std::endl;
    if (SIG_ERR == ::signal((int)SIGTERM, SignalTerminateCallback))
        std::cout << "Could not setup Signal handler for signal=SIGTERM" << std::endl;
}

inline void WaitForShutdown()
{
    std::unique_lock<std::mutex> lk(gShutdownMtx);
    gShutdown.wait(lk);
}


int main(int argc, char* argv[])
{
    ttsdk::Environment env = ttsdk::Environment::UatCert;
    std::string sEnv("UatCert");
    std::string app_key;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if ((arg == "-k") || (arg == "--key"))
        {
            if (i + 1 < argc)
            {
                i++;
                app_key = argv[i];
            }
            else
            {
                std::cerr << "--key option requires one argument." << std::endl;
                return 1;
            }
        }
        else if ((arg == "-e") || (arg == "--env"))
        {
            if (i + 1 < argc)
            {
                i++;
                sEnv = argv[i];
                if (sEnv == "ProdLive") env = ttsdk::Environment::ProdLive;
                else if (sEnv == "ProdSim") env = ttsdk::Environment::ProdSim;
                else if (sEnv == "ProdDelayed") env = ttsdk::Environment::ProdDelayed;
                else if (sEnv == "UatCert") env = ttsdk::Environment::UatCert;
                else if (sEnv == "DevCert")env = ttsdk::Environment::DevCert;
                else {
                    std::cerr << "Invalid env argument: " << sEnv << std::endl;
                    return 1;
                }
            }
            else
            {
                std::cerr << "--env option requires one argument." << std::endl;
                return 1;
            }
        }
    }

    if (app_key.empty())
    {
        auto env_app_key = std::getenv("TT_APP_KEY");
        if (!env_app_key || strcmp(env_app_key, "") == 0)
        {
            std::cout << "app_key is invalid. " << std::endl;
            return 1;
        }
        app_key = env_app_key;
    }

    std::cout << "using app_key: " << app_key << " in " << sEnv << "(" << env << ")" << std::endl;

    SetupSignalHandlers();
    std::cout << "Starting the TTSDK" << std::endl;
    // start TTSDK

    TTWrapper ttsdk;
    if (ttsdk.Start(app_key, env))
    {
        std::cout << "Application Ready. Ctrl+\\ to exit." << std::endl;

        WaitForShutdown();

        std::cout << "Application Shutting Down." << std::endl;
        ttsdk.Stop();
        std::cout << "TTSDK Shutdown." << std::endl;
    }
    else
    {
        std::cout << "Unable to start the TTSDK." << std::endl;
    }
    return 0;
}




