#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../lib/usb_keyboard.h"

#define TOTAL_MEASUREMENTS 100
#define INTERVAL_MS 100
#define NIGHTMODE_ON KEY_F1, KEY_LEFT_GUI
#define NIGHTMODE_OFF KEY_F2, KEY_LEFT_GUI
#define NIGHT_LDR_VALUE 100

#define DAY_MODE 0
#define NIGHT_MODE 1

int main(void)
{
	// setup pin for christmas lights
	DDRB = (1<<5);

	// setup adc
	DDRF = 0;
	ADCSRA  = (1 << ADEN) | (0b111 << ADPS0);
	ADCSRB =  (1 << MUX5);
	ADMUX |= (1 << REFS0);

	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while (!usb_configured()) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);

	uint8_t measurements = 0, mode = DAY_MODE;
	uint32_t ldr_value = 0;
	for (;;) {
		// take measurement
		ADCSRA |= (1 << ADSC); 
		while(ADCSRA & (1<<ADSC));

		// add to total
		ldr_value += ADC;
		measurements++;

		// decide to turn off/on nightmode based on measurement avg
		if (measurements >= TOTAL_MEASUREMENTS) {
			if (mode != NIGHT_MODE && ldr_value/measurements < NIGHT_LDR_VALUE) {
				mode = NIGHT_MODE;
				usb_keyboard_press(NIGHTMODE_ON);
				PORTB |= (1<<5);
			} else if (mode != DAY_MODE && ldr_value/measurements > NIGHT_LDR_VALUE) {
				mode = DAY_MODE;
				usb_keyboard_press(NIGHTMODE_OFF);
				PORTB &= ~(1<<5);
			}

			// reset for next run
			measurements = 0;
			ldr_value = 0;
		}

		_delay_ms(INTERVAL_MS);
	}
}
