# ##########################################################
#
# VxWorks-7 Bluemix SDK User Guide
#
# ##########################################################

Overview:
This Readme file provide a quick summary of building and running IBM Bluemix SDK residing on the
device. The IBM Bluemix SDK means the IoT embedded C client library for interacting with the IBM
Watson Internet of Things Platform. The embedded C client library is not provided in VxWorks-7
RPM packages or DVDs by default.

1.  First you must fetch SDK source codes from maintainer web site https://github.com/ibm-messaging/iotf-embeddedc.git.
    And then apply a patch with some tiny changes in order to make Bluemix SDK compatible for VxWorks-7. Some unnecessary
    files are removed from SDK directory. All these steps are executed by running a script setup.sh in bluemix/src directory.
        cd src
        ./setup

2.  To create VSB and VIP with IBM Bluemix SDK

    VSB and VIP could be created in workbench UI environment as well as command line with vxprj tool,
    Here just list how to create them using vxprj tool, and take itl_quark BSP as an example.

1). VSB create:
        vxprj vsb create -force -bsp itl_quark vsb_PENTIUM_32_up -S
        cd vsb_PENTIUM_32_up
        vxprj vsb add IBM_BLUEMIX
        make -j

2). VIP create:
        vxprj create -force -vsb vsb_PENTIUM_32_up itl_quark gnu vip_quark_kernel -profile PROFILE_STANDALONE_DEVELOPMENT
        cd vip_quark_kernel
        vxprj component add DRV_VXBEND_QRK_GMAC
        vxprj component add INCLUDE_SHELL INCLUDE_NETWORK INCLUDE_IFCONFIG INCLUDE_PING
        vxprj component add INCLUDE_IBM_BLUEMIX
        vxprj parameter set DNSC_PRIMARY_NAME_SERVER "\"128.224.160.11\""
        vxprj parameter set DNSC_SECONDARY_NAME_SERVER "\"147.11.57.128\""

    A sample iotfclient test code is provided in cfg/usrBluemixDemo.c and bluemix/src directory, it can be used to connect 
    the device to Bluemix cloud, publish events to the cloud and subscribe commands from Bluemix cloud. To enable this demo,
    you need to add INCLUDE_BLUEMIX_DEMO component as below:
        vxprj component add INCLUDE_BLUEMIX_DEMO

    If you want to create connection to quickstart service, please set parameters as follow:
        vxprj parameter set BLUEMIX_QUICKSTART_MODE TRUE

    If you want to create connection to registered service, please set parameters as follow:
        vxprj parameter set BLUEMIX_QUICKSTART_MODE FALSE
        vxprj parameter set BLUEMIX_DEVICE_ID       "\"galileo_bb2b\""
        vxprj parameter set BLUEMIX_DEVICE_TYPE     "\"vx7_Galileo\""
        vxprj parameter set BLUEMIX_ORG_ID          "\"8hvetd\""
        vxprj parameter set BLUEMIX_TOKEN           "\"dhVLQ1U7@yqAffPME7\""

    Note: The value of above four parameters should be consistent with the information of device
    registered in Watson IoT platform

        vxprj build

3). To set up secure connection over SSL, First you need to include OPENSSL layer in VSB, as follow,
        vxprj vsb add OPENSSL

    After doing that, you can set BLUEMIX_SECURE_CONNECTION to TRUE or FALSE in VIP, to decide
    if creating secure connection, by default it is TRUE after adding OPENSSL layer.
        vxprj parameter set BLUEMIX_SECURE_CONNECTION   TURE
        vxprj parameter set BLUEMIX_CAFILE_PATH         "/romfs/SSLCACert.pem"
        mkdir romfs
        cp bluemix/certs/SSLCACert.pem                  ${VIP_DIR}/romfs
      or
        vxprj parameter set BLUEMIX_SECURE_CONNECTION   FALSE

4). The Bluemix sample could also be run in user space. A Bluemix rtp file bluemix.vxe is generated in 
    ${VSB_DIR}/usr/root/gnu/bin when building VSB. To auto-spawn the DEMO in RTP space, you need to add
    INCLUDE_ROMFS component, then set parameter BLUEMIX_RTP_APP as TRUE, and set BLUEMIX_RTP_PATH as 
    "/romfs/bluemix.vxe". You need to enter instructions as follow:
        vxprj component add INCLUDE_ROMFS
        vxprj parameter set BLUEMIX_RTP_APP TRUE
        vxprj parameter set BLUEMIX_RTP_PATH "/romfs/bluemix.vxe"
        mkdir romfs
        cp ${VSB_DIR}/usr/root/gnu/bin/bluemix.vxe romfs
        vxprj build
    Now you can bring up the board, and it will auto-run Bluemix SDK in user space.

3.  Run image with Bluemix SDK and view the visualization at the web site of IBM Bluemix.
    1) For quickstart connection, view connection status at:
        https://quickstart.internetofthings.ibmcloud.com/#/device/
    Note: The device MAC address needs to be entered in this page, and it must be in lower case.

    2) For registered connection, view the device connection status at:
        https://${ORG_ID}.internetofthings.ibmcloud.com/dashboard/#/devices/browse
       Replace ${ORG_ID} with your real organization id registered in IBM Bluemix.Just like:
        https://8hvetd.internetofthings.ibmcloud.com/dashboard/#/devices/browse

    3) You can also create Node-red application in IBM Bluemix platform, in order to do more implements.
       View Bluemix cloud application dashboard at:
        https://console.ng.bluemix.net/?direct=classic/#/resources
