mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=

APPL_CXXOBJS += \
	calibration.o \
	decoder.o \
	file_output.o \
	hsv_converter.o \
	color_judge.o \
	localization.o \
	motion.o \
	move.o \
	onoff_control.o \
	pid_control.o \
	starter.o \
	util.o \
	pid.o \
	left_trace.o \
	right_trace.o \
	var_entering.o \
	mileage_stopper.o \
	angle_stopper.o \
	direction_stopper.o \
	color_stopper.o \
	line_stopper.o \
	gray_stopper.o \
	marker.o \
	block_field.o \
	sumo.o \
	left_idaten.o \
	left_block.o \
	left_parking.o \
	right_idaten.o \
	right_sumo.o \
	right_parking.o \
	right_prize.o \
	pid_trace.o

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
	$(mkfile_path)stopper \
	$(mkfile_path)block \
	$(mkfile_path)sumo \
	$(mkfile_path)left_course \
	$(mkfile_path)right_course

INCLUDES += \
	-I$(mkfile_path)app \
	-I$(mkfile_path)unit \
	-I$(mkfile_path)util \
	-I$(mkfile_path)control \
	-I$(mkfile_path)stopper \
	-I$(mkfile_path)block \
	-I$(mkfile_path)sumo \
	-I$(mkfile_path)left_course \
	-I$(mkfile_path)right_course

COPTS += -fno-use-cxa-atexit
