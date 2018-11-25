# see LICENSE file for copyright and license details.

DEV=/dev/ttyACM0
MODEL=atmega32u4
LIB=../lib/usb_keyboard.c 
LIB_OBJ=${LIB:.c=.o}
NAME=light_activated_nightmode
CC=avr-gcc

options:
	@echo ${NAME} make options:
	@echo "   clean:  clean temporary files"
	@echo "   hex  :  compile source to hex binary"
	@echo "   flash:  upload hex binary to arduino"

clean:
	@echo cleaning
	@rm -f ${NAME} ${LIB_OBJ} *.o *.hex

.c.o:
	@echo compiling libs
	@${CC} -Os -DF_CPU=16000000UL -mmcu=${MODEL} -c -o ${<:.c=.o} $<

hex: ${LIB_OBJ}
	@echo compiling binary
	@${CC} -Os -DF_CPU=16000000UL -mmcu=${MODEL} -c -o ${NAME}.o ${NAME}.c
	@${CC} -mmcu=${MODEL} ${NAME}.o ${LIB_OBJ} -o ${NAME}
	@avr-objcopy -O ihex -R .eeprom ${NAME} ${NAME}.hex

flash: hex
	@echo preparing to flash
	@avrdude -F -V -c avr109 -p ${MODEL} -P ${DEV} -b 115200 -U flash:w:${NAME}.hex
