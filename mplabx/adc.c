#include "adc.h"

// Configuração do ADC
void ADC_Init() {
    // 1. Configurar os pinos analógicos
    TRISBbits.TRISB0 = 1;  // Configura o pino RB0 como entrada (AN1)
    AD1PCFGbits.PCFG0 = 0; // Configura o pino RB0 (AN1) como analógico
    
    TRISBbits.TRISB1 = 1;  // Configura o pino RB1 como entrada (AN1)
    AD1PCFGbits.PCFG1 = 0; // Configura o pino RB1 (AN1) como analógico

    // 2. Configurar o ADC
    AD1CON1bits.ON = 0;    // Desliga o ADC para configurar
    AD1CON1bits.FORM = 0;  // Formato do resultado: inteiro
    AD1CON1bits.SSRC = 7;  // Modo de conversão automático
    AD1CON1bits.ASAM = 0;  // Amostragem manual

    AD1CON2bits.VCFG = 0;  // Tensão de referência: AVDD e AVSS
    AD1CON2bits.CSCNA = 0; // Não usar varredura de canais
    AD1CON2bits.SMPI = 0;  // Interrupção após 1 conversão
    AD1CON2bits.BUFM = 0;  // Buffer em um único bloco
    AD1CON2bits.ALTS = 0;  // Usar sempre o mesmo canal

    AD1CON3bits.ADRC = 0;  // Usar clock do sistema
    AD1CON3bits.SAMC = 15; // Tempo de amostragem: 15 TAD
    AD1CON3bits.ADCS = 2;  // Clock de conversão: 2 * TPB

    AD1CHSbits.CH0NA = 0;  // Referência negativa: VREF-

    AD1CON1bits.ON = 1;    // Liga o ADC
}

// Função para ler o valor do ADC
unsigned int ADC_Read(int channel) {
    AD1CHSbits.CH0SA = channel;  // Seleciona o canal analógico
    AD1CON1bits.SAMP = 1;  // Inicia a amostragem
    while (!AD1CON1bits.DONE); // Espera a conversão terminar
    AD1CON1bits.DONE = 0;  // Limpa o flag de conversão concluída
    return ADC1BUF0;       // Retorna o valor convertido
}