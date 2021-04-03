#include <windows.h>
#include <tchar.h>
#include <strsafe.h>


SC_HANDLE schSCManager;
SC_HANDLE schService;
LPCWSTR szSvcName;

TCHAR type[50];
TCHAR state[50];
TCHAR start[50];
TCHAR error[50];
TCHAR binpath[100];
TCHAR displayname[50];
TCHAR reset[50];
TCHAR reboot[50];
TCHAR action[50];
TCHAR command[50];

void create();
void del();
void qdesc();
void startsc();
void stopsc();
BOOL StopDep();
VOID failuresc();
VOID configsc();
void querysc();
void queryall();

void main(int argc, TCHAR* argv[])
{
    //printf("tatti");
    printf("%s",argv[1]);
    if (argc < 2)
    {
        printf("ERROR:\tIncorrect number of arguments\n\n");
        //DisplayUsage();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("t")) == 0)
    {
        //SvcInstall();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("query")) == 0)
    {
        //SvcInstall();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("create")) == 0)
    {
        //SvcInstall();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("qdescription")) == 0)
    {
        printf("tatti");
        if (argc == 3)
        {
            printf("tatti");
            szSvcName = argv[2];
            qdesc();
        }
        return;
    }
    if (lstrcmpi(argv[1], TEXT("create")) == 0)
    {
        //SvcInstall();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("start")) == 0)
    {
        //SvcInstall();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("stop")) == 0)
    {
        //SvcInstall();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("delete")) == 0)
    {
        //SvcInstall();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("config")) == 0)
    {
        //SvcInstall();
        //return;
    }
    if (lstrcmpi(argv[1], TEXT("failure")) == 0)
    {
        //SvcInstall();
        //return;
    }
    

    //queryall();
    /*szSvcName = TEXT("eventlog");
    querysc();
    szSvcName = TEXT("tatti");
    create();
    qdesc();
    querysc();
    startsc();
    stopsc();
    del();*/
}

void create()
{
    

    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    WCHAR szPath[100];
    //szPath = TEXT("C:\\Windows\\System32\\notepad.exe");
    if (!GetModuleFileName(NULL, szPath, 100))
    {
        printf("Cannot install service (%d)\n", GetLastError());
        return;
    }

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

    // Create the service

    schService = CreateService(
        schSCManager,              // SCM database 
        L"TATTI",                   // name of service 
        L"TATTI",                   // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        L"C:\\Windows\\System32\\notepad.exe",                    // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 

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
    //LPCWSTR svcname;
    //svcname = TEXT("tatti");
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
    
    //szSvcName = TEXT("Tatti");
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
    
    _tprintf(TEXT("\nSERVICE_NAME: %s  \n"), L"Tatti");
    _tprintf(TEXT("BINARY_PATH: %s\n"), lpsc->lpBinaryPathName);
    _tprintf(TEXT("DESCRIPTION: "));

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
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    //SERVICE_DESCRIPTION sd;
    SERVICE_FAILURE_ACTIONS sd;
    //LPTSTR szDesc = TEXT("This is a test description");
    //char *t = "sss";
    char text[] = "something";
    wchar_t wtext[20];
    size_t outSize;
    mbstowcs_s(&outSize,wtext, strlen(text) + 1, text, strlen(text));
    LPWSTR rebootmsg= wtext;

    char text1[] = "something";
    wchar_t wtext1[20];
    size_t outSize1;
    mbstowcs_s(&outSize1, wtext1, strlen(text1) + 1, text1, strlen(text1));
    LPWSTR lpcommand = wtext1;
   
    const DWORD cac = 1;
    //const int i = (int) cac;
    SC_ACTION failActions[cac];
    failActions[0].Type = SC_ACTION_NONE;
    failActions[0].Delay = 120000;

    DWORD resetperiod= 10;
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
        schSCManager,            // SCM database 
        szSvcName,               // name of service 
        SERVICE_CONFIG_FAILURE_ACTIONS);  // need change config access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Change the service description.
    sd.dwResetPeriod= resetperiod;
    sd.lpRebootMsg= rebootmsg;
    sd.lpCommand = lpcommand;
    sd.cActions=cac;
    sd.lpsaActions = failActions;

    //sd.lpDescription = szDesc;

    if (!ChangeServiceConfig2(
        schService,                 // handle to service
        SERVICE_CONFIG_DESCRIPTION, // change: description
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
        DWORD     ServiceType= SERVICE_NO_CHANGE;
        DWORD     StartType = SERVICE_NO_CHANGE;
        DWORD     Error = SERVICE_NO_CHANGE;
        LPCWSTR   BinPath = NULL;
        LPCWSTR   LoadOGroup = NULL;
        LPDWORD   TagId = NULL;
        LPCWSTR   Dependencies = NULL;
        LPCWSTR   ServiceStartName = NULL;
        LPCWSTR   Password = NULL;
        LPCWSTR   DisplayName = NULL;
        
        /*
        char para[20]="yes";
        char value[20];
        if (strcmp(para, "type"))
        {
            if (strcmp(value, "own"))
            {
                ServiceType = SERVICE_WIN32_OWN_PROCESS;
            }
            else if (strcmp(value, "share"))
            {
                ServiceType = SERVICE_WIN32_SHARE_PROCESS;
            }
            else if (strcmp(value, "interact"))
            {
                ServiceType = SERVICE_INTERACTIVE_PROCESS;
            }
            else if (strcmp(value, "kernel"))
            {
                ServiceType = SERVICE_KERNEL_DRIVER;
            }
            else if (strcmp(value, "filesys")) 
            {
                ServiceType = SERVICE_FILE_SYSTEM_DRIVER;
            }
            


        }
        else if (strcmp(para, "start"))
        {
                
            if (strcmp(value, "boot"))
            {
                StartType = SERVICE_BOOT_START;
            }
            else if (strcmp(value, "system"))
            {
                StartType = SERVICE_SYSTEM_START;
            }
            else if (strcmp(value, "auto"))
            {
                StartType = SERVICE_AUTO_START;
            }
            else if (strcmp(value, "demand"))
            {
                StartType = SERVICE_DEMAND_START;
            }
            else if (strcmp(value, "disabled"))
            {
                StartType = SERVICE_DISABLED;
            }
            else if (strcmp(value, "delayed-auto"))
            {
                StartType = ;
            }
            
        }
        else if (strcmp(para, "error"))
        {
            if (strcmp(value, "normal"))
            {
                Error = SERVICE_ERROR_NORMAL;
            }
            else if (strcmp(value, "severe"))
            {
                Error = SERVICE_ERROR_SEVERE;
            }
            else if (strcmp(value, "critical"))
            {
                Error = SERVICE_ERROR_CRITICAL;
            }
            else if (strcmp(value, "ignore"))
            {
                Error = SERVICE_ERROR_IGNORE;
            }
        }
        else if (strcmp(para, "binPath"))
        {
        }
        
        else if (strcmp(para, "DisplayName"))
        {
        }
        else if (strcmp(para, "password"))
        {
        }
        */
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
        schSCManager,            // SCM database 
        szSvcName,               // name of service 
        SERVICE_CHANGE_CONFIG);  // need change config access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // Change the service start type.

    if (!ChangeServiceConfig(
        schService,        // handle of service 
        SERVICE_NO_CHANGE, // service type: no change 
        SERVICE_DISABLED,  // service start type 
        SERVICE_NO_CHANGE, // error control: no change 
        NULL,              // binary path: no change 
        NULL,              // load order group: no change 
        NULL,              // tag ID: no change 
        NULL,              // dependencies: no change 
        NULL,              // account name: no change 
        NULL,              // password: no change 
        NULL))            // display name: no change
    {
        printf("ChangeServiceConfig failed (%d)\n", GetLastError());
    }
    else printf("Service disabled successfully.\n");

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
   
    _tprintf(TEXT("\nSERVICE_NAME: %s  \n"), szSvcName);
    char hexString[20];
    _itoa_s(ssStatus.dwServiceType, hexString, 16);
    printf("  TYPE: %s\n", hexString);
    printf("  STATE: %d\n", ssStatus.dwCurrentState); 
    printf("   %d\n", ssStatus.dwControlsAccepted);
    printf("  WIN32_EXIT_CODE: %d\n", ssStatus.dwWin32ExitCode);
    printf("  SERVICE_EXIT_CODE: %d\n", ssStatus.dwServiceSpecificExitCode);
    printf("  ChHECKPOINT: %d\n", ssStatus.dwCheckPoint);
    printf("  WAIT_HINT: %d\n", ssStatus.dwWaitHint);
    
    

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

void queryall()
{
    DWORD        dwServiceType= SERVICE_WIN32;
    DWORD        dwServiceState= SERVICE_STATE_ALL;

    char para[20] = "yes";
    char value[20];
    if (strcmp(para, "type"))
    {
        if (strcmp(value, "driver"))
        {
            dwServiceType = SERVICE_DRIVER;
        }
        else if (strcmp(value, "service"))
        {
            dwServiceType = SERVICE_WIN32;
        }
        else if (strcmp(value, "all"))
        {
            dwServiceType = SERVICE_DRIVER | SERVICE_WIN32;
        }
        else if (strcmp(value, "own"))
        {
            dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        }
        else if (strcmp(value, "share"))
        {
            dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
        }
        else if (strcmp(value, "kernel"))
        {
            dwServiceType = SERVICE_KERNEL_DRIVER;
        }
        else if (strcmp(value, "filesys"))
        {
            dwServiceType = SERVICE_FILE_SYSTEM_DRIVER;
        }

    }
    else if (strcmp(para, "state"))
    {

        if (strcmp(value, "active"))
        {
            dwServiceState = SERVICE_ACTIVE;
        }
        else if (strcmp(value, "inactive"))
        {
            dwServiceState = SERVICE_INACTIVE;
        }
        else if (strcmp(value, "all"))
        {
            dwServiceState = SERVICE_STATE_ALL;
        }


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

    for (i = 0; i < servicesNum; i++)
    {
        //printf("%s\n", services[i].lpServiceName);
        printf("%d", i);
        _tprintf(TEXT("\nSERVICE_NAME: %s  \n"), services[i].lpServiceName);
        
        szSvcName = services[i].lpServiceName;
       // _tprintf(TEXT("\nSERVICE_NAME: %s  \n"), szSvcName);

        querysc();
        
    }

    
}