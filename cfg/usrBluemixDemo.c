/* usrBluemixDemo.c - Bluemix sdk sample initialization */

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

#include <stdio.h>
#include <net/if.h>
#include <ioctl.h>
#include <time.h>
#include <rtpLib.h>

static char *getMacAddress(void)
{
    int fd;
    struct ifreq req;
    static char macAddress[16];

    /* get MAC from first interface */

    fd = socket( AF_INET, SOCK_RAW, 0 );
    if(fd < 0)
        {
        printf("[ERROR] %s - socket() error = %d\n", __FUNCTION__, fd);
        return NULL;
        }

    memset((char *)&req, 0, sizeof(req));
    req.ifr_ifindex = 2;

    if(ioctl(fd, SIOCGIFNAME, &req ) < 0 ||
       ioctl(fd, SIOCGIFLLADDR, &req ) < 0)
        {
        printf("[ERROR] %s - Failed to get MAC address. ioctl() error\n",__FUNCTION__);
        return NULL;
        }

    close(fd);

    /* MAC is now in req.ifr_addr.sa_data[] put it in string format */

    sprintf(macAddress, "%02x%02x%02x%02x%02x%02x", \
        (unsigned char)req.ifr_addr.sa_data[0], (unsigned char)req.ifr_addr.sa_data[1], \
        (unsigned char)req.ifr_addr.sa_data[2], (unsigned char)req.ifr_addr.sa_data[3], \
        (unsigned char)req.ifr_addr.sa_data[4], (unsigned char)req.ifr_addr.sa_data[5]);

    return(macAddress);
}

void usrBluemixDemoInit (void)
{
    char * deviceType;
    char * deviceId;
    char * bmOrg;
    char * bmToken;
    char * CAfile;
    char   dnsIp[64];
    size_t dnsSize = sizeof (dnsIp);

    /* Verify we have a name server */

    if (ipcom_sysvar_get ("ipdnsc.primaryns", dnsIp, &dnsSize) == NULL || dnsIp[0] == '\0')
        {
        printf("[BluemixDemo] Please set DNS server IP address() first.\n");
        return;
        }

    /* Check Bluemix connection mode */

    if (BLUEMIX_QUICKSTART_MODE)
        {
        deviceId = getMacAddress();
        if (deviceId == NULL)
            return;

        deviceType = "iotsample";

        bmOrg   = "quickstart";
        bmToken = "quickstart";
        }
    else
        {
        deviceId   = BLUEMIX_DEVICE_ID;
        deviceType = BLUEMIX_DEVICE_TYPE;

        bmOrg   = BLUEMIX_ORG_ID;
        bmToken = BLUEMIX_TOKEN;
        }

    if (deviceId == NULL || deviceId[0] == '\0')
        {
        printf("[BluemixDemo] Please set BLUEMIX_DEVICE_ID.\n");
        return;
        }

    if (deviceType == NULL || deviceType[0] == '\0')
        {
        printf("[BluemixDemo] Please set BLUEMIX_DEVICE_TYPE.\n");
        return;
        }

    if (bmOrg == NULL || bmOrg[0] == '\0')
        {
        printf("[BluemixDemo] Please set BLUEMIX_ORG_ID.\n");
        return;
        }

    if (!BLUEMIX_QUICKSTART_MODE && (bmToken == NULL || bmToken[0] == '\0'))
        {
        printf("[BluemixDemo] Please set BLUEMIX_TOKEN.\n");
        return;
        }

    if(BLUEMIX_SECURE_CONNECTION)
        {
        struct tm tms;
        struct timeval tval;

        CAfile = (char *)BLUEMIX_CAFILE_PATH;

        if (gettimeofday(&tval, NULL) == 0)
            {
            tms = *localtime((time_t *)&tval.tv_sec);
            if (tms.tm_year < 116)
                {
                printf("Warning! System clock is %s", ctime ((time_t *)&tval.tv_sec));

                /* Set time to May 1, 2016 */

                tms.tm_year  = 116;
                tms.tm_mon   = 4;
                tms.tm_mday  = 1;
                tval.tv_sec  = mktime(&tms);
                tval.tv_usec = 0;
                if (settimeofday(&tval, NULL) == 0)
                    {
                    printf("Stepping clock to %s", ctime ((time_t *)&tval.tv_sec));
                    }
                }
            }
        }
    else
        CAfile = NULL;

    /* Check if Bluemix runs in kernel or RTP */

#ifdef _WRS_CONFIG_RTP
    if (BLUEMIX_RTP_APP)
        {
        const char * argv[16];
        int argc = 0;
        RTP_ID rtpId;

        /*
         * From CDF typically /romfs/bluemix.vxe
         * argv[0] = "/romfs/bluemix.vxe";
         */

        argv[argc++] = BLUEMIX_RTP_PATH;
        argv[argc++] = BLUEMIX_QUICKSTART_MODE ? "1" : "0";
        argv[argc++] = BLUEMIX_SECURE_CONNECTION ? "1" : "0";
        argv[argc++] = deviceType;
        argv[argc++] = deviceId;
        argv[argc++] = bmOrg;
        argv[argc++] = bmToken;
        argv[argc++] = CAfile;
        argv[argc] = NULL;

        printf("[BluemixDemo] running iotf real-time process.\n");

        rtpId = rtpSpawn (argv[0], (const char **)argv, NULL, 220, 0,
                          RTP_LOADED_WAIT, VX_FP_TASK);

        if (rtpId == RTP_ID_ERROR)
            printf("[BluemixDemo] spawn RTP failed\n");
        }
    else
#endif
        {
        extern int run_vx_iotf(BOOL with_quick, BOOL with_ssl, char *deviceType,
                               char *deviceId, char *bmOrg, char *bmToken, char *CAfile);

        printf("[BluemixDemo] running iotf kernel task.\n");

        (void)run_vx_iotf(BLUEMIX_QUICKSTART_MODE, BLUEMIX_SECURE_CONNECTION,
                          deviceType, deviceId, bmOrg, bmToken, CAfile);
        }
}
