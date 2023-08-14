#pragma once
typedef struct nodo {
    int x, y;
    bool activo;
    struct nodo *adelante;
} Nodo;