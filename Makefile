# Makefile - IBM Bluemix SDK Makefile
#
# Copyright 2016 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 03may16,msx  created
#

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
