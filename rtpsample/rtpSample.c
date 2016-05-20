/* rtpSample.c - Bluemix sdk rtp application */

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
        printf ("usage: bluemix.vxe <quickstart> <secure> <device type> <device id> <org id> <token> <CAfile>\n");
        printf ("<quickstart> and <secure> accepts 0 or 1\n");
        printf ("set <device type> to 'iotsample' in quickstart\n");
        printf ("set <org id> to 'quickstart' in quickstart\n");
        }

    /* Prevent process termination */

    taskExit (0);

    return 0;
}
