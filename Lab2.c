/*
 * File:   Lab2.c
 * Author: Pablo
 *
 * Created on 29 de julio de 2022, 10:02 AM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h> 
#include <stdio.h>
#include "LCD8.h"
#include "adc.h"
#include "oscilador.h"
#include "tmr0.h"
#define _XTAL_FREQ 1000000
#define RS RC6
#define EN RC7
#define IN_MIN 0                
#define IN_MAX 1023 
#define OUT_MIN 0
#define OUT_MAX 500

/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
uint16_t VAR_POT = 0;
int UNI;
int DECI;
uint16_t VAR_POT2 = 0;
int UNI2;
int DECI2;
char VALORES1[10];
char VALORES2[10];
uint8_t CONT_TMR0 = 0;
uint8_t MOSTRAR = 0;
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);
unsigned short map(uint16_t  val, uint8_t  in_min, uint16_t in_max, 
            unsigned short out_min, unsigned short out_max);


void __interrupt() isr (void){
    if(INTCONbits.T0IF){
        CONT_TMR0++;                        // Incrementar CONT_TMR0 en 1
        if (CONT_TMR0 == 100){               // CONT_TMR0 se repite 10 veces 
            MOSTRAR = !MOSTRAR; // Encender/Apagar el pin RA0
            CONT_TMR0 = 0;                  // Limpiar CONT_TMR0
        }
        tmr0_reload();
    }
    if(PIR1bits.ADIF){                      
        if(ADCON0bits.CHS == 0){            
            VAR_POT = map(adc_read(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
            UNI = (VAR_POT/100);
            DECI = (VAR_POT - UNI*100);
            sprintf(VALORES1, "    %d.%dV", UNI, DECI);
        }
        else if (ADCON0bits.CHS == 1){ 
            VAR_POT2 = map(adc_read(), IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
            UNI2 = (VAR_POT2/100);
            DECI2 = (VAR_POT2 - UNI2*100);
            sprintf(VALORES2, "    %d.%dV", UNI2, DECI2);
        }
        PIR1bits.ADIF = 0;                  
    }
    return;
}


void main(void) {
    int_osc_MHz(1);
    setup();
    adc_init(0, 0, 0);
    tmr0_init(255);
    tmr0_reload();
    unsigned int a;
    Lcd8_Init();
    while(1)
    {
        if (ADCON0bits.GO == 0){
            if(ADCON0bits.CHS == 0){        
                ADCON0bits.CHS = 0b0001;    
            }
            else if (ADCON0bits.CHS == 1){  
                ADCON0bits.CHS = 0b0000;           
            }
             __delay_us(40);                
            ADCON0bits.GO = 1;              
        }
        switch(MOSTRAR){
            case 0:                     //
                
                Lcd8_Set_Cursor(1,1);
                Lcd8_Write_String(" Potenciometro 1");
                Lcd8_Set_Cursor(2,1);
                Lcd8_Write_String(VALORES1);
                break;
            case 1:                     // 
                Lcd8_Set_Cursor(1,1);
                Lcd8_Write_String(" Potenciometro 2");
                Lcd8_Set_Cursor(2,1);
                Lcd8_Write_String(VALORES2);
                
                break;
        }
        __delay_ms(100);

        
    }
    return;
}


void setup(void){       
    // Configuración de puertos
    ANSEL = 0b00000011;         
    ANSELH = 0b00000000;        
    TRISA = 0b00000011;         
    TRISC = 0b00000000;
   
    
   // Configuración de interrupciones
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitar interrupciones de perifericos
    return;
}

/*---
/*------------------------------------------------------------------------------
 * FUNCIONES
 ------------------------------------------------------------------------------*/
unsigned short map(uint16_t x, uint8_t x0, uint16_t x1, 
            unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));
}