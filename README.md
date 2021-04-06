SC.EXE

Implementation of windows command line program sc that is used for communicating with the Service Control Manager and services. The implementaion is done using with C++ using WIN32 APIs.


----------------------------
How to use:

-----------------------------


sc.exe

DESCRIPTION:

        SC is a command line program used for communicating with the Service Control Manager and services.
USAGE:

        sc.exe [command] [service name] <option1> <option2>...

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

sc.exe failure

DESCRIPTION:

        Changes the actions upon failure
        
USAGE:

        sc.exe failure [service name] <option1> <option2>...

OPTIONS:

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
sc.exe create

DESCRIPTION:

        Creates a service entry in the registry and Service Database.
USAGE:

        sc.exe create [service name] [binPath= ] <option1> <option2>...

OPTIONS:

        NOTE: The option name includes the equal sign.
        A space is required between the equal sign and the value.
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

sc.exe qdescription

DESCRIPTION:

        Retrieves the description string of a service.
USAGE:

        sc.exe qdescription [service name] 


-----------------

sc.exe delete

DESCRIPTION:

        Deletes a service entry from the registry. If the service is running, or another process has an open handle to the service, the service is simply marked for deletion.
        
USAGE:

        sc.exe delete [service name]\n


----------------

sc.exe start

DESCRIPTION:

        Starts a service running.
        
USAGE:

        sc.exe start [service name] 

-----------------

sc.exe stop

DESCRIPTION:

        Sends a STOP control request to a service.
        
USAGE:

        sc.exe stop [service name] 

-------------------

sc.exe config

DESCRIPTION:

        Modifies a service entry in the registry and Service Database.
USAGE:

        sc.exe config [service name] <option1> <option2>...

OPTIONS:


NOTE: The option name includes the equal sign.
      A space is required between the equal sign and the value. To remove the dependency, use a single / as dependency value.
 
     type= <own|share|interact|kernel|filesys>
     start= <boot|system|auto|demand|disabled>
     error= <normal|severe|critical|ignore>
     binPath= <BinaryPathName to the .exe file>
     DisplayName= <display name>
     password= <password>

-------------

sc.exe query

DESCRIPTION:


        Obtains and displays information about the specified service, driver, type of service, or type of driver. If the query command is followed by a service name, the status for that service is returned.  Further options do not apply in this case.  If the query command is followed by nothing or one of the options listed below, the services are enumerated.\n
USAGE:

        sc.exe query [service name] <option1> <option2>...
        sc.exe query <option1> <option2>...

OPTIONS:


    type=    Type of services to enumerate (driver, service, userservice, all)
             (default = service)
    state=   State of services to enumerate (inactive, all)
             (default = active)

