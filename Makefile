# Copyright (c) 2016, Wind River Systems, Inc.
#
# Redistribution and use in source and binary forms, with or without modification, are
# permitted provided that the following conditions are met:
#
# 1) Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2) Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3) Neither the name of Wind River Systems nor the names of its contributors may be
# used to endorse or promote products derived from this software without specific
# prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

VSBL_LIBNAME =  IBM_BLUEMIX

KERNEL_PUBLIC_H_DIRS = src/iotf-embeddedc/mqtt

KERNEL_PUBLIC_H_FILES = src/iotf-embeddedc/iotfclient.h \
                        src/iotf-embeddedc/gatewayclient.h \
                        src/iotf-embeddedc/devicemanagementclient.h \
                        src/iotf-embeddedc/openssl.h

USER_PUBLIC_H_DIRS = src/iotf-embeddedc/mqtt

USER_PUBLIC_H_FILES = src/iotf-embeddedc/iotfclient.h \
                        src/iotf-embeddedc/gatewayclient.h \
                        src/iotf-embeddedc/devicemanagementclient.h \
                        src/iotf-embeddedc/openssl.h

BUILD_DIRS = src
BUILD_USER_DIRS = src
POSTBUILD_RTP_DIRS = rtpsample

POST_NOBUILD_CDFDIRS = cdf

POST_NOBUILD_CFGDIRS = cfg

ifeq ($(SPACE), user)
include $(WIND_USR_MK)/rules.layers.mk
else
include $(WIND_KRNL_MK)/rules.layers.mk
endif
