#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

// JPEG decoder library
// #include <JPEGDecoder.h>
// #include "jpeg1.h"

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

#define TFT_GREY 0x5AEB // New colour
// #define TFT_WHITE 0xFFFFFF
//  #define TFT_BLUE 0x03078A
int blue = 0x0000ff;
int white = 0xFFFFFF;
#define LOAD_GLCD  // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2 // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4 // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6 // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7 // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8 // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// Variables for the menu encoder
#define clk 35
#define data 34

int counter = 0;
int RPM_SERVO_1 = 0;
int RPM_SERVO_2 = 0;
int page = 0;
int Ready = 1;
int submenu = 0;
int last_counter = 0;
bool clk_State;
bool Last_State;
bool dt_State;

///////////////Estados Switch case/////////////////////

#define CERO 0
#define PARAMETROS 1
#define M_PARAMETROS_SERVO1 2
#define M_PARAMETROS_SERVO2 3
#define M_PARAMETROS_CANTIDAD_VUELTAS 4
#define M_PARAMETROS_CANTIDAD_CICLOS 5
#define PRODUCCION 6
#define M_PRODUCCION 7
#define M_PARAMETROS_BACK 8
#define INICIO_VELOCIDAD_SERVO_1 9
#define VELOCIDAD_SERVO_1 10
#define SET_VELOCIDAD_SERVO_1 11
#define BACK_VELOCIDAD_SERVO_1 12
#define INICIO_VELOCIDAD_SERVO_2 13
#define VELOCIDAD_SERVO_2 14
#define SET_VELOCIDAD_SERVO_2 15
#define BACK_VELOCIDAD_SERVO_2 16

uint8_t estado;
bool boton = false;

