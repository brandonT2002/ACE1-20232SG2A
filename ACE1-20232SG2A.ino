#include <LedControl.h>

int puntaje = 0;
int desfase = 0;
int cambioDesfase = 20;
int cambioPuntaje = 20;
int btnStart = 24;
int btnArriba = 25;
int btnAbajo = 26;
int btnIzq = 27;
int btnDer = 28;
int coordx;
int scrollDerecha = 0;
int mensajeContinuo = 0;
bool startPresionado = false;
int presionadoInicial = 0;
String direccionActual = "right";
int xActual = 0;
int yActual = 0;
int serpienteX[144]; 
int serpienteY[144];
int longitudSerpiente;
int manzanaX;
int manzanaY;
int tiempoActual;
int contadorPasos;
int velEscogida;
int velActual = 250;
int pasosIniciales = 0;
bool perdio = false;
float n_desfase;
int seleccionDificultad = 1;
int estado = 1;

LedControl driver = LedControl(12,10,11,1);
byte numeros[10][8][4] = {{{0,0,0,0},
                          {0,0,1,0},
                          {0,1,0,1},
                          {0,1,0,1},
                          {0,1,0,1},
                          {0,1,0,1},
                          {0,0,1,0},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,0,1,0},
                          {0,1,1,0},
                          {0,0,1,0},
                          {0,0,1,0},
                          {0,0,1,0},
                          {0,0,1,0},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,1,1,0},
                          {0,0,0,1},
                          {0,0,1,1},
                          {0,1,0,0},
                          {0,1,0,0},
                          {0,1,1,1},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,1,1,0},
                          {0,0,0,1},
                          {0,1,1,0},
                          {0,0,0,1},
                          {0,0,0,1},
                          {0,1,1,0},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,1,0,1},
                          {0,1,0,1},
                          {0,1,1,1},
                          {0,0,0,1},
                          {0,0,0,1},
                          {0,0,0,1},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,1,1,0},
                          {0,1,0,0},
                          {0,1,1,0},
                          {0,0,0,1},
                          {0,0,0,1},
                          {0,1,1,0},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,0,1,0},
                          {0,1,0,0},
                          {0,1,1,0},
                          {0,1,0,1},
                          {0,1,0,1},
                          {0,0,1,0},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,1,1,1},
                          {0,0,0,1},
                          {0,0,1,0},
                          {0,0,1,0},
                          {0,0,1,0},
                          {0,0,1,0},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,0,1,0},
                          {0,1,0,1},
                          {0,0,1,0},
                          {0,1,0,1},
                          {0,1,0,1},
                          {0,0,1,0},
                          {0,0,0,0}},
                          {{0,0,0,0},
                          {0,0,1,0},
                          {0,1,0,1},
                          {0,0,1,1},
                          {0,0,0,1},
                          {0,0,0,1},
                          {0,0,1,0},
                          {0,0,0,0}}};

