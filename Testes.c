// Arquivo meio baguncado, fiz teste do PULLUP e da EEPROM

#include <EEPROM.h>
int valor1, valor2, valor3, valor4;
int armazena1 = 2;
int ende = 10;
int leitura;

void setup(){
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  Serial.begin(9600);
  //EEPROM.write(ende, armazena1);
}

void loop(){
  leitura = EEPROM.read(ende);
  Serial.println(leitura);
  valor1 = digitalRead(2);
  valor2 = digitalRead(3);
  valor3 = digitalRead(4);
  valor4 = digitalRead(5);
//  Serial.println(valor1);
//  Serial.println(valor2);
//  Serial.println(valor3);
//  Serial.println(valor4);
  if (valor1==0){
    armazena1 = 3;
    EEPROM.write(ende, armazena1);
  }
  Serial.println(" ");
  delay(500);
 