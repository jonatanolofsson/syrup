include $(MAKEDIR)/header.mk
###############################################################################

BUILDDIRS += $(BUILD_PATH)/$(d)/framebuffer
BUILDDIRS += $(BUILD_PATH)/$(d)/stream
BUILDDIRS += $(BUILD_PATH)/$(d)/comm
BUILDDIRS += $(BUILD_PATH)/$(d)/drivers/display
BUILDDIRS += $(BUILD_PATH)/$(d)/drivers/sensors
BUILDDIRS += $(BUILD_PATH)/$(d)/drivers/actuators
BUILDDIRS += $(BUILD_PATH)/$(d)/utils

# Safe includes for Wirish.
SYRUP_INCLUDES := -I$(d)/include

# Local flags. Add -I$(d) to allow for private includes.
CFLAGS_$(d) := $(LIBMAPLE_INCLUDES) $(SYRUP_INCLUDES) -I$(d)

# Local rules and targets
cppSRCS_$(d) += drivers/display/SSD1308.cpp
cppSRCS_$(d) += drivers/sensors/MPU6050.cpp
cppSRCS_$(d) += drivers/sensors/MS5611.cpp
cppSRCS_$(d) += drivers/sensors/HMC5883L.cpp
cppSRCS_$(d) += drivers/sensors/TCS230.cpp

cppSRCS_$(d) += drivers/actuators/HDDVoiceCoil.cpp

cppSRCS_$(d) += framebuffer/Framebuffer.cpp
cppSRCS_$(d) += framebuffer/Scope.cpp
cppSRCS_$(d) += framebuffer/TextConsole.cpp

cppSRCS_$(d) += stream/BaseOutStream.cpp
#~ cppSRCS_$(d) += stream/USARTStream.cpp
cppSRCS_$(d) += stream/formatting.cpp

#~ cppFILES_$(d) := $(cppSRCS_$(d):%=$(d)/%)

#~ OBJS_$(d)     := $(cppFILES_$(d):%.cpp=$(BUILD_PATH)/%.o)
#~ DEPS_$(d)     := $(OBJS_$(d):%.o=%.d)

#~ $(OBJS_$(d)): TGT_CFLAGS := $(CFLAGS_$(d))

#~ TGT_BIN += $(OBJS_$(d))

# Standard things
#~ -include        $(DEPS_$(d))
#~ d               := $(dirstack_$(sp))
#~ sp              := $(basename $(sp))

###############################################################################
include $(MAKEDIR)/footer.mk
