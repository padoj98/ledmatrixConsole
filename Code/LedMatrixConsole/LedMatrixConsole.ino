#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include "EasyBuzzer.h"
#include <LiquidCrystal.h>

#include "pantalla_lib.h"


//Designamos nuestro pin de datos
#define PIN 9
#define ZumbadorPin 10

//Designamos cuantos pixeles tenemos en nuestra cinta led RGB
#define NUMPIXELS      256

//Definimos el número de pixeles de la cinta y el pin de datos
//   Parámetro 1 = número de pixeles de la cinta
//   Parámetro 2 = número de pin de datos del Arduino
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//VARIABLES DE INTERRUPCIONES BOTONES
int chequeo = 0;          //Evento interrupcion botones
unsigned long  rebote1 = 0;
unsigned long  rebote2 = 0;
unsigned long  rebote3 = 0;
unsigned long  rebote4 = 0;

//Estados
const int INICIO = 0;//Animacion de encendido
const int ELEGIR = 1;   //Elegimos cancion
const int SIMON = 2;
const int JUEGO = 3;      //Empieza el juego con la cancion seleccionada
const int FINAL = 4;      //Animacion de fallo y vuelta a ENCENDIENDO
const int PIANO = 5;      //Modo piano


int estado = INICIO;

int puntuacion_aux = 0;   //Auxiliar en recuento de puntuacion(tambien sirve para reproducir diferentes notas en estado JUEGO)
int puntuacion_total = 0; //Contador de puntuacion conseguida
int posLCD=17;            //Contador pos LCD para mover el texto
int aux = 0;              //Contador auxiliar para animacion de puntuacion final
// VARIABLERS PIANO
int c1 = 0;
int conter = 9;

//VARIABLES SIMON SAYS
const int MAX_LEVEL = 100;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level = 1;
int velocity = 1000;

int c=261; 
int d=294;
int e=329;
int f=349;
int g=391;
int gS=415;
int a=440;
int aS=455;
int b=466;
int cH=523;
int cSH=554;
int dH=587;
int dSH=622;
int eH=659;
int fH=698;
int fSH=740;
int gH=783;
int gSH=830;
int aH=880;

//FILA / COLUMNA
int matriz30[]={0,1,2,3,28,29,30,31,32,33,34,35,60,61,62,63};
int matriz20[]={64,65,66,67,92,93,94,95,96,97,98,99,124,125,126,127};
int matriz10[]={128,129,130,131,156,157,158,159,160,161,162,163,188,189,190,191};
int matriz00[]={192,193,194,195,220,221,222,223,224,225,226,227,252,253,254,255};

int matriz31[]={4,5,6,7,24,25,26,27,36,37,38,39,56,57,58,59};
int matriz21[]={68,69,70,71,88,89,90,91,100,101,102,103,120,121,122,123};
int matriz11[]={132,133,134,135,152,153,154,155,164,165,166,167,184,185,186,187};
int matriz01[]={196,197,198,199,216,217,218,219,228,229,230,231,248,249,250,251};

int matriz32[]={8,9,10,11,20,21,22,23,40,41,42,43,52,53,54,55};
int matriz22[]={72,73,74,75,84,85,86,87,104,105,106,107,116,117,118,119};
int matriz12[]={136,137,138,139,148,149,150,151,168,169,170,171,180,181,182,183};
int matriz02[]={200,201,202,203,212,213,214,215,232,233,234,235,244,245,246,247};

int matriz33[]={16,17,18,19,12,13,14,15,48,49,50,51,44,45,46,47};
int matriz23[]={80,81,82,83,76,77,78,79,112,113,114,115,108,109,110,111};
int matriz13[]={144,145,146,147,140,141,142,143,176,177,178,179,172,173,174,175};
int matriz03[]={208,209,210,211,204,205,206,207,240,241,242,243,236,237,238,239};

//MATRIZ QUE CAMBIA A 1 SI EL SUPERLED(4x4 leds) ESTA ENCENDIDO
int leds[4][4]={{0,0,0,0},
                {0,0,0,0},
                {0,0,0,0},
                {0,0,0,0}};

