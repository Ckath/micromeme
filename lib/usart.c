/*
 * usart.c
 *
 * Description: Implementation of USART functions
 */ 

/******************************************************************************
  Includes
******************************************************************************/
#include <avr/io.h>
#include "usart.h"

/******************************************************************************
  Function      : USART_init
  Description   : Initializes the USART
  Pre           : F_CPU is defined
  Post          : U2X bit is set
                  Transmitter and receiver are enabled
                  
******************************************************************************/
void USART_init(unsigned long int baud)
{
    /* enable double speed */
    UCSR1A |= (1 << U2X1);
    
    /* Set baud rate */
    int16_t ubbr = (F_CPU / (8ul * baud)) - 1;
    UBRR1H = (uint8_t) (ubbr / 256);
    UBRR1L = (uint8_t) (ubbr);

    /* Default is async, no parity, 8 data bits, 1 stop bit */
    /* UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);    8 data bits, 1 stop bit */
    
    /* Enable Receiver and Transmitter */
    UCSR1B = (1 << TXEN1) | (1 << RXEN1);
}

/******************************************************************************
  Function      : USART_putch
  Description   : Writes one character to the transmit register of the USART
                  and waits until it is transmitted
  Pre           : USART is initialized
  Post          : Original character is returned
******************************************************************************/
int USART_putch( char c )
{
    // Wait for UDR to be empty
    while( !(UCSR1A & (1<<UDRE1)) )         /* loop while bit is set */
	{
	}
    UDR1 = c;
    return (c);
}

/******************************************************************************
  Function      : USART_getch
  Description   : Gets one character from the receive register of the USART
                  It will wait until a character is available
  Pre           : USART is initialized
  Post          : The read character is returned
******************************************************************************/
unsigned char USART_getch (void)
{
    // Wait for data received by the UART
    while( !(UCSR1A & (1<<RXC1)) )          /* loop while bit is set */
	{
	}
    return UDR1;
}

/******************************************************************************
  Function      : USART_putstr
  Description   : Writes a string of characters to the USART transmit register
  Pre           : USART is initialized
                  String is terminated with �\0�
  Post          : -
******************************************************************************/
void USART_putstr( char p[] )
{
    int i = 0;

    while(p[i] != 0)
    {
        if (p[i] == LF)
        {
            USART_putch(CR);
        }
        
        USART_putch(p[i]);
        i++;
    }
}

/******************************************************************************
  Function      : UART_getstr
  Description   : Gets a string of characters from the USART receive register
                  The string is placed in the buffer pointed to by p
                  It returns as soon as the the character 'CR' is received
  Pre           : USART is initialized
                  p points to a character buffer with enough places available
  Post          : String is terminated with �\0�
******************************************************************************/
void USART_getstr (char p[])
{
    int i = 0;
    
    do 
    {
        p[i] = USART_getch();
        i++;
        
    } while (p[i-1] != CR);
    
    p[i] = '\0';
}

/******************************************************************************
  EOF
******************************************************************************/
