#ifndef  _CONSOLE_H_
#define  _CONSOLE_H_

/************************ HEADERS **********************************/
//#include "define.h"

#define BAUD_RATE 9600 //19200		//9600
#define ConsoleIsGetReady()     (PIR1bits.RCIF)
#define ConsoleIsPutReady()     (TXSTAbits.TRMT)
/************************ DEFINITIONS ******************************/
/************************ FUNCTION PROTOTYPES **********************/

        void ConsoleInit(void);
        void ConsolePut(unsigned char c);
        void ConsolePutString(unsigned char *s);
        void ConsolePutROMString(const rom char *str);
        unsigned char ConsoleGet(void);
//        unsigned char ConsoleGetString(char *buffer, unsigned char bufferLen);
        void PrintChar(unsigned char);
        void PrintDec(unsigned char);

#define printf(x) ConsolePutROMString((ROM char*)x)
#endif


