TARGET = nucleo-blink
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Пути
SRC_DIR = src
BUILD_DIR = build
CMSIS_DIR = libs/STM32CubeF4/Drivers

# Пути к заголовочным файлам
INCLUDES = -I. \
           -I$(CMSIS_DIR)/CMSIS/Include \
           -I$(CMSIS_DIR)/CMSIS/Device/ST/STM32F4xx/Include

# Файлы
SOURCES = $(SRC_DIR)/main.c
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.c=.o)))

# Флаги компилятора
CFLAGS = -mcpu=cortex-m4 \
         -mthumb \
         -mfloat-abi=hard \
         -mfpu=fpv4-sp-d16 \
         -DSTM32F411xE \
         $(INCLUDES) \
         -O0 \
         -g3 \
         -Wall \
         -ffunction-sections \
         -fdata-sections

LDFLAGS = -TSTM32F411RETx_FLASH.ld \
          --specs=nosys.specs \
          --specs=nano.specs \
          -nostartfiles \
          -Wl,--gc-sections

# Сборка
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) | $(BUILD_DIR)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	$(OBJCOPY) -O ihex $@ $(BUILD_DIR)/$(TARGET).hex
	$(OBJCOPY) -O binary $@ $(BUILD_DIR)/$(TARGET).bin
	$(SIZE) $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean flash

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/stlink.cfg \
	        -f target/stm32f4x.cfg \
	        -c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"

flash-elf: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg \
	        -f target/stm32f4x.cfg \
	        -c "program $< verify reset exit"