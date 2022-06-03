#define TRIAC_PORT TRISIObits.TRISIO1
#define TRIAC_PIN GPIObits.GP1

#define ADC_PORT TRISIObits.TRISIO4


#define DATA_PORT TRISIObits.TRISIO2
#define DATA_PIN GPIObits.GP2

#define ENABLE 1
#define DISABLE 0

#define FALLING 0

#define OUTPUT 0
#define INPUT 1

#define HIGH 1
#define LOW 0

void GP0_GP1_as_io()
{
    ANSEL = 0x00;
    ANSEL = 0x00;
    ADCON0 = 0x00;
    CMCON = 0x0F; 
    VRCON = 0x00;
}

void init(){
    
    TRIAC_PORT = OUTPUT;
    TRIAC_PIN = LOW;
}


