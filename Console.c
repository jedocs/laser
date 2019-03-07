
/************************ HEADERS **********************************/
#include	"define.h"

/************************ VARIABLES ********************************/
const rom char CharacterArray[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//const rom char
/************************ DEFINITIONS ******************************/
#define USART_USE_BRGH_HIGH
#if defined(USART_USE_BRGH_LOW)
#define SPBRG_VAL   ( ((CLOCK_FREQ/BAUD_RATE)/64) - 1)
#else
#define SPBRG_VAL   ( ((CLOCK_FREQ/BAUD_RATE)/16) - 1)
#endif

#if SPBRG_VAL > 255
#error "Calculated SPBRG value is out of range for currnet CLOCK_FREQ."
#endif

/************************ FUNCTIONS ********************************/

/*********************************************************************
 * Function:         void ConsoleInit(void)
 *
 * Overview:		    This function will configure the UART for use at
 *                   in 8 bits, 1 stop, no flowcontrol mode
 ********************************************************************/
void ConsoleInit(void) {
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB7 = 0;
#if defined(USART_USE_BRGH_HIGH)
    TXSTA = 0x24;
#else
    TXSTA = 0x20;
#endif

    RCSTA = 0x90; // 0b10010000;
    BAUDCON = 0b01000000;
    SPBRG = SPBRG_VAL;
}

/*********************************************************************
 * Function:         void ConsolePutROMString(ROM char* str)
 *
 * Input:		    str - ROM string that needs to be printed
 *
 * Overview:		    This function will print the inputed ROM string
 ********************************************************************/
void ConsolePutROMString(const rom char* str) {
    unsigned char c;

    while (c = *str++)
        ConsolePut(c);
}

/*********************************************************************
 * Function:         void ConsolePutString(ROM char* str)
 *
 * Input:		    str - ROM string that needs to be printed
 *
 * Overview:		    This function will print the inputed ROM string
 ********************************************************************/
void ConsolePutString(unsigned char *s) {
    unsigned char c;

    while (c = *s++)
        ConsolePut(c);
}

/*********************************************************************
 * Function:         void ConsolePut(unsigned char c)
 *
 * Input:		    c - character to be printed
 *
 * Overview:		    This function will print the inputed character
 ********************************************************************/
void ConsolePut(unsigned char c) {
    while (!ConsoleIsPutReady());
    TXREG = c;
    while (!ConsoleIsPutReady());
}

/*********************************************************************
 * Function:         unsigned char ConsoleGet(void)
 *
 * Output:		    one unsigned char received by UART
 *
 * Overview:		    This function will receive one unsigned char from UART
 ********************************************************************/
unsigned char ConsoleGet(void) {
    // Clear overrun error if it has occured
    // New unsigned chars cannot be received if the error occurs and isn't cleared
    if (RCSTAbits.OERR) {
        RCSTAbits.CREN = 0; // Disable UART receiver
        RCSTAbits.CREN = 1; // Enable UART receiver
    }

    return RCREG;
}

/*********************************************************************
 * Function:         void PrintChar(unsigned char toPrint)
 *
 * Overview:		    This function will print the inputed unsigned char to
 *                   the console in hexidecimal form
 ********************************************************************/
void PrintChar(unsigned char toPrint) {
    unsigned char PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint >> 4)&0x0F;
    ConsolePut(CharacterArray[toPrint]);
    toPrint = (PRINT_VAR)&0x0F;
    ConsolePut(CharacterArray[toPrint]);
    return;
}

/*********************************************************************
 * Function:         void PrintDec(unsigned char toPrint)
 *
 * Input:		    toPrint - character to be printed. Range is 0-99
 *
 * Overview:		    This function will print the inputed unsigned char to
 *                   the console in decimal form
 ********************************************************************/
void PrintDec(unsigned char toPrint) {
    if (toPrint >= 100)
        ConsolePut(CharacterArray[toPrint / 100]);
    if (toPrint >= 10)
        ConsolePut(CharacterArray[(toPrint % 100) / 10]);
    ConsolePut(CharacterArray[toPrint % 10]);
}

