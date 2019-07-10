#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../lib/usb_keyboard.h"

#define INTERVAL_MS 100
#define NIGHTMODE_ON KEY_F1, KEY_LEFT_GUI
#define NIGHTMODE_OFF KEY_F2, KEY_LEFT_GUI

#define DAY_MODE 0
#define NIGHT_MODE 1

int main(void)
{
	// setup pin for christmas lights
	DDRB = (1<<5);

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);

	uint8_t mode = DAY_MODE;
	for (;;) {
		// decide to turn off/on nightmode based on switch pos
		if (mode != NIGHT_MODE && PINB&(1<<4)) {
			mode = NIGHT_MODE;
			usb_keyboard_press(NIGHTMODE_ON);
			PORTB |= (1<<5);
		} else if (mode != DAY_MODE && !(PINB&(1<<4))) {
			mode = DAY_MODE;
			usb_keyboard_press(NIGHTMODE_OFF);
			PORTB &= ~(1<<5);
		}

		_delay_ms(INTERVAL_MS);
	}
}
