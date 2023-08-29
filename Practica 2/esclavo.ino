#include <Wire.h>
bool secuencia_terminada;
bool wait;
bool jugando;
int punt;
int cm;
void setup()
{
  for(int i= 2; i<8;i++){
    pinMode(i,OUTPUT);
  }
  Wire.begin(1);  
  Serial.begin(9600); 
  Wire.onReceive(eventoRecepcion);
  Wire.onRequest(eventoSolicitud); 
  
}

void loop(){
}


void eventoRecepcion(int numBytes){
  byte l = Wire.read();
    if(l == 'I' ){
      secuencia_terminada = false;
      for(int j= 0; j<4;j++){
        for(int i= 2; i<8;i++){
          digitalWrite(i,HIGH);
          delay(1500);
          digitalWrite(i,LOW);
        }
      }
      digitalWrite(2,HIGH);
      delay(100);
      digitalWrite(2,LOW);
      secuencia_terminada = true;
      jugando = false;
    }else if(l == 'J'){
      digitalWrite(2+punt,HIGH);
      punt++;
      if(punt>6){
        for(int i= 2; i<8;i++){
          digitalWrite(i,LOW);
        }
        punt = 0;
      }
      delay(10000);
    }else{
      for(int i= 2; i<8;i++){
        digitalWrite(i,LOW);
      }
      jugando = false;
    }
}

void eventoSolicitud(){
    if(!jugando){
      if(secuencia_terminada){
        jugando = true;
        punt = 0;
      	Wire.write(1);
      }else{
        Wire.write(0);
      }
    }else{
      Wire.write(leerSensor(8)); //anotar
      Wire.write(leerSensor(9)); //bola entra
      Wire.write(leerSensor(10)); // pierde vida
	}
}

bool leerSensor(int Pin){
  pinMode(Pin, OUTPUT);  // Clear the trigger
  digitalWrite(Pin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Pin, LOW);
  pinMode(Pin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  cm = 0.01723 * pulseIn(Pin, HIGH);
  return cm<10; 
}

