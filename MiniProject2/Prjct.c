#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void Timer1_init(void){
	TCCR1B= (1<<CS10)| (1<<WGM12)| (1<<CS11);
	TCNT1=0;
	TCCR1A=(1<<FOC1A);
	OCR1A=15624;
	TIMSK|=(1<<OCIE1A);
	SREG|=(1<<7);
}
void INT0_init(void){
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01);
	PORTD|=(1<<PD2);
	DDRD &= ~(1<<PD2);
}
void INT1_init(void){
	MCUCR|= (1<<ISC11)|(1<<ISC10);
	GICR|=(1<<INT1);
	DDRD &= ~(1<<PD3);
}
void INT2_init(void){
	GICR|= (1<<INT2);
	MCUCSR &= ~(1<<ISC2);
	DDRB &= ~(1<<PB2);
	PORTB |=(1<<PB2);
}
unsigned char seconds=0;
unsigned char min=0;
unsigned char hours=0;
ISR(TIMER1_COMPA_vect){
	seconds++;
	if(seconds==60){
		seconds=0;
		min++;
	}
	if(min>=60){
		min=0;
		hours++;
	}
	if(hours>=24){
		hours=0;
	}
}
ISR(INT0_vect){
	seconds=0;
	min=0;
	hours=0;
}
ISR(INT1_vect){
	TCCR1B &= ~(1<<CS10)& ~(1<<CS11);
	//TCCR1B=0xF8;
}
ISR(INT2_vect){
	TCCR1B|= (1<<CS10)| (1<<CS11);
}

int main(void){
	DDRC |= 0x0F;
	DDRA |= 0x3F;
	PORTC &= 0xF0;
	Timer1_init();
	INT0_init();
	INT1_init();
	INT2_init();


	while(1){
		PORTA=(PORTA & 0xC0)|(1<<0);
		PORTC= (PORTC & 0xF0)|((seconds%10)& 0x0F);
		_delay_ms(2);
		PORTA =(PORTA & 0xC0)|(1<<1);
		PORTC= (PORTC & 0xF0)|((seconds/10)& 0x0F);
		_delay_ms(2);
		PORTA= (PORTA & 0xC0)|(1<<2);
		PORTC= (PORTC & 0xF0)|((min%10)& 0x0F);
		_delay_ms(2);
		PORTA= (PORTA & 0xC0)|(1<<3);
		PORTC= (PORTC & 0xF0)|((min/10)& 0x0F);
		_delay_ms(2);
		PORTA= (PORTA & 0xC0)|(1<<4);
		PORTC= (PORTC & 0xF0)|((hours%10)& 0x0F);
		_delay_ms(2);
		PORTA= (PORTA & 0xC0)|(1<<5);
		PORTC= (PORTC & 0xF0)|((hours/10)& 0x0F);
		_delay_ms(2);


	}

}
