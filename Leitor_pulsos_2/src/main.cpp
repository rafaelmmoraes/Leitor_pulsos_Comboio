#include <LiquidCrystal.h>	//Importar a LCD library.
#include <Arduino.h>
//leitura pulsos
volatile int IRQcount;
int pin = 2;
int pin_irq = 0;

//encoder defines e variaveis
const int buttonDigito = 10;
const int pinoCLK = 9; //PINO DIGITAL (CLK)
const int pinoDT = 8;  //PINO DIGITAL (DT)
int contadorPos = 0;  //CONTADOR DE POSIÇÕES DO ENCODER
int ultPosicao; //REGISTRA A ÚLTIMA POSIÇÃO DO ENCODER
int leituraCLK; //VARIÁVEL PARA ARMAZENAR LEITURA DO PINO CLK
boolean bCW; //VARIÁVEL DE CONTROLE DO SENTIDO (HORÁRIO / ANTI-HORÁRIO)

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
const int buttonMais = 9;     // the number of the pushbutton pin
const int buttonMenos = 8;     // the number of the pushbutton pin
float fator = 52.52;
int buttonStateMais = 0;         // variable for reading the pushbutton status
int buttonStateMenos = 0;         // variable for reading the pushbutton status
int digito = 0;

void leitura_encoder(){
  leituraCLK = digitalRead(pinoCLK); //VARIÁVEL RECEBE A LEITURA DO PINO CLK 
   if (leituraCLK != ultPosicao){ //SE VALOR DA VARIÁVEL FOR DIFERENTE DO VALOR DE "ultPosicao", FAZ
     if (digitalRead(pinoDT) != leituraCLK) { //SE LEITURA DO PINO FOR DIFERENTE DA LEITURA DE "leituraCLK",
      //SIGNIFICA QUE O EIXO ESTÁ SENDO GIRADO NO SENTIDO HORÁRIO E FAZ
       if(digito == 0){
         fator++;
       }if(digito==1){
         fator = fator + 0.01;
       }
       contadorPos ++; //INCREMENTA CONTAGEM DA VARIÁVEL EM +1
       bCW = true; //VARIÁVEL BOOLEANA RECEBE VERDADEIRO (SENTIDO HORÁRIO)
     } else { //SENÃO, SIGNIFICA QUE O EIXO ESTÁ SENDO GIRADO NO SENTIDO ANTI-HORÁRIO E FAZ
       bCW = false; //VARIÁVEL BOOLEANA RECEBE FALSO (SENTIDO ANTI-HORÁRIO)
       contadorPos--; //DECREMENTA CONTAGEM DA VARIÁVEL EM -1
       if(digito == 0){
         fator--;
       }if(digito==1){
         fator = fator - 0.01;
       }
     }
     Serial.print(" / Posição do encoder: "); //IMPRIME O TEXTO NA SERIAL
     Serial.println(contadorPos); //IMPRIME NO MONITOR SERIAL A POSIÇÃO ATUAL DO ENCODER
   }
   if(digitalRead(buttonDigito) == LOW){ //SE LEITURA DO PINO FOR IGUAL A LOW, FAZ
      digito = !digito;
      Serial.println ("Botão pressionado"); //IMPRIME O TEXTO NA SERIAL
      delay(200); //INTERVALO DE 200 MILISSEGUNDOS
   }
   ultPosicao = leituraCLK; //VARIÁVEL RECEBE O VALOR DE "leituraCLK"
}

void IRQcounter(){
  IRQcount++;
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(pin),IRQcounter,RISING);
  delay(5);
  detachInterrupt(pin);
  Serial.begin(9600);
	lcd.begin(16, 4);
  pinMode(buttonDigito,INPUT_PULLUP);
  pinMode(buttonMais, INPUT);
  pinMode(buttonMenos, INPUT);
  pinMode (pinoCLK,INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode (pinoDT,INPUT); //DEFINE O PINO COMO ENTRADA
  ultPosicao = digitalRead(pinoCLK); //VARIÁVEL RECEBE A LEITURA DO PINO CLK  
}

void loop(){
  leitura_encoder();
  lcd.setCursor(0, 0);
  lcd.print("LEITOR DE PULSOS");
  
  lcd.setCursor(0, 1);
  lcd.print("Pulsos:");
  lcd.setCursor(7, 1);
  lcd.print(IRQcount);
  
  lcd.setCursor(0, 2);
  lcd.print("Litros:");
  lcd.setCursor(7, 2);
  lcd.print(IRQcount/fator);
  
  lcd.setCursor(0, 3);
  lcd.print("Fator:");
  lcd.setCursor(6, 3);
  lcd.print(fator);
}