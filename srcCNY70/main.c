/*
 * cny70.c
 *
 * Created: 15.03.2016 18:21:55
 * Author : oeiken
 */ 

#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#define black 0x80

// Diese Beispiel zeigt die Anwendung des ADC eines ATmega169
// unter Verwendung der internen Referenzspannung von nominell 1,1V.
// Zur Anpassung an andere AVR und/oder andere Referenzspannungen
// siehe Erläuterungen in diesem Tutorial und im Datenblatt
 
/* ADC initialisieren */
void ADC_Init(void)
{
  // die Versorgungsspannung AVcc als Referenz wählen:
  //ADMUX = (1<<REFS0);    
  // oder interne Referenzspannung als Referenz für den ADC wählen:
  //ADMUX = (1<<REFS1) | (1<<REFS0);
 
  // Bit ADFR ("free running") in ADCSRA steht beim Einschalten
  // schon auf 0, also single conversion
  ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);     // Frequenzvorteiler 128
  ADCSRA |= (1<<ADEN);                  // ADC aktivieren
 
  /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
     also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */
 
  ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung 
  while (ADCSRA & (1<<ADSC) ) {         // auf Abschluss der Konvertierung warten
  }
  /* ADCW muss einmal gelesen werden, sonst wird Ergebnis der nächsten
     Wandlung nicht übernommen. */
  (void) ADCW;
}
 
/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel )
{
  // Kanal waehlen, ohne andere Bits zu beeinflußen
  ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
  ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
  while (ADCSRA & (1<<ADSC) ) {   // auf Abschluss der Konvertierung warten
  }
  return ADCW;                    // ADC auslesen und zurückgeben
}
 
/* ADC Mehrfachmessung mit Mittelwertbbildung */
/* beachte: Wertebereich der Summenvariablen */
uint16_t ADC_Read_Avg( uint8_t channel, uint8_t nsamples )
{
  uint32_t sum = 0;
 
  for (uint8_t i = 0; i < nsamples; ++i ) {
    sum += ADC_Read( channel );
  }
 
  return (uint16_t)( sum / nsamples );
}
 

 
/* Beispielaufrufe: */
 
int main( void )
{
  uint16_t adcval;
  
  //clock_prescale_set(clock_div_1);
  ADC_Init();
 
  while( 1 ) {
    adcval = ADC_Read(2);  // Kanal 2
    // mach was mit adcval
	if (adcval>=black)
	{
		PORTB |= (1<<PB0);
	} 
	else
	{
		PORTB &= ~(1<<PB0);
	}
	adcval = ADC_Read(3);  // Kanal 3
	if (adcval>=black)
	{
		PORTB |= (1<<PB1);
	}
	else
	{
		PORTB &= ~(1<<PB1);
	}	
 }
}

