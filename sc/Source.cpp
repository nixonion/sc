#include <windows.h>
#include <tchar.h>
#include <strsafe.h>


SC_HANDLE schSCManager;
SC_HANDLE schService;
LPCWSTR szSvcName;

CHAR type[50]="";
CHAR state[50]="";
CHAR start[50];
CHAR error[50];
LPCWSTR binpath=NULL;
LPCWSTR password=NULL;
LPCWSTR displayname=NULL;
CHAR reset[50]="";
CHAR reboot[100]="";
CHAR action[100]="";
CHAR command[100]="";

void create(); //sc create 
void del(); //sc delete 
void qdesc(); //sc qdescription
void startsc(); //sc start
void stopsc(); //sc stop
BOOL StopDep(); //dependent function for stopsc
VOID failuresc(); //sc failure
VOID configsc(); //sc config
void querysc(); //sc query  servicename
void queryall(); //sc query

void dispsc()
{
    printf("\nDESCRIPTION:\n\tSC is a command line program used for communicating with the Service Control Managerand services.\nUSAGE :\n\tsc[command][service name] <option1> <option2>...");

    printf("\n\tFurther help on commands can be obtained by typing : 'sc [command]'");
    printf("\n\n\tCommands :\n");
    printf("\n\tquery---------- - Queries the status for a service, or \n\tenumerates the status for types of services. \n\tstart---------- - Starts a service.");
    printf("\n\tstop------------Sends a STOP request to a service.");
    printf("\n\tconfig----------Changes the configuration of a service(persistent).");
    printf("\n\tfailure-------- - Changes the actions taken by a service upon failure.");
    printf("\n\tqdescription----Queries the description for a service. \n\tdelete----------Deletes a service(from the registry). \n\tcreate----------Creates a service. (adds it to the registry).\n");
}

void dispcreate()
{
    printf("DESCRIPTION:\n"
        "        Creates a service entry in the registry and Service Database.\n"
        "USAGE:\n"
        "        sc.exe create [service name] [binPath= ] <option1> <option2>...\n"
        "\n"
        "OPTIONS:\n"
        "NOTE: The option name includes the equal sign.\n"
        "      A space is required between the equal sign and the value.\n"
        " type= <own|share|interact|kernel|filesys>\n"
        "       (default = own)\n"
        " start= <boot|system|auto|demand|disabled>\n"
        "       (default = demand)\n"
        " error= <normal|severe|critical|ignore>\n"
        "       (default = normal)\n"
        " binPath= <BinaryPathName to the .exe file>\n"
        " DisplayName= <display name>\n"
        " password= <password>");
}

void dispfailure()
{
    printf("DESCRIPTION:\n"
        "        Changes the actions upon failure\n"
        "USAGE:\n"
        "        sc.exe failure [service name] <option1> <option2>...\n"
        "\n"
        "OPTIONS:\n"
        "        reset=   <Length of period of no failures (in seconds)\n"
        "                  after which to reset the failure count to 0 (may be INFINITE)>\n"
        "                  (Must be used in conjunction with actions= )\n"
        "        reboot=  <Message broadcast before rebooting on failure>\n"
        "        command= <Command line to be run on failure>\n"
        "        actions= <Failure actions and their delay time (in milliseconds),\n"
        "                  separated by / (forward slash) -- e.g., run/5000/reboot/800\n"
        "                  Valid actions are <run|restart|reboot> >\n"
        "                  (Must be used in conjunction with the reset= option)");
}
void dispdesc()
{
    printf("DESCRIPTION:\n"
        "        Retrieves the description string of a service.\n"
        "USAGE:\n"
        "        sc.exe qdescription [service name] ");
}

void dispdelete()
{
    printf("DESCRIPTION:\n"
        "        Deletes a service entry from the registry.\n"
        "        If the service is running, or another process has an\n"
        "        open handle to the service, the service is simply marked\n"
        "        for deletion.\n"
        "USAGE:\n"
        "        sc.exe delete [service name]");
}

void dispstart()
{
    printf("DESCRIPTION:\n"
        "        Starts a service running.\n"
        "USAGE:\n"
        "        sc.exe start [service name] ");
}

