/* 11folder_bluemix_sdk.cdf - Bluemix SDK component description file */

/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
07apr16,xms  created
*/

Folder FOLDER_IBM_BLUEMIX {
        NAME            IBM Bluemix
        SYNOPSIS        IBM Bluemix
        _CHILDREN       FOLDER_CLOUD
}

Component INCLUDE_IBM_BLUEMIX {
        NAME            Bluemix MQTT and iotfclient library
        SYNOPSIS        Bluemix MQTT and iotfclient library
        ARCHIVE         libbluemix.a
        _CHILDREN       FOLDER_IBM_BLUEMIX
#ifdef _WRS_CONFIG_OPENSSL
        REQUIRES        INCLUDE_IPDNSC INCLUDE_GETADDRINFO INCLUDE_IPSSL
#else
        REQUIRES        INCLUDE_IPDNSC INCLUDE_GETADDRINFO
#endif
}

Component INCLUDE_BLUEMIX_DEMO {
        NAME            Bluemix Demo
        SYNOPSIS        Bluemix Demo Application
        REQUIRES        INCLUDE_IBM_BLUEMIX
        CONFIGLETTES    usrBluemixDemo.c
        PROTOTYPE       void usrBluemixDemoInit (void);
        INIT_RTN        usrBluemixDemoInit ();
        _INIT_ORDER     usrRoot
        INIT_AFTER      usrAppInit
        _CHILDREN       FOLDER_IBM_BLUEMIX
        REQUIRES        INCLUDE_IBM_BLUEMIX
        CFG_PARAMS      BLUEMIX_QUICKSTART_MODE     \
                        BLUEMIX_SECURE_CONNECTION   \
                        BLUEMIX_RTP_APP             \
                        BLUEMIX_RTP_PATH            \
                        BLUEMIX_DEVICE_ID           \
                        BLUEMIX_DEVICE_TYPE         \
                        BLUEMIX_ORG_ID              \
                        BLUEMIX_TOKEN               \
                        BLUEMIX_CAFILE_PATH
}

Parameter BLUEMIX_QUICKSTART_MODE {
        NAME            Bluemix quickstart mode
        SYNOPSIS        Bluemix quickstart mode. FALSE means register mode
        TYPE            BOOL
        DEFAULT         TRUE
}

Parameter BLUEMIX_RTP_APP {
        NAME            Bluemix demo running in user space
        SYNOPSIS        Bluemix demo running in user space,FALSE means in kernel space
        TYPE            BOOL
        DEFAULT         FALSE
}

Parameter BLUEMIX_RTP_PATH {
        NAME            Bluemix RTP demo file
        SYNOPSIS        Bluemix RTP demo file
        TYPE            STRING
        DEFAULT         "/romfs/bluemix.vxe"
}

Parameter BLUEMIX_SECURE_CONNECTION {
        NAME            Bluemix secure connection
        SYNOPSIS        Bluemix secure SSL connection
        TYPE            BOOL
#ifdef _WRS_CONFIG_OPENSSL
        DEFAULT         TRUE
#else
        DEFAULT         FALSE
#endif
}

Parameter BLUEMIX_DEVICE_ID {
        NAME            Device ID configured in registered mode
        SYNOPSIS        Device ID configured in registered mode
        TYPE            STRING
        DEFAULT         ""
}

Parameter BLUEMIX_DEVICE_TYPE {
        NAME            Device Type configured in registered mode
        SYNOPSIS        Device Type configured in registered mode
        TYPE            STRING
        DEFAULT         ""
}

Parameter BLUEMIX_ORG_ID {
        NAME            Organization ID configured in registered mode
        SYNOPSIS        Organization ID configured in registered mode
        TYPE            STRING
        DEFAULT         ""
}

Parameter BLUEMIX_TOKEN {
        NAME            Token configured in registered mode
        SYNOPSIS        Token configured in registered mode
        TYPE            STRING
        DEFAULT         ""
}

Parameter BLUEMIX_CAFILE_PATH {
        NAME            Bluemix CA certificate file
        SYNOPSIS        Bluemix CA certificate file
        TYPE            STRING
        DEFAULT         "/romfs/SSLCACert.pem"
}
