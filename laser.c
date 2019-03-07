#include "define.h"
#include <p18F14K22.h>

// PIC18F14K22 Configuration Bit Settings
// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = IRC       // Oscillator Selection bits (Internal RC oscillator)
#pragma config PLLEN = OFF      // 4 X PLL Enable bit (PLL is under software control)
#pragma config PCLKEN = ON      // Primary Clock Enable bit (Primary clock enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 19        // Brown Out Reset Voltage bits (VBOR set to 1.9 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up bit (HFINTOSC starts clocking the CPU without waiting for the oscillator to stablize.)
#pragma config MCLRE = OFF       // MCLR Pin Enable bit

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = OFF      // Boot Block Size Select bit (1kW boot block size)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block not protected from table reads executed in other blocks)

//---------------------------------------------------------------------
// Variable declarations
//---------------------------------------------------------------------

unsigned char inpbuf[8], buf_index = 0, udata, comcount = 0, data[20], com_parameter;

union int_char {
    int INT;

    struct {
        unsigned char L;
        unsigned char H;
    };
};

union int_char laser_power, temp_value;

volatile struct { // Holds status bits
    unsigned f0 : 1;
    unsigned step : 1;
    unsigned dir : 1;
    unsigned cw : 1;
    unsigned ccw : 1;
    unsigned f5 : 1;
    unsigned f6 : 1;
    unsigned f7 : 1;
} flag;


//---------------------------------------------------------------------´
// Interrupt Code
//---------------------------------------------------------------------

#pragma interruptlow LowISR //interruptLOW!!!!!!!!!!!!!!!!!!!!!

void LowISR(void) {
}

#pragma interrupt HighISR

void HighISR(void) {

    if (INTCONbits.INT0IF) {
        if (CW) {
            if (DIR) {
               temp_value.INT = laser_power.INT;
            } else {
                temp_value.INT = 0;
            }
            if (temp_value.INT < 0) temp_value.INT = 0;
            if (temp_value.INT > 4095) temp_value.INT = 4095;
            PORTC = temp_value.L;
            CSLSB = 0;
            CSLSB = 1;
            PORTC = temp_value.H;
            CSMSB = 0;
            CSMSB = 1;
        }
        INTCONbits.INT0IF = 0;
    }
}
/***********************************************************************/

#pragma code highVector=0x08

void HighVector(void) {
    _asm goto HighISR _endasm
}

#pragma code lowVector=0x18

void lowVector(void) {
    _asm GOTO LowISR _endasm
}

#pragma code /* return to the default code section */

//---------------------------------------------------------------------
//	Setup() initializes program variables and peripheral registers
//---------------------------------------------------------------------

void teszt(void) {
}

void Setup(void) {

    OSCCON = 0b01110000;
    //  OSCTUNE = 0b00000000;

    SLRCON = 0; // standard slewrate

    TRISA = 0b00001111;
    PORTA = 0;

    CSLSB = 1;
    CSMSB = 1;
    WPUB = 0;
    TRISB = 0b00100000;

    TRISC = 0;
    PORTC = 0;

    ANSEL = 0; //digital ports
    ANSELH = 0; //digital ports

    PIE1 = 0; //peripherial interrupt disabled
    PIE2 = 0; //peripherial interrupt disabled

    // PR2 = 116;
    //OpenTimer2(T2_PS_1_1 & T2_POST_1_1 & TIMER_INT_ON); //  int

    RCONbits.IPEN = 1; //int priority enabled
    //IPR1bits.TMR2IP = 1; // TMR2 priority: high
    //PIE1bits.TMR2IE = 1; // enable TMR2 int
    INTCONbits.GIEH = 1; //enable HIGH int
    INTCONbits.GIEL = 0; //disable LOW int
    INTCONbits.INT0IE = 1; //enable int0

    INTCON2bits.RABPU = 0; //RA,RB pullups enable
    INTCON2bits.INTEDG0 = 0;
    INTCON2bits.INTEDG1 = 0;
    INTCON2bits.INTEDG2 = 0;

    T0CON = 0; //TMR0 off

    laser_power.INT = 0;
    PORTC = laser_power.L;
    CSLSB = 0;
    CSLSB = 1;
    PORTC = laser_power.H;
    CSMSB = 0;
    CSMSB = 1;
    TTL = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB7 = 0;

}

//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

