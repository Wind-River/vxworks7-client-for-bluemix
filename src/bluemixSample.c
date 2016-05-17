/* bluemixSample.c - Bluemix sdk sample codes */

/* Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
12apr16,msx  written
*/

/*
DESCRIPTION
Bluemix sample application code.
*/

#include <stdlib.h>
#include <stdio.h>
#include <taskLib.h>
#include "iotfclient.h"
#ifdef _WRS_CONFIG_JSON
#include <jansson.h>
#endif

#define VX_IOTF_TASK_PRI        100
#define VX_IOTF_TASK_STK        (16 * 1024)

int connect_status = 0;

/*  payload format is like: {"d":{"temp":75.83996}} */

void handle_cmd_cb (char *commandName, char *format, void *payload)
{
    printf("------------------------------------\n" );
    printf("The command received :: %s\n", commandName);
    printf("format : %s\n", format);
    /* printf("Payload is : %s\n", (char *)payload); */
    if(!strncmp(commandName, "overtemp", 8) && \
        !strncmp(format, "json", 4)){
        printf("      OVER TEMPERATURE WARNING!!!     \n" );

#ifdef _WRS_CONFIG_JSON
        double jsontemp;
        json_error_t error;
        json_t *root = json_loads((const char *)payload, 0, &error);
        if(!root){
            printf("json_loads failed: on line %d: %s\n", error.line, error.text);
            return ;
        }

        if (json_unpack(root, "{s:{s:f}}", "d", "temp", &jsontemp)) {
            printf("json_unpack failed, please check json format\n");
        }

        json_decref(root);
        printf("Indicated temperature (from json data) is : %.2f\n", jsontemp);
#endif
    }
    printf("------------------------------------\n" );
}

/* This routine get random temperature data
 * for device to publish to WATSON IoT platform
 */
char *getRandomData(void)
{
    double temperature, humidity;
    char *sensor_data;    /* simulated data */

    /*
     * generate a group of double type data,
     * temperature(0-100),humidity(0-50)
     * and send it to Watson IoT platform
     */

    temperature = (100.0*rand()/(RAND_MAX+1.0));
    humidity = (50.0*rand()/(RAND_MAX+1.0));

#ifdef _WRS_CONFIG_JSON
    json_t *root,*d_node;

    d_node = json_pack("{s:f,s:f}", "temp", temperature, "humid", humidity);
    root = json_pack("{so}","d", d_node);
    if(!json_is_object(root))
        printf("json_pack root data failed\n");

    sensor_data = json_dumps(root,0);
    json_decref(root);
#else
    sprintf (sensor_data, "{\"d\" : {\"temp\" : %.2f,\"humid\" : %.2f}}", temperature, humidity);
#endif

    return sensor_data;
}

/* This task routine create connection with bluemix Watson Platform, and send
 * simulated data to bluemix node-red applications periodically.
 */

int vx_iotf_task(BOOL with_quick, BOOL with_ssl, char *deviceType,
                 char *deviceId, char *bmOrg, char *bmToken, char *CAfile)
{
    int rc = -1;
    Iotfclient client;
    int try_cnt = 3;
    char *auth;

    if (with_ssl)
        {
        client.with_ssl = 1;
        client.CAfile = CAfile;
#ifndef _WRS_CONFIG_OPENSSL
        printf("Secure connection not available without OpenSSL.\n Quitting..\n");
        return -1;
#endif
        }
    else
        {
        client.with_ssl = 0;
        }

    if (with_quick)
        auth = NULL;
    else
        auth = "token";

    rc = initialize(&client, bmOrg, deviceType, deviceId, auth, bmToken);

    if(rc != SUCCESS){
        printf("Initialize returned rc = %d.\n Quitting..\n", rc);
        return -1;
    }

    do{
        rc = connectiotf(&client);
        if(rc == SUCCESS)
            break;
        sleep(2);
    }while(try_cnt--);

    if(rc != SUCCESS){
        printf("Connection returned rc = %d.\n Quitting..\n", rc);
        return -1;
    }

    printf("Connection Successful.\n");
    if (with_quick)
        printf("View the visualization at https://quickstart.internetofthings.ibmcloud.com/#/device/%s\n", deviceId);
    else
        printf("View the visualization at https://%s.internetofthings.ibmcloud.com/dashboard/#/devices/browse\n", bmOrg);

    setCommandHandler(&client, handle_cmd_cb);
    while(!connect_status) {
        rc= publishEvent(&client,"status","json", (unsigned char*)getRandomData(), QOS0);
        if(rc == -1) {
            printf("Network may be disconnected\n");
            break;
        }

        yield(&client, 1000);
        sleep(2);
    }

    printf("Connection Quitting!!\n");

    disconnect(&client);
    return 0;
}

/* run iotf task */

int run_vx_iotf(BOOL with_quick, BOOL with_ssl, char *deviceType,
                char *deviceId, char *bmOrg, char *bmToken, char *CAfile)
{
    TASK_ID tid;

    tid = taskSpawn("tVxBluemix", VX_IOTF_TASK_PRI, 0, VX_IOTF_TASK_STK,
                    (FUNCPTR) vx_iotf_task, (_Vx_usr_arg_t)with_quick, (_Vx_usr_arg_t)with_ssl,
                    (_Vx_usr_arg_t)deviceType, (_Vx_usr_arg_t)deviceId, (_Vx_usr_arg_t)bmOrg,
                    (_Vx_usr_arg_t)bmToken, (_Vx_usr_arg_t)CAfile, 0, 0, 0);

    if(tid == TASK_ID_ERROR)
        {
        printf("spawn vx_iotf_task failed\n");
        return -1;
        }

    return 0;
}

void disconnect_bluemix(void)
{
    connect_status = 1;
}
