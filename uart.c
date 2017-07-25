/*
 * uart.c
 *
 *  Created on: Feb 16, 2016
 *      Author: suhail
 */



#include "uart.h"
#include "defines.h"
#include <stdint.h>
#include <stddef.h>
#include <msp430.h>

struct baud_value
{
    uint32_t baud;
    uint16_t UCAxBR0;
    uint16_t UCAxBR1;
    uint16_t UCAxMCTL;
};

/* Table of baud rate register values from reference manual (SLAU144) */
static const struct baud_value baud_tbl[] = {
    {9600, 104, 0, 0x2}
};


/**
 * \brief Initialize the UART peripheral
 * \param[in] config - the UART configuration
 * \return 0 on success, -1 otherwise
 */
int uart_init(uart_config_t *config)
{
    int status = -1;

    /* USCI should be in reset before configuring - only configure once */
    if (UCA0CTL1 & UCSWRST) {
        size_t i;

        /* Set clock source to SMCLK */
        UCA0CTL1 |= UCSSEL_2;

        /* Find the settings from the baud rate table */
        for (i = 0; i < ARRAY_SIZE(baud_tbl); i++) {
            if (baud_tbl[i].baud == config->baud) {
                break;
            }
        }

        if (i < ARRAY_SIZE(baud_tbl)) {
            /* Set the baud rate */
            UCA0BR0 = baud_tbl[i].UCAxBR0;
            UCA0BR1 = baud_tbl[i].UCAxBR1;
            UCA0MCTL = baud_tbl[i].UCAxMCTL;

            /* Enable the USCI peripheral (take it out of reset) */
            UCA0CTL1 &= ~UCSWRST;
            status = 0;
        }
    }

    return status;
}



/**
 * \brief Read a character from UART
 * \return the character read on success, -1 if nothing was read
 */
int uart_getchar(void)
{
	  while (!(IFG2&UCA0RXIFG));            // Wait till RX buffer is ready
	  return UCA0RXBUF;                     // Load the character with RX buffer
}

/**
 * \brief Write a character to UART
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
int uart_putchar(int c)
{
    /* Wait for the transmit buffer to be ready */
    while (!(IFG2 & UCA0TXIFG));

    /* Transmit data */
    UCA0TXBUF = (char ) c;

    return 0;
}

/**
 * \brief Write a string to UART
 * \return 0 on sucesss, -1 otherwise
 */
int uart_puts(const char *str)
{
    int status = -1;

    if (str != NULL) {
        status = 0;

        while (*str != '\0') {
            /* Wait for the transmit buffer to be ready */
            while (!(IFG2 & UCA0TXIFG));

            /* Transmit data */
            UCA0TXBUF = *str;

            /*  If there is a line-feed, add a carriage return */
            if (*str == '\n') {
                /* Wait for the transmit buffer to be ready */
                while (!(IFG2 & UCA0TXIFG));
                UCA0TXBUF = '\r';
            }

            str++;
        }
    }

    return status;
}




void initial_setup()
{

	 P1DIR=0xFF;  //first setting up port 1 as ooutput
	 P1OUT=0x00;  // all pins low, now lets rock.

	///////////////////////////Setting up uart/////////////////////////////////
	    uart_config_t config;


	    /* Configure the clock module - MCLK = 1MHz */
	    DCOCTL = 0;
	    BCSCTL1 = CALBC1_1MHZ;
	    DCOCTL = CALDCO_1MHZ;

	    /* Configure ACLK to to be sourced from VLO = ~12KHz */
	    BCSCTL3 |= LFXT1S_2;


	    /* Configure P1.1=Rx and P1.2=Tx for UART (USCI_A0) */
	    P1SEL = 0x6;
	    P1SEL2 = 0x6;


	    /* Global interrupt enable */
	      IE2 |= UCA0RXIE; 					//Enable USCI_A0 RX interrupt

	       config.baud = 9600;

	          if (uart_init(&config) != 0) {
	              while (1);
	          }
	/////////////////////////////////////////////////////////////////////////
}

