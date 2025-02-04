#ifndef DISPLAY_H
#define DISPLAY_H

#include <xc.h>
#include <math.h>

// Definições de constantes
#define SEGMENT_PORT LATE
#define DIGIT_PORT LATB
#define _XTAL_FREQ 80000000
#define TICK_PER_MS (_XTAL_FREQ/2/1000)

// Protótipos das funções
void __delay_ms(uint32_t ms);
void Turn_Off_Display();
void Display_Number(int digit, int number, int isFloat);
void Separate_Float(float result, int *int_part, int *st_dec, int *nd_dec, int *negative);
void Separate_Integer(int number, int *thousands, int *hundreds, int *tens, int *units);
void Display_Integer_Result(float result);
void Display_Float_Result(float result);

#endif // DISPLAY_H