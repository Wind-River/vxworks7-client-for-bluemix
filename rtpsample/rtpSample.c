/* rtpSample.c - Bluemix sdk rtp application */

/* Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06may16,msx  written
*/

/*
DESCRIPTION
Bluemix user space application.
*/

#include <string.h>
#include <stdio.h>
#include <taskLib.h>

int main (int argc, char* argv[])
{
    BOOL with_quick = 0;
    BOOL with_ssl = 0;
    extern int run_vx_iotf(BOOL with_quick, BOOL with_ssl, char *deviceType,
                           char *deviceId, char *bmOrg, char *bmToken, char *CAfile);

    if (argc >= 6)
        {
        if (argv[1] != NULL)
            with_quick = (strcmp (argv[1], "1") == 0);

        if (argv[2] != NULL)
            with_ssl = (strcmp (argv[2], "1") == 0);

        (void)run_vx_iotf(with_quick, with_ssl, argv[3], argv[4], argv[5], argv[6], argv[7]);
        }
    else
        {
        printf ("usage: bluemix.vxe <quickstart> <secure> <device type> <device id> <org id> [token]\n");
        printf ("<quickstart> and <secure> accepts 0 or 1\n");
        printf ("set <device type> to 'iotsample' in quickstart\n");
        printf ("set <org id> to 'quickstart' in quickstart\n");
        printf ("[token] is not used in quickstart\n");
        }

    /* Prevent process termination */

    taskExit (0);

    return 0;
}
