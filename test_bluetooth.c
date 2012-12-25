
#include <msp430.h>

#include "psubot.h"
#include "uart.h"
#include "shell.h"
#include "beep.h"

#include <stdlib.h>

SHELL_ENABLE();

int i_led_current = LED_RED;

int main( void ) {
   WDTCTL = WDTPW + WDTHOLD;

   psubot_init();

   psubot_eye_enable();
   //psubot_eye_pos( 50 );

   psubot_wheels_enable();

   P2OUT = i_led_current;

   uart_serial_init();

   beep_init();

   // TODO: Check to see if we're connected.
   uart_echo( "\r\n+STWMOD=0\r\n" );
   uart_echo( "\r\n+STNA=PSUBot\r\n" );
   uart_echo( "\r\n+STPIN=2222\r\n" );
   uart_echo( "\r\n+STOAUT=1\r\n" );

   __delay_cycles( 500000 );
   uart_echo( "\r\n+INQ=1\r\n" );
   __delay_cycles( 500000 );

   beep( 40, 250 );

   shell_init();

   /* Go to sleep. */
   __bis_SR_register( LPM3_bits + GIE );

   return 0;
}

void command_led( void ) {
   
   if( shell_strcmp( "RED", gac_args[1] ) ) {
      i_led_current = LED_RED;
   } else if( shell_strcmp( "GREEN", gac_args[1] ) ) {
      i_led_current = LED_GREEN;
   } else if( shell_strcmp( "BLUE", gac_args[1] ) ) {
      i_led_current = LED_BLUE;
   }

   #if 0
   switch( i_led_current ) {
      case LED_RED:
         i_led_current = LED_GREEN;
         break;

      case LED_GREEN:
         i_led_current = LED_BLUE;
         break;

      case LED_BLUE:
         i_led_current = LED_RED;
         break;

   }
   #endif

   P2OUT = i_led_current;
}

void command_eye( void ) {
   if( shell_strcmp( "POS", gac_args[1] ) ) {
      psubot_eye_pos( atoi( gac_args[2] ) );
   } else if( shell_strcmp( "R", gac_args[1] ) ) {
      psubot_eye_right( atoi( gac_args[2] ) );
   } else if( shell_strcmp( "L", gac_args[1] ) ) {
      psubot_eye_left( atoi( gac_args[2] ) );
   }
}

void command_beep( void ) {
   beep( atoi( gac_args[1] ), atoi( gac_args[2] ) );
}

void command_drive( void ) {
   if( shell_strcmp( "R", gac_args[1] ) ) {
      psubot_wheel_drive( WHEELS_DIRECTION_RIGHT, atoi( gac_args[2] ) );
   } else if( shell_strcmp( "L", gac_args[1] ) ) {
      psubot_wheel_drive( WHEELS_DIRECTION_LEFT, atoi( gac_args[2] ) );
   } else if( shell_strcmp( "PR", gac_args[1] ) ) {
      psubot_wheel_drive( WHEELS_DIRECTION_RIGHT_PIVOT, atoi( gac_args[2] ) );
   } else if( shell_strcmp( "PL", gac_args[1] ) ) {
      psubot_wheel_drive( WHEELS_DIRECTION_LEFT_PIVOT, atoi( gac_args[2] ) );
   } else if( shell_strcmp( "F", gac_args[1] ) ) {
      psubot_wheel_drive( WHEELS_DIRECTION_FORWARD, atoi( gac_args[2] ) );
   } else if( shell_strcmp( "B", gac_args[1] ) ) {
      psubot_wheel_drive( WHEELS_DIRECTION_REVERSE, atoi( gac_args[2] ) );
   }
}

SHELL_COMMANDS_BLOCK_START( 4 )
SHELL_COMMANDS_BLOCK_ITEM( "LED", "[EYE COLOR]", command_led ),
SHELL_COMMANDS_BLOCK_ITEM( "EYE", "[POS] [INCR] OR [R/L] [INCR]", command_eye ),
SHELL_COMMANDS_BLOCK_ITEM( "BEEP", "[FREQ] [TIME]", command_beep ),
SHELL_COMMANDS_BLOCK_ITEM( "DRIVE", "[F/B/R/L/RP/LP] [TIME]", command_drive ),
SHELL_COMMANDS_BLOCK_END()