byte mensaje[24][8][6] = {
  {{0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,1,0}, //<
   {0,0,0,1,0,0},
   {0,0,1,0,0,0},
   {0,0,0,1,0,0},
   {0,0,0,0,1,0},
   {0,0,0,0,0,0}},
  
  {{0,0,0,0,0,0},
   {0,1,1,1,0,0},
   {0,1,0,0,1,0}, //P
   {0,1,0,0,1,0},
   {0,1,1,1,0,0},
   {0,1,0,0,0,0},
   {0,1,0,0,0,0},
   {0,1,0,0,0,0}},
  
  {{0,0,0,0,0,0},
   {0,1,1,1,0,0},
   {0,1,0,0,1,0}, //R
   {0,1,0,0,1,0},
   {0,1,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}},
  
  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}, //A
   {0,1,0,0,1,0},
   {0,1,1,1,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}},
   
  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,0,0}, //C
   {0,1,0,0,0,0},
   {0,1,0,0,0,0},
   {0,1,0,0,1,0},
   {0,0,1,1,0,0}},
   
  {{0,0,0,0,0,0},
   {0,0,1,0,0,0},
   {0,1,1,0,0,0},
   {0,0,1,0,0,0}, //1
   {0,0,1,0,0,0},
   {0,0,1,0,0,0},
   {0,0,1,0,0,0},
   {0,1,1,1,0,0}},
   
  {{0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0}, //-
   {0,1,1,1,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0}},
  
  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,0,0}, //G
   {0,1,0,0,0,0},
   {0,1,0,1,1,0},
   {0,1,0,0,1,0},
   {0,0,1,1,0,0}},

  {{0,0,0,0,0,0},
   {0,1,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}, //R
   {0,1,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}},
  
  {{0,0,0,0,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}, //U
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,0,1,1,0,0}},
  
  {{0,0,0,0,0,0},
   {0,1,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}, //P
   {0,1,1,1,0,0},
   {0,1,0,0,0,0},
   {0,1,0,0,0,0},
   {0,1,0,0,0,0}},
  
  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}, //O
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,0,1,1,0,0}},
  
  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0}, //2
   {0,0,0,0,1,0},
   {0,0,0,1,0,0},
   {0,0,1,0,0,0},
   {0,1,0,0,0,0},
   {0,1,1,1,1,0}},
  
  {{0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0}, //-
   {0,1,1,1,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0}},
  

  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,0,0}, //S
   {0,0,1,1,0,0},
   {0,0,0,0,1,0},
   {0,1,0,0,1,0},
   {0,0,1,1,0,0}},
  
  {{0,0,0,0,0,0},
   {0,1,1,1,1,0},
   {0,1,0,0,0,0},
   {0,1,0,0,0,0}, //E
   {0,1,1,1,0,0},
   {0,1,0,0,0,0},
   {0,1,0,0,0,0},
   {0,1,1,1,1,0}},
  
  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,0,0}, //C
   {0,1,0,0,0,0},
   {0,1,0,0,0,0},
   {0,1,0,0,1,0},
   {0,0,1,1,0,0}},

  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,0,0}, //C
   {0,1,0,0,0,0},
   {0,1,0,0,0,0},
   {0,1,0,0,1,0},
   {0,0,1,1,0,0}},
  
  {{0,0,0,0,0,0},
   {0,1,1,1,0,0},
   {0,0,1,0,0,0},
   {0,0,1,0,0,0}, //I
   {0,0,1,0,0,0},
   {0,0,1,0,0,0},
   {0,0,1,0,0,0},
   {0,1,1,1,0,0}},
  
  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}, //O
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,0,1,1,0,0}},
  
  {{0,0,0,0,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}, //N
   {0,1,1,0,1,0},
   {0,1,0,1,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}},
  
  {{0,0,0,0,0,0},
   {0,0,1,1,0,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}, //A
   {0,1,0,0,1,0},
   {0,1,1,1,1,0},
   {0,1,0,0,1,0},
   {0,1,0,0,1,0}},
   
  {{0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,1,0,0,0}, //>
   {0,0,0,1,0,0},
   {0,0,0,0,1,0},
   {0,0,0,1,0,0},
   {0,0,1,0,0,0},
   {0,0,0,0,0,0}}};



void setup() {
  Serial3.begin(9600);
  randomSeed(analogRead(A3));
  seleccionDificultad = 1;
  for(int i=0;i<8;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
  }

  for(int i=46;i<57;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }

  for(int i=22;i<29;i++){
    pinMode(i,INPUT);
  }
  
  scrollDerecha = digitalRead(22);
  mensajeContinuo = digitalRead(23);
  driver.shutdown(0,false);
  driver.setIntensity(0,8);

}

void pintarled(int x, int y){
  if(x<8){
    digitalWrite(x+46,HIGH);
    digitalWrite(y,LOW);
    delay(1);
    digitalWrite(x+46,LOW);
    digitalWrite(y,HIGH);
  }else{
    driver.setLed(0,y,x-8,true);
  }
}

void printNumero(int n){
  int centenas = int(n/100);
  int decenas = int((n-centenas*100)/10);
  int unidades = int(n-centenas*100-decenas*10);

  for(int x = 0;x<4;x++){
    for(int y = 0;y<8;y++){
      if(numeros[centenas][y][x]){
        pintarled(x+1,y);
      }
    }
  }

  for(int x = 0;x<4;x++){
    for(int y = 0;y<8;y++){
      if(numeros[decenas][y][x]){
        pintarled(x+6,y);
      }
    }
  }

  for(int x = 0;x<4;x++){
    for(int y = 0;y<8;y++){
      if(numeros[unidades][y][x]){
        pintarled(x+11,y);
      }
    }
  }
}

void printMensajeContinuo(){
  for(int letra = 0; letra <23;letra++){
    for(int x = 0; x<6;x++){
      coordx = desfase+x+6*letra;
      while(coordx < 0){
        coordx+=138;
      }
      coordx= coordx%138;
      if(coordx>15){
        continue;
      }
      for(int y = 0; y<8;y++){
        if(mensaje[letra][y][x]){
          pintarled(coordx%138,y);
        }
      }
    }
  }
}

void printMensajeLetra(){
  if(desfase<0){
      desfase = 23;
  }
  
  for(int x = 0; x<6;x++){
    coordx = 5+x;
    for(int y = 0; y<8;y++){
      if(mensaje[desfase%23][y][x]){
        pintarled(coordx,y);
      }
    }
  }

}

void printSerpiente(){
  for(int i = 0; i<longitudSerpiente; i++){
    pintarled(serpienteX[i],serpienteY[i]);
  }
}

void printManzana(){
  pintarled(manzanaX,manzanaY);
}

void initSerpiente(){
  if(random(1) == 0){
      serpienteX[0]=0;
    }else{
      serpienteX[0]=8;
    }
  serpienteY[0]=random(7);
}

void initManzana(){
  manzanaX = random(15);
  manzanaY = random(7);
  
  while(revisarColision(manzanaX,manzanaY,0)){
    manzanaX = random(15);
    manzanaY = random(7);
  }
}

void moverSerpiente(){
  int auxX;
  int auxY;

  for(int i = longitudSerpiente-1; i>=0; i--){
    serpienteX[i+1] = serpienteX[i];
    serpienteY[i+1] = serpienteY[i];
  }

  longitudSerpiente++;

  if(direccionActual == "up"){
    serpienteY[0]--;
  }else if(direccionActual == "down"){
    serpienteY[0]++;
  }else if(direccionActual == "left"){
    serpienteX[0]--;
  }else if(direccionActual == "right"){
    serpienteX[0]++;
  }

  xActual = serpienteX[0];
  yActual = serpienteY[0];

  if(xActual<0||xActual>15||yActual<0||yActual>7){
    perdio = true;
    Serial3.print(xActual);
    Serial3.print(", ");
    Serial3.println(yActual);
    Serial3.println("Perdio por colision con borde");
  }

  if(revisarColision(xActual,yActual,1)){
    perdio = true;
    Serial3.println("Perdio por colision consigo mismo");
  }
}

void quitarSegmento(){
  longitudSerpiente--;
}

void cambiarDireccion(String dir){
  if(direccionActual == "up"){
    if(dir != "down"){
      direccionActual = dir;
    }
  }else if(direccionActual == "down"){
    if(dir != "up"){
      direccionActual = dir;
    }
  }else if(direccionActual == "left"){
    if(dir != "right"){
      direccionActual = dir;
    }
  }else if(direccionActual == "right"){
    if(dir != "left"){
      direccionActual = dir;
    }
  }
}

void borrarSerpiente(){
  for(int i = 0; i<longitudSerpiente; i++){
    serpienteX[i] = 0;
    serpienteY[i] = 0;
  }
}

bool revisarColision(int x, int y, int inicio){
  for(int i = inicio; i<longitudSerpiente; i++){
    if(serpienteX[i] == x && serpienteY[i] == y){
      return true;
    }
  }
  return false;
}

void loop() {

  if(digitalRead(btnStart)){
    if(!startPresionado){
      startPresionado = true;
      presionadoInicial = millis();
    }
  }else{
    if(startPresionado){
      startPresionado = false;
      int tiempoTotal = millis() - presionadoInicial;
      if(tiempoTotal > 3000){
        if(estado == 1){
          estado = 2;
          driver.clearDisplay(0);
        }else{
          estado = 1;
          driver.clearDisplay(0);
        }
      }
    }
  }

  switch(estado){
    case 1:
      scrollDerecha = digitalRead(22);
      mensajeContinuo = digitalRead(23);
      n_desfase = (analogRead(A0)/1023.0);
      
      if(mensajeContinuo){
        printMensajeContinuo();
        if(int(millis())-cambioDesfase >= 60.0+600.0*n_desfase){
          cambioDesfase = millis();
          driver.clearDisplay(0);
          if(scrollDerecha){
            desfase++;
          }else{
            desfase--;
          }
        }
      }else{
        printMensajeLetra();
        cambioDesfase--;
        if(int(millis())-cambioDesfase >= 200+1800*n_desfase){
          cambioDesfase = millis();
          driver.clearDisplay(0);
          if(scrollDerecha){
            desfase--;
          }else{
            desfase++;
          }
        }
      }
    break;

    case 2:

      printNumero(seleccionDificultad);
      if(digitalRead(btnArriba) && seleccionDificultad < 4){
        seleccionDificultad++;
        driver.clearDisplay(0);
        delay(250);
      }else if(digitalRead(btnAbajo) && seleccionDificultad > 1){
        seleccionDificultad--;
        driver.clearDisplay(0);
        delay(250);
      }
      
      if(digitalRead(btnStart)){
        velEscogida = 450-50*seleccionDificultad;
        velActual = velEscogida;
        tiempoActual = millis();
        contadorPasos = 0;
        estado = 3;
        initSerpiente();
        initManzana();
        longitudSerpiente = 1;
        direccionActual = "right";
        pasosIniciales = 0;
        puntaje = 0;
        perdio = false;
        driver.clearDisplay(0);
      }

    break;

    case 3:

      if(digitalRead(btnArriba)){
        cambiarDireccion("up");
      }else if(digitalRead(btnAbajo)){
        cambiarDireccion("down");
      }else if(digitalRead(btnIzq)){
        cambiarDireccion("left");
      }else if(digitalRead(btnDer)){
        cambiarDireccion("right");
      }
      
      
      printSerpiente();
      printManzana();
      
      tiempoActual = millis();
      if(tiempoActual-contadorPasos >= velActual){
        Serial3.print(tiempoActual);
        Serial3.print("-");
        Serial3.print(contadorPasos);
        Serial3.print("=");
        Serial3.print(tiempoActual-contadorPasos);
        Serial3.print(" / ");
        Serial3.println(velActual);

        contadorPasos = millis();
        moverSerpiente();
        
        if(!revisarColision(manzanaX,manzanaY,0)){
            if(pasosIniciales >= 1){
              quitarSegmento();
            }else{
              pasosIniciales++;
            }       
        }else{
          initManzana();
          puntaje++;
          float quitar = velEscogida*(puntaje/20.0);
          Serial3.println(quitar);

          velActual = velEscogida+1-quitar;
        }

        if(perdio){
          estado = 4;
          borrarSerpiente();
          perdio = false;
        }

        driver.clearDisplay(0);
      }
    break;

    case 4:
      printNumero(puntaje);
    break;

  }
  
}
