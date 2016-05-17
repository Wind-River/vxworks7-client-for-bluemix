# Makefile - RTP sample for Bluemix SDK Makefile
#
# Copyright 2016 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 06may16,msx  created
#

EXE = bluemix.vxe

RTP_BASE_DIR = rtpsample

OBJS =  rtpSample.o 

ADDED_LIBS += -lbluemix -ljson

include $(WIND_USR_MK)/defs.library.mk

ifdef _WRS_CONFIG_OPENSSL
ADDED_LIBS += -lHASH
ADDED_LIBS += -lOPENSSL
endif

include $(WIND_USR_MK)/rules.rtp.mk
