# Makefile - Bluemix SDK C libary Makefile
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

LIB_BASE_NAME = bluemix

ifeq ($(SPACE), user)
include $(WIND_USR_MK)/defs.library.mk
else
include $(WIND_KRNL_MK)/defs.library.mk
endif

BUILD_ALL_OBJS = TRUE

vpath %.c iotf-embeddedc iotf-embeddedc/mqtt

OBJS = $(patsubst %.c,%.o,$(notdir $(wildcard iotf-embeddedc/*.c) \
       $(wildcard iotf-embeddedc/mqtt/*.c)))

OBJS += bluemixSample.o

ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM += -Xdialect-c99
endif

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM += -std=c99
endif

EXTRA_INCLUDE+= -Iiotf-embeddedc -Iiotf-embeddedc/mqtt

DOC_FILES=

ifeq ($(SPACE), user)
include $(WIND_USR_MK)/rules.library.mk
else
include $(WIND_KRNL_MK)/rules.library.mk
endif
