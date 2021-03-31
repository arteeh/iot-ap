# Fedora: make arm-none-eabi-gcc arm-none-eabi-newlib
CC=arm-none-eabi-gcc

# Projectmap
PROJ=week6

# Naam van map in 'projects' map, wordt ook naam van binary
PROJNAME=workshop5a

# Allerlei locaties voor includes en cfiles
PROJ_DIR=$(PROJ)/projects/$(PROJNAME)

SRC=$(PROJ_DIR)/src
STARTUP=$(PROJ_DIR)/startup
PROJ_INC=$(PROJ_DIR)/inc

CMSIS=$(PROJ)/libraries/CMSIS
CMSIS_INC=$(CMSIS)/Include
CMSIS_STM_INC=$(CMSIS)/ST/STM32F0xx/Include

STDPERIPH=$(PROJ)/libraries/STM32F0xx_StdPeriph_Driver
STDPERIPH_INC=$(STDPERIPH)/inc
STDPERIPH_SRC=$(STDPERIPH)/src

DISCOVERY=$(PROJ)/utilities/STM32F0-Discovery

# include locaties overgenomen uit .uvprojx bestand
INC_ALL=-I$(STARTUP) \
		-I$(PROJ_INC) \
		-I$(CMSIS_INC) \
		-I$(CMSIS_STM_INC) \
		-I$(STDPERIPH_INC) \
		-I$(DISCOVERY)

# uVision heeft c bestanden in groepen verdeeld, ik doe het even ook
# C files overgenomen uit .uvprojx bestand
CFILES_MAIN=$(SRC)/main.c \
		$(SRC)/stm32f0xx_it.c \
		$(SRC)/usart.c

CFILES_DISCO=$(DISCOVERY)/stm32f0_discovery.c

CFILES_STARTUP=$(STARTUP)/system_stm32f0xx.c $(STARTUP)/startup_stm32f0xx.s

# Pakt als goed is alle c bestanden, maar dit is lang niet allemaal nodig
CFILES_PERIPH_ALL=$(STDPERIPH_SRC)/*

# gebruik in plaats daarvan deze om specifieke periphs te selecteren
CFILES_PERIPH=$(STDPERIPH_SRC)/stm32f0xx_syscfg.c \
		$(STDPERIPH_SRC)/stm32f0xx_adc.c \
		$(STDPERIPH_SRC)/stm32f0xx_rcc.c \
		$(STDPERIPH_SRC)/stm32f0xx_dac.c \
		$(STDPERIPH_SRC)/stm32f0xx_exti.c \
		$(STDPERIPH_SRC)/stm32f0xx_gpio.c \
		$(STDPERIPH_SRC)/stm32f0xx_misc.c \
		$(STDPERIPH_SRC)/stm32f0xx_tim.c \
		$(STDPERIPH_SRC)/stm32f0xx_usart.c \
		$(STDPERIPH_SRC)/stm32f0xx_pwr.c
		

CFILES=$(CFILES_MAIN) $(CFILES_DISCO) $(CFILES_STARTUP) $(CFILES_PERIPH)

compile:
	$(CC) $(INC_ALL) $(CFILES) -o $(PROJNAME)

run:
	./$(PROJNAME)

clean:
	rm $(PROJNAME)
