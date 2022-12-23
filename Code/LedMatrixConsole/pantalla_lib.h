#ifndef PANTALLA_LIB_H
#define PANTALLA_LIB_H

#include <Arduino.h>

void cambiaColor(int matriz[], uint32_t color);

void IniciaLeds(int pantalla[][4],int leds[][4]);

void dibujaLineaVertical(int pantalla[][4], int columna, uint32_t color);

void dibujaUnaLineaVertical(int pantalla[][4], int columna, uint32_t color);

void dibujaLineaHorizontal(int pantalla[][4], int fila, uint32_t color);

void dibujaUnaLineaHorizontal(int pantalla[][4], int fila, uint32_t color);

uint32_t creaColor(int r, int g ,int b);
  
#endif