void setup()
{
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(15, INPUT);
  digitalWrite(15, LOW);
  attachInterrupt(34, isr, CHANGE);
  attachInterrupt(35, isr, CHANGE);

  Last_State = digitalRead(clk);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(blue);
  tft.setCursor(50, 110, 2);
  tft.setTextColor(white, blue);
  tft.setTextSize(6);
  tft.println("MACROLAB");
  delay(2000);
  tft.fillScreen(TFT_WHITE);
  tft.fillRect(100, 5, 270, 90, blue);
  tft.setTextColor(white, blue);
  tft.setTextSize(4);
  tft.setCursor(107, 20, 2);
  tft.println("MACROLAB");
  tft.drawRoundRect(120, 140, 230, 60, 10, TFT_GREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(135, 145, 2);
  tft.println("Parametros");
  tft.drawRoundRect(120, 210, 230, 60, 10, TFT_GREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(135, 215, 2);
  tft.println("Produccion");
  delay(10);
}

void loop()
{
  ///////////////////////////Cambios de Submenu/////////////////////////////

  ///////////////////////////Fin Cambios de Submenu//////////////////////////

  if ((last_counter > counter) || (last_counter < counter) || (digitalRead(15) == HIGH)) // Only print on the LCD when a step is detected or the button is pushed.
  {
    Ready = 1;
    ////////////////////////////////////////////////////INICIO PAGINAS DEL MENU////////////////
    /*PAGINA DE INICIO MACROLAB*/
    if (submenu == 0)
    {
      if (0 < counter && counter < 2)
      {
        estado = PARAMETROS;
        page = 1;
        delay(10);
      }

      if (2 < counter && counter < 4)
      {
        estado = PRODUCCION;
        page = 2;
        delay(10);
      }
    }
    /*FIN PAGINA DE INICIO MACROLAB*/

    /*INICIO PAGINA PARAMETROS*/
    if (submenu == 1)
    {
      if (0 < counter && counter < 2)
      {
        estado = M_PARAMETROS_SERVO1;
        page = 3;
        delay(10);
      }

      if (2 < counter && counter < 4)
      {
        estado = M_PARAMETROS_SERVO2;
        page = 4;
        delay(10);
      }

      if (4 < counter && counter < 6)
      {
        estado = M_PARAMETROS_CANTIDAD_VUELTAS;
        page = 5;
        delay(10);
      }

      if (6 < counter && counter < 8)
      {
        estado = M_PARAMETROS_CANTIDAD_CICLOS;
        page = 6;
        delay(10);
      }

      if (8 < counter && counter < 10)
      {
        estado = M_PARAMETROS_BACK;
        page = 7;
        delay(10);
      }
    }
    /*FIN PAGINA PARAMETROS*/

    /*INICIO PAGINA VELOCIDAD_SERVO_1*/
    if (submenu == 3)
    {
      if (0 < counter && counter < 2)
      {
        estado = INICIO_VELOCIDAD_SERVO_1;
        page = 8;
        delay(10);
      }
      if (2 < counter && counter < 4)
      {
        estado = VELOCIDAD_SERVO_1;
        page = 9;
        delay(10);
      }
      if (4 < counter && counter < 6)
      {
        estado = BACK_VELOCIDAD_SERVO_1;
        page = 10;
        delay(10);
      }
    }
    if (submenu == 4)
    {
      if (0 < counter && counter < 2)
      {
        estado = SET_VELOCIDAD_SERVO_1;
        page = 11;
        delay(10);
      }
    }
    /*FIN PAGINA VELOCIDAD_SERVO_1*/

    /*INICIO PAGINA VELOCIDAD_SERVO_2*/
    if (submenu == 5)
    {
      if (0 < counter && counter < 2)
      {
        estado = INICIO_VELOCIDAD_SERVO_2;
        page = 12;
        delay(10);
      }
      if (2 < counter && counter < 4)
      {
        estado = VELOCIDAD_SERVO_2;
        page = 12;
        delay(10);
      }
      if (4 < counter && counter < 6)
      {
        estado = BACK_VELOCIDAD_SERVO_2;
        page = 13;
        delay(10);
      }
    }
    if (submenu == 6)
    {
      if (0 < counter && counter < 2)
      {
        estado = SET_VELOCIDAD_SERVO_2;
        page = 14;
        delay(10);
      }
    }

    /*FIN PAGINA VELOCIDAD_SERVO_2*/

    /*-----------------------------------------------------------------------------------------*/

    ////////////////////////////////////////////////////INICIO LIMITES ENCODER////////////////

    if (submenu == 0 && counter > 4)
    {
      counter = 4;
    }

    if (submenu == 1 && counter > 10)
    {
      counter = 10;
    }

    if (submenu == 3 && counter > 4)
    {
      counter = 4;
    }

    if (submenu == 5 && counter > 4)
    {
      counter = 4;
    }

    if (counter <= 0)
    {
      counter = 0;
    }
  }

  ////////////////////////////////////////////////////FIN LIMITES ENCODER////////////////.

  /* ------------------------------------------------------------------------------------------*/

  ///////////////////////////////////////////////////INICIO ACCION PULSADOR/////////////////
  if (digitalRead(15) == HIGH && page == 7) // BACK_PARAMETROS
  {
    submenu = 0;
    counter = 0;
    estado = PARAMETROS;
    page = 7;
    delay(300);
  }
  if (digitalRead(15) == HIGH && page == 1)
  {
    submenu = 1;
    counter = 0;
    estado = M_PARAMETROS_SERVO1;
    delay(300);
  }
  ///////////////////////////////////////////////////ACCION PULSADOR VELOCIDAD SERVO 1///
  if (digitalRead(15) == HIGH && page == 3)
  {
    submenu = 3;
    counter = 0;
    estado = INICIO_VELOCIDAD_SERVO_1;
    delay(300);
  }
  if (digitalRead(15) == HIGH && page == 9)
  {
    submenu = 4;
    counter = 0;
    estado = SET_VELOCIDAD_SERVO_1;
    delay(300);
  }
  if (digitalRead(15) == HIGH && page == 10)
  {
    submenu = 1;
    counter = 0;
    estado = M_PARAMETROS_SERVO1;
    delay(300);
  }
  if (digitalRead(15) == HIGH && page == 11)
  {
    submenu = 3;
    counter = 0;
    estado = INICIO_VELOCIDAD_SERVO_1;
    delay(300);
  }
  ///////////////////////////////////////////////////FIN ACCION PULSADOR VELOCIDAD SERVO 1

  ///////////////////////////////////////////////////ACCION PULSADOR VELOCIDAD SERVO 2///
  if (digitalRead(15) == HIGH && page == 4)
  {
    submenu = 5;
    counter = 0;
    estado = INICIO_VELOCIDAD_SERVO_2;
    delay(300);
  }
  if (digitalRead(15) == HIGH && page == 12)
  {
    submenu = 6;
    counter = 0;
    estado = SET_VELOCIDAD_SERVO_2;
    delay(300);
  }
  if (digitalRead(15) == HIGH && page == 13)
  {
    submenu = 1;
    counter = 0;
    estado = M_PARAMETROS_SERVO1;
    delay(300);
  }
  if (digitalRead(15) == HIGH && page == 14)
  {
    submenu = 5;
    counter = 0;
    estado = INICIO_VELOCIDAD_SERVO_2;
    delay(300);
  }
  ///////////////////////////////////////////////////FIN ACCION PULSADOR VELOCIDAD SERVO 2

  ///////////////////////////////////////////////////FIN ACCIONES PULSADORES/////////////////

  /* ------------------------------------------------------------------------------------------*/

  //////////////////////////////////////////////////INICIO SWITCH CASE//////////////////////////

  switch (estado)
  {
  case PARAMETROS:
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(2);
    tft.println(counter);
    tft.setTextSize(2);
    tft.setCursor(50, 0);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.print("PARAMETROS");
    tft.fillRect(100, 5, 270, 90, blue);
    tft.setTextColor(white, blue);
    tft.setTextSize(4);
    tft.setCursor(107, 20, 2);
    tft.println("MACROLAB");
    tft.fillRoundRect(120, 140, 230, 60, 10, TFT_GREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(135, 145, 2);
    tft.println("Parametros");
    tft.drawRoundRect(120, 210, 230, 60, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(135, 215, 2);
    tft.println("Produccion");
    delay(10);
    if (digitalRead(15) == HIGH)
    {
      delay(100);
    }
    estado = CERO;
    break;

  case PRODUCCION:

    tft.fillScreen(white);
    tft.setCursor(0, 0, 2);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.println(counter);
    tft.setTextSize(2);
    tft.setCursor(50, 0);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.print("PRODUCCION");
    tft.fillRect(100, 5, 270, 90, blue);
    tft.setTextColor(white, blue);
    tft.setTextSize(4);
    tft.setCursor(107, 20, 2);
    tft.println("MACROLAB");
    tft.drawRoundRect(120, 140, 230, 60, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(135, 145, 2);
    tft.println("Parametros");
    tft.fillRoundRect(120, 210, 230, 60, 10, TFT_GREY);
    tft.setTextColor(white);
    tft.setTextSize(3);
    tft.setCursor(135, 215, 2);
    tft.println("Produccion");
    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;
  case M_PARAMETROS_SERVO1:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 50, 45, TFT_BLUE);
    tft.setCursor(390, 40);
    tft.setTextSize(0);
    tft.setTextColor(TFT_WHITE);
    tft.println("PARAMETROS");

    tft.fillRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(13, 25, 2);
    tft.println("Velocidad SERVO 1");

    tft.drawRoundRect(10, 90, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 95, 2);
    tft.println("Velocidad SERVO 2");

    tft.drawRoundRect(10, 160, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 165, 2);
    tft.println("Vueltas para PARO");

    tft.drawRoundRect(10, 230, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 235, 2);
    tft.println("Cantidad de Ciclos");
    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;
  case M_PARAMETROS_SERVO2:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 50, 45, TFT_BLUE);
    tft.setCursor(390, 40);
    tft.setTextSize(0);
    tft.setTextColor(TFT_WHITE);
    tft.println("PARAMETROS");
    tft.drawRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 25, 2);
    tft.println("Velocidad SERVO 1");

    tft.fillRoundRect(10, 90, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(13, 95, 2);
    tft.println("Velocidad SERVO 2");

    tft.drawRoundRect(10, 160, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 165, 2);
    tft.println("Vueltas para PARO");

    tft.drawRoundRect(10, 230, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 235, 2);
    tft.println("Cantidad de Ciclos");
    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;
  case M_PARAMETROS_CANTIDAD_VUELTAS:
    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 50, 45, TFT_BLUE);
    tft.setCursor(390, 40);
    tft.setTextSize(0);
    tft.setTextColor(TFT_WHITE);
    tft.println("PARAMETROS");
    tft.drawRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 25, 2);
    tft.println("Velocidad SERVO 1");

    tft.drawRoundRect(10, 90, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 95, 2);
    tft.println("Velocidad SERVO 2");

    tft.fillRoundRect(10, 160, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(13, 165, 2);
    tft.println("Vueltas para PARO");

    tft.drawRoundRect(10, 230, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 235, 2);
    tft.println("Cantidad de Ciclos");
    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;
  case M_PARAMETROS_CANTIDAD_CICLOS:
    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 50, 45, TFT_BLUE);
    tft.setCursor(390, 40);
    tft.setTextSize(0);
    tft.setTextColor(TFT_WHITE);
    tft.println("PARAMETROS");
    tft.drawRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 25, 2);
    tft.println("Velocidad SERVO 1");

    tft.drawRoundRect(10, 90, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 95, 2);
    tft.println("Velocidad SERVO 2");

    tft.drawRoundRect(10, 160, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 165, 2);
    tft.println("Vueltas para PARO");

    tft.fillRoundRect(10, 230, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(13, 235, 2);
    tft.println("Cantidad de Ciclos");
    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(5);
    break;
  case M_PARAMETROS_BACK:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 50, 45, TFT_GREY);
    tft.setCursor(390, 40);
    tft.setTextSize(0);
    tft.setTextColor(TFT_WHITE);
    tft.println("PARAMETROS");
    tft.drawRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 25, 2);
    tft.println("Velocidad SERVO 1");

    tft.drawRoundRect(10, 90, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 95, 2);
    tft.println("Velocidad SERVO 2");

    tft.drawRoundRect(10, 160, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 165, 2);
    tft.println("Vueltas para PARO");

    tft.drawRoundRect(10, 230, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(13, 235, 2);
    tft.println("Cantidad de Ciclos");
    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;

  case INICIO_VELOCIDAD_SERVO_1:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 270, 45, TFT_GREY);
    tft.setCursor(400, 250);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("BACK");
    // tft.drawRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(50, 70, 2);
    tft.println("Velocidad SERVO 1");
    // tft.fillRoundRect
    // tft.drawRoundRect(197, 165, 85, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(200, 120, 2);
    tft.println("RPM");
    tft.setCursor(200, 170, 2);
    tft.println(RPM_SERVO_1);

    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;

  case VELOCIDAD_SERVO_1:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 270, 45, TFT_GREY);
    tft.setCursor(400, 250);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("BACK");
    // tft.drawRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(50, 70, 2);
    tft.println("Velocidad SERVO 1");
    // tft.fillRoundRect
    tft.drawRoundRect(197, 165, 85, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(200, 120, 2);
    tft.println("RPM");
    tft.setCursor(200, 170, 2);
    tft.println(RPM_SERVO_1);

    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;

  case BACK_VELOCIDAD_SERVO_1:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 270, 45, TFT_BLUE);
    tft.setCursor(400, 250);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("BACK");

    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(50, 70, 2);
    tft.println("Velocidad SERVO 1");

    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(200, 120, 2);
    tft.println("RPM");

    tft.setCursor(200, 170, 2);
    tft.println(RPM_SERVO_1);

    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;

  case SET_VELOCIDAD_SERVO_1:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 270, 45, TFT_GREY);
    tft.setCursor(400, 250);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("BACK");

    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(50, 70, 2);
    tft.println("Velocidad SERVO 1");

    tft.fillRoundRect(197, 165, 85, 55, 10, TFT_BLUE);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(200, 120, 2);
    tft.println("RPM");
    tft.setCursor(200, 170, 2);
    tft.println(RPM_SERVO_1);

    if (digitalRead(15))
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;

  ///////////////////////////////////////////////////////////////////////////////////////////////
  case INICIO_VELOCIDAD_SERVO_2:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 270, 45, TFT_GREY);
    tft.setCursor(400, 250);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("BACK");
    // tft.drawRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(50, 70, 2);
    tft.println("Velocidad SERVO 2");
    // tft.fillRoundRect
    // tft.drawRoundRect(197, 165, 85, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(200, 120, 2);
    tft.println("RPM");
    tft.setCursor(200, 170, 2);
    tft.println(RPM_SERVO_2);

    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;

  case VELOCIDAD_SERVO_2:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 270, 45, TFT_GREY);
    tft.setCursor(400, 250);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("BACK");
    // tft.drawRoundRect(10, 20, 360, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(50, 70, 2);
    tft.println("Velocidad SERVO 2");
    // tft.fillRoundRect
    tft.drawRoundRect(197, 165, 85, 55, 10, TFT_GREY);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(200, 120, 2);
    tft.println("RPM");
    tft.setCursor(200, 170, 2);
    tft.println(RPM_SERVO_2);

    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;

  case BACK_VELOCIDAD_SERVO_2:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 270, 45, TFT_BLUE);
    tft.setCursor(400, 250);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("BACK");

    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(50, 70, 2);
    tft.println("Velocidad SERVO 2");

    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(200, 120, 2);
    tft.println("RPM");

    tft.setCursor(200, 170, 2);
    tft.println(RPM_SERVO_2);

    if (digitalRead(15) == HIGH)
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;

  case SET_VELOCIDAD_SERVO_2:

    tft.fillScreen(TFT_WHITE);
    tft.fillCircle(430, 270, 45, TFT_GREY);
    tft.setCursor(400, 250);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("BACK");

    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(50, 70, 2);
    tft.println("Velocidad SERVO 2");

    tft.fillRoundRect(197, 165, 85, 55, 10, TFT_BLUE);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(200, 120, 2);
    tft.println("RPM");
    tft.setCursor(200, 170, 2);
    tft.println(RPM_SERVO_2);

    if (digitalRead(15))
    {
      delay(10);
    }
    estado = CERO;
    delay(10);
    break;
  }

  //////////////////////////////////////////////////FIN SWITCH CASE//////////////////////////
}
/* ------------------------------------------------------------------------------------------*/