//Matriz pantalla (16x16)
int mi_pantalla[4][4]={ {matriz00, matriz01, matriz02,matriz03},
                     {matriz10, matriz11, matriz12, matriz13},
                     {matriz20, matriz21, matriz22, matriz23},
                     {matriz30, matriz31, matriz32, matriz33}};    

uint32_t rojo = creaColor(150,0,0);
uint32_t verde = creaColor(0,150,0);
uint32_t azul = creaColor(0,0,150);
uint32_t amarillo = creaColor(255, 233, 0);
uint32_t blanco = creaColor(150,150,150);
uint32_t apagado = creaColor(0,0,0);

LiquidCrystal lcd(12, 11, 7, 6, 5, 4); //TODO: CAMBIAR PUERTOS?

void setup() {
  uint32_t azul = creaColor(0,0,150);
  uint32_t verde = creaColor(0,150,0);
// Inicializamos nuestra cinta led RGB
  pinMode(ZumbadorPin, OUTPUT); // definimos la variable ZumbadorPin como una salida.

  pinMode(3, INPUT_PULLUP);           
  attachInterrupt(digitalPinToInterrupt(3), Boton1, RISING); //Activamos pin 3 como Interrupción Boton1
  
  pinMode(2, INPUT_PULLUP);           
  attachInterrupt(digitalPinToInterrupt(2), Boton2, RISING); //Activamos pin 2 como Interrupción Boton2
  
  pinMode(0, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(0), Boton3, RISING);//Activamos pin 0 como Interrupción Boton3

  pinMode(1, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(1), Boton4, RISING);//Activamos pin 1 como Interrupción Boton4

   lcd.begin(16, 2);
   lcd.clear();
   
  IniciaLeds( mi_pantalla, leds);

}

void(* resetFunc) (void) = 0;

void loop() {

  //Definimos nuestras variables de tipo uint32 para cada color que utilizaremos


   int delayval = 50; // Pausa de cincuenta milisegundos

  /*dibujaLineaVertical(mi_pantalla, 2, apagado);
  dibujaLineaHorizontal(mi_pantalla, 0, verde);
  
  delay(1000);
  dibujaLineaHorizontal(mi_pantalla, 0, apagado);
  dibujaLineaHorizontal(mi_pantalla, 2, rojo);
  delay(1000);
  dibujaLineaHorizontal(mi_pantalla, 2, apagado);
  dibujaLineaVertical(mi_pantalla, 2, azul);
  delay(1000);*/
  switch(estado){
    case (INICIO):
      lcd.setCursor(1,0);
      lcd.print("INDICA ACCION:");
      lcd.setCursor(3,1);
      lcd.print("1.GameA 2.GameB 3.GameC");
      lcd.setCursor(16,1);
      estado= ELEGIR;
    break;
    
    case (ELEGIR): //INICIO
    //Movemos la segunda linea del LCD
      lcd.scrollDisplayLeft();
      lcd.setCursor(posLCD-16,0);
      lcd.print("INDICA ACCION:");
        delay(500);
      posLCD++;
      if(posLCD==35){ //TODO: Aumentar si es mas larga la 2a linea
        posLCD=17;
        lcd.clear();
        lcd.setCursor(3,1);
        lcd.print("1.GameA 2.GameB 3.GameC");
      }
      if (chequeo == 1){ //PULSAMOS BOTON 1
        chequeo=0;
        tone(ZumbadorPin, a, 500);
        lcd.noAutoscroll();
        estado= PIANO;      
          //delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
      }
      if (chequeo == 2){ //PULSAMOS BOTON 2
        chequeo=0;
        tone(ZumbadorPin, c, 500);
        lcd.noAutoscroll();
        lcd.noAutoscroll();
        estado= SIMON;    
      }
      if (chequeo == 3){ //PULSAMOS BOTON 3
        chequeo=0;
        tone(ZumbadorPin, c, 500);
        lcd.noAutoscroll();
        estado=0;
      }
      if (chequeo == 4){ //PULSAMOS BOTON 4
        chequeo=0;
        tone(ZumbadorPin, c, 500);
        lcd.noAutoscroll();
        estado=0;
      }
    break;
    
    case (PIANO) : 
      while (digitalRead(3) == LOW){ //Mientras Boton1 pulsado
        c1 = 1;
        tone(ZumbadorPin, 440, 15);
      }
      while (digitalRead(2) == LOW){ //Mientras Boton2 pulsado
        c1 = 2;
        tone(ZumbadorPin, 261.63, 25);
      }
      while (digitalRead(0) == LOW){ //Mientras Boton3 pulsado
          c1 = 3;
          tone(ZumbadorPin, 392, 25);
      }
      while (digitalRead(1) == LOW){ //Mientras Boton4 pulsado
        c1 = 4;
        tone(ZumbadorPin, 329.63, 25);
      }
      /***********************/
      if (c1 != 0){ //TODO: Meter en cada while?^
        uint32_t randomColor = creaColor(random(0, 255),random(0, 255),random(0, 255));
        dibujaUnaLineaVertical(mi_pantalla, c1-1, randomColor);
        c1=0;
      }
      delay(50);  
    break;

    case (SIMON): //SIMONSAYS ****************************************************************
      if (level == 1){
          generate_sequence();//generate a sequence;
      }
  
        show_sequence();    //show the sequence
        get_sequence();     //wait for your sequence
    break;

    case (FINAL): //*************************************************************************
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Puntuacion final:");
      lcd.setCursor(0,1);
  
      while (aux < puntuacion_total / 5){
        tone(ZumbadorPin, aux * 100, 80); //cada vez más agudo
        cambiaColor(mi_pantalla[aux][aux], azul);
        aux++;
        delay(100);//Es el unico momento donde no me importa hacer delay...(Para no complicar más con flags de timer...)
        chequeo = 0;
      }
      if (chequeo){
        resetFunc();
      }
      break;
    
    default: //*********************************************************************************
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error...");
  }
}



void show_sequence(){
  dibujaUnaLineaVertical(mi_pantalla, 0, apagado);

  for (int i = 0; i < level; i++)
  {
    if (sequence[i] == 1)
    {
      //Enciende led0
      dibujaUnaLineaVertical(mi_pantalla, 0, rojo);
      tone(ZumbadorPin, 440, 50);
    }
    if (sequence[i] == 2)
    {
      //Enciende led1
      dibujaUnaLineaVertical(mi_pantalla, 1, amarillo);
      tone(ZumbadorPin, 261.63, 50);
    }
    if (sequence[i] == 3)
    {
      //Enciende fila 3
      dibujaUnaLineaVertical(mi_pantalla, 2, verde);
      tone(ZumbadorPin, 392, 50);
    }
    if (sequence[i] == 4)
    {
      //Enciende fila 4
      dibujaUnaLineaVertical(mi_pantalla, 3, azul);

      tone(ZumbadorPin, 329.63, 50);
    }
    delay(velocity);
    //Apagamos la matriz
    dibujaUnaLineaVertical(mi_pantalla, 3, apagado);
    delay(200);
  }
  chequeo == 0;
}

void get_sequence(){
  int flag = 0; //this flag indicates if the sequence is correct

  for (int i = 0; i < level; i++){
    flag = 0;
    while (flag == 0){
      if (chequeo == 1){ //Boton1
        chequeo = 0;
        dibujaUnaLineaVertical(mi_pantalla, 0, rojo);
        tone(ZumbadorPin, 440, 50);

        your_sequence[i] = 1;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        dibujaUnaLineaVertical(mi_pantalla, 0, apagado);
      }
      if (chequeo == 2){ //Boton2
        chequeo = 0;
        dibujaUnaLineaVertical(mi_pantalla, 1, amarillo);
        tone(ZumbadorPin, 261.63, 50);

        your_sequence[i] = 2;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i]){
          wrong_sequence();
          return;
        }
        dibujaUnaLineaVertical(mi_pantalla, 0, apagado);
      }
      if (chequeo == 3){
        chequeo = 0;
        dibujaUnaLineaVertical(mi_pantalla, 2, verde);
        tone(ZumbadorPin, 392, 50);

        your_sequence[i] = 3;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        dibujaUnaLineaVertical(mi_pantalla, 3, apagado);
      }
      if (chequeo == 4){
        chequeo = 0;
        dibujaUnaLineaVertical(mi_pantalla, 3, azul);
        tone(ZumbadorPin, 329.63, 50);

        your_sequence[i] = 4;
        flag = 1;
        delay(200);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        dibujaUnaLineaVertical(mi_pantalla, 4, apagado);
      }
    }
  }
  right_sequence();
}

