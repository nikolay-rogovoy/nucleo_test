TARGET = nucleo-blink
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Пути
SRC_DIR = src
BUILD_DIR = build

# Пути к заголовочным файлам (ИСПРАВЛЕНО!)
INCLUDES = -I. \
           -I"C:/work/projects/STM32CubeF4/Drivers/CMSIS/Include" \
           -I"C:/work/projects/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include"

# Файлы
SOURCES = $(SRC_DIR)/main.c
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.c=.o)))

# Флаги компилятора (добавлен INCLUDES)
CFLAGS = -mcpu=cortex-m4 \
         -mthumb \
         -mfloat-abi=soft \
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
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

.PHONY: clean flash

clean:
	rm -rf $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/stlink.cfg \
	        -f target/stm32f4x.cfg \
	        -c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"