#include "adc.h"
#include "display.h"

float getVoltage(int adc_diff) {
    return (adc_diff / 1023.0) * 3.2;
}

void voltageMode(float voltage) {
    Display_Float_Result(voltage);
}

void resistanceMode(float voltage) {
    float resistance = voltage / (3.2 - voltage) * 100;
    Display_Integer_Result(resistance);
}

void changeMode(int *mode) {
    *mode = (*mode + 1) % 3;
}

int main() {
    TRISE = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    TRISFbits.TRISF0 = 0;
    TRISFbits.TRISF1 = 0;
    TRISFbits.TRISF2 = 0;
    TRISAbits.TRISA3 = 1;
    
    unsigned int adc_value0, adc_value1;
    int adc_diff;
    int mode = 0;

    // Inicializa o ADC
    ADC_Init();

    while (1) {
        // Lê o valor do ADC
        adc_value0 = ADC_Read(0);
        adc_value1 = ADC_Read(1);
        
        if (!PORTAbits.RA3) {
            changeMode(&mode);
        }
        
        adc_diff = adc_value1 - adc_value0;
        float voltage = getVoltage(adc_diff);
        switch (mode) {
            case 0:
                voltageMode(voltage);
                break;
            case 1:
                resistanceMode(voltage);
                break;
            case 2:
                Display_Integer_Result(9999);
                break;
        }
        
        PORTFbits.RF0 = mode == 0;
        PORTFbits.RF1 = mode == 1;
        PORTFbits.RF2 = mode == 2;
    }

    return 0;
}