 /*
 * File:   Final_Project_C.c
 * Author: Andres Coit, Nick Villarreal, Luke Holbrook
 *
 * Created on March 18, 2018, 3:44 PM
 */


#include <xc.h>
#include <pic16f1829.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "time.h"

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

//#define _XTAL_FREQ 4000000
#define BAUD 38400
#define FOSC 8000000L
//#define DIVIDER ((int)(_XTAL_FREQ/(16UL * BAUD) -1)) // Should be 25 for9600/4MhZ
#define DIVIDER 25
#define NINE_BITS 0
#define SPEED 0x4
#define RX_PIN TRISC5
#define TX_PIN TRISC4
#define _XTAL_FREQ 8000000

// BEGIN CONFIG: LCD Pinout
#define RS  RC0
#define RW  RC6
#define EN  RC3
#define D4  RB4
#define D5  RB5
#define D6  RB6
#define D7  RB7
// END CONFIG: LCD Pinout

// BEGIN CONFIG: Ping Sensor Pinout 
#define PS_TRIG RA0 // RB6
#define PS_P1 RA1
#define PS_P2 RA2

#define PORT_P1 'A'
#define POS_P1 1 // RB4
#define PORT_P2 'A'
#define POS_P2 2 // RB5
#define PORT_P3 'A'
#define POS_P3 3
    
void pinConfig(void);
int echo(char port, int pin);
void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void setTimeout(int milliseconds);
int ping(int a, int b);

/*
    Implementation simple timeout

    Input: count milliseconds as number

    Usage:
        setTimeout(1000) - timeout on 1 second
        setTimeout(10100) - timeout on 10 seconds and 100 milliseconds
 */
#define CLOSE_THRES 10
#define FAR_THRES 200

int main(int argc, char** argv)
{
    pinConfig();
    __delay_ms(30);
//Set up for Basic Clock, From Lab 10 LCD 

    OSCCON = 0X6A;
    ANSELC=0x20;

    //RA5 ^= 1;
    RW = 0;


    // input from user for time of delay in seconds
    int initialize = 0;
    int *pinitialize = &initialize;             // *initializes device reading
    int delay_counter = 0;
    int *pdelay_counter = &delay_counter;          // counts number of 30s iterations
    
    int toggle = 0;                 // toggles which PING_VAL to store PING value in
    //int ping_val1 = echo(PORT_P1, POS_P1);              // Holds value of PING 0 = error, 1 = car in spot, 2 = no car in spot
    //int ping_val2 = echo(PORT_P2,POS_P2);              // Holds value of PING 0 = error, 1 = car in spot, 2 = no car in spot
    
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("WELCOME TO UTSA");
    __delay_ms(1000);
    
    //RA5 = 1;
    // test comment
    while(1)
    {
        //RA5 = 1;
        RW = 0;
        int ping_val1 = echo(PORT_P1,POS_P1);              // Holds value of PING 0 = error, 1 = car in spot, 2 = no car in spot
        int ping_val2 = echo(PORT_P2,POS_P2);              // Holds value of PING 0 = error, 1 = car in spot, 2 = no car in spot

        char bufA[8];
        itoa(bufA,ping_val1,10);
        char bufB[8];
        itoa(bufB,ping_val2,10);

        //__delay_ms(100);
        
        //if(ping(CLOSE_THRES,FAR_THRES)==1)    // If there is a car in spot
        if(ping_val1 < 100 && ping_val2 < 100)
        {
            *pdelay_counter++;

            if (*pinitialize == 0)
                {
                
                    Lcd_Clear();
                    Lcd_Set_Cursor(1,1);
                    Lcd_Write_String("In range: ");
                    Lcd_Set_Cursor(2,1);
                    Lcd_Write_String(bufA);
                    Lcd_Write_String(" cm, ");
                    Lcd_Write_String(bufB);
                    Lcd_Write_String(" cm");
                    __delay_ms(500);
                     
                    *pinitialize = 1;
                    
                }
            
        }
        
        //else if(ping(CLOSE_THRES,FAR_THRES)==2)       // If there is no car in spot
        else if(ping_val1 > 100 && ping_val2 > 100)
        {
            *pdelay_counter = 0;
            *pinitialize = 0;
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Out of Range: ");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(bufA);
            Lcd_Write_String(" cm, ");
            Lcd_Write_String(bufB);
            Lcd_Write_String(" cm");
            __delay_ms(500);

        }
        
        else        // Any other PING case
        {
            RA5 = 0;
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Distance: ");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String(bufA);
            Lcd_Write_String(" cm, ");
            Lcd_Write_String(bufB);
            Lcd_Write_String(" cm");
            __delay_ms(500);
        }
        
//        if(toggle == 0)
//        {
//            ping_val1 = echo(PORT_P1,POS_P1);   
//        }
//        
//        else
//        {    
//            ping_val2 = echo(PORT_P2,POS_P2);
//        }
        
        toggle = ~toggle;
        
        if(*pdelay_counter > 50)
        {
            RA5 ^= 1;
            *pinitialize = 0;
        }
        
    }

    return 0;
}

int echo(char port, int pin) {
    char* sfrdev;
    switch(port){
        case 'A' :
            sfrdev = &PORTA;
            break;
        case 'B' :
            sfrdev = &PORTB;
            break;
        case 'C' :
            sfrdev = &PORTC;
    }
    
    int a;

    T1CON = 0x10; // 0b0001'0000          //Initialize Timer Module

    TMR1H = 0;                //Sets the Initial Value of Timer
    TMR1L = 0;                //Sets the Initial Value of Timer

    PS_TRIG = 1;                  //TRIGGER HIGH
    __delay_us(10);           //10uS Delay 
    PS_TRIG = 0;                  //TRIGGER LOW

    while( !(*sfrdev & (1<<pin)) );              //Waiting for Echo
    TMR1ON = 1;               //Timer Starts
    while( (*sfrdev & (1<<pin)) );               //Waiting for Echo goes LOW
    TMR1ON = 0;               //Timer Stops

    a = (TMR1L | (TMR1H<<8)); //Reads Timer Value
    a = (int)((float)a/58.82*2);              //Converts Time to Distance
    a = a + 1;                //Distance Calibration
        
    T1CON = 0x00; // stop timer module
    return a;
    
    //}
}

int ping(int a,int b) { // valid range
    int pingVal[3]; 
    pingVal[0] = echo(PORT_P1,POS_P1);
    pingVal[1] = echo(PORT_P2,POS_P2);
    pingVal[2] = echo(PORT_P3,POS_P3);
    int i;
    for(i=0;i<3;++i) {
        if(pingVal[i] < a) {
            pingVal[i] = 0;
        } else if(pingVal[i] < b) {
            pingVal[i] = 1;
        }else{
            pingVal[i] = 2;
        }
    }
    for(i=0;i<3;++i) 
        if (pingVal[i] == pingVal[(i+1)%3]) 
            return pingVal[i];    
}

void setTimeout(int milliseconds)
{
    // If milliseconds is less or equal to 0
    // will be simple return from function without throw error
	
    if (milliseconds <= 0) {
        //fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        //printf("Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    // a current time of milliseconds
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;

    // needed count milliseconds of return from this timeout
    int end = milliseconds_since + milliseconds;

    // wait while until needed time comes
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}

void pinConfig(void){
    TRISA = ~(1<<0)&(1<<1)|(1<<2); //RA<1:2> ECHO Inputs; RA0 Trig Output
    ANSELA = 0; // Digital I/O
    TRISB = (1<<4)|(1<<5)|(1<<6)|(1<<7);         //RB<4:7> as LCD D<4:7>
    ANSELB = 0; // Digital I/O
    TRISC = ~(1<<0)&~(1<<3)&~(1<<6)&~(1<<4)|(1<<5); // RC<0,3,6>: RS, RW, EN
                                                    // RC<4:5>: TX,RX 
    ANSELC = 0; // Digital I/O`
    OSCCON = 0x6A; /* b6..4 = 1101 = 4MHz */
    TXCKSEL = 1; // both bits in APFCON0 MUST BE 1 for 1829 0 for 1825
    RXDTSEL = 1; /* makes RC4 & 5 TX & RX for USART (Allows ICSP)*/
    //TRISC = 0x20; // 0b0010,0000/* set as output */
    ANSELC=0x20;
    //INTCON = 0; // purpose of disabling the interrupts.
    TRISA = ~(1<<0)&(1<<1)|(1<<2); //RA<1:2> ECHO Inputs; RA0 Trig Output
    ANSELA = 0; // Digital I/O
    TRISB = TRISB & ~( (1<<4)|(1<<5)|(1<<6)|(1<<7) );
    ANSELB = ANSELB & ~( (1<<4)|(1<<5)|(1<<6)|(1<<7) );
    TRISC = TRISC & ~( (1<<0)|(1<<3)|(1<<6) );
    ANSELC = ANSELC & ~( (1<<0)|(1<<3)|(1<<6) );
}