void dispstop()
{
    printf("DESCRIPTION:\n"
        "        Sends a STOP control request to a service.\n"
        "USAGE:\n"
        "        sc.exe stop [service name] ");

}
void dispconfig()
{
    printf("DESCRIPTION:\n"
        "        Modifies a service entry in the registry and Service Database.\n"
        "USAGE:\n"
        "        sc.exe config [service name] <option1> <option2>...\n"
        "\n"
        "OPTIONS:\n"
        "NOTE: The option name includes the equal sign.\n"
        "      A space is required between the equal sign and the value.\n"
        "      To remove the dependency, use a single / as dependency value.\n"
        " type= <own|share|interact|kernel|filesys>\n"
        " start= <boot|system|auto|demand|disabled>\n"
        " error= <normal|severe|critical|ignore>\n"
        " binPath= <BinaryPathName to the .exe file>\n"
        " DisplayName= <display name>\n"
        " password= <password>\n"
        "");
}
void dispquery()
{
    printf("DESCRIPTION:\n"
        "        Obtains and displays information about the specified service, driver, type of service, or type of driver. If the query command is followed by a service name, the status for that service is returned.  Further options do not apply in\n"
        "        this case.  If the query command is followed by nothing or one of\n"
        "        the options listed below, the services are enumerated.\n"
        "USAGE:\n"
        "        sc.exe query [service name] <option1> <option2>...\n"
        "        sc.exe query <option1> <option2>...\n"
        "\n"
        "OPTIONS:\n"
        "\n"
        "    type=    Type of services to enumerate (driver, service, userservice, all)\n"
        "             (default = service)\n"
        "    state=   State of services to enumerate (inactive, all)\n"
        "             (default = active)\n"
        "\n"
        "");
}
void main(int argc, CHAR* argv[])
{
    
    //return if arguments are less
    if (argc < 2)
    {
        
        dispsc();
        dispquery();
        return;
        
    }

    wchar_t wtext[100];
    size_t outSize;

    wchar_t wtext1[100];
    size_t outSize1;

    wchar_t wtext2[100];
    size_t outSize2;

    wchar_t wtext3[100];
    size_t outSize3;

    int i;
    
    // sc query
    if (strcmp(argv[1], "query") == 0)
    {
        if (argc == 2)
        {
            //query the entire database
            queryall();
        }
        else if (argc == 3)
        {
            //query specific service
            mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            szSvcName = wtext;
            querysc();
        }
        else 
        {
            //check different flags for query 
            for ( i = 2; i < argc; i = i + 2)
            {
                if (strcmp(argv[i], "state=") == 0)
                {
                    strcpy_s(state,sizeof(state), argv[i+1]);
                }
                else if (strcmp(argv[i], "type=") == 0)
                {
                    strcpy_s(type,sizeof(type), argv[i+1]);
                }
            }
            queryall();
        }
        return;
        
    }

    //sc create
    if (strcmp(argv[1], "create") == 0)
    {
        if (argc <= 4)
        {
            dispcreate();
            return;
        }
        else if (_strcmpi(argv[3], "binPath=") != 0 )
        {
            //checking if binarypath parameter does not exist
            dispcreate();
            return;
        }
        else
        {
            //initiliaze the service name
            mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            szSvcName = wtext;
            
            //initiliaze the binary path
            mbstowcs_s(&outSize1, wtext1, strlen(argv[4]) + 1, argv[4], strlen(argv[4]));
            binpath = wtext1;
            
            //set different flags 
            for (i = 5; i < argc; i = i + 2)
            {


                if (strcmp(argv[i], "start=") == 0)
                {       
                    strcpy_s(start, sizeof(start), argv[i + 1]);
                }
                else if (strcmp(argv[i], "type=") == 0)
                {
                    strcpy_s(type, sizeof(type), argv[i + 1]);
                }
                else if (strcmp(argv[i], "error=") == 0)
                {
                    strcpy_s(error, sizeof(error), argv[i + 1]);
                }
                else if (_strcmpi(argv[i], "DisplayName=") == 0)
                {
                    mbstowcs_s(&outSize2, wtext2, strlen(argv[i+1]) + 1, argv[i+1], strlen(argv[i+1]));
                    displayname = wtext2;
                }
                else if (_strcmpi(argv[i], "password=") == 0)
                {
                    mbstowcs_s(&outSize3, wtext3, strlen(argv[i+1]) + 1, argv[i+1], strlen(argv[i+1]));
                    password = wtext3;
                }
            }
            create();
            return;
        }
           
    }
    //sc qdescription
    if (strcmp(argv[1], "qdescription") == 0)
    {
        if (argc == 3)
        {
            //initiliase service name
            mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            szSvcName = wtext;
            qdesc();
            return;
        }
        dispdesc();
        return;
    }

    //sc start
    if (strcmp(argv[1], "start") == 0)
    {
        if (argc == 3)
        {
            //initiliase service name
            mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            szSvcName = wtext;
            startsc();
            return;
        }
        dispstart();
        return;
    }

    //sc stop
    if (strcmp(argv[1], "stop") == 0)
    {
        if (argc == 3)
        {
            //initiliase service name
            mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            szSvcName = wtext;
            stopsc();
            return;
        }
        dispstop();
        return;
    }

    //sc delete
    if (strcmp(argv[1], "delete") == 0)
    {
        if (argc == 3)
        {
            //initiliase service name
            mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            szSvcName = wtext;
            del();
            return;
        }
        dispdelete();
        return;
    }

    //sc config
    if (strcmp(argv[1], "config") == 0)
    {
        
        if (argc <= 3)
        {
            //return if insufficient parameters
            dispconfig();
            return;
        }
        else
        {
            //initiliase service name
            mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
            szSvcName = wtext;

            for (i = 3; i < argc; i = i + 2)
            {

                //check  different flags
                if (strcmp(argv[i], "start=") == 0)
                {
                    strcpy_s(start, sizeof(start), argv[i + 1]);
                }
                else if (strcmp(argv[i], "type=") == 0)
                {
                    strcpy_s(type, sizeof(type), argv[i + 1]);
                }
                else if (strcmp(argv[i], "error=") == 0)
                {
                    strcpy_s(error, sizeof(error), argv[i + 1]);
                }
                else if (_strcmpi(argv[i], "DisplayName=") == 0)
                {
                    mbstowcs_s(&outSize2, wtext2, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    displayname = wtext2;
                }
                else if (_strcmpi(argv[i], "password=") == 0)
                {
                    mbstowcs_s(&outSize3, wtext3, strlen(argv[i + 1]) + 1, argv[i + 1], strlen(argv[i + 1]));
                    password = wtext3;
                }
                else if (_strcmpi(argv[i], "binPath=") == 0)
                {
                    mbstowcs_s(&outSize1, wtext1, strlen(argv[i+1]) + 1, argv[i+1], strlen(argv[i+1]));
                    binpath = wtext1;
                }
            }
            configsc();
            return;
        }
        

    }
    
    //sc failure
    if (strcmp(argv[1], "failure") == 0)
    {
        
        if (argc <= 3)
        {
            //return if insufficient parameters
            dispfailure();
            return;
        }
        
        mbstowcs_s(&outSize, wtext, strlen(argv[2]) + 1, argv[2], strlen(argv[2]));
        szSvcName = wtext;

        //set different flags
        for (i = 3; i < argc; i = i + 2)
        {
            if (strcmp(argv[i], "reset=") == 0)
            {
                strcpy_s(reset, sizeof(reset), argv[i + 1]);
            }
            else if (strcmp(argv[i], "reboot=") == 0)
            {
                strcpy_s(reboot, sizeof(reboot), argv[i + 1]);
            }
            else if (strcmp(argv[i], "command=") == 0)
            {
                strcpy_s(command, sizeof(type), argv[i + 1]);
            }
            else if (strcmp(argv[i], "actions=") == 0)
            {
                strcpy_s(action, sizeof(action), argv[i + 1]);
            }
        }
        failuresc();


        return;
    }
    
    dispsc();
    
}

void create()
{
    

    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    DWORD     dwServiceType= SERVICE_WIN32_OWN_PROCESS;
    DWORD     dwStartType= SERVICE_DEMAND_START;
    DWORD     dwErrorControl= SERVICE_ERROR_NORMAL;
    
    //create scm handle
    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    //check the service type for service

    if (strcmp(type, "own") == 0)
    {
        dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    }
    else if (strcmp(type, "share") == 0)
    {
        dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    }
    else if (strcmp(type, "kernel") == 0)
    {
        dwServiceType = SERVICE_KERNEL_DRIVER;
    }
    else if (strcmp(type, "filesys") == 0)
    {
        dwServiceType = SERVICE_FILE_SYSTEM_DRIVER;
    }
    else if (strcmp(type, "rec") == 0)
    {
        dwServiceType = SERVICE_RECOGNIZER_DRIVER;
    }

  //check start type of service
    if (strcmp(start, "boot") == 0)
    {
        dwStartType = SERVICE_BOOT_START;
    }
    else if (strcmp(start, "system") == 0)
    {
        dwStartType = SERVICE_SYSTEM_START;
    }
    else if (strcmp(start, "auto") == 0)
    {
        dwStartType = SERVICE_AUTO_START;
    }
    else if (strcmp(start, "demand") == 0)
    {
        dwStartType = SERVICE_DEMAND_START;
    }
    else if (strcmp(start, "disabled") == 0)
    {
        dwStartType = SERVICE_DISABLED;
    }

    //check the error type
    
    if (strcmp(error, "normal") == 0)
    {
        dwErrorControl = SERVICE_ERROR_NORMAL;
    }
    else if (strcmp(error, "severe") == 0)
    {
        dwErrorControl = SERVICE_ERROR_SEVERE;
    }
    else if (strcmp(error, "critical") == 0)
    {
        dwErrorControl = SERVICE_ERROR_CRITICAL;
    }
    else if (strcmp(error, "ignore") == 0)
    {
        dwErrorControl = SERVICE_ERROR_IGNORE;
    }
   

    // Create the service

    schService = CreateService(
        schSCManager,               // SCM database 
        szSvcName,                  // name of service 
        displayname,                // service name to display 
        SERVICE_ALL_ACCESS,         // desired access 
        dwServiceType,              // service type 
        dwStartType,                // start type 
        dwErrorControl,             // error control type 
        binpath,                    //L"C:\Windows\System32\notepad.exe",          path to service's binary 
        NULL,                       // no load ordering group 
        NULL,                       // no tag identifier 
        NULL,                       // no dependencies 
        NULL,                       // LocalSystem account 
        password);                  //password 

    if (schService == NULL)
    {
        printf("CreateService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service installed successfully\n");

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

void del()
{
    
    SERVICE_STATUS ssStatus;
    
    // Get a handle to the SCM database. 

    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    schService = OpenService(
        schSCManager,       // SCM database 
        szSvcName,          // name of service 
        DELETE);            // need delete access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Delete the service.

    if (!DeleteService(schService))
    {
        printf("DeleteService failed (%d)\n", GetLastError());
    }
    else printf("Service deleted successfully\n");

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

void qdesc()
{
    
    LPQUERY_SERVICE_CONFIG lpsc;
    LPSERVICE_DESCRIPTION lpsd;
    lpsc = NULL;
    lpsd = NULL;
    DWORD dwBytesNeeded, cbBufSize, dwError;
    
    
    // Get a handle to the SCM database. 

    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    schService = OpenService(
        schSCManager,          // SCM database 
        szSvcName,             // name of service 
        SERVICE_QUERY_CONFIG); // need query config access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Get the configuration information.

    if (!QueryServiceConfig(
        schService,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig failed (%d)", dwError);
            goto cleanup;
        }
    }

    if (!QueryServiceConfig(
        schService,
        lpsc,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig failed (%d)", GetLastError());
        goto cleanup;
    }

    if (!QueryServiceConfig2(
        schService,
        SERVICE_CONFIG_DESCRIPTION,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsd = (LPSERVICE_DESCRIPTION)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig2 failed (%d)", dwError);
            goto cleanup;
        }
    }

    if (!QueryServiceConfig2(
        schService,
        SERVICE_CONFIG_DESCRIPTION,
        (LPBYTE)lpsd,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig2 failed (%d)", GetLastError());
        goto cleanup;
    }

    // Print the configuration information.
    
    _tprintf(TEXT("\n\nSERVICE_NAME: %s  \n"), szSvcName);
    _tprintf(TEXT("BINARY_PATH: %s\n"), lpsc->lpBinaryPathName);
    _tprintf(TEXT("DESCRIPTION: "));
    //check if description is null or not
    if (lpsd->lpDescription != NULL && lstrcmp(lpsd->lpDescription, TEXT("")) != 0)
        _tprintf(TEXT("%s"), lpsd->lpDescription);
    _tprintf(TEXT("\n\n"));
   
    LocalFree(lpsc);
    LocalFree(lpsd);

cleanup:
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}
void startsc()
{
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwOldCheckPoint;
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwBytesNeeded;

    // Get a handle to the SCM database. 
    //szSvcName = TEXT("tatti");
    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    schService = OpenService(
        schSCManager,         // SCM database 
        szSvcName,            // name of service 
        //L"tatti",
        SERVICE_ALL_ACCESS);  // full access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Check the status in case the service is not stopped. 

    if (!QueryServiceStatusEx(
        schService,                     // handle to service 
        SC_STATUS_PROCESS_INFO,         // information level
        (LPBYTE)&ssStatus,             // address of structure
        sizeof(SERVICE_STATUS_PROCESS), // size of structure
        &dwBytesNeeded))              // size needed if buffer is too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }

    // Check if the service is already running. It would be possible 
    // to stop the service here, but for simplicity this example just returns. 

    if (ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
    {
        printf("Cannot start the service because it is already running\n");
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }

    // Save the tick count and initial checkpoint.

    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    // Wait for the service to stop before attempting to start it.

    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth of the wait hint but not less than 1 second  
        // and not more than 10 seconds. 

        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        // Check the status until the service is no longer stop pending. 

        if (!QueryServiceStatusEx(
            schService,                     // handle to service 
            SC_STATUS_PROCESS_INFO,         // information level
            (LPBYTE)&ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded))              // size needed if buffer is too small
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint)
        {
            // Continue to wait and check.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
            {
                printf("Timeout waiting for service to stop\n");
                CloseServiceHandle(schService);
                CloseServiceHandle(schSCManager);
                return;
            }
        }
    }

    // Attempt to start the service.

    if (!StartService(
        schService,  // handle to service 
        0,           // number of arguments 
        NULL))      // no arguments 
    {
        printf("StartService failed (%d)\n", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service start pending...\n");

    // Check the status until the service is no longer start pending. 

    if (!QueryServiceStatusEx(
        schService,                     // handle to service 
        SC_STATUS_PROCESS_INFO,         // info level
        (LPBYTE)&ssStatus,             // address of structure
        sizeof(SERVICE_STATUS_PROCESS), // size of structure
        &dwBytesNeeded))              // if buffer too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }

    // Save the tick count and initial checkpoint.

    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth the wait hint, but no less than 1 second and no 
        // more than 10 seconds. 

        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        // Check the status again. 

        if (!QueryServiceStatusEx(
            schService,             // handle to service 
            SC_STATUS_PROCESS_INFO, // info level
            (LPBYTE)&ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded))              // if buffer too small
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            break;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint)
        {
            // Continue to wait and check.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
            {
                // No progress made within the wait hint.
                break;
            }
        }
    }

    // Determine whether the service is running.

    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        printf("Service started successfully.\n");
    }
    else
    {
        printf("Service not started. \n");
        printf("  Current State: %d\n", ssStatus.dwCurrentState);
        printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
        printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
        printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

void stopsc()
{
    SERVICE_STATUS_PROCESS ssp;
    DWORD dwStartTime = GetTickCount();
    DWORD dwBytesNeeded;
    DWORD dwTimeout = 30000; // 30-second time-out
    DWORD dwWaitTime;

    // Get a handle to the SCM database. 

    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    schService = OpenService(
        schSCManager,         // SCM database 
        szSvcName,            // name of service 
        SERVICE_STOP |
        SERVICE_QUERY_STATUS |
        SERVICE_ENUMERATE_DEPENDENTS);

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Make sure the service is not already stopped.

    if (!QueryServiceStatusEx(
        schService,
        SC_STATUS_PROCESS_INFO,
        (LPBYTE)&ssp,
        sizeof(SERVICE_STATUS_PROCESS),
        &dwBytesNeeded))
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        goto stop_cleanup;
    }

    if (ssp.dwCurrentState == SERVICE_STOPPED)
    {
        printf("Service is already stopped.\n");
        goto stop_cleanup;
    }

    // If a stop is pending, wait for it.

    while (ssp.dwCurrentState == SERVICE_STOP_PENDING)
    {
        printf("Service stop pending...\n");

        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth of the wait hint but not less than 1 second  
        // and not more than 10 seconds. 

        dwWaitTime = ssp.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        if (!QueryServiceStatusEx(
            schService,
            SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssp,
            sizeof(SERVICE_STATUS_PROCESS),
            &dwBytesNeeded))
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            goto stop_cleanup;
        }

        if (ssp.dwCurrentState == SERVICE_STOPPED)
        {
            printf("Service stopped successfully.\n");
            goto stop_cleanup;
        }

        if (GetTickCount() - dwStartTime > dwTimeout)
        {
            printf("Service stop timed out.\n");
            goto stop_cleanup;
        }
    }

    // If the service is running, dependencies must be stopped first.

    StopDep();

    // Send a stop code to the service.

    if (!ControlService(
        schService,
        SERVICE_CONTROL_STOP,
        (LPSERVICE_STATUS)&ssp))
    {
        printf("ControlService failed (%d)\n", GetLastError());
        goto stop_cleanup;
    }

    // Wait for the service to stop.

    while (ssp.dwCurrentState != SERVICE_STOPPED)
    {
        Sleep(ssp.dwWaitHint);
        if (!QueryServiceStatusEx(
            schService,
            SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssp,
            sizeof(SERVICE_STATUS_PROCESS),
            &dwBytesNeeded))
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            goto stop_cleanup;
        }

        if (ssp.dwCurrentState == SERVICE_STOPPED)
            break;

        if (GetTickCount() - dwStartTime > dwTimeout)
        {
            printf("Wait timed out\n");
            goto stop_cleanup;
        }
    }
    printf("Service stopped successfully\n");

stop_cleanup:
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

BOOL StopDep()
{
    DWORD i;
    DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = NULL;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;
    SERVICE_STATUS_PROCESS  ssp;

    DWORD dwStartTime = GetTickCount();
    DWORD dwTimeout = 30000; // 30-second time-out

    // Pass a zero-length buffer to get the required buffer size.
    if (EnumDependentServices(schService, SERVICE_ACTIVE,
        lpDependencies, 0, &dwBytesNeeded, &dwCount))
    {
        // If the Enum call succeeds, then there are no dependent
        // services, so do nothing.
        return TRUE;
    }
    else
    {
        if (GetLastError() != ERROR_MORE_DATA)
            return FALSE; // Unexpected error

        // Allocate a buffer for the dependencies.
        lpDependencies = (LPENUM_SERVICE_STATUS)HeapAlloc(
            GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded);

        if (!lpDependencies)
            return FALSE;

        __try {
            // Enumerate the dependencies.
            if (!EnumDependentServices(schService, SERVICE_ACTIVE,
                lpDependencies, dwBytesNeeded, &dwBytesNeeded,
                &dwCount))
                return FALSE;

            for (i = 0; i < dwCount; i++)
            {
                ess = *(lpDependencies + i);
                // Open the service.
                hDepService = OpenService(schSCManager,
                    ess.lpServiceName,
                    SERVICE_STOP | SERVICE_QUERY_STATUS);

                if (!hDepService)
                    return FALSE;

                __try {
                    // Send a stop code.
                    if (!ControlService(hDepService,
                        SERVICE_CONTROL_STOP,
                        (LPSERVICE_STATUS)&ssp))
                        return FALSE;

                    // Wait for the service to stop.
                    while (ssp.dwCurrentState != SERVICE_STOPPED)
                    {
                        Sleep(ssp.dwWaitHint);
                        if (!QueryServiceStatusEx(
                            hDepService,
                            SC_STATUS_PROCESS_INFO,
                            (LPBYTE)&ssp,
                            sizeof(SERVICE_STATUS_PROCESS),
                            &dwBytesNeeded))
                            return FALSE;

                        if (ssp.dwCurrentState == SERVICE_STOPPED)
                            break;

                        if (GetTickCount() - dwStartTime > dwTimeout)
                            return FALSE;
                    }
                }
                __finally
                {
                    // Always release the service handle.
                    CloseServiceHandle(hDepService);
                }
            }
        }
        __finally
        {
            // Always free the enumeration buffer.
            HeapFree(GetProcessHeap(), 0, lpDependencies);
        }
    }
    return TRUE;
}

VOID failuresc()
{
    
    
    // Get a handle to the SCM database. 

    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    LPSERVICE_FAILURE_ACTIONS lpsd=NULL;
    DWORD dwBytesNeeded, cbBufSize, dwError;

    schService = OpenService(
        schSCManager,          // SCM database 
        szSvcName,             // name of service 
        SERVICE_QUERY_CONFIG); // need query config access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }
    //get the current failure configuratiion for the given service
    if (!QueryServiceConfig2(
        schService,
        SERVICE_CONFIG_FAILURE_ACTIONS,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsd = (LPSERVICE_FAILURE_ACTIONS)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig2 failed (%d)", dwError);
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
        }
    }

    if (!QueryServiceConfig2(
        schService,
        SERVICE_CONFIG_FAILURE_ACTIONS,
        (LPBYTE)lpsd,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig2 failed (%d)", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    }

    

    //initialise parameters for failure configuration using previous configuration
    LPWSTR rebootmsg = lpsd->lpRebootMsg;
    LPWSTR lpcommand = lpsd->lpCommand;
    DWORD cac = lpsd->cActions;
    DWORD resetperiod = lpsd->dwResetPeriod;
    SERVICE_FAILURE_ACTIONS sd;
    sd.lpsaActions= lpsd->lpsaActions;

    CloseServiceHandle(schService);

    //set the value of reboot message
    if (strlen(reboot)!=0)
    {
        wchar_t wtext[150];
        size_t outSize;
        mbstowcs_s(&outSize, wtext, strlen(reboot) + 1, reboot, strlen(reboot));
        rebootmsg = wtext;
    }
    sd.lpRebootMsg = rebootmsg;

    //set the value of commands
    if (strlen(command) != 0)
    {
        wchar_t wtext1[150];
        size_t outSize1;
        mbstowcs_s(&outSize1, wtext1, strlen(command) + 1, command, strlen(command));
        lpcommand = wtext1;
    }
    sd.lpCommand = lpcommand;

    //set the value of reset time
    if (strlen(reset) != 0)
    {
        sscanf_s(reset, "%d", &resetperiod, sizeof(resetperiod));
    }
    sd.dwResetPeriod = resetperiod;
    


    //set the value for actions
    if (strlen(action) != 0)
    {
        SC_ACTION_TYPE typeinp[6];
        DWORD delayt[6];
        
        char* next_token;
        char* token = strtok_s(action, "/", &next_token);
        int ci = 0;
        while (token != NULL) 
        {
            printf("%s =", token, &next_token);
            if (strcmp(token, "run") == 0)
            {
                typeinp[ci]= SC_ACTION_RUN_COMMAND;
            }
            else if (strcmp(token, "restart") == 0)
            {
                typeinp[ci] = SC_ACTION_RESTART;
            }
            else if (strcmp(token, "reboot") == 0)
            {
                typeinp[ci] = SC_ACTION_REBOOT;
            }
            else if (strcmp(token, "none") == 0)
            {
                typeinp[ci] = SC_ACTION_NONE;
            }
            token = strtok_s(NULL, "/", &next_token);

            if (token != NULL)
            {
                printf(" %s\n", token);
                delayt[ci] = atoi(token);
                token = strtok_s(NULL, "/", &next_token);

                ci = ci + 1;
            }
        }
        if (ci != 0)
        {
            cac = ci;
            SC_ACTION failActions[3];
            for (int j = 0; j < ci; j++)
            {
                failActions[j].Type = typeinp[j];
                failActions[j].Delay = delayt[j];
            }
            
            sd.lpsaActions = failActions;
        }
        

        
    }
    //set the value of number of actions
    sd.cActions = cac;
    

    

    // Get a handle to the service.

    schService = OpenService(
        schSCManager,            // SCM database 
        szSvcName,               // name of service 
        SERVICE_CHANGE_CONFIG);  // need change config access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Update the failure configuration
    

    
    
    if (!ChangeServiceConfig2(
        schService,                 // handle to service
        SERVICE_CONFIG_FAILURE_ACTIONS, // change: failure
        &sd))                      // new description
    {
        printf("ChangeServiceConfig2 failed\n");
    }
    else printf("Service description updated successfully.\n");

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

VOID configsc()
{

    //get the current service configuration
    DWORD dwBytesNeeded, cbBufSize, dwError;
    LPQUERY_SERVICE_CONFIG lpsc=NULL;

    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    schService = OpenService(
        schSCManager,         // SCM database 
        szSvcName,            // name of service 
        SERVICE_QUERY_CONFIG);  // full access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }
    if (!QueryServiceConfig(
        schService,
        NULL,
        0,
        &dwBytesNeeded))
    {
        dwError = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == dwError)
        {
            cbBufSize = dwBytesNeeded;
            lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else
        {
            printf("QueryServiceConfig failed (%d)", dwError);
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return;
        }
    }

    if (!QueryServiceConfig(
        schService,
        lpsc,
        cbBufSize,
        &dwBytesNeeded))
    {
        printf("QueryServiceConfig failed (%d)", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }
   

    CloseServiceHandle(schService);

    //initialize the current configuration in variables
        DWORD     ServiceType= lpsc->dwServiceType;
        DWORD     StartType = lpsc->dwStartType;
        DWORD     Error = lpsc->dwErrorControl;
        LPCWSTR   BinaryPa = lpsc->lpBinaryPathName;
        LPCWSTR   Pass = NULL;
        LPCWSTR   Display = lpsc->lpDisplayName;

        //set binary path
        if (binpath != NULL)
        {
            BinaryPa = binpath;
        }

        //set password
        if (password != NULL)
        {
            Pass = password;
        }

        //set displayname
        if (displayname != NULL)
        {
            Display = displayname;
        }

        //set service type
        if (strcmp(type, "own") == 0)
        {
            ServiceType = SERVICE_WIN32_OWN_PROCESS;
        }
        else if (strcmp(type, "share") == 0)
        {
            ServiceType = SERVICE_WIN32_SHARE_PROCESS;
        }
        else if (strcmp(type, "kernel") == 0)
        {
            ServiceType = SERVICE_KERNEL_DRIVER;
        }
        else if (strcmp(type, "filesys") == 0)
        {
            ServiceType = SERVICE_FILE_SYSTEM_DRIVER;
        }
        else if (strcmp(type, "rec") == 0)
        {
            ServiceType = SERVICE_RECOGNIZER_DRIVER;
        }

        //set start type of service
        if (strcmp(start, "boot") == 0)
        {
            StartType = SERVICE_BOOT_START;
        }
        else if (strcmp(start, "system") == 0)
        {
            StartType = SERVICE_SYSTEM_START;
        }
        else if (strcmp(start, "auto") == 0)
        {
            StartType = SERVICE_AUTO_START;
        }
        else if (strcmp(start, "demand") == 0)
        {
            StartType = SERVICE_DEMAND_START;
        }
        else if (strcmp(start, "disabled") == 0)
        {
            StartType = SERVICE_DISABLED;
        }


        //set error type
        if (strcmp(error, "normal") == 0)
        {
            Error = SERVICE_ERROR_NORMAL;
        }
        else if (strcmp(error, "severe") == 0)
        {
            Error = SERVICE_ERROR_SEVERE;
        }
        else if (strcmp(error, "critical") == 0)
        {
            Error = SERVICE_ERROR_CRITICAL;
        }
        else if (strcmp(error, "ignore") == 0)
        {
            Error = SERVICE_ERROR_IGNORE;
        }
    

    // Get a handle to the service.

    schService = OpenService(
        schSCManager,            // SCM database 
        szSvcName,               // name of service 
        SERVICE_CHANGE_CONFIG);  // need change config access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Change the service sconfiguration

    if (!ChangeServiceConfig(
        schService,        // handle of service 
        ServiceType, // service type:  
        StartType,  // service start type 
        Error, // error control: 
        BinaryPa,              // binary path:
        NULL,              // load order group: no change 
        NULL,              // tag ID: no change 
        NULL,              // dependencies: no change 
        NULL,              // account name: no change 
        Pass,              // password: 
        Display))            // display name: 
    {
        printf("ChangeServiceConfig failed (%d)\n", GetLastError());
    }
    else printf("Service configuration changed successfully.\n");

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

void querysc()
{
    
    DWORD dwOldCheckPoint;
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwBytesNeeded;
    SERVICE_STATUS_PROCESS ssStatus;
    // Get a handle to the SCM database. 

    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Get a handle to the service.

    schService = OpenService(
        schSCManager,         // SCM database 
        szSvcName,            // name of service 
        SERVICE_ALL_ACCESS);  // full access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Query the service details

    if (!QueryServiceStatusEx(
        schService,                     // handle to service 
        SC_STATUS_PROCESS_INFO,         // information level
        (LPBYTE)&ssStatus,             // address of structure
        sizeof(SERVICE_STATUS_PROCESS), // size of structure
        &dwBytesNeeded))              // size needed if buffer is too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return;
    }
   
    _tprintf(TEXT("\nSERVICE_NAME: %s  \n"), szSvcName);
    DWORD cch = 0;

    //Display name is stored in a different system data type
    if (!GetServiceDisplayNameW(schSCManager, szSvcName, nullptr, &cch))
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            PWSTR buff = (PWSTR)malloc(++cch * sizeof(WCHAR));
            if (GetServiceDisplayNameW(schSCManager, szSvcName, buff, &cch))
            {
                
                _tprintf(TEXT("DISPLAY_NAME: %s  \n"), buff);
            }
        }
    }
    
    printf("  TYPE: %x\n", ssStatus.dwServiceType);
    printf("  STATE: %d\n", ssStatus.dwCurrentState); 
    printf("   %x\n", ssStatus.dwControlsAccepted);
    printf("  WIN32_EXIT_CODE: %d\n", ssStatus.dwWin32ExitCode);
    printf("  SERVICE_EXIT_CODE: %d\n", ssStatus.dwServiceSpecificExitCode);
    printf("  ChHECKPOINT: %d\n", ssStatus.dwCheckPoint);
    printf("  WAIT_HINT: %d\n", ssStatus.dwWaitHint);
    
    

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

void queryall()
{
    DWORD        dwServiceType=  SERVICE_WIN32 ;
    DWORD        dwServiceState= SERVICE_ACTIVE;

    
        //set the service type   
        if (strcmp(type, "driver")==0)
        {
            dwServiceType = SERVICE_DRIVER;
        }
        else if (strcmp(type, "service") == 0)
        {
            dwServiceType = SERVICE_WIN32;
        }
        else if (strcmp(type, "all") == 0)
        {
            dwServiceType = SERVICE_DRIVER | SERVICE_WIN32 | SERVICE_KERNEL_DRIVER;
        }
        else if (strcmp(type, "own") == 0)
        {
            dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        }
        else if (strcmp(type, "share") == 0)
        {
            dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
        }
        else if (strcmp(type, "kernel") == 0)
        {
            dwServiceType = SERVICE_KERNEL_DRIVER;
        }
        else if (strcmp(type, "filesys") == 0)
        {
            dwServiceType = SERVICE_FILE_SYSTEM_DRIVER;
        }
        

    
        //set the state of the service
        if (strcmp(state, "active") == 0)
        {
            dwServiceState = SERVICE_ACTIVE;
        }
        else if (strcmp(state, "inactive") == 0)
        {
            dwServiceState = SERVICE_INACTIVE;
        }
        else if (strcmp(state, "all") == 0)
        {
            dwServiceState = SERVICE_STATE_ALL;
        }

       
        
    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
    
    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    DWORD bytesNeeded;
    DWORD servicesNum;
    //enumerate all services
    BOOL status = EnumServicesStatusEx(
        schSCManager,
        SC_ENUM_PROCESS_INFO,
        dwServiceType,
        dwServiceState,
        NULL,
        0,
        &bytesNeeded,
        &servicesNum,
        NULL,
        NULL
    );

    //this set is used to determine correcr buffer size
    PBYTE lpBytes = (PBYTE)malloc(bytesNeeded);

    status = EnumServicesStatusEx(
        schSCManager,
        SC_ENUM_PROCESS_INFO,
        dwServiceType,
        dwServiceState,
        lpBytes,
        bytesNeeded,
        &bytesNeeded,
        &servicesNum,
        NULL,
        NULL
    );

    int i = 0;
    int j = 0;
    ENUM_SERVICE_STATUS_PROCESS* services = (ENUM_SERVICE_STATUS_PROCESS*)lpBytes;
   
    CloseServiceHandle(schSCManager);
    //enumerate through different services
    for (i = 0; i < servicesNum; i++)
    {
        szSvcName = services[i].lpServiceName;
        querysc(); 
    }

    
}