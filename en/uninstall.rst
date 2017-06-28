
:meta-keywords: uninstall cubrid linux, uninstall cubrid windows.
:meta-description: Uninstalling CUBRID in Linux and Windows.

.. _uninstall:

Uninstalling CUBRID
===================

Uninstalling CUBRID in Linux
----------------------------

**Uninstalling CUBRID of SH package or compressed package**

To uninstall SH package(file extension of installing package is .sh) or compressed package(file extension of installing package is .tar.gz), proceed the following steps.

*   Remove databases after stopping CUBRID service.

    Remove all created databases after stopping CUBRID service.
    
    For example, if there are testdb1, testdb2 as databases, do the following commands.
    
    ::
    
        $ cubrid service stop
        $ cubrid deletedb testdb1
        $ cubrid deletedb testdb2
        
*   Remove CUBRID engine.

    Remove $CUBRID and its subdirectories.
    
    ::
    
        $ echo $CUBRID
            /home1/user1/CUBRID
            
        $ rm -rf /home1/user1/CUBRID

*   Remove auto-starting script.

    If you stored cubrid script in /etc/init.d directory, remove this file.

    ::
    
        cd /etc/init.d
        rm cubrid

**Uninstalling CUBRID of RPM package**

If you have installed CUBRID in RPM package, you can uninstall CUBRID by "rpm" command.
    
*   Remove databases after stopping CUBRID service.

    Remove all created databases after stopping CUBRID service.
    
    For example, if there are testdb1, testdb2 as databases, do the following commands.
    
    ::
    
        $ cubrid service stop
        $ cubrid deletedb testdb1
        $ cubrid deletedb testdb2

*   Remove CUBRID engine.

    ::
    
        $ rpm -q cubrid
        cubrid-9.2.0.0123-el5.x86_64

        $ rpm -e cubrid
        warning: /opt/cubrid/conf/cubrid.conf saved as /opt/cubrid/conf/cubrid.conf.rpmsave
        
*   Remove auto-starting script.

    If you stored cubrid script in /etc/init.d directory, remove this file.

    ::
    
        cd /etc/init.d
        rm cubrid

Uninstalling CUBRID in Windows
------------------------------

Choose "CUBRID" in "Control panel > Uninstall a program" and uninstall it.
