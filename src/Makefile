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

#
# this code is 3rd party. Suppress all compiler warnings
#
CC_WARNINGS = $(CC_WARNINGS_NONE)
