/* 00iot.cdf - IoT component description file */
  
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
14apr16,xms  created
*/    

Folder  FOLDER_IOT {
        NAME            IoT
        SYNOPSIS        IoT related middleware and configuration options
        _CHILDREN       FOLDER_ROOT 
}

Folder FOLDER_CLOUD {
        NAME            Cloud connectors
        SYNOPSIS        Cloud SDK configuration options
        _CHILDREN       FOLDER_IOT
}

