#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lib/usb_keyboard.h"

#define USER \
	usb_keyboard_press(KEY_U, 0); \
	usb_keyboard_press(KEY_R, 0); \
	usb_keyboard_press(KEY_E, 0); \
	usb_keyboard_press(KEY_R, 0);
#define PASS \
	usb_keyboard_press(KEY_P, 0); \
	usb_keyboard_press(KEY_A, 0); \
	usb_keyboard_press(KEY_S, 0); \
	usb_keyboard_press(KEY_S, 0); \
	usb_keyboard_press(KEY_W, 0); \
	usb_keyboard_press(KEY_O, 0); \
	usb_keyboard_press(KEY_R, 0); \
	usb_keyboard_press(KEY_D, 0);


int main(void)
{
	// one button on PB5, with pullup
	DDRB = 0;
	PORTB = 1<<5;

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);

	uint8_t press = 0, prev_press = 0;
	for (;;) {
		press = !(PINB & (1<<5));
		if (press && !prev_press) {
			USER
			usb_keyboard_press(KEY_ENTER, 0);
			PASS
			usb_keyboard_press(KEY_ENTER, 0);
		}

		// wait for debounce, store previous to stop re presses
		_delay_ms(50);
		prev_press = press;
	}
}
