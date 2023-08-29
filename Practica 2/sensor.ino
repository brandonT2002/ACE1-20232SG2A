#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

#define SERVOIZQ 8
#define SERVODER 9
#define PULSADOR1 7
#define PULSADOR2 6
#define echo 11
#define trig 10
#define led 12

#define LOOP while (true)
const float sonido = 34300.0; // Velocidad del sonido en cm/s
const float tope = 30.0;

float calcularDistancia()
{
  // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
  unsigned long tiempo = pulseIn(echo, HIGH);

  float distancia = tiempo * 0.000001 * sonido / 2.0;
  Serial.print(distancia);
  Serial.print("cm");
  Serial.println();
  delay(500);
 
  return distancia;
}

void iniciarTrigger()
{

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig, LOW);
}

enum Estado
{
    MENSAJE,
    MENU,
    JUEGO,
    PUNTAJES,
    VIDAS
};

Estado estado;

Adafruit_LiquidCrystal lcd(0);

Servo palancaIzq;
Servo palancaDer;

int opcion = 0;
int opcionI = 1;

bool pulsacion1 = false;
bool abierto1 = false;
bool tirado1 = false;

bool pulsacion2 = false;
bool abierto2 = false;
bool tirado2 = false;

unsigned long puntos = 0;

unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;

String nombres[] = {
    "David    ",
    "Sebastian",
    "Patricia ",
    "Brandon  "
};

void mostrarNombres() {
    lcd.print("Grupo #2");
    for (int i = 0; i < 4; i ++) {
        lcd.setCursor(0, 1);
        lcd.print(nombres[i]);
        delay(1000);
    }
}


String puntajes[10][2] = {
    {"PRIMER     LUGAR", ""},
    {"SEGUNDO    LUGAR", ""},
    {"TERCER     LUGAR", ""},
    {"CUARTO     LUGAR", ""},
    {"QUINTO     LUGAR", ""},
    {"SEXTO      LUGAR", ""},
    {"SEPTIMO    LUGAR", ""},
    {"OCTAVO     LUGAR", ""},
    {"NOVENO     LUGAR", ""},
    {"DECIMO     LUGAR", ""},
};

int posicion = 0;
void mostrarPuntaje() {
    lcd.clear();
    if (puntajes[0][1] != "") {
        posicion = puntajes[posicion][1] == "" ? 0 : posicion;
        lcd.setCursor(0, 0);
        lcd.print(puntajes[posicion][0]);
        lcd.setCursor(0, 1);
        lcd.print(puntajes[posicion][1]);
        return;
    }
    posicion = 0;
    lcd.setCursor(0, 0);
    lcd.print("SIN REGISTROS");
}


void mostrarVidas(float distancia) {
  int vidas = 3;
  int i;
  if (distancia < tope){
  	for (int i=3; i>0; i--){
  	vidas = i;
    }
    if (vidas=0){
  	posicion = 0;
  	lcd.setCursor(0, 0);
  	lcd.print("GAME OVER");
  }
  }
  
}



void sort() {
    String puntajeTemporal = "";
    for (int i = 1; i < 10 && puntajes[i][1] != ""; ++ i) {
        for (int j = i; j > 0 && puntajes[j][1].toInt() > puntajes[j - 1][1].toInt(); -- j) {
            puntajeTemporal = puntajes[j][1];
            puntajes[j][1] = puntajes[j - 1][1];
            puntajes[j - 1][1] = puntajeTemporal;
        }
    }
}

void registrarPuntaje(unsigned long puntaje) {
    if (puntajes[9][1] == "") {
        for (int i = 0; i < 10; ++ i) {
            if (puntajes[i][1] == "") {
                puntajes[i][1] = String(puntaje);
                sort();
                return;
            }
        }
        return;
    }
    puntajes[9][1] = String(puntaje);
    sort();
}

void setup() {
    lcd.begin(16, 2);
    // registrarPuntaje(2326548);
    // registrarPuntaje(654389);
    // registrarPuntaje(6548993);
  	palancaIzq.attach(SERVOIZQ);
    palancaDer.attach(SERVODER);
    pinMode(PULSADOR1, INPUT);
    pinMode(PULSADOR2, INPUT);
  	pinMode(trig, OUTPUT);
  	pinMode(echo, INPUT);
  	pinMode(led, OUTPUT);
    palancaIzq.write(0);
    palancaDer.write(180);
    estado = Estado::MENU;

}

void loop() {
  iniciarTrigger();
  float distancia = calcularDistancia();
 
  if (distancia > tope)
  {

    digitalWrite(led, LOW);
    void mostrarVidas();
  }
 else if (distancia < tope)
  {

    digitalWrite(led, HIGH);
   }
  
     switch (estado) {
    case MENSAJE:
        lcd.clear();
        mostrarNombres();
        estado = Estado::MENU;
        break;
    case MENU:
        lcd.clear();
        lcd.print(">");
        lcd.setCursor(2, 0);
        lcd.print("NUEVO JUEGO");
        lcd.setCursor(2, 1);
        lcd.print("PUNTAJES ALTOS");
      	opcion = 0;
      	opcionI = 1;
        LOOP
        {
            // SUBE / BAJA
            if (!digitalRead(PULSADOR1)) {
                pulsacion1 = false;
            }
            if (!pulsacion1 && digitalRead(PULSADOR1)) {
                pulsacion1 = true;
                opcionI = (opcionI + 1) % 2;
                opcion = (opcion + 1) % 2;
                lcd.setCursor(0, opcionI);
                lcd.print(" ");
                lcd.setCursor(0, opcion);
                lcd.print(">");
            }
            // SELECCIONAR
            if (!digitalRead(PULSADOR2)) {
                pulsacion2 = false;
            }
            if (!pulsacion2 && digitalRead(PULSADOR2)) {
                pulsacion2 = true;
                // ELECCION
                if (opcion == 0) {
                    estado = Estado::JUEGO;
                    break;
                }
                else if (opcion == 1) {
                    estado = Estado::PUNTAJES;
                    break;
                }
            }
        }
        break;
    case JUEGO:
        lcd.clear();
        lcd.print("JUGANDO...");
        puntos = 0;
        LOOP {
            // LADO IZQUIERDO
            if(!digitalRead(PULSADOR1)) {
                tirado1 = false;
            }
            if(!tirado1 && digitalRead(PULSADOR1)) {
              	lcd.setCursor(0, 1);
              	lcd.print("IZQ");
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
            if(!digitalRead(PULSADOR2)) {
                tirado2 = false;
            }
            if(!tirado2 && digitalRead(PULSADOR2)) {
              	lcd.setCursor(0, 1);
              	lcd.print("DER");
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
        break;
    case PUNTAJES:
        lcd.clear();
        mostrarPuntaje();
        posicion = 0;
        LOOP {
            // AVANZA
            if (!digitalRead(PULSADOR1)) {
                pulsacion1 = false;
            }
            if (!pulsacion1 && digitalRead(PULSADOR1)) {
                pulsacion1 = true;
                posicion = (posicion + 1) % 10;
                mostrarPuntaje();
            }
            // SELECCIONAR
            if (!digitalRead(PULSADOR2)) {
                pulsacion2 = false;
            }
            if (!pulsacion2 && digitalRead(PULSADOR2)) {
                pulsacion2 = true;
                estado = Estado::MENU;
                break;
            }
        }
        break;
    }
}


