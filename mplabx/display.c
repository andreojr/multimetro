#include "display.h"

// Configurações do PIC32
#pragma config FNOSC = FRCPLL   // Usar oscilador interno
#pragma config FPLLIDIV = DIV_2 // Dividir clock por 2
#pragma config FPLLMUL = MUL_20 // Multiplicar clock por 20
#pragma config FPLLODIV = DIV_1 // Dividir clock por 1

void __delay_ms(uint32_t ms)
{
    uint32_t Start=_CP0_GET_COUNT();
    uint32_t Duration=TICK_PER_MS*ms;
    while((_CP0_GET_COUNT()-Start)<Duration);
}

// Códigos para os dígitos do display
const unsigned char DIGIT_CODE[] = {
    0b01111111, // 1º dígito
    0b10111111, // 2º dígito
    0b11011111, // 3º dígito
    0b11101111, // 4º Sinal
    0b11111111, // Nenhum dígito
};

// Códigos para os segmentos do display (0-9)
const unsigned char SEGMENT_CODE[] = {
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000, // 9
    0b10111111, // -
};

// Função para exibir um número em um dígito específico
void Turn_Off_Display() {
    DIGIT_PORT = DIGIT_CODE[4];  // Seleciona o dígito
}

// Função para exibir um número em um dígito específico
void Display_Number(int digit, int number, int isFloat) {
    DIGIT_PORT = DIGIT_CODE[digit];  // Seleciona o dígito
    if (isFloat && digit == 2) {
        SEGMENT_PORT = SEGMENT_CODE[number] - 0b10000000; // Exibe o número sem ponto decimal
    } else {
        SEGMENT_PORT = SEGMENT_CODE[number];
    }
}

// Função para separar a parte inteira e as casas decimais de um float
void Separate_Float(float result, int *int_part, int *st_dec, int *nd_dec, int *negative) {
    *negative = result < 0;
    result = fabs(result);
    *int_part = (int)floor(result);
    float decimal_part = result - *int_part;
    *st_dec = (int)floor(decimal_part * 10);
    *nd_dec = (int)floor((decimal_part * 100) + 0.5) % 10;
}

// Função para separar os dígitos de um número inteiro (0 a 9999)
void Separate_Integer(int number, int *thousands, int *hundreds, int *tens, int *units) {
    *thousands = number / 1000;        // Dígito dos milhares
    *hundreds = (number % 1000) / 100; // Dígito das centenas
    *tens = (number % 100) / 10;       // Dígito das dezenas
    *units = number % 10;              // Dígito das unidades
}

// Função para exibir o resultado no display
void Display_Float_Result(float result) {
    int int_part, st_dec, nd_dec, negative;
    
    Separate_Float(result, &int_part, &st_dec, &nd_dec, &negative);

    // Limita a parte inteira a um dígito
    if (int_part > 9)
        int_part = 9;

    // Exibe cada dígito em um loop rápido (multiplexação)
    for (int i = 0; i < 50; i++) { // Atualiza o display 50 vezes
        if (negative)
            Display_Number(3, 10, 0); // Exibe sinal negativo
        __delay_ms(1);
        Display_Number(2, int_part, 1); // Exibe o dígito mais à esquerda
        __delay_ms(1); // Delay curto para multiplexação
        Display_Number(1, st_dec, 1); // Exibe o dígito mais à esquerda
        __delay_ms(1); // Delay curto para multiplexação
        Display_Number(0, nd_dec, 1); // Exibe o dígito mais à esquerda
        __delay_ms(1); // Delay curto para multiplexação
    }
}

void Display_Integer_Result(float result) {
    int thousands, hundreds, tens, units;
    
    // Separa os dígitos
    Separate_Integer((int)result, &thousands, &hundreds, &tens, &units);

    // Exibe cada dígito em um loop rápido (multiplexação)
    for (int i = 0; i < 50; i++) { // Atualiza o display 50 vezes
        Display_Number(3, thousands, 0); // Exibe o dígito dos milhares
        __delay_ms(1); // Delay curto para multiplexação
        Display_Number(2, hundreds, 0);  // Exibe o dígito das centenas
        __delay_ms(1); // Delay curto para multiplexação
        Display_Number(1, tens, 0);      // Exibe o dígito das dezenas
        __delay_ms(1); // Delay curto para multiplexação
        Display_Number(0, units, 0);     // Exibe o dígito das unidades
        __delay_ms(1); // Delay curto para multiplexação
    }
}