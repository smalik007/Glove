/*
 * uart.h
 *
 *  Created on: Feb 16, 2016
 *      Author: cube26
 */
#include <stdint.h>

typedef struct
{
    uint32_t baud;
} uart_config_t;

/**
 * \brief Initialize the UART peripheral
 * \param[in] config - the UART configuration
 * \return 0 on success, -1 otherwise
 */
int uart_init(uart_config_t *config);

/**
 * \brief Read a character from UART
 * \return the character read on success, -1 if nothing was read
 */
int uart_getchar(void);

/**
 * \brief Write a character to UART
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
int uart_putchar(int c);

/**
 * \brief Write a string to UART
 * \return 0 on sucesss, -1 otherwise
 */
int uart_puts(const char *str);

void initial_setup();

