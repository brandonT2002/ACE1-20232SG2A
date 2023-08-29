#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>

#define SERVOIZQ 10
#define SERVODER 9

Servo palancaIzq;
Servo palancaDer;

bool pulsacion1 = false;
bool abierto1 = false;
bool tirado1 = false;

bool pulsacion2 = false;
bool abierto2 = false;
bool tirado2 = false;

unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;

int vidas;
int puntaje;
double puntos_nivel;
int nivel;
bool anotacion;
bool entrada;
bool vidamenos;
bool seleccionado = true;
bool enviado;
int estado = 0;
int btn_izq = 7;
int btn_der = 6;
String nombres[4]= {"PATRICIA LOURDES", "DAVID NORIEGA","BRANDON TEJAXUN","SEBASTIAN GODOY"};

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  lcd_1.begin(16, 2); // Set up the number of columns and rows on the LCD
  pinMode(btn_izq,INPUT);
  pinMode(btn_der,INPUT);
  // Print a message to the LCD.
  lcd_1.print("<GRUPO        2>");
  palancaIzq.attach(SERVOIZQ);
  palancaDer.attach(SERVODER);
  palancaIzq.write(0);
  palancaDer.write(180);
}

void loop()
{
  switch(estado){
    case 0:
    lcd_1.setCursor(0, 0);
    lcd_1.print("<GRUPO        2>");
    for(int i=0; i <4;i++){
      lcd_1.setCursor(0, 1);
      int espacios = 16-nombres[i].length();
      lcd_1.print(nombres[i]);
      for(int j = 0; j<espacios;j++){
        lcd_1.print(" ");
      }
      delay(1000);
    }
    estado = 1;
    seleccionado = true;
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("> Nuevo juego");
    lcd_1.setCursor(0, 1);
    lcd_1.print("  Puntajes altos");
    break;
  
    case 1:
    if(digitalRead(btn_izq)){
      seleccionado = !seleccionado;
      if(seleccionado){
        lcd_1.setCursor(0, 0);
        lcd_1.print(">");
        lcd_1.setCursor(0, 1);
        lcd_1.print(" ");
      }else{
        lcd_1.setCursor(0, 0);
        lcd_1.print(" ");
        lcd_1.setCursor(0, 1);
        lcd_1.print(">");
      }
      delay(500);
    }
    
    if(digitalRead(btn_der)){
      if(seleccionado){
      	estado = 2;
       	lcd_1.clear();
        lcd_1.setCursor(0, 0);
    	lcd_1.print("Cargando...");
      }else{
        estado = 0;
        lcd_1.clear();
      }
      delay(500);
    }
    
    break;
  
  	case 2:
    Wire.beginTransmission(1); 
    Wire.write('I');
    Wire.endTransmission();
    
    Wire.requestFrom(1, 1);
    
    if(Wire.read() == 1){
      estado = 3;
      vidas = 3;
      nivel = 1;
      puntaje = 0;
      puntos_nivel = 0;
      lcd_1.clear();
      lcd_1.setCursor(0, 0);
      lcd_1.print("NIVEL ");
      lcd_1.print(nivel);
      for(int i = 0;i<3;i++){
       	lcd_1.setCursor(12+i, 0);
        if(i<=vidas-1){
          lcd_1.print("*");
        }else{
          lcd_1.print("o");
        }
      }
      lcd_1.setCursor(0, 1);
      lcd_1.print(puntaje);
      estado = 3;
    }
    break;
    
    case 3: //antes de entrar pelota
    Wire.requestFrom(1, 3);
    anotacion = Wire.read();
    entrada = Wire.read();
    vidamenos = Wire.read();
    if(entrada){
    	estado = 4;
    }
    break;
    
    case 4://juego
    mover();
    Wire.requestFrom(1, 3);
    anotacion = Wire.read();
    entrada = Wire.read();
    vidamenos = Wire.read();
    if(anotacion){
      Wire.beginTransmission(1); 
      Wire.write('J');
      Wire.endTransmission();
      puntos_nivel++;
      puntaje += 100*nivel;
      lcd_1.setCursor(0, 1);
      lcd_1.print(puntaje);
      if(puntos_nivel > 6){
        nivel+=1;
        lcd_1.setCursor(0, 0);
        lcd_1.print("NIVEL ");
        lcd_1.print(nivel);
        puntos_nivel = 0;
      }
    }
    
    if(vidamenos){
      vidas--;
      for(int i = 0;i<3;i++){
        lcd_1.setCursor(12+i, 0);
        if(i<=vidas-1){
          lcd_1.print("*");
        }else{
          lcd_1.print("o");
        }
      }
      
      if(vidas <= 0){
      	estado = 0;
        Wire.beginTransmission(1); 
        Wire.write('P');
        Wire.endTransmission();
      }else{
      	estado = 3;
      }
      
    }
    break;
    
  }
  
  
}


void mover(){
  if(!digitalRead(btn_izq)) {
    tirado1 = false;
  }
  if(!tirado1 && digitalRead(btn_izq)) {
    pulsacion1 = true;
    tirado1 = true;
    lastTime1 = millis();
  }
  if(pulsacion1) {
    if(!abierto1) {
    	palancaIzq.write(60);
    }
    if(!abierto1 && millis() - lastTime1 >= 300) {
    	lastTime1 = millis();
      	abierto1 = true;
    }
    if(abierto1) {
    	palancaIzq.write(0);
    }
    if(abierto1 && millis() - lastTime1 >= 300) {
    	lastTime1 = millis();
      	abierto1 = false;
      	pulsacion1 = false;
    }
  }
  // LADO DERECHO
  if(!digitalRead(btn_der)) {
    tirado2 = false;
  }
  if(!tirado2 && digitalRead(btn_der)) {
    pulsacion2 = true;
    tirado2 = true;
    lastTime2 = millis();
  }
  if(pulsacion2) {
    if(!abierto2) {
    	palancaDer.write(120);
    }
    if(!abierto2 && millis() - lastTime2 >= 300) {
    	lastTime2 = millis();
      	abierto2 = true;
    }
    if(abierto2) {
    	palancaDer.write(180);
    }
    if(abierto2 && millis() - lastTime2 >= 300) {
    	lastTime2 = millis();
      	abierto2 = false;
      	pulsacion2 = false;
    }
  } 
}