#include "pantalla_lib.h"
#include <Adafruit_NeoPixel.h>

#define PIN 9
#define NUMPIXELS      256
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void cambiaColor(int matriz[], uint32_t color){
  for(int i=0;i< 16;i++){
    pixels.setPixelColor(matriz[i], color);
  }
  pixels.show();
}

void IniciaLeds(int pantalla[][4],int leds[][4]){
  uint32_t apagado = pixels.Color(0,0,0);
  pixels.begin();
  pixels.setBrightness(65);
  for(int i=0;i< 4;i++){
    for(int j=0;j< 4;j++){
      leds[i][j]=0;
      cambiaColor(pantalla[i][j], apagado);
    }
  }
  pixels.show();
}

void dibujaLineaVertical(int pantalla[][4], int columna, uint32_t color){
  for(int i=0; i< 4; i++){   
    cambiaColor(pantalla[i][columna], color); 
  }
  pixels.show();
}

void dibujaUnaLineaVertical(int pantalla[][4], int columna, uint32_t color){
  for(int i=0;i<4; i++){ //Apagamos todos los leds menos la linea del boton pulsado
    dibujaLineaVertical(pantalla, i, creaColor(0,0,0)); 
    if(i == columna){
      dibujaLineaVertical(pantalla, i, color);
      }
  }
  pixels.show();
}

void dibujaLineaHorizontal(int pantalla[][4], int fila, uint32_t color){
  for(int i=0; i< 4; i++){   
    cambiaColor(pantalla[fila][i], color); 
  }
  pixels.show();
}

void dibujaUnaLineaHorizontal(int pantalla[][4], int fila, uint32_t color){
  for(int i=0;i<4; i++){ //Apagamos todos los leds menos la linea del boton pulsado
    dibujaLineaHorizontal(pantalla, i, creaColor(0,0,0)); 
    if(i == fila){
      dibujaLineaHorizontal(pantalla, i, color);
      }
  }
  pixels.show();
}

uint32_t creaColor(int r, int g ,int b){
  return pixels.Color(r, g, b);
}
