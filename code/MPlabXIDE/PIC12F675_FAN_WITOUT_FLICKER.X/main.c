/*
 * File:   main.c
 * Author: aswin
 *
 * Created on 21 May, 2022, 11:55 AM
 */


#include <xc.h>
#include "config.h"
#include "pinmanager.h"
#include "peripherals.h"

unsigned int ADC_AVG;

void enable_interrupt(){
    INTERRUPT_EDGE = FALLING; 
    GLOBAL_INTERRUPT = ENABLE;
    EXTERNAL_INTERRUPT = ENABLE;
}

void disable_interrupt(){
    GLOBAL_INTERRUPT = DISABLE;
    EXTERNAL_INTERRUPT = DISABLE;
}

void ADCInit()
{
   ADCON0bits.ADFM = 0; // Right Hustified bit 7
   ADCON0bits.VCFG = 0; // Voltage Refereance VDD bit 6
   ADCON0bits.CHS1 = 1; //set AN3 as input
   ADCON0bits.CHS0 = 1;//set AN3 as input
   ADCON0bits.ADON = 1;
   ADC_PORT = INPUT;
   ANSELbits.ANS3 = 1;
}

void __custom_delay(unsigned int delay_data){
    
    for(int i =0; i < delay_data; i++)
    {
        __delay_us(1);
    }
}

unsigned int GetAdcvalue()
{
    ADCON0bits.CHS = 0b10; //select analog input, AN3 -- WILL CHANGE LATER TO AN1
    ADCON0bits.GO = 1; //start the conversion
    __delay_us(200);
    while(ADCON0bits.GO==1){}; //wait for the conversion to end
    return ADRESH; //take the most significant bytes
}


void __interrupt() my_isr(){
    
    if(EXTERNAL_INTERRUPT_FLAG)
    {
        disable_interrupt();
        EXTERNAL_INTERRUPT_FLAG =0;
        
        for(int j = 0; j<10; j++)
        {
            ADC_AVG = ADC_AVG + GetAdcvalue();
            __delay_us(50);
        }
        ADC_AVG = ADC_AVG / 10;
        
        __custom_delay(ADC_AVG);
        TRIAC_PIN = HIGH;
        __delay_us(50);
        TRIAC_PIN = LOW;
        enable_interrupt();
   }
}

void main(void) {
    
    GP0_GP1_as_io();
    init();
    enable_interrupt();
    ADCInit();
    
  while(1)
    {
       
    }
    
    return;
}
