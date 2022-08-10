#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)
// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f1827.h>
int ADCLOW=0, ADCHI=0, ADCVal;
int count=0, flag=0,ENABLE1=1;
int unit, dec, cent, Seg=0;
int ascii=0;
int asc0, asc1, asc2, asc3;
int count1=0,flag1=0;
void segment(void);
void conteo(void);
void adc(void);
void uart(void);
void main(void) {
    OSCCON=0x78;//reloj de 16Mhz
    OSCTUNE=0x00;//0000000
    T1CON = 0x60; //Timer1, prescaler 1:4, reloj interno
    TRISA = 0x21;//Definicion de entradas y salidas en puertos RA
    TRISB = 0xc0;//Definicion de entradas y salidas en puertos RB
    ANSELA = 0x01;//Definicion del puerto RA0 como entrada analoga (IN)
    ANSELB = 0x00;//Definicion de los puertos RB como entradas y salidas digitales
    APFCON1 = 0x00; // Puerto RB2 como TX
    PIE1 = 0x00;//Habilita interrupciones del ADC y UART
    INTCON = 0x00;//Habilita interrupciones globales y perifericas
    ADCON0 = 0x01;//Configuracion operacion ADC, AN0, conversion no en progreso, ADC habilitado
    ADCON1 = 0xA0;//Configuracion operacion ADC, justificado a la derecha, Fosc/32, Vref- a vss, vref+ a vdd
    PIR1 = 0x00;//Bandera de interrupcion ADC desactivada
    
    while(1){
        conteo(); //llama funion conteo
        adc(); //llama funcion adc
        unit=(ADCVal/100); //toma el valir de la unidad
        dec=((ADCVal%100)/10);//toma el valor de la decena
        cent=(ADCVal%10);//toma el valor de la centena
        segment();//llama funcion segmento
        uart();//llama funcion de la transmision uart

        }
    } 
    void conteo(){
        if (TMR1 >=1000)//antes de timer1 contar 1000 entra al if
        {
            count=count+1;//cuenta hasta 1000 (1segundo)
            flag=1;//enable del seg
            flag1=flag1+1;//bandera de la transmision
        }
    }
    void adc(){
    
       if(count>=800){
           ADCON0bits.GO = 1;//inicializa conversion ADC
            count=0;
            ENABLE1=1;
            
       }
       if(PIR1bits.ADIF && ENABLE1){
         ADCLOW = ADRESL; 
         ADCHI = ADRESH;
         ADCVal=0;
         ADCVal = (ADCHI << 8) | ADCLOW;//(ADCHI*256)+ADCLOW;
         ADCVal=0.9796*ADCVal-0.0156;//regresion
        } 
         
  
 } 
