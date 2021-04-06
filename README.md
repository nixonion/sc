SC.EXE

Implementation of windows command line program sc that is used for communicating with the Service Control Manager and services. The implementaion is done using with C++ using WIN32 APIs.

How to use:


sc.exe\n
DESCRIPTION:\n
        SC is a command line program used for communicating with the Service Control Manager and services.
USAGE:\n
        sc [command] [service name] <option1> <option2>...
\n
        Further help on commands can be obtained by typing: "sc [command]" 
        Commands:
          query-----------Queries the status for a service, or
                          enumerates the status for types of services.
          start-----------Starts a service.
          stop------------Sends a STOP request to a service.
          config----------Changes the configuration of a service (persistent).
          failure---------Changes the actions taken by a service upon failure.
          qdescription----Queries the description for a service.
          delete----------Deletes a service (from the registry).
          create----------Creates a service. (adds it to the registry).
          
--------------------

sc.exe failure\n
DESCRIPTION:\n
        Changes the actions upon failure\n
USAGE:\n
        sc.exe failure [service name] <option1> <option2>...\n

OPTIONS:\n
        reset=   <Length of period of no failures (in seconds)
                  after which to reset the failure count to 0 (may be INFINITE)>
                  (Must be used in conjunction with actions= )
        reboot=  <Message broadcast before rebooting on failure>
        command= <Command line to be run on failure>
        actions= <Failure actions and their delay time (in milliseconds),
                  separated by / (forward slash) -- e.g., run/5000/reboot/800
                  Valid actions are <run|restart|reboot> >
                  (Must be used in conjunction with the reset= option)

--------------------
sc.exe create\n
DESCRIPTION:\n
        Creates a service entry in the registry and Service Database.\n
USAGE:\n
        sc.exe create [service name] [binPath= ] <option1> <option2>...\n

OPTIONS:\n
NOTE: The option name includes the equal sign.
      A space is required between the equal sign and the value.\n
                 type= <own|share|interact|kernel|filesys>
                       (default = own)
                 start= <boot|system|auto|demand|disabled>
                       (default = demand)
                 error= <normal|severe|critical|ignore>
                       (default = normal)
                 binPath= <BinaryPathName to the .exe file>
                 DisplayName= <display name>
                 password= <password>

--------------

sc.exe qdescription\n

DESCRIPTION:\n
        Retrieves the description string of a service.\n
USAGE:\n
        sc.exe qdescription [service name] \n


-----------------

sc.exe delete\n
DESCRIPTION:\n
        Deletes a service entry from the registry. If the service is running, or another process has an open handle to the service, the service is simply marked for deletion.\n
USAGE:\n
        sc.exe delete [service name]\n


----------------

sc.exe start\n
DESCRIPTION:\n
        Starts a service running.\n
USAGE:\n
        sc.exe start [service name] \n

-----------------

sc.exe stop\n
DESCRIPTION:\n
        Sends a STOP control request to a service.\n
USAGE:\n
        sc.exe stop [service name] \n

-------------------

sc.exe config\n
DESCRIPTION:\n
        Modifies a service entry in the registry and Service Database.\n
USAGE:\n
        sc.exe config [service name] <option1> <option2>...\n

OPTIONS:\n
NOTE: The option name includes the equal sign.
      A space is required between the equal sign and the value. To remove the dependency, use a single / as dependency value.\n
 
     type= <own|share|interact|kernel|filesys>
     start= <boot|system|auto|demand|disabled>
     error= <normal|severe|critical|ignore>
     binPath= <BinaryPathName to the .exe file>
     DisplayName= <display name>
     password= <password>

-------------

sc.exe query\n
DESCRIPTION:\n
        Obtains and displays information about the specified service, driver, type of service, or type of driver. If the query command is followed by a service name, the status for that service is returned.  Further options do not apply in this case.  If the query command is followed by nothing or one of the options listed below, the services are enumerated.\n
USAGE:\n
        sc.exe query [service name] <option1> <option2>...
        sc.exe query <option1> <option2>...

OPTIONS:\n

    type=    Type of services to enumerate (driver, service, userservice, all)
             (default = service)
    state=   State of services to enumerate (inactive, all)
             (default = active)

