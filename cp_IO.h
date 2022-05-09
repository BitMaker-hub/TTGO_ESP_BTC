/*
 *  cp_IO
 *      Author: Cryptopasivo
 */

#include <stdint.h>
#include <Arduino.h>
#include "Lib/images.h"
#include "Lib/Free_Fonts.h"

#define byte uint8_t

/**********************************
 * WIFI Y GENERAL
 **********************************/
#define DELAY_T           10      // retardo del bucle principal (ms)
#define ONE_SEC_T         100     //Vueltas del ciclo para hacer 1 segundo

/*********************************
 *  PANTALLA
 *********************************/
#define D_ANCHO           240     // screen width
#define D_ALTO            128     // screen height
#define D2_ANCHO          125     // half screen width
#define D2_ALTO           64     // half screen height
#define HEADER_HEIGHT     25
#define BUTTON_HEIGHT     25

//Variables de control del Módulo
typedef struct {
   String SSID;                     //Nombre de la red WiFi
   String password;             
   String StaticIP;                 //Ip estática de la red WiFi (ip/netmask/gateway)
   String StaticNM;                 //Netmask estática de la red WiFi
   String StaticGW;                 //Gateway estática de la red WiFi
}sConfig;

typedef struct {
   /*** HEADER y BOTONES ***/
   String headerLog;                //Texto que aparce en el header
   uint16_t headerTextColor;
   /*** PANEL VARS ***/
   bool refreshPanel;               //Indica si es preciso refrescar los datos del panel
   String coinPrice[4];             //Array de strings con el precio
   String coinChange[4];            //Array de strings con el cambio 24h
}sDisplay;

/**********************************************/

void Init_IO(void);
void process_IO(void);



