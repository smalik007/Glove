#include <msp430.h> 
#include "uart.h"
#include "defines.h"



int i;
char rx[3];
//char rx;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	initial_setup();

	P2OUT = 0x00;
	while(1)
	{
		//P1DIR |= BIT0;   // P1.0 LED1 directed as output. or in our case left motor
		//P1DIR |= BIT6;   // P1.6 LED2 directed as output . or in our case right motor.

		P2DIR |= BIT3 + BIT4 + BIT5;

		for(i=0; i<3; i++)
		{
			rx[i]=uart_getchar();
		}

		//rx = uart_getchar();

		if(rx[0]=='V'&&rx[1]=='L'&&rx[2]=='M')
			{
			           P2OUT ^= BIT4;
						_delay_cycles(500000);
						P2OUT ^= BIT4;
			}

		else if(rx[0]=='V'&&rx[1]=='R'&&rx[2]=='M')
					{
			            P2OUT ^= BIT5;
						_delay_cycles(500000);
						P2OUT ^= BIT5;
					}

		/*

		if(rx=='a'|rx=='A')
		{
			P2OUT ^= BIT4;
			_delay_cycles(500000);
			P2OUT ^= BIT4;
		}

		else if(rx=='s'|rx=='S')
				{
					P2OUT ^= BIT4;
					_delay_cycles(200000);
				    P2OUT ^= BIT4;
				}


		else if(rx=='k'|rx=='K')
		{
			P2OUT ^= BIT5;
			_delay_cycles(500000);
		    P2OUT ^= BIT5;
		}

		else if(rx=='l'|rx=='L')
				{
					P2OUT ^= BIT5;
					_delay_cycles(200000);
				    P2OUT ^= BIT5;
				}
				*/
	}

   return 0;
}