void segment (){
    
        if(flag==1) //tiempo de 1ms por segmento
     { 
        flag=0;
         switch (Seg)
         {
         case 0: //display 1 
             RA1=1;
             RB5=0;
             RB4=0;
             RB1=1;
             numero(unit);
             Seg=Seg+1;
             break;
         case 1: //display 2
             RA1=0;
             RB5=1;
             RB4=0; 
             RB1=0;
             numero(dec);
             Seg=Seg+1;
             break;
         case 2: //display 3
             RA1=0;
             RB5=0;
             RB4=1;
             RB1=0;
             numero(cent);
             Seg=0;
             break; 
        }
    }
}
int numero(int num){
   
   switch (num)
     {
         case 0:
             RA7=1;//segmento A
             RB3=1;//segmento B
             RB0=1;//segmento C
	         RA3=1;//segmento D
             RA2=1;//segmento E
             RA6=1;//segmento F
             RA4=0;//segmento G
             break;
         case 1:
             RA7=0;//segmento A
             RB3=1;//segmento B
             RB0=1;//segmento C
	         RA3=0;//segmento D
             RA2=0;//segmento E
             RA6=0;//segmento F
             RA4=0;//segmento G
             break;
         case 2:
             RA7=1;//segmento A
             RB3=1;//segmento B
             RB0=0;//segmento C
	         RA3=1;//segmento D
             RA2=1;//segmento E
             RA6=0;//segmento F
             RA4=1;//segmento G
             break; 
         case 3:
             RA7=1;//segmento A
             RB3=1;//segmento B
             RB0=1;//segmento C
	         RA3=1;//segmento D
             RA2=0;//segmento E
             RA6=0;//segmento F
             RA4=1;//segmento G
             break;
         case 4:
             RA7=0;//segmento A
             RB3=1;//segmento B
             RB0=1;//segmento C
	         RA3=0;//segmento D
             RA2=0;//segmento E
             RA6=1;//segmento F
             RA4=1;//segmento G
             break;
         case 5:
             RA7=1;//segmento A
             RB3=0;//segmento B
             RB0=1;//segmento C
	         RA3=1;//segmento D
             RA2=0;//segmento E
             RA6=1;//segmento F
             RA4=1;//segmento G
             break;
         case 6:
             RA7=1;//segmento A
             RB3=0;//segmento B
             RB0=1;//segmento C
	         RA3=1;//segmento D
             RA2=1;//segmento E
             RA6=1;//segmento F
             RA4=1;//segmento G
             break;
         case 7:
             RA7=1;//segmento A
             RB3=1;//segmento B
             RB0=1;//segmento C
	         RA3=0;//segmento D
             RA2=0;//segmento E
             RA6=0;//segmento F
             RA4=0;//segmento G
             break;
         case 8:
             RA7=1;//segmento A
             RB3=1;//segmento B
             RB0=1;//segmento C
	         RA3=1;//segmento D
             RA2=1;//segmento E
             RA6=1;//segmento F
             RA4=1;//segmento G
             break;
         case 9:
             RA7=1;//segmento A
             RB3=1;//segmento B
             RB0=1;//segmento C
	         RA3=0;//segmento D
             RA2=0;//segmento E
             RA6=1;//segmento F
             RA4=1;//segmento G
             break;
        }
    }
int asc(int dig){
   switch (dig)
     {
         case 0:
             ascii=0x30;//0
             break;
         case 1:
             ascii=0x31;//1
             break;
         case 2:
             ascii=0x32;//2
             break; 
         case 3:
             ascii=0x33;//3
             break;
         case 4:
             ascii=0x34;//4
             break;
         case 5:
             ascii=0x35;//5
             break;
         case 6:
             ascii=0x36;//6
             break;
         case 7:
             ascii=0x37;//7
             break;
         case 8:
             ascii=0x38;//8
             break;
         case 9:
             ascii=0x39;//9
             break;   
     }
   return ascii;
}
void uart(){

     asc0 = asc(unit);//unidades
     asc1 = asc(10);//
     asc2 = asc(dec);//decenas
     asc3 = asc(cent);//centenas
    // Registros de comunicacion serial
    TXSTA = 0x22;
    BAUDCON = 0x40;
    RCSTA = 0x80;
    SPBRG = 25; //2400 baudios
    //Comunicacion Serial
     if(PIR1bits.TXIF && flag1>=4)
     { 
         flag1=0;
        switch (count1)
     {
         case 0:
             TXREG = asc0; //Trasmision unidad
             PIR1bits.TXIF = 0;
             count1=count1+1;
             break;
         case 1:
             TXREG = 0x2E;//Trasmision punto
             PIR1bits.TXIF = 0;
             count1=count1+1;
             break;
         case 2: 
             TXREG = asc2;//Trasmision decena
             PIR1bits.TXIF = 0;
             count1=count1+1;
             break; 
         case 3:
             TXREG = asc3;//Trasmision centena
             PIR1bits.TXIF = 0;
             count1=count1+1;
             break;
         case 4:
             TXREG = 0x0A;//Trasmision punto
             PIR1bits.TXIF = 0;
             count1=count1+1;
             break;
         case 5:
             count1=0;
             break;
     }
     }
}