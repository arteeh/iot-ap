PROJ=reviews/week4-1
PROJNAME=assignment4

SRC=$(PROJ)/projects/$(PROJNAME)/src
STARTUP=$(PROJ)/projects/$(PROJNAME)/startup

INC=$(PROJ)/projects/$(PROJNAME)/inc
INC_CMSIS=$(PROJ)/libraries/CMSIS/Include
INC_CMSIS_STM=$(PROJ)/libraries/CMSIS/ST/STM32F0xx/Include

CFILES=$(SRC)/main.c $(SRC)/usart.c

compile:
	arm-none-eabi-gcc $(CFILES) -I$(STARTUP) -I$(INC) -I$(INC_CMSIS) -I$(INC_CMSIS_STM)
