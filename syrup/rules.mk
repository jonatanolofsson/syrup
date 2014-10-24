include $(MAKEDIR)/header.mk
###############################################################################

BUILDDIRS += $(BUILD_PATH)/$(d)/comm
BUILDDIRS += $(BUILD_PATH)/$(d)/drivers/sensors

# Safe includes for Wirish.
SYRUP_INCLUDES := -I$(d)/include

# Local flags. Add -I$(d) to allow for private includes.
CFLAGS_$(d) := $(LIBMAPLE_INCLUDES) $(SYRUP_INCLUDES) -I$(d) -Os
CXXFLAGS_$(d) :=  -Os -fdata-sections -ffunction-sections -Wl,-gc-sections
LDFLAGS_$(d) :=  -Wl,-gc-sections -dead-strip

# Local rules and targets
cppSRCS_$(d) += isr.cpp
cppSRCS_$(d) += comm/ByteInterface.cpp

#~ cppSRCS_$(d) += drivers/display/SSD1308.cpp
cppSRCS_$(d) += drivers/sensors/MPU6050.cpp
cppSRCS_$(d) += drivers/sensors/MS5611.cpp
cppSRCS_$(d) += drivers/sensors/HMC5883L.cpp
cppSRCS_$(d) += drivers/sensors/TCS230.cpp
cppSRCS_$(d) += drivers/sensors/AnalogSensor.cpp
cppSRCS_$(d) += drivers/sensors/SRF04.cpp
cppSRCS_$(d) += drivers/sensors/ADNS3080.cpp

###############################################################################
include $(MAKEDIR)/footer.mk
