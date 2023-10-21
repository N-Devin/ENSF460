/* 
 * File:   main.c
 * Author:
 * Created on September 22, 2023, 5:27 PM
 */
#include <xc.h>

void main (void) {
    
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 1;
    
    TRISBbits.TRISB0 = 0; // sets RB0 as output
    TRISBbits.TRISB1 = 0; // sets RB1 as output
    TRISBbits.TRISB2 = 0; // sets RB2 as output
    TRISBbits.TRISB4 = 0; // sets RB4 as output
    TRISBbits.TRISB7 = 0; // sets RB7 as output
    TRISBbits.TRISB8 = 0; // sets RB8 as output
    TRISBbits.TRISB9 = 0; // sets RB9 as output
    TRISBbits.TRISB12 = 0; // sets RB12 as output
    TRISBbits.TRISB13 = 0; // sets RB13 as output
    
    while(1)
    {
        if ((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 0) && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 0))//0000
        {
           LATB = 0x0000;  
        }
        
        else if ((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 0) && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 1))//0001
        {
            LATB = 0x000f; 
        }
        
        else if ((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 0) && (PORTAbits.RA4 == 1) && (PORTAbits.RA5 == 0))//0010
        {
            LATB = 0x0000; 
        }        
        
        else if ((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 0) && (PORTAbits.RA4 == 1) && (PORTAbits.RA5 == 1))//0011
        {
            LATB = 0x0000; //0
        }   
        else if ((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 1) && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 0))//0100
        {
            LATB = 0x0001; //1
        }   
        else if ((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 1) && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 1))//0101
        {
            LATB = 0x01ff; //6
        }   
        else if ((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 1) && (PORTAbits.RA4 == 1) && (PORTAbits.RA5 == 0)) //0110
        {
            LATB = 0x0001; //1
        }  
        else if ((PORTAbits.RA0 == 0) && (PORTAbits.RA1 == 1) && (PORTAbits.RA4 == 1) && (PORTAbits.RA5 == 1)) //0111
        {
            LATB = 0x000f; //3
        }   
        else if ((PORTAbits.RA0 == 1) && (PORTAbits.RA1 == 0) && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 0)) //1000
        {
            LATB = 0x000f; //1
        }   
        else if ((PORTAbits.RA0 == 1) && (PORTAbits.RA1 == 0) && (PORTAbits.RA4 == 0) && (PORTAbits.RA5 == 1)) //1001
        {
            LATB = 0x07ff; //8
        }   
        else 
        {
            LATB = 0x0000; 
        }   
    }
return;
}