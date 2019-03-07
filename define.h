#ifndef DEFINE_H
#define	DEFINE_H

#include	<p18f14k22.h>
#include 	<delays.h>
#include	"GenericTypeDefs.h"
#include  	<stdlib.h>
#include 	<string.h>
#include 	<timers.h>					// Timer library functions
#include        "Console.h"

#define CLOCK_FREQ          8000000//16000000
    //#define GetInstructionClock()   (CLOCK_FREQ / 4)

#define ENABLE_CONSOLE

    //******************* uC IO defs ***********************************************

#define STEP		PORTAbits.RA0
#define	DIR		PORTAbits.RA1
#define	CCW		PORTAbits.RA2
#define CW		PORTAbits.RA3
#define _OUT		PORTAbits.RA4
#define TTL		PORTAbits.RA5
#define LED             PORTBbits.RB7

#define CSMSB		PORTBbits.RB4
#define RX		PORTBbits.RB5
#define CSLSB		PORTBbits.RB6
#define TX      	PORTBbits.RB7

    /*****   COMMON FUNCTION PROTOTYPES   *****/
void Setup(void); // Configures peripherals and variables
void isr(void);
void isrhandler(void); // Located at high interrupt vector

void putrsUSART(const rom char *data); // Writes a string from ROM to the USART
void putsUSART(char *data); // Writes a string from RAM to the USART

void Delay10us(UINT32 tenMicroSecondCounter);
void DelayMs(UINT16 ms);
void DoCommand(void);
void cmd_interpreter(void);
void teszt(void);