///////////////////////////////////////////////////INICIO DE ISR/////////////////////////////

// Interruption vector
void isr()
{
  clk_State = digitalRead(clk);
  dt_State = digitalRead(data);
  if (clk_State != Last_State)
  {
    // void menu();
    // If the data state is different to the clock state, that means the encoder is rotating clockwise
    if (dt_State != clk_State)
    {
      counter++;
      delayMicroseconds(1000);
    }
    else
    {
      counter--;
      delayMicroseconds(1000);
    }
  }
  if (submenu == 4 && clk_State != Last_State)
  {
    // void menu();
    // If the data state is different to the clock state, that means the encoder is rotating clockwise
    if (dt_State != clk_State)
    {
      RPM_SERVO_1++;
      delayMicroseconds(1000);
      estado = SET_VELOCIDAD_SERVO_1;
    }
    else
    {
      RPM_SERVO_1--;
      delayMicroseconds(1000);
      estado = SET_VELOCIDAD_SERVO_1;
    }
  }

  if (submenu == 6 && clk_State != Last_State)
  {
    // void menu();
    // If the data state is different to the clock state, that means the encoder is rotating clockwise
    if (dt_State != clk_State)
    {
      RPM_SERVO_2++;
      delayMicroseconds(1000);
      estado = SET_VELOCIDAD_SERVO_2;
    }
    else
    {
      RPM_SERVO_2--;
      delayMicroseconds(1000);
      estado = SET_VELOCIDAD_SERVO_2;
    }
  }
  Last_State = clk_State; // Updates the previous state of the data with the current state
}
