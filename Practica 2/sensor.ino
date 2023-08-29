#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

#define SERVOIZQ 8
#define SERVODER 9
#define PULSADOR1 7
#define PULSADOR2 6
#define LOOP while (true)

// Sensor puntos
int TRIG = 10;
int ECO = 9;

// Sensor game over
int TRIG1 = 12;
int ECO1 = 13;

// Deteccion del objeto
int DURACION;
int DISTANCIA;

int DURACION1;
int DISTANCIA1;

// niveles y puntaje
int nivel = 1;
int incremento = 10;
int pts = 0;

int LEDS[] = {8, 7, 6, 5, 4, 3};
int encendidos = 0;
Estado estado = APAGADO;

enum Estado
{
    MENSAJE,
    MENU,
    JUEGO,
    ENCENDIDO,
    APAGADO,
    PUNTAJES
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
    "Brandon  "};

void mostrarNombres()
{
    lcd.print("Grupo #2");
    for (int i = 0; i < 4; i++)
    {
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
void mostrarPuntaje()
{
    lcd.clear();
    if (puntajes[0][1] != "")
    {
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

void sort()
{
    String puntajeTemporal = "";
    for (int i = 1; i < 10 && puntajes[i][1] != ""; ++i)
    {
        for (int j = i; j > 0 && puntajes[j][1].toInt() > puntajes[j - 1][1].toInt(); --j)
        {
            puntajeTemporal = puntajes[j][1];
            puntajes[j][1] = puntajes[j - 1][1];
            puntajes[j - 1][1] = puntajeTemporal;
        }
    }
}

void registrarPuntaje(unsigned long puntaje)
{
    if (puntajes[9][1] == "")
    {
        for (int i = 0; i < 10; ++i)
        {
            if (puntajes[i][1] == "")
            {
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

void setup()
{
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

    panelJuego();

    pinMode(TRIG, OUTPUT);
    pinMode(TRIG1, OUTPUT);
    pinMode(ECO, INPUT);
    pinMode(ECO1, INPUT);
    for (int i = 0; i < 6; i++)
    {
        pinMode(LEDS[i], OUTPUT);
        digitalWrite(LEDS[i], LOW);
    }
    Serial.begin(9600);
}

void panelJuego()
{
    lcd.begin(16, 2);
    String niv = "Nivel " + String(nivel);
    lcd.print(niv);
    lcd.setCursor(13, 0);
    lcd.print("I");
    lcd.setCursor(14, 0);
    lcd.print("I");
    lcd.setCursor(15, 0);
    lcd.print("I");

    lcd.setCursor(0, 1);
    lcd.print(pts);
}

void encenderSiguienteLed()
{
    if (encendidos < 6)
    {
        digitalWrite(LEDS[encendidos], HIGH);
        encendidos++;
    }
}

void apagarLeds()
{
    for (int i = 0; i < 6; i++)
    {
        digitalWrite(LEDS[i], LOW);
    }
    encendidos = 0;
    incremento += 10;
    nivel++;
    lcd.setCursor(6, 0);
    lcd.print(nivel);
}

void loop()
{
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

    switch (estado)
    {
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
            if (!digitalRead(PULSADOR1))
            {
                pulsacion1 = false;
            }
            if (!pulsacion1 && digitalRead(PULSADOR1))
            {
                pulsacion1 = true;
                opcionI = (opcionI + 1) % 2;
                opcion = (opcion + 1) % 2;
                lcd.setCursor(0, opcionI);
                lcd.print(" ");
                lcd.setCursor(0, opcion);
                lcd.print(">");
            }
            // SELECCIONAR
            if (!digitalRead(PULSADOR2))
            {
                pulsacion2 = false;
            }
            if (!pulsacion2 && digitalRead(PULSADOR2))
            {
                pulsacion2 = true;
                // ELECCION
                if (opcion == 0)
                {
                    estado = Estado::JUEGO;
                    break;
                }
                else if (opcion == 1)
                {
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
        LOOP
        {
            // LADO IZQUIERDO
            if (!digitalRead(PULSADOR1))
            {
                tirado1 = false;
            }
            if (!tirado1 && digitalRead(PULSADOR1))
            {
                pulsacion1 = true;
                tirado1 = true;
                lastTime1 = millis();
            }
            if (pulsacion1)
            {
                if (!abierto1)
                {
                    palancaIzq.write(60);
                }
                if (!abierto1 && millis() - lastTime1 >= 300)
                {
                    lastTime1 = millis();
                    abierto1 = true;
                }
                if (abierto1)
                {
                    palancaIzq.write(0);
                }
                if (abierto1 && millis() - lastTime1 >= 300)
                {
                    lastTime1 = millis();
                    abierto1 = false;
                    pulsacion1 = false;
                }
            }
            // LADO DERECHO
            if (!digitalRead(PULSADOR2))
            {
                tirado2 = false;
            }
            if (!tirado2 && digitalRead(PULSADOR2))
            {
                pulsacion2 = true;
                tirado2 = true;
                lastTime2 = millis();
            }
            if (pulsacion2)
            {
                if (!abierto2)
                {
                    palancaDer.write(120);
                }
                if (!abierto2 && millis() - lastTime2 >= 300)
                {
                    lastTime2 = millis();
                    abierto2 = true;
                }
                if (abierto2)
                {
                    palancaDer.write(180);
                }
                if (abierto2 && millis() - lastTime2 >= 300)
                {
                    lastTime2 = millis();
                    abierto2 = false;
                    pulsacion2 = false;
                }
            }

            // sensor para los puntos
            digitalWrite(TRIG, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG, LOW);
            DURACION = pulseIn(ECO, HIGH);
            DISTANCIA = DURACION / 58.2;
            
            // sensor para las vidas
            digitalWrite(TRIG1, HIGH);
            delayMicroseconds(10);
            digitalWrite(TRIG1, LOW);
            DURACION1 = pulseIn(ECO1, HIGH);
            DISTANCIA1 = DURACION1 / 58.2;
            
            if (DISTANCIA <= 20 && DISTANCIA >= 0) {
                if (estado == APAGADO) {
                estado = ENCENDIDO;
                encenderSiguienteLed();
                pts += incremento;
                lcd.setCursor(0, 1);
                lcd.print(pts);
                }
            } else {
                estado = APAGADO;
            }
            
            if (DISTANCIA1 <= 20 && DISTANCIA1 >= 0) {
                if (vidas > 1) {
                    vidas --;
                if (vidas == 2) {
                    lcd.setCursor(15, 0);
                    lcd.print(" ");
                } else if (vidas == 1) {
                    lcd.setCursor(14, 0);
                    lcd.print(" ");
                }
                } else {
                    lcd.clear();
                    lcd.print("Game Over");
                }
            }
            
            if (encendidos == 6) {
                delay(200);
                apagarLeds();
            }
        }
        break;
    case PUNTAJES:
        lcd.clear();
        mostrarPuntaje();
        posicion = 0;
        LOOP
        {
            // AVANZA
            if (!digitalRead(PULSADOR1))
            {
                pulsacion1 = false;
            }
            if (!pulsacion1 && digitalRead(PULSADOR1))
            {
                pulsacion1 = true;
                posicion = (posicion + 1) % 10;
                mostrarPuntaje();
            }
            // SELECCIONAR
            if (!digitalRead(PULSADOR2))
            {
                pulsacion2 = false;
            }
            if (!pulsacion2 && digitalRead(PULSADOR2))
            {
                pulsacion2 = true;
                estado = Estado::MENU;
                break;
            }
        }
        break;
    }
}
