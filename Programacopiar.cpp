
#include <string.h>
#include <Wire.h>
#include <DS1307.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
char dateTime[20];
int RTCValues[7];
char acertahora[8];

int programas[21];
LiquidCrystal_I2C lcd(0x20,20,4);
const int analogS = 0; // Sensor connected to Analog 0
const int relay1 =12; // Led connected at pin 13 ( Dry ON- Wet OFF)
const int led2= 13; // Led connectected at pin 12(Dry OFF - Wet ON)
const int relay2 = 11; // Realay connected at pin 8

boolean up = 8;
boolean sete = 9;
boolean dow = 10;

int i = 0;
int p = 0;
byte z = 0;
byte h = 0;
byte db = 0;
boolean m = 0;
boolean o = 0;
int d = 0;
int d1 = 0;
byte menu = 0;
byte program =0;
int humidad = 0;
int reading = 0;
byte estouro = 0;
int deslbomba = 0;
int deslbomba2 = 0;

void setup() {
  
  Serial.begin(9600);// initialize serial communications at 9600 bps
  DS1307.begin();
  lcd.init();
  lcd.backlight();
  
   Timer1.initialize(500000);
   Timer1.attachInterrupt(interrup);
  
  pinMode(relay1, OUTPUT);// declare the ledPin as an OUTPUT:
  pinMode(relay2, OUTPUT); // Declare relay as Output
  pinMode(led2, OUTPUT); //  
  pinMode(up, INPUT);
  digitalWrite(up, HIGH);
  // As duas linhas acima podem ser trocadas pelo comando abaixo
  //pinMode(up, INPUT_PULLUP); // Lembrar que nessa configuracao a entrada funciona com logica inversa (High-sensor desligado, LOW-sensor ligado)
  pinMode(dow, INPUT);
  digitalWrite(dow, HIGH);
  pinMode(sete, INPUT);
  digitalWrite(sete, HIGH);
  digitalWrite(relay1, LOW); // Led off at start up
  
  // Temos 1024 bytes na EEPROM do ATMEGA328P
  humidad = EEPROM.read(50);
  o = EEPROM.read(51);
  for (int x=0; x<10; x++){
  	programas[x] = EEPROM.read(x);
  }
  
  *((char*)&deslbomba) = EEPROM.read(52);
  *((char*)&deslbomba + 1) = EEPROM.read(53);
  
  
}

void interrup()
{

  d1 = d1++;
  if(!menu == 0){
  	estouro = estouro++;
  }
  if(!p == 0){
  	estouro = estouro++;
  }
  if(!db == 0){
  	estouro = estouro++;
  }
  if(!h == 0){
  	estouro = estouro++;
  }
  if(!i == 0){
  	estouro = estouro++;
  }
  if(m == 1){
  	estouro = estouro++;
  }
  if(o == 1 && digitalRead(relay2)==HIGH && (reading > humidad)){
  	deslbomba2 = deslbomba2++;
  }
  if(o == 0 && digitalRead(relay2)==HIGH && reading >= humidad){
  	deslbomba2 = deslbomba2++;
  }
  // ou seja a bomba vai ligar ate atingir a humidade necessaria, quando a humidade for atingida ela conta um tempo que é o delay off bomba e depois desliga.
}