void generate_sequence()
{
  randomSeed(millis()); //in this way is really random!!!

  for (int i = 0; i < MAX_LEVEL; i++)
  {
    sequence[i] = random(1, 5);
    Serial.print(sequence[i]);
  }
}

void wrong_sequence()
{
  for (unsigned i = 0; i <4; i++)
  {
    dibujaUnaLineaVertical(mi_pantalla, i, rojo);
  }
  tone(ZumbadorPin, 50, 500); // Play thisNote for duration.
  delay(500);
  dibujaUnaLineaVertical(mi_pantalla, 4, apagado);

  puntuacion_total = (level - 1) * 5;
  estado = FINAL;
}

void right_sequence(){

  dibujaUnaLineaVertical(mi_pantalla, 4, apagado);
  delay(250);

  for (int i = 0; i < 4; i++)
  {
    dibujaUnaLineaVertical(mi_pantalla, i, verde);
  }

  tone(ZumbadorPin, 1080, 500);
  delay(500);

  dibujaUnaLineaVertical(mi_pantalla, 4, apagado);
  delay(500);

  if (level < MAX_LEVEL)
    level++;

  velocity -= 50; //increase difficulty
}

void Boton1(){
  static unsigned long ultimo_rebote1 = 0;
  rebote1 = millis();

  if (rebote1 - ultimo_rebote1 > 250)
  {
    chequeo = 1;
  }
  ultimo_rebote1 = rebote1;
}
void Boton2(){
  static unsigned long ultimo_rebote2 = 0;
  rebote2 = millis();

  if (rebote2 - ultimo_rebote2 > 250)
  {
    chequeo = 2;
  }
  ultimo_rebote2 = rebote2;
}
void Boton3(){
  static unsigned long ultimo_rebote3 = 0;
  rebote3 = millis();

  if (rebote3 - ultimo_rebote3 > 250)
  {
    chequeo = 3;
  }
  ultimo_rebote3 = rebote3;
}
void Boton4(){
  static unsigned long ultimo_rebote4 = 0;
  rebote4 = millis();

  if (rebote4 - ultimo_rebote4 > 250)
  {
    chequeo = 4;
  }
  ultimo_rebote4 = rebote4;
}

