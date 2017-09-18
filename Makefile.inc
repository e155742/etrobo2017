mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=

APPL_CXXOBJS += \
	decoder.o \
	file_output.o \
	hsv_converter.o \
	localization.o \
	motion.o \
	move.o \
	onoff_control.o \
	pid_control.o \
	util.o \
	var_entering.o \
	mileage_stopper.o \
	angle_stopper.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += \
	$(mkfile_path)app \
	$(mkfile_path)unit \
	$(mkfile_path)util \
	$(mkfile_path)control \
	$(mkfile_path)stopper

INCLUDES += \
	-I$(mkfile_path)app \
	-I$(mkfile_path)unit \
	-I$(mkfile_path)util \
	-I$(mkfile_path)control \
	-I$(mkfile_path)stopper

COPTS += -fno-use-cxa-atexit