void acertartc()// COMANDO PARA ACERTAR A DATA E A HORA.
{
 wdt_disable(); // DESABILITA A FUNÇÃO WDT NESTA PARTE DO PROGRAMA.
 delay(1000);

     while( i> 0 && i<8)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          if(i==1){
          	lcd.print(" Entre com Ano  ");
          }
          if(i==2){
          	lcd.print(" Entre com Mes  ");
          }
          if(i==3){
          	lcd.print(" Entre com Dia  ");
          }
          if(i==4){
          	lcd.print(" Entre cm Semana");
          }
          if(i==5){
          	lcd.print(" Entre com Hora ");
          }
          if(i==6){
          	lcd.print(" Entre com Minut");
          }
          if(i==7){
          	lcd.print(" Entre com Segun");
          }
          if(digitalRead(up) == LOW){
          	acertahora[i]++;
          	estouro = 0;
          }
          if(digitalRead(dow) == LOW && acertahora[i] >= 0){
          	acertahora[i]--;
          	estouro = 0;
          }
          if(digitalRead(sete) == LOW){
          	i ++;
          	estouro = 0;
          }
          if(acertahora[1] < 1 ){
          	acertahora[1] = 13;
          }
          if(acertahora[2] > 12){
          	acertahora[2] = 1;
          }
          if(acertahora[2] == 0){
          	acertahora[2] = 12;
          }
          if(acertahora[3] > 31){
          	acertahora[3] = 1;
          }
          if(acertahora[3] == 0){
          	acertahora[3] = 31;
          }
          if(acertahora[4] > 7){
          	acertahora[4] = 1;
          }
          if(acertahora[4] == 0){
          	acertahora[4] = 7;
          }
          if(acertahora[5] > 23){
          	acertahora[5] = 0;
          }
          if(acertahora[5] == -1){
          	acertahora[5] = 23;
          }
          if(acertahora[6] > 59){
          	acertahora[6] = 0;
          }
          if(acertahora[6] == -1){
          	acertahora[6] = 59;
          }
          if(acertahora[7] > 59){
          	acertahora[7] = 0;
          }
          if(acertahora[7] == -1){
          	acertahora[7] = 59;
          }

          lcd.setCursor(9, 1);
          lcd.print(acertahora[i], DEC);

          if(estouro == 20){
          	i = 0 ;
          	estouro = 0;
          }
          delay(200);
        }

    DS1307.setDate(acertahora[1], acertahora[2], acertahora[3], acertahora[4], acertahora[5], acertahora[6], acertahora[7]); // AQUI ENVIAMOS OS DADOS DEFINIDOS PARA O REAL TIME CLOK
    lcd.clear();
      
}

void menus()
{  
   wdt_disable();
   delay(1000);
   lcd.clear();
   while(menu > 0)
   {
      lcd.setCursor(0,0);
      if(menu==1){
      	lcd.print("Definir Horarios");
      	if(digitalRead(sete)==LOW){
      		p=1;
      		menu=0;
      		programa();
      	}
      }
      if(menu==2){
      	lcd.print("Definir Umidade ");
      	if(digitalRead(sete)==LOW){
      		h = 1;
      		menu=0;
      		humidade();
      	}
      }
      if(menu==3){
      	lcd.print("Acerta dt e a Hr");
      	if(digitalRead(sete)==LOW){
      		i=1;
      		menu=0;
      		acertartc();
      	}
      }
      if(menu==4){
      	lcd.print("delay off bomba ");
      	if(digitalRead(sete)==LOW){
      		db=1;
      		menu=0;
      		offbomba();
      	}
      }
      if(menu==5){
      	lcd.print("Zerar programas ");
      	if(digitalRead(sete)==LOW){
      		z=1;
      		menu=0;
      		zera();
      	}
      } 
      if(menu==6){
      	lcd.print("Modo de Operacao");
      	if(digitalRead(sete)==LOW){
      		m=1;
      		menu=0;
      		operacao();
      	}
      }   
      if(digitalRead(up)==LOW && menu < 6){
      	menu++;
      	estouro = 0;
      }
      if(digitalRead(dow)==LOW && menu > 0){
      	menu--;
      	estouro = 0;
      }
      if(estouro == 20){
      	menu = 0;
      	estouro = 0;
      }
      delay(400);
   }
   lcd.clear();
}

