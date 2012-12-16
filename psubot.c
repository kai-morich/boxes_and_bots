
#include "psubot.h"

int gi_debounce_counter_button = 0;
BOOL gb_sleeping = 0;

void psubot_init( void ) {

   /* Turn off all indicators.*/
   P1OUT = 0;
   P2OUT = 0;
}

void psubot_button_enable( void ) {

   /* Enable button interrupt. */
   P2IFG &= ~BUTTON;
   P2REN |= BUTTON;
   P2IE |= BUTTON;

   /* Listen for button release. */
   P2IES &= ~BUTTON;
}

void psubot_eye_enable( void ) {

   /* Setup Motor I/O */
   P1DIR = 0;
   P1DIR |= EYE_R;
   P1DIR |= EYE_L;
   P1DIR |= EYE_ON;
   
   /* Setup LED I/O */
   P2DIR = 0;
   P2DIR |= LED_RED;
   P2DIR |= LED_GREEN;
   P2DIR |= LED_BLUE;
}

/* Parameters:                                                                *
 *    i_pos_in - The percentage to the left to position the eye.              */
void psubot_eye_pos( int i_pos_in ) {
   //int i_pos_target = 0;
   int i;

   /* Turn on the motor. */
   P1OUT |= EYE_ON;

   /* Move the eye to position zero. */
   P1OUT |= EYE_R;
   while( !(P2IN & EYE_SENSE) ) {
      __delay_cycles( 10 );
   }
   P1OUT &= ~EYE_R;

   /* Turn off the motor. */
   P1OUT &= ~EYE_ON;

   psubot_eye_left( i_pos_in );
}

/* Purpose: Push the eye to the robot's left by the given increment.          */
void psubot_eye_left( int i_pos_in ) {
   int i;

   /* Turn on the motor. */
   P1OUT |= EYE_ON;

   /* Move the eye to the selected position. */
   P1OUT |= EYE_L;
   for( i = 0 ; i < i_pos_in ; i++ ) {}
   P1OUT &= ~EYE_L;

   /* Turn off the motor. */
   P1OUT &= ~EYE_ON;
}

/* Purpose: Push the eye to the robot's right by the given increment.         */
void psubot_eye_right( int i_pos_in ) {
   int i;

   /* Turn on the motor. */
   P1OUT |= EYE_ON;

   /* Move the eye to the selected position. */
   P1OUT |= EYE_R;
   for( i = 0 ; i < i_pos_in ; i++ ) {}
   P1OUT &= ~EYE_R;

   /* Turn off the motor. */
   P1OUT &= ~EYE_ON;
}

void psubot_serial_init( void ) {
   
   /* Set DCO to 1MHz. */
   BCSCTL1 = CALBC1_1MHZ;
   DCOCTL = CALDCO_1MHZ;
   
   /* Set P1.1 and P1.2 to RX and TX. */
   P1SEL = BIT1 + BIT2;                
   P1SEL2 = BIT1 + BIT2;
   
   /* Use SMCLK. */
   UCA0CTL1 |= UCSSEL_2;

   /* Set bitrate to 9600. */
   UCA0BR0 = 104;
   UCA0BR1 = 0;
   
   /* Use modulation. */
   UCA0MCTL = UCBRS_1;

   /* Start USCI. */
   UCA0CTL1 &= ~UCSWRST;

   /* Enable UART RX interrupt. */
   IE2 |= UCA0RXIE;
}