void main(void) {

    Setup();

    ConsoleInit();
    ConsolePutROMString("\r\n laser test\r\n");

    while (1) {

        /*
        if (flag.step != STEP) {
            //  ConsolePutROMString("\r\n STEP");
            flag.step = STEP;
        }
        if (flag.dir != DIR) {
            //ConsolePutROMString("\r\n DIR");

            //TTL=DIR;
            flag.dir = DIR;
        }
        if (flag.ccw != CCW) {

            if (flag.ccw) {
                laser_power.INT = 4000;
                PORTC = laser_power.L;
                CSLSB = 0;
                CSLSB = 1;
                PORTC = laser_power.H;
                CSMSB = 0;
                CSMSB = 1;

            }
            if (!flag.ccw) {
                laser_power.INT = 500;
                PORTC = laser_power.L;
                CSLSB = 0;
                CSLSB = 1;
                PORTC = laser_power.H;
                CSMSB = 0;
                CSMSB = 1;

            }


            //  TTL = EN;
            // ConsolePutROMString("\r\n CCW");
            flag.ccw = CCW;
        }
         */
        // if (laser_power.INT == 0) LED = 1;
        // else LED = 0;

        if (flag.cw != CW) {
            flag.cw = CW;
            if (!CW) {
                laser_power.INT = 0;
                PORTC = laser_power.L;
                CSLSB = 0;
                CSLSB = 1;
                PORTC = laser_power.H;
                CSMSB = 0;
                CSMSB = 1;
            }
            //ConsolePutROMString("\r\n CW");

        }
        if (PIR1bits.RCIF) cmd_interpreter();
    }

    /*************************************cmd intrtpreter******************************/
}

void cmd_interpreter(void) {

    switch (udata = RCREG) {

        case 'q':
            ConsolePutROMString("\r\n\n");
            ConsolePutROMString("Laser power: ");
            itoa(laser_power.INT, data);
            ConsolePutString(data);
            ConsolePutROMString("\r\n");

            break;

        case 'a':
            if (laser_power.INT < 0) laser_power.INT = 0;
            if (laser_power.INT > 4095) laser_power.INT = 4095;

            PORTC = laser_power.L;
            CSLSB = 0;
            CSLSB = 1;
            PORTC = laser_power.H;
            CSMSB = 0;
            CSMSB = 1;

            ConsolePutROMString("\r\n\n");
            ConsolePutROMString("Laser ON, power: ");
            itoa(laser_power.INT, data);
            ConsolePutString(data);
            ConsolePutROMString("\r\n");
            break;

        case 's':
            PORTC = 0;
            CSLSB = 0;
            CSLSB = 1;
            PORTC = 0;
            CSMSB = 0;
            CSMSB = 1;

            ConsolePutROMString("\r\n\n");
            ConsolePutROMString("Laser OFF\r\n");

            break;

        case ',': DoCommand(); // process the string
            // clear the input buffer
            inpbuf[0] = 0;
            inpbuf[1] = 0;
            inpbuf[2] = 0;
            inpbuf[3] = 0;
            inpbuf[4] = 0;
            inpbuf[5] = 0;
            inpbuf[6] = 0;
            inpbuf[7] = 0;

            buf_index = 0; // clear the buffer index
            
            ConsolePut(udata); // echo the character
            break;

        case 0x0d: DoCommand(); // process the string
            inpbuf[0] = 0;
            inpbuf[1] = 0;
            inpbuf[2] = 0;
            inpbuf[3] = 0;
            inpbuf[4] = 0;
            inpbuf[5] = 0;
            inpbuf[6] = 0;
            inpbuf[7] = 0;
            buf_index = 0; // clear the buffer index
            comcount = 0; // clear comma count
            ConsolePutROMString("\r\n return\r\n"); // put prompt to USART
            break;

        default: inpbuf[buf_index] = udata; // get received char
            buf_index++; // increment buffer index
            if (buf_index > 7) // If more than 8 chars
            { // received before getting
                ConsolePutROMString("\r\n tul sok\r\n"); // a <CR>, clear input
                inpbuf[0] = 0;
                inpbuf[1] = 0;
                inpbuf[2] = 0;
                inpbuf[3] = 0;
                inpbuf[4] = 0;
                inpbuf[5] = 0;
                inpbuf[6] = 0;
                inpbuf[7] = 0;
                buf_index = 0; // the buffer index
            } else ConsolePut(udata); // echo character
            break; //
    }
}

/***********************************************************************/

void DoCommand(void) {
    if (comcount == 0) // If this is the first parameter of the input
    { // command...
        switch (inpbuf[0]) {

            case 'p': com_parameter = 'p';
            comcount++; // increment comma count
            break;
                //case 'z': com_parameter = 'z';
                //  break;

            default:
                ConsolePutROMString("\r\n Hibás parancs.\r\n");
                comcount=0;
                break;
        }

    } else if (comcount == 1) // If this is the second parameter of the
    { // input command.
        switch (com_parameter) {

            case 'p':

                laser_power.INT = atoi(inpbuf); // Get value for
                if (laser_power.INT < 0) laser_power.INT = 0;
                if (laser_power.INT > 4095) laser_power.INT = 4095;

                ConsolePutROMString("\r\n\n");
                ConsolePutROMString("Laser power: ");
                itoa(laser_power.INT, data);
                ConsolePutString(data);
                ConsolePutROMString("\r\n");
                break;

                // case 'z':
                //   break;

            default: break;
        }
    } else;
}