void programa()
{
   wdt_disable();
   delay(1000);
   lcd.clear(); 
        while( p> 0 && p<7)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          if(p==1 || p==2){
          	lcd.print("   Programa 1   ");
          	lcd.setCursor(2, 1);
          	lcd.print(programas[1], DEC);
          	lcd.print(':');
          	lcd.print(programas[2]);
          	lcd.print(" horas");
          }
          if(p==3 || p==4){
          	lcd.print("   Programa 2   ");
          	lcd.setCursor(2, 1);
          	lcd.print(programas[3], DEC);
          	lcd.print(':');
          	lcd.print(programas[4]);
          	lcd.print(" horas");
          }
          if(p==5 || p==6){
          	lcd.print("   Programa 3   ");
          	lcd.setCursor(2, 1);
          	lcd.print(programas[5], DEC);
          	lcd.print(':');
          	lcd.print(programas[6]);
          	lcd.print(" horas");
          }

              
          if(digitalRead(up) == LOW ){
          	programas[p]++;
          	estouro = 0;
          }
          if(digitalRead(dow) == LOW && programas[p] > 0 ){
          	programas[p]--;
          	estouro = 0;
          }
          if(digitalRead(sete) == LOW){
          	p ++;
          	estouro = 0;
          }
          if(estouro == 30){
          	p = 0;
          	estouro = 0;
          }
          if(programas[1] > 23){
          	programas[1] = 0;
          }
          if(programas[3] > 23){
          	programas[3] = 0;
          }
          if(programas[5] > 23){
          	programas[5] = 0;
          }
          if(programas[2] > 59){
          	programas[2] = 0;
          }
          if(programas[4] > 59){
          	programas[4] = 0;
          }
          if(programas[6] > 59){
          	programas[6] = 0;
          }

          delay(400);
        }
for (int x=0; x<10; x++){
	EEPROM.write(x, programas[x]);
	}
lcd.clear();
p = 0;
}

void humidade(){
	wdt_disable();
	delay(1000);
	lcd.clear();
	while(h > 0){
		lcd.setCursor(0,0);
		lcd.print("Definir Umidade ");
		if(digitalRead(up) == LOW && humidad < 100 ){
			humidad ++;
			estouro = 0;
		}
    if(digitalRead(dow) == LOW && humidad > 0){
    	humidad --;
    	estouro = 0;
    }
    if(digitalRead(sete) == LOW){
    	h = 0;
    	estouro = 0;
    }
    lcd.setCursor(5, 1);
    lcd.print(humidad);
    lcd.print("%   ");
    if(estouro == 20){
    	h = 0 ;
    	estouro = 0;
    }
    delay(150);
  }
EEPROM.write(50, humidad);
lcd.clear();
}

void offbomba(){
 { 
  wdt_disable();
  delay(1000);
  lcd.clear();
  while(db > 0){
    lcd.setCursor(0,0);
    lcd.print("Delay off bomba");
    if(digitalRead(up) == LOW){
    	deslbomba ++;
    	estouro = 0;
    }
    if(digitalRead(dow) == LOW && deslbomba > 0){
    	deslbomba --;
    	estouro = 0;
    }
    if(digitalRead(sete) == LOW){
    	db = 0;
    	estouro = 0;
    }
    lcd.setCursor(5, 1);
    lcd.print(deslbomba);
    lcd.print(" Segundos");
    delay(150);
    if(estouro == 30){
    	db = 0 ;
    	estouro = 0;
    }
  }
EEPROM.write(52,*((char*)&deslbomba ));
EEPROM.write(53, *((char*)&deslbomba + 1));
} 
lcd.clear();
}
  
void zera()
{
  lcd.setCursor(4, 0);
  lcd.clear();
  lcd.print("Zerando");

 for (int x=0; x<10; x++){
 	EEPROM.write(x, 0);
 } 
for (int x=0; x<10; x++){
	programas[x] = EEPROM.read(x);
}
lcd.clear();
}

