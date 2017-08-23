// Programa : Display LCD 16x2 com modulo I2C e 4 botoes para menu interativo
// Criado por Claudio Regis em 21 Aug 21:40
// Revisado por: Claudio Regis em 21 Aug 21:40

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Primeiramente executa o i2cscanner e identifique o endereco do display
#define LCDEnd 0x3F
#define DS1307_ADDRESS 0x68

byte zero = 0x00; 

// Inicializa o display no endereco 0x3F
LiquidCrystal_I2C lcd(LCDEnd,2,1,0,4,5,6,7,3, POSITIVE);
int botoes[4] = {2,3,4,5};
int valor_botoes[4];

// Declaracao de funcoes
void print_data();
void le_botoes();
byte ConverteparaDecimal(byte val)
void Mostrarelogio_lcd()
// Fim declaracao de funcoes
 
void setup(){
  Wire.begin();
  lcd.begin(16,2);
  lcd.setBacklight(HIGH);
 }
 
void loop(){
  print_data();
  delay(1000);
}


void print_data(){
  lcd.setCursor(0,0);
  lcd.print("Ola Mundo");
}

void le_botoes(){
  for (int i=0; i<4; i++){
    valor_botoes[i] = digitalRead(botoes[i]);
  }
}

byte ConverteparaDecimal(byte val){ //Converte de BCD para decimal
  return ( (val/16*10) + (val%16) );
}

void Mostrarelogio_lcd(){
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  int segundos = ConverteparaDecimal(Wire.read());
  int minutos = ConverteparaDecimal(Wire.read());
  int horas = ConverteparaDecimal(Wire.read() & 0b111111); 
  int diadasemana = ConverteparaDecimal(Wire.read()); 
  int diadomes = ConverteparaDecimal(Wire.read());
  int mes = ConverteparaDecimal(Wire.read());
  int ano = ConverteparaDecimal(Wire.read());
  //Mostra a data no LCD
  lcd.setCursor(0,0);
  lcd.print(horas);
  lcd.setCursor(2,0);
  lcd.print(":");
  lcd.setCursor(3,0);
  lcd.print(minutos);
  lcd.setCursor(5,0);
  lcd.print(":");
  lcd.setCursor(6,0);
  lcd.print(segundos);

  lcd.setCursor(11,0);
  lcd.print(diadomes);
  lcd.setCursor(13,0);
  lcd.print("/");
  lcd.setCursor(14,0);
  lcd.print(mes);
}