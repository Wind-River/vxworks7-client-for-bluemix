/* bluemixSample.c - Bluemix sdk sample codes */

/* Copyright (c) 2016, Wind River Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * 3) Neither the name of Wind River Systems nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#define SIM_DATA_LEN            100

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
char *getRandomData(char* sensor_data)
{
    double temperature, humidity;

    /*
     * generate a group of double type data,
     * temperature(0-100),humidity(0-50)
     * and send it to Watson IoT platform
     */

    temperature = (100.0*rand()/(RAND_MAX+1.0));
    humidity = (50.0*rand()/(RAND_MAX+1.0));

    memset(sensor_data, 0, sizeof(sensor_data));

#ifdef _WRS_CONFIG_JSON
    json_t *root,*d_node;

    d_node = json_pack("{s:f,s:f}", "temp", temperature, "humid", humidity);
    root = json_pack("{so}","d", d_node);
    if(!json_is_object(root))
        printf("json_pack root data failed\n");

    char *json_dump = json_dumps(root,0);
    strcpy(sensor_data, json_dump);
    free(json_dump);
    json_decref(d_node);
    json_decref(root);
#else
    snprintf (sensor_data + strlen(sensor_data), SIM_DATA_LEN, "{\"d\" : {\"temp\" : %.2f,\"humid\" : %.2f}}", temperature, humidity);
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

    char* sensor_data = (char *)malloc(SIM_DATA_LEN + 1);    /* simulated data */

    while(!connect_status) {
        rc= publishEvent(&client,"status","json", (unsigned char*)getRandomData(sensor_data), QOS0);
        if(rc == -1) {
            printf("Network may be disconnected\n");
            break;
        }

        yield(&client, 1000);
        sleep(2);
    }

    printf("Connection Quitting!!\n");
    free(sensor_data);
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