void cancion(){
  tone(ZumbadorPin, a, 500);
    delay(500+50);
    tone(ZumbadorPin, a, 500);
    delay(500+50);     
    tone(ZumbadorPin, a, 500); 
    delay(500+50);
    tone(ZumbadorPin, f, 350);  
    delay(350+50);
    tone(ZumbadorPin, cH, 150);
    delay(150+50); 
    
    tone(ZumbadorPin, a, 500);
    delay(500+50);
    tone(ZumbadorPin, f, 350);
    delay(350+50);
    tone(ZumbadorPin, cH, 150);
    delay(150+50);
    tone(ZumbadorPin, a, 1000);   
    delay(1000+50);
   
    tone(ZumbadorPin, eH, 500);
    delay(500+50);
    tone(ZumbadorPin, eH, 500);
    delay(500+50);
    tone(ZumbadorPin, eH, 500); 
    delay(500+50);   
    tone(ZumbadorPin, fH, 350);
    delay(350+50); 
    tone(ZumbadorPin, cH, 150);
    delay(150+50);
    
    tone(ZumbadorPin, gS, 500);
    delay(500+50);
    tone(ZumbadorPin, f, 350);
    delay(350+50);
    tone(ZumbadorPin, cH, 150);
    delay(150+50);
    tone(ZumbadorPin, a, 1000);
    delay(1000+50);
    
    tone(ZumbadorPin, aH, 500);
    delay(500+50);
    tone(ZumbadorPin, a, 350);
    delay(350+50); 
    tone(ZumbadorPin, a, 150);
    delay(150+50);
    tone(ZumbadorPin, aH, 500);
    delay(500+50);
    tone(ZumbadorPin, gSH, 250); 
    delay(250+50);
    tone(ZumbadorPin, gH, 250);
    delay(250+50);
    
    tone(ZumbadorPin, fSH, 125);
    delay(125+50);
    tone(ZumbadorPin, fH, 125);
    delay(125+50);    
    tone(ZumbadorPin, fSH, 250);
    delay(250+50);
    delay(250);
    tone(ZumbadorPin, aS, 250);
    delay(250+50);    
    tone(ZumbadorPin, dSH, 500); 
    delay(500+50); 
    tone(ZumbadorPin, dH, 250); 
    delay(250+50); 
    tone(ZumbadorPin, cSH, 250);
    delay(250+50);  
    
    // virtuosismo :)))
    
    tone(ZumbadorPin, cH, 125);
    delay(125+50);  
    tone(ZumbadorPin, b, 125); 
    delay(125+50); 
    tone(ZumbadorPin, cH, 250); 
    delay(250+50);     
    delay(250);
    tone(ZumbadorPin, f, 125); 
    delay(125+50); 
    tone(ZumbadorPin, gS, 500); 
    delay(500+50); 
    tone(ZumbadorPin, f, 375); 
    delay(375+50); 
    tone(ZumbadorPin, a, 125);
    delay(125+50); 
    
    tone(ZumbadorPin, cH, 500); 
    delay(500+50);
    tone(ZumbadorPin, a, 375);  
    delay(375+50);
    tone(ZumbadorPin, cH, 125); 
    delay(125+50);
    tone(ZumbadorPin, eH, 1000); 
    delay(1000+50);
 
    
    tone(ZumbadorPin, aH, 500);
    delay(500+50);
    tone(ZumbadorPin, a, 350); 
    delay(350+50);
    tone(ZumbadorPin, a, 150);
    delay(150+50);
    tone(ZumbadorPin, aH, 500);
    delay(500+50);
    tone(ZumbadorPin, gSH, 250);
    delay(250+50); 
    tone(ZumbadorPin, gH, 250);
    delay(250+50);
    
    tone(ZumbadorPin, fSH, 125);
    delay(125+50);
    tone(ZumbadorPin, fH, 125);  
    delay(125+50);  
    tone(ZumbadorPin, fSH, 250);
    delay(250+50);
    delay(250);
    tone(ZumbadorPin, aS, 250);  
    delay(250+50);  
    tone(ZumbadorPin, dSH, 500);  
    delay(500+50);
    tone(ZumbadorPin, dH, 250);  
    delay(250+50);
    tone(ZumbadorPin, cSH, 250);
    delay(250+50);  
 
    
    tone(ZumbadorPin, cH, 125);  
    delay(125+50);
    tone(ZumbadorPin, b, 125);  
    delay(125+50);
    tone(ZumbadorPin, cH, 250);
    delay(250+50);      
    delay(250);
    tone(ZumbadorPin, f, 250); 
    delay(250+50); 
    tone(ZumbadorPin, gS, 500); 
    delay(500+50); 
    tone(ZumbadorPin, f, 375);  
    delay(375+50);
    tone(ZumbadorPin, cH, 125); 
    delay(125+50);
           
    tone(ZumbadorPin, a, 500); 
    delay(500+50);           
    tone(ZumbadorPin, f, 375);  
    delay(375+50);          
    tone(ZumbadorPin, c, 125); 
    delay(125+50);  
    tone(ZumbadorPin, a, 1000);
    delay(1000+50); 
}
