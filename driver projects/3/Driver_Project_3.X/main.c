/*
 * File:   main.c
 * Author: Braden Julia Nimna
 *
 * Created on September 25, 2023, 2:52 PM
 */



// LINKED .h SOURCE FILES HERE
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>


//// CONFIGURATION BITS - PRE-PROCESSOR DIRECTIVES ////

// Code protection 
#pragma config BSS = OFF // Boot segment code protect disabled
#pragma config BWRP = OFF // Boot sengment flash write protection off
#pragma config GCP = OFF // general segment code protecion off
#pragma config GWRP = OFF

// CLOCK CONTROL 
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO. 
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled

// WDT
#pragma config FWDTEN = OFF // WDT is off
#pragma config WINDIS = OFF // STANDARD WDT/. Applicable if WDT is on
#pragma config FWPSA = PR32 // WDT is selected uses prescaler of 32
#pragma config WDTPS = PS1 // WDT postscler is 1 if WDT selected

//MCLR/RA5 CONTROL
#pragma config MCLRE = OFF // RA5 pin configured as input, MCLR reset on RA5 diabled

//BOR  - FPOR Register
#pragma config BORV = LPBOR // LPBOR value=2V is BOR enabled
#pragma config BOREN = BOR0 // BOR controlled using SBOREN bit
#pragma config PWRTEN = OFF // Powerup timer disabled
#pragma config I2C1SEL = PRI // Default location for SCL1/SDA1 pin

//JTAG FICD Register
#pragma config BKBUG = OFF // Background Debugger functions disabled
#pragma config ICS = PGx2 // PGC2 (pin2) & PGD2 (pin3) are used to connect PICKIT3 debugger

// Deep Sleep RTCC WDT
#pragma config DSWDTEN = OFF // Deep Sleep WDT is disabled
#pragma config DSBOREN = OFF // Deep Sleep BOR is disabled
#pragma config RTCOSC = LPRC// RTCC uses LPRC 32kHz for clock
#pragma config DSWDTOSC = LPRC // DeepSleep WDT uses Lo Power RC clk
#pragma config DSWDTPS = DSWDTPS7 // DSWDT postscaler set to 32768 


// GLOBAL VARIABLES



// MACROS
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   // set sleep mode
#define Idle() {__asm__ volatile ("pwrsav #1");}
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}


#define STATE_IDLE 0
#define STATE_PB1 1
#define STATE_PB2 2
#define STATE_PB3 3
#define STATE_PB1_PB2 4

int currentState = STATE_IDLE;
int buttonPressed = 0;

void IOinit(){ 
    // initializes the IO ports and placed in IOs.c
    TRISBbits.TRISB8 = 0; // RB8 as output (LED)
    TRISAbits.TRISA2 = 1; // RA2 as input (PB1)
    TRISAbits.TRISA4 = 1; // RA4 as input (PB2)
    TRISBbits.TRISB4 = 1; // RB4 as input (PB3)
    
    return 0;
}

void IOcheck(){
    //implements the IO checks and 
    //LED blinking functions and placed in IOs.c
     // Check button states
    int pb1 = PORTAbits.RA2; // PB1 state
    int pb2 = PORTAbits.RA4; // PB2 state
    int pb3 = PORTBbits.RB4; // PB3 state

    // Determine the current state
    if (pb1 && !pb2 && !pb3) {
        currentState = STATE_PB1;
    } else if (!pb1 && pb2 && !pb3) {
        currentState = STATE_PB2;
    } else if (!pb1 && !pb2 && pb3) {
        currentState = STATE_PB3;
    } else if (pb1 && pb2 && !pb3) {
        currentState = STATE_PB1_PB2;
    } else {
        currentState = STATE_IDLE;
    }

    // Implement LED control based on the current state
    switch (currentState) {
        case STATE_IDLE:
            LATBbits.LATB8 = 0; // Turn off the LED
            break;

        case STATE_PB1:
            // Implement LED blinking logic with delay_ms(1000)
            LATBbits.LATB8 = !LATBbits.LATB8; // Toggle the LED state
            delay_ms(1000);
            break;

        case STATE_PB2:
            // Implement LED blinking logic with delay_ms(2000)
            LATBbits.LATB8 = !LATBbits.LATB8; // Toggle the LED state
            delay_ms(2000);
            break;

        case STATE_PB3:
            // Implement LED blinking logic with delay_ms(3000)
            LATBbits.LATB8 = !LATBbits.LATB8; // Toggle the LED state
            delay_ms(3000);
            break;

        case STATE_PB1_PB2:
            // Implement LED blinking logic with delay_ms(1)
            LATBbits.LATB8 = !LATBbits.LATB8; // Toggle the LED state
            delay_ms(1);
            break;
    }
}
   
void delay_ms(int time_ms) {
    //implements the delay functions used to time the LED blinks.
    //time_ms is the user specified time delay in milliseconds.
    T1CON = 0x8030; // Timer1, Tcy/2, 1:256 prescale
    PR1 = time_ms * 1000; // Set the period for the delay
    TMR1 = 0; // Reset the timer
    IFS0bits.T1IF = 0; // Clear the interrupt flag
    T1CONbits.TON = 1; // Start the timer

    // Wait until the timer overflows
    while (!IFS0bits.T1IF);
    T1CONbits.TON = 0; // Stop the timer
    IFS0bits.T1IF = 0; // Clear the interrupt flag

    
}




        
int main(void) {
    
     //Clock output on REFO pin - pin 18 RB15
     TRISBbits.TRISB15 = 0;  // Set RB15 as output for REFO
     REFOCONbits.ROEN = 1; // Ref oscillator is enabled
     REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep
     REFOCONbits.ROSEL = 0; // Output base clk showing clock switching
     REFOCONbits.RODIV = 0b0000;
     
     //IO Inititalizations
     AD1PCFG = 0xFFFF;  // Turn all analog pins to digital
     CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
     CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RB4/CN1
     CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA2/CN30
    
     IOinit();
    while(1)  // infinite while loop
     {
        
        IOcheck();

                 
     }
    return 0;
}
