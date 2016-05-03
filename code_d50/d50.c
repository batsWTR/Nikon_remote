// Programme de télécommande infra rouge pour Nikon
// construit autour d'un Attiny85
// Sept 2015
// B.WENTZLER



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void d50(void);
ISR(PCINT0_vect);

int main(void)
{
// Configuration du timer pour une freq de 38Khz sur PB4

	cli();	// clear interrupts
	//DDRB = 1<<PB4; // pb4 en sortie
	TCCR1 =  1<<CS10; // freq CPU
	GTCCR = 1<<PWM1B | 1<<COM1B0 ; // pwm actif, toggle OC1B
	OCR1C = 26; // le compteur compte jusque 152
	OCR1B = 13;  // déclenchement a 128

//**************************************************************

// Config pour le bouton poussoir sur PB3
// par defaut au niveau logique HIGH
// declenchement au niveau LOW par inter PCINT0
	DDRB = 0<<PB3;
	GIMSK = 1<<PCIE;  //General inter mask reg, pin change a 1
	PCMSK = 1<<PCINT3; //pin change mask, sur PB3
	
	sei();	// active les interuptions


//***************************************************************


// Config du mode sleep

	MCUCR = 1<<SM1 | 0<<SM0; // MCU control reg, mode power down
	
//************************************************************

// config led temoin

	DDRB = 1<<DDB2;

	



	while(1)
	{
		MCUCR = 1<<SE;  //sleep enable
		sleep_mode();   // dors, dors

	}



}

// Code infrarouge envoyé deux fois,séparés de 63ms

void d50(void)
{
	PORTB = 1<<PORTB2;  // pour une output HIGH avec DDRB

	DDRB = 1<<PB4;
	_delay_us(2000);

	DDRB = 0<<PB4;
	_delay_ms(28);

	DDRB = 1<<PB4;
	_delay_us(500);

	DDRB = 0<<PB4;
	_delay_us(1500);

	DDRB = 1<<PB4;
	_delay_us(500);

	DDRB = 0<<PB4;
	_delay_us(3500);

	DDRB = 1<<PB4;
	_delay_us(500);

	DDRB = 0<<PB4;

	_delay_ms(63);


	
	DDRB = 1<<PB4;
	_delay_us(2000);

	DDRB = 0<<PB4;
	_delay_ms(28);

	DDRB = 1<<PB4;
	_delay_us(500);

	DDRB = 0<<PB4;
	_delay_us(1500);

	DDRB = 1<<PB4;
	_delay_us(500);

	DDRB = 0<<PB4;
	_delay_us(3500);

	DDRB = 1<<PB4;
	_delay_us(500);

	DDRB = 0<<PB4;

	
}




// Interruption sur le bouton poussoir


ISR(PCINT0_vect)
{
	
	cli();   // on desactive
	MCUCR = 0<<SE;   // desactive sleep mode
	_delay_ms(20);   // attente fin rebonds

	// test l appui et non le relachement
	if(!(PINB & (1<<PB3)))
	{
		PORTB = 1<<PORTB2;  // Activation de la led témoin
		d50();		// on lance la trame IR
		_delay_ms(100); // un ptit delai pr la led
		PORTB = 0<<PORTB2;  // extinction de la led
	}


	sei();
	

}	