void operacao(){
  wdt_disable();
  delay(500);
  lcd.clear();
  while(m > 0){
    lcd.setCursor(0,0);
    lcd.print("Modo de Controle");
    lcd.setCursor(0,1);
    lcd.print("Dow = Um Up = Pr");
    if(digitalRead(up) == LOW){
    	o = 0;
    	m = 0 ;
    }
    if(digitalRead(dow) == LOW){
    	o = 1;
    	m = 0 ;
    }
    if(estouro == 30){
    	m = 0 ;
    	estouro = 0;
    }
  }
EEPROM.write(51, o); 
for(int x = 0; x < 16; x++) {
	delay(200);
	lcd.setCursor(x,1);
	lcd.write(B11111111);
}
lcd.clear(); 
}
   
void mostrahora()
{
   DS1307.getDate(RTCValues);
   sprintf(dateTime, "%02d:%02d:%02d   %02d/%02d", RTCValues[4], RTCValues[5], RTCValues[6], RTCValues[2], RTCValues[1]);
   delay(50);
   lcd.setCursor(0,0);
   lcd.print(dateTime);
   delay(50);
}

void teste()
{
  if(programas[1] == RTCValues[4] && programas[2] == RTCValues[5] && RTCValues[6] == 0 && reading < humidad){
  	digitalWrite(relay2, HIGH);
  }
  if(programas[3] == RTCValues[4] && programas[4] == RTCValues[5] && RTCValues[6] == 0 && reading < humidad){
  	digitalWrite(relay2, HIGH);
  }
  if(programas[5] == RTCValues[4] && programas[6] == RTCValues[5] && RTCValues[6] == 0 && reading < humidad){
  	digitalWrite(relay2, HIGH);
  }
  if(o == 1 && reading < humidad){
  digitalWrite(relay2, HIGH);
  }
  else{
  	if(o == 1 && (deslbomba2/2) >= deslbomba){
  		digitalWrite(relay2, LOW);
  		}
  	}// Caso queira que a bomba não desligue imediatamente coloque aqui seu delay.

  
  if(RTCValues[4] == 0 && RTCValues[5] == 0 && RTCValues[6] == 0){
  	digitalWrite(relay2, LOW);
  }
 
}

void informa(){
            if(d1<= 10){
            	lcd.setCursor(0,1);
            	lcd.print("Setpoint. ");
            	lcd.print(humidad);
            	lcd.print("%             ");
            }
            if(d1>10 && d1<=15){
            	lcd.setCursor(0,1);
            	lcd.print("Sensor ");
            	lcd.print(reading);
            	lcd.print("%             "); 
            }
            if(d1>15 && d1<=20){
            	lcd.setCursor(0,1);
            	if(digitalRead(relay2)==HIGH){
            		lcd.print("> Bomba ligada <");
            	}
            	else{
            		lcd.print("Bomba Desligada ");
            	}
            }
            if(d1>20 && d1<=25){
            	lcd.setCursor(0,1);
            	if(reading < humidad){
            		lcd.print("Umidade Baixa!!!");
            	}
            	else{
            		lcd.print(" Umidade Normal ");
            	}
            }
            if(d1>25 && d1<=30){
            	lcd.setCursor(0,1);
            	lcd.print("Irrigador Autom.");
            }
            if(d1>=30){d1=0;}
     
}

void Nivel(){
  int nivelMax =  analogRead(1); 
  int nivelMini = analogRead(2); 
  if(nivelMax < 10){
  	digitalWrite(relay1 , LOW);
  }
  if(nivelMini < 10 && nivelMax > 500){
  	digitalWrite(relay1 , HIGH);
  }
}
  void loop(){
 
    if(digitalRead(sete)==LOW){
    	menu =1;
    	menus();
    }
    reading = map(analogRead(analogS),0, 1023, 0, 100);
    mostrahora();
    informa();
    teste();
    Nivel();
    
    if(digitalRead(relay2)==HIGH && (deslbomba2/2) >= deslbomba){
    	digitalWrite(relay2, LOW);
    	deslbomba2 = 0;
    }
    if(reading < humidad){
    	digitalWrite(led2, HIGH);
    }
    else{
    	digitalWrite(led2, LOW);
}


   //for (int b=0; b<21; b++){
 // Serial.println(programas[b]);}
  //Serial.println(humidad , DEC);
}