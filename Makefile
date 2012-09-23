# Try "make help" first

.DEFAULT_GOAL := library

##
## Useful paths, constants, etc.
##

LIB_NAME := syrup
MPLROOT := $(LIB_MAPLE_HOME)
SRCROOT ?= .
BUILD_PATH = build
SUPPORT_PATH := $(MPLROOT)/support
WIRISH_PATH := $(MPLROOT)/wirish
LIBMAPLE_PATH := $(MPLROOT)/libmaple
LIBMAPLE_LIBPATH := ${MPLROOT}/libraries
# Support files for linker
LDDIR := $(SUPPORT_PATH)/ld
# Support files for this Makefile
MAKEDIR := $(SUPPORT_PATH)/make
BOARD_INCLUDE_DIR := $(MAKEDIR)/board-includes

##
## Target-specific configuration.  This determines some compiler and
## linker options/flags.
##

# Try "make help" for more information on BOARD and MEMORY_TARGET;
# these default to a Maple Flash build.
BOARD ?= maple_mini
MEMORY_TARGET ?= flash

# $(BOARD)- and $(MEMORY_TARGET)-specific configuration
include $(MAKEDIR)/target-config.mk

##
## Build rules and useful templates
##

include $(MAKEDIR)/build-rules.mk
include $(MAKEDIR)/build-templates.mk

##
## Compilation flags
##

# FIXME: the following allows for deprecated include style, e.g.:
#     #include "libmaple.h"
# or
#     #include "wirish.h"
# It slows compilation noticeably; remove after 1 release.
TARGET_FLAGS    += \
					-I$(LIB_PATH)/include                      		\
					-I$(LIBMAPLE_PATH)/include             			\
					-I$(LIBMAPLE_LIBPATH)             				\
					-I$(WIRISH_PATH)/include						\
					-I$(WIRISH_PATH)/include/wirish					\
					-I${LIBSYRUP_MODULE_SERIES}/include				\
					-I${WIRISH_PATH}/boards/$(BOARD)/include		\
					-I$(LIBMAPLE_MODULE_SERIES)/include
GLOBAL_CFLAGS   := -Os -g3 -gdwarf-2 -nostdlib\
                   -ffunction-sections -fdata-sections \
		   -Wl,--gc-sections $(TARGET_FLAGS)
GLOBAL_CXXFLAGS := -fno-rtti -fno-exceptions -Wall $(TARGET_FLAGS) -std=c++0x
GLOBAL_ASFLAGS  := -x assembler-with-cpp $(TARGET_FLAGS)
LDFLAGS  = $(TARGET_LDFLAGS) $(TOOLCHAIN_LDFLAGS) -mcpu=cortex-m3 -mthumb\
           -Xlinker --gc-sections \
           -Xassembler --march=armv7-m -Wall
#          -Xlinker --print-gc-sections \

##
## Set all submodules here
##

MODULES += $(SRCROOT)/${LIB_NAME}

# Call each module's rules.mk:
$(foreach m,$(MODULES),$(eval $(call LIBMAPLE_MODULE_template,$(m))))

##
## Targets
##

.PHONY: clean library

# Force a rebuild if the target changed
PREV_BUILD_TYPE = $(shell cat $(BUILD_PATH)/build-type 2>/dev/null)
build-check:
ifneq ($(PREV_BUILD_TYPE), $(MEMORY_TARGET))
	$(shell rm -rf $(BUILD_PATH))
endif

$(BUILD_PATH)/lib${LIB_NAME}.a: $(BUILDDIRS) $(TGT_BIN)
	- rm -f $@
	$(AR) crv $(BUILD_PATH)/lib${LIB_NAME}.a $(TGT_BIN)

library: $(BUILD_PATH)/lib${LIB_NAME}.a

$(BUILDDIRS):
	@mkdir -p $@

clean:
	rm -rf build
