// Importa biblioteca para Display LCD
#include <Adafruit_LiquidCrystal.h>
// Declara endereço do Display LCD (0x20)
Adafruit_LiquidCrystal lcd_1(0);
// Inicializa portas
const int VOLTAGE = 13;
const int RESISTANCE = 12;
const int CURRENT = 8;
const int AMPLIFIER = 4;
const int BUTTON = 2;
// Define tensao maxima e modo inicial
const float MAX_VOLTAGE = 5.0; // tensao maxima: 5V
int mode = 0; // modo incial: voltimetro

void setup()
{
  // Inicializa Display LCD com 16 colunas e 2 linhas
  lcd_1.begin(16,2);
  // Define entradas e saidas
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(VOLTAGE, OUTPUT);
  pinMode(CURRENT, OUTPUT);
  pinMode(AMPLIFIER, OUTPUT);
  pinMode(RESISTANCE, OUTPUT);
  // Intercepta clique do botao
  attachInterrupt(digitalPinToInterrupt(BUTTON), changeMode, FALLING);
}

void loop()
{
  // Alterna niveis das saidas digitas conforme modo atual
  digitalWrite(VOLTAGE, mode == 0);
  digitalWrite(CURRENT, mode == 1);
  digitalWrite(AMPLIFIER, mode == 1);
  digitalWrite(RESISTANCE, mode == 2);
  // Executa funcao do modo atual
  run();
  // Delay e limpa tela
  delay(1000);
  lcd_1.clear();
}

void run() {
  switch (mode) {
    case 0:
      // Executa voltimetro 
      setVoltageMode();
      break;
    case 1:
      // Executa amperimetro
      setCurrentMode();
      break;
    case 2:
      // Executa ohmimetro
      setResistanceMode();
      break;
  }
}

void printd(float value, String unit) {
  // Exibe valor calculado e unidade de medida
  String message = String(value) + " " + unit;
  lcd_1.print(message);
}

void changeMode() {
  // Altera o modo atual
  mode = (mode + 1) % 3;
}

// TENSAO
void setVoltageMode() {
  // Escreve cabeçalho
  lcd_1.setCursor(0,0);
  lcd_1.print("Voltage");
  // Escreve tensao calculada
  lcd_1.setCursor(0,1);
  printd(getVoltage(), "V");
}

float getVoltage() {
  // Obtem a ddp entre a3 e a2
  const float a2 = analogRead(2);
  const float a3 = analogRead(3);
  return getVoltage(a3, a2);
}

float getVoltage(float pos, float neg) {
  // Calcula a tensao
  float voltage = (pos - neg) * (MAX_VOLTAGE / 1023);
  return voltage;
}

// CORRENTE
void setCurrentMode() {
  // Escreve cabeçalho
  lcd_1.setCursor(0,0);
  lcd_1.print("Current");
  // Escreve corrente calculada
  lcd_1.setCursor(0,1);
  printd(getCurrent(), "mA");
}

float getCurrent() {
  // Obtem a tensao amplificada
  const float a0 = analogRead(0);
  const float a1 = analogRead(1);
  float voltage = getVoltage(a1, a0);
  // Declara resistor shunt
  const float shunt = 1;
  // Calcula a corrente
  float current = voltage / shunt;
  // Retorna a corrente em mA
  return current;
}

// RESISTENCIA
void setResistanceMode() {
  // Escreve cabeçalho
  lcd_1.setCursor(0,0);
  lcd_1.print("Resistance");
  // Escreve resistencia calculada
  lcd_1.setCursor(0,1);
  printd(getResistance(), "kO");
}

float getResistance() {
  // Obtem a tensao
  float voltage = getVoltage();
  // Declara resistor shunt
  const float shunt = 1000;
  // Calcula a resistencia
  float resistance = voltage / (MAX_VOLTAGE - voltage) * shunt;
  // Retorna a resistencia em kOhms
  return resistance / 1000;
}
