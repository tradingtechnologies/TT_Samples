#include <iostream>
#include <iomanip>
#include <atomic>
#include <sstream>
#include <string.h>
#include <csignal>
#include <condition_variable>
#include "TTWrapper.h"
#include "StrategyManager.h"


std::condition_variable gShutdown;
std::mutex gShutdownMtx;

void SignalTerminateCallback(int signal)
{
    gShutdown.notify_all();
}

inline void SetupSignalHandlers()
{
    if (SIG_ERR == ::signal((int)SIGQUIT, SignalTerminateCallback))
        std::cout << "Could not setup Signal handler for signal=SIGQUIT" << std::endl;
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
    int port = 9000;
    std::string ipAddress = "127.0.0.1";
    if (argc == 3)
    {
        ipAddress = argv[1];
        port = atoi(argv[2]);      
    }
    auto app_key = std::getenv("HELLO_APP_KEY");
    if (!app_key || strcmp(app_key, "") == 0)
    {
        std::cout << "app_key is invalid. " << std::endl;
        return 1;
    }

    std::cout << "using app_key: " << app_key << std::endl;

    
    SetupSignalHandlers();
    std::cout << "Starting the TTSDK" << std::endl;
    // start TTSDK

    TTWrapper ttsdk;
    if (ttsdk.Start(app_key))
    {
 
        ttsample::StrategyManager manager;
        if (!manager.Start(ipAddress, port))
        {
            std::cout << "Unable to start the strategy manager." << std::endl;
            return -1;
        }
        std::cout << "Application Ready. Ctrl+\\ to exit." << std::endl;

        WaitForShutdown();

        std::cout << "Application Shutting Down." << std::endl;
        ttsdk.Stop();
        manager.Stop();
    }
    else
    {
        std::cout << "Unable to start the TTSDK." << std::endl;
    }

    return 0;
}
