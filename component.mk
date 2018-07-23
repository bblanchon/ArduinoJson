#IBN_GEN_LIB := $(patsubst $(COMPONENT_PATH)/%,%,$(sort $(dir $(wildcard $(COMPONENT_PATH)/libraries/*/*/))))

COMPONENT_ADD_INCLUDEDIRS := .
#COMPONENT_PRIV_INCLUDEDIRS := cores/esp32/libb64
#COMPONENT_SRCDIRS := cores/esp32/libb64 cores/esp32 variants/esp32 $(ARDUINO_CORE_LIBS)
#CXXFLAGS += -fno-rtti
