#include <avr/io.h>
#include"util/delay.h"
#include <avr/interrupt.h>

char seconds=0,minutes=0,hours=0;

void int0_init(void){

	DDRD &=~(1<<PD2);
	PORTD|=(1<<PD2);
    // internal pull up

    //reset_function
    GICR|=(1<<INT0);
    MCUCR&=~(1<<ISC00);
    MCUCR|=(1<<ISC01);
}

void int1_init(void){

	DDRD &= ~(1<<PD3);

    //pause_function
    GICR|=(1<<INT1);
    MCUCR|=(1<<ISC00)|(1<<ISC01);
}
void int2_init(void){

DDRB &=~(1<<PB2);
	PORTB |= (1<<PB2);   // internal pull up


    //resume_function
    GICR|=(1<<INT2);
    MCUCSR&=~(1<<ISC2);
}
//timer1 config
void timer1_init(void){
   TCCR1A = (1<<FOC1A);
   TCCR1B&=(1<<CS12);
   TCCR1B|=(1<<CS10)|(1<<CS11)|(1<<WGM12);//prescaler=64,activate ctc mode
   TCNT1=0;
   OCR1A=15625;
   TIMSK|=(1<<OCIE1A);
}

ISR(INT0_vect){
    seconds=0;
    minutes=0;
    hours=0;
}
ISR(INT1_vect){
    TCCR1B &=~(1<<CS00)&~(1<<CS01)&~(1<<CS02);
}
ISR(INT2_vect){
    TCCR1B&=(1<<CS12);
   TCCR1B|=(1<<CS10)|(1<<CS11)|(1<<WGM12);//prescaler=64,activate ctc mode
}
ISR(TIMER1_COMPA_vect){
    if(seconds < 59)
        ++seconds;
    else
    {
        seconds=0;
        ++minutes;
    }

    if(minutes==60)
    {
        ++hours;
        minutes=0;
    }

    if(hours==12)
    	hours=0;

}

void main(void) {
    //Inittialaization_code
    DDRA = 0xff;	//configure the 7seg data pins to input
	PORTA = 0xff;
	DDRC = 0x0f;	//configure the 7seg data pins to output
	PORTC = 0x00;  // at beginning all LEDS off


    //extal_int
    SREG|=(1<<7); //Enable global interrupt
    int0_init();
    int1_init();
    int2_init();
    timer1_init();


    while(1){
    //application_code
	PORTA = (1<<5);
	PORTC = seconds % 10;
	_delay_ms(2);
	PORTA = (1<<4);
	PORTC = seconds / 10;
	_delay_ms(2);
	PORTA = (1<<3);
	PORTC = minutes % 10;
	_delay_ms(2);
	PORTA = (1<<2);
	PORTC = minutes / 10;
	_delay_ms(2);
	PORTA = (1<<1);
	PORTC = hours % 10;
	_delay_ms(2);
	PORTA = (1<<0);
	PORTC = hours / 10;
	_delay_ms(2);

   }

}
