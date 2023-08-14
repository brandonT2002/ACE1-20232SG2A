#pragma once
#include "Utilidades.h"

bool revisarColision(Nodo *serpiente, Nodo *objeto)
{
    Nodo *aux = serpiente;
    while (aux != NULL)
    {
        if (objeto->x == aux->x && objeto->y == aux->y && aux->activo)
        {
            return true;
        }
        aux = aux->adelante;
    }
    return false;
}

Nodo *initSerpiente()
{
    Nodo *f = (Nodo *)malloc(sizeof(Nodo));
    if (random(1) == 0)
    {
        f->x = 0;
    }
    else
    {
        f->x = 8;
    }
    f->y = random(7);
    f->activo = true;
    f->adelante = NULL;
    xActual = f->x;
    yActual = f->y;
    return f;
}

Nodo *initManzana()
{
    Nodo *m = (Nodo *)malloc(sizeof(Nodo));
    m->x = random(15);
    m->y = random(7);

    while (revisarColision(Serpiente, m))
    {
        m->x = random(15);
        m->y = random(7);
    }

    return m;
}

void moverSerpiente(Nodo *f)
{
    Nodo *aux = f;
    while (aux->adelante != NULL)
    {
        aux = aux->adelante;
    }

    Nodo *n_nodo = (Nodo *)malloc(sizeof(Nodo));

    if (direccionActual == "up")
    {
        n_nodo->x = xActual;
        n_nodo->y = yActual - 1;
    }
    else if (direccionActual == "down")
    {
        n_nodo->x = xActual;
        n_nodo->y = yActual + 1;
    }
    else if (direccionActual == "left")
    {
        n_nodo->x = xActual - 1;
        n_nodo->y = yActual;
    }
    else if (direccionActual == "right")
    {
        n_nodo->x = xActual + 1;
        n_nodo->y = yActual;
    }

    xActual = n_nodo->x;
    yActual = n_nodo->y;
    n_nodo->adelante = NULL;
    n_nodo->activo = true;

    if (xActual < 0 || xActual > 15 || yActual < 0 || yActual > 7)
    {
        perdio = true;
        Serial.print(xActual);
        Serial.print(", ");
        Serial.println(yActual);
        Serial.println("Perdio por colision con borde");
    }

    if (revisarColision(f, n_nodo))
    {
        perdio = true;
        Serial.println("Perdio por colision consigo mismo");
    }

    aux->adelante = n_nodo;
}

Nodo *quitarSegmento(Nodo *f)
{
    Nodo *aux = f->adelante;
    f->activo = false;
    free(f);
    return aux;
}

void cambiarDireccion(String dir)
{
    if (direccionActual == "up")
    {
        if (dir != "down")
        {
            direccionActual = dir;
        }
    }
    else if (direccionActual == "down")
    {
        if (dir != "up")
        {
            direccionActual = dir;
        }
    }
    else if (direccionActual == "left")
    {
        if (dir != "right")
        {
            direccionActual = dir;
        }
    }
    else if (direccionActual == "right")
    {
        if (dir != "left")
        {
            direccionActual = dir;
        }
    }
}

void borrarSerpiente(Nodo *t)
{

    Nodo *tmp;

    while (t != NULL)
    {
        tmp = t;
        t = t->adelante;
        tmp->activo = false;
        free(tmp);
    }
}