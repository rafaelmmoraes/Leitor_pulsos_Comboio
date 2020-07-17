#include <LiquidCrystal.h>	//Importar a LCD library.
#include <Arduino.h>

//encoder defines e variaveis
#define ENC_A 8
#define ENC_B 9
#define ENC_PORT PINB
char read_encoder(); 
unsigned short int counter = 0; //this variable will be changed by encoder input
const int buttonDigito = 10;

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
const int buttonMais = 9;     // the number of the pushbutton pin
const int buttonMenos = 8;     // the number of the pushbutton pin
float fator = 52.52;
int buttonStateMais = 0;         // variable for reading the pushbutton status
int buttonStateMenos = 0;         // variable for reading the pushbutton status
int digito = 0;

ISR(TIMER2_OVF_vect)
{
    TCNT2=178;          // Reinicializa o registrador do Timer2

    float tmpdata;
    /**/
    tmpdata = read_encoder();
    if( tmpdata ) 
    {
      Serial.print("Counter value: ");
      //Serial.println(counter);
      Serial.println(tmpdata/100,DEC);
      counter += tmpdata;
      if (digito == 0){
      fator += tmpdata;
      }
      if(digito == 1){
      fator = fator+(tmpdata/100);
      }
    }
} //end ISR
char read_encoder()                              //Função para leitura de Rotary Encoder
{
  static char enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static unsigned char old_AB = 0;
  /**/
  old_AB <<= 2;                   //remember previous state
  old_AB |= ( ENC_PORT & 0x03 );  //add current state
  return ( enc_states[( old_AB & 0x0f )]);

} //end read_encoder

void setup() {
  Serial.begin(9600);
	lcd.begin(16, 4);
  pinMode(buttonDigito,INPUT_PULLUP);
  pinMode(buttonMais, INPUT);
  pinMode(buttonMenos, INPUT);
  //encoder
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(13, OUTPUT);
  //-- Configura Interrupção --
  TCCR2A = 0x00;   //Timer operando em modo normal
  TCCR2B = 0x07;   //Prescaler 1:1024
  TCNT2  = 178;    //~~ 5 ms para o overflow
  TIMSK2 = 0x01;   //Habilita interrupção do Timer2
}

void loop(){
  lcd.setCursor(0, 0);
  lcd.print("LEITOR DE PULSOS");
  
  lcd.setCursor(0, 1);
  lcd.print("Pulsos:");
  lcd.setCursor(7, 1);
  lcd.print(millis() / 100);
  
  lcd.setCursor(0, 2);
  lcd.print("Litros:");
  lcd.setCursor(7, 2);
  lcd.print((millis()/100)/fator);
  
  lcd.setCursor(0, 3);
  lcd.print("Fator:");
  lcd.setCursor(6, 3);
  lcd.print(fator);
  int buttonState = digitalRead(buttonDigito);
if (buttonState == LOW){
    digito = !digito;
    Serial.println(digito);
    delay(200);
  }
  /*//rotina que lê dois botões de incremento e decremento
  buttonStateMais  = digitalRead(buttonMais);
  buttonStateMenos = digitalRead(buttonMenos);
  if (buttonStateMais == HIGH) {
    // turn LED on:
    fator = fator + 0.01;
  } 
  if (buttonStateMenos == HIGH) {
    // turn LED on:
    fator = fator - 0.01;
  }*/
}