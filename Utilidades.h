#pragma once
#include <LedControl.h>
#include "Nodo.h"

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
Nodo *Serpiente;
Nodo *Manzana;
int contadorPasos = 0;
int velEscogida;
int velActual = 250;
int pasosIniciales = 0;
bool perdio = false;
int seleccionDificultad = 1;
int estado = 1;

LedControl driver = LedControl(12, 10, 11, 1);

void inicializarPines() {
    for (int i = 0; i < 8; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }

    for (int i = 46; i < 57; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }

    for (int i = 22; i < 29; i++)
    {
        pinMode(i, INPUT);
    }
}