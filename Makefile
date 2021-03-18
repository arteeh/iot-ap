CC=gcc
PROJ=reviews/week4-4
PROJNAME=workshop4

SRC=$(PROJ)/projects/$(PROJNAME)/src
STARTUP=$(PROJ)/projects/$(PROJNAME)/startup

INC_PROJ=$(PROJ)/projects/$(PROJNAME)/inc
INC_CMSIS=$(PROJ)/libraries/CMSIS/Include
INC_CMSIS_STM=$(PROJ)/libraries/CMSIS/ST/STM32F0xx/Include
INC_ALL=-I$(STARTUP) -I$(INC_PROJ) -I$(INC_CMSIS) -I$(INC_CMSIS_STM)

CFILES=$(SRC)/main.c $(SRC)/usart.c

compile:
	$(CC) $(CFILES) $(INC_ALL)  -o $(PROJNAME)

run:
	./$(PROJNAME)

clean:
	rm $(PROJNAME)
