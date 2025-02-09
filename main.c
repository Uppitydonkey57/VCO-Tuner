#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define TUNER_DDR			DDRB
#define TUNE_OFFSET_DDR		DDRD

#define TUNER_PORT			PORTB
#define TUNE_OFFSET_PORT	PORTD

#define ON_TUNE_PORT		PORTD6	
#define OVER_TUNED_PORT		PORTD5	
#define UNDER_TUNED_PORT	PORTD7	

#define CLOCK_SPEED			1000000
#define PRESCALER			256
#define CYCLE_TO_SECOND_CONSTANT 2*(CLOCK_SPEED/PRESCALER)

// last bit notates sharp
#define A_LED_CONFIG		0b11101110
#define A_SHARP_LED_CONFIG	0b11101111
#define B_LED_CONFIG		0b00111110
#define C_LED_CONFIG		0b00011010
#define C_SHARP_LED_CONFIG	0b00011011
#define D_LED_CONFIG		0b01111010
#define D_SHARP_LED_CONFIG	0b01111011
#define E_LED_CONFIG		0b10011110
#define F_LED_CONFIG		0b10001110
#define F_SHARP_LED_CONFIG	0b10001111
#define G_LED_CONFIG		0b11110110
#define G_SHARP_LED_CONFIG	0b11110111

#define COMMON_ANODE

enum Notes {
	A,
	A_SHARP,
	B,
	C,
	C_SHARP,
	D,
	D_SHARP,
	E,
	F,
	F_SHARP,
	G,
	G_SHARP
};

uint8_t on = 0;
double frequency = 0;
uint16_t pulse_counter = 0;
uint16_t note = 0;
uint16_t octave = 0;
uint16_t capture = 0;

void init_external_interrupt0() {
	EIMSK |= (1 << INT0); // Enable interrupt zero
	EICRA |= (1 << ISC01) | (1 << ISC00); // Trigger interrupt on rising edge
}

void init_timer1() {
	TCCR1B |= (1 << CS12); // Set clock prescaler to 256
	TCNT1 = 0; // Reset timer
}

ISR(INT0_vect) {
	capture = TCNT1;
	TCNT1 = 0;
}

void set_tuner_display(uint8_t config) {
#ifdef COMMON_CATHODE
	TUNER_PORT = config;
#elifdef COMMON_ANODE
	TUNER_PORT = ~config;
#endif
}

int main()
{
	init_external_interrupt0();
	init_timer1();
	sei();
	TUNER_DDR = 0xFF;
	TUNE_OFFSET_DDR |= (1 << ON_TUNE_PORT) | (1 << OVER_TUNED_PORT) | (1 << UNDER_TUNED_PORT);
	//DDRC = (;

    while (1) {
		double period = (((double)capture)/7812.5);
		frequency = (1.0/period);
		double unrounded_note = (12.0*log(frequency/(27.5))/log(2.0));
		note = (uint16_t)round((12.0*log(frequency/(27.5))/log(2.0)));
		octave = (uint16_t)(floor(((double)note)/12.0));
		uint16_t note_relative = note - (octave*12);
		double unrounded_relative_note = unrounded_note - note;

		/*if (unrounded_relative_note >= 1.0/4.0 && unrounded_relative_note <= 2.0*(1.0/4.0)) {
			TUNE_OFFSET_PORT &=	~(1 << ON_TUNE_PORT);
			TUNE_OFFSET_PORT &=	~(1 << OVER_TUNED_PORT);
			TUNE_OFFSET_PORT |=	(1 << UNDER_TUNED_PORT);
		} else if (unrounded_relative_note <= 1.0/4.0 || unrounded_relative_note >= 3.0*(1.0/4.0)) {
			TUNE_OFFSET_PORT &=	~(1 << UNDER_TUNED_PORT);
			TUNE_OFFSET_PORT &=	~(1 << OVER_TUNED_PORT);
			TUNE_OFFSET_PORT |=	(1 << ON_TUNE_PORT);
		} else if (unrounded_relative_note >= 2.0*(1.0/4.0) && unrounded_relative_note <= 3.0*(1.0/4.0)) {
			TUNE_OFFSET_PORT &=	~(1 << UNDER_TUNED_PORT);
			TUNE_OFFSET_PORT &=	~(1 << ON_TUNE_PORT);
			TUNE_OFFSET_PORT |=	(1 << OVER_TUNED_PORT);
		}*/

		switch (note_relative) {
			case A:
				set_tuner_display(A_LED_CONFIG);
				break;
			case A_SHARP:
				set_tuner_display(A_SHARP_LED_CONFIG);
				break;
			case B:
				set_tuner_display(B_LED_CONFIG);
				break;
			case C:
				set_tuner_display(C_LED_CONFIG);
				break;
			case C_SHARP:
				set_tuner_display(C_SHARP_LED_CONFIG);
				break;
			case D:
				set_tuner_display(D_LED_CONFIG);
				break;
			case D_SHARP:
				set_tuner_display(D_SHARP_LED_CONFIG);
				break;
			case E:
				set_tuner_display(E_LED_CONFIG);
				break;
			case F:
				set_tuner_display(F_LED_CONFIG);
				break;
			case F_SHARP:
				set_tuner_display(F_SHARP_LED_CONFIG);
				break;
			case G:
				set_tuner_display(G_LED_CONFIG);
				break;
			case G_SHARP:
				set_tuner_display(F_SHARP_LED_CONFIG);
				break;
		}
		
	}
    return 0;
}
