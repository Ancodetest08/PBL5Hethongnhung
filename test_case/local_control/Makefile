PROJECT_NAME := local_control

DEVELOPMENT_BOARD := board_esp32c3_devkitc.h

EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/../device_firmware/components/light_driver
EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/../device_firmware/components/button
EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/../device_firmware/components/app_storage

include $(IDF_PATH)/make/project.mk

CPPFLAGS += -DDEVELOPMENT_BOARD=\"$(DEVELOPMENT_BOARD)\"
