
# compiler options -Os meaning optimise for small size not nessesarrly speed
# DF_CPU=16000000UL marks how fast i want the microcontrollers clock to run
# A lot of mcus let you slow down the clock to save power
# mmcu=atmega328p  is how we select what processor to compile for
CC=avr-gcc
CFLAGS= -Os -DF_CPU=16000000UL -mmcu=atmega328p
BINS=blinktimer.c

#Compiling
all: $(BINS)

# detect what port my arduino is on.
# might have to change this if you have two connected or are not using mac
# helps automate the flash process 
#USBPORT=/dev/tty.usbmodem14401
USBPORT:=$(shell ls /dev/ttyACM0)

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@
#standard rule to compile .c to .o

%.hex: %.out
	avr-objcopy -O ihex -R .eeprom $< $@
# takes a compiled .o file and copies out part of it/ extracts 
#the part in the eeprom segment whick is what we want to put on our microcontroller
# its going to extract it in intel hex format thats the format that our programmer wants it in

#upload to board rule is to install a .hex file onto the device
#avrdude has a lot of diffrent options
# 1 im programming an arduino board -c arduino -p ATMEGA328P
# 2im telling it what serial port to use -P ${USBPORT}
# 3 what baud rate or communication speed -b 115200
# 4 what action i want to take -U flash:w:$< which is to write new code to the processors flash memory
install.%: %.hex
	avrdude -F -V -c arduino -p ATMEGA328P -P ${USBPORT} -b 115200 -U flash:w:$<

clean:
	rm -f  *.hex *.out
