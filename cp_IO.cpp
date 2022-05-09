/*
 * Periferic IO
 *  Created on: 08/04/2020
 *      Author: Roger Colet
 */

#include "cp_IO.h"
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include "funcs.h"


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

/************** VARS *************************************************/
extern sConfig Config; 
extern sDisplay Display;

/*****************************************************************************
** Function name:		Init_IO
** Descriptions:		Inicialitza el port RF segons configuració
*****************************************************************************/
void Init_IO(void){

  /******** INIT DISPLAY ************/
  tft.init();
  tft.setRotation(1);
    
  /******** PRINT INIT SCREEN *****/
  printInitScreen();
  delay(3000);
  tft.fillScreen(TFT_BLACK);

  /******** SETUP INIT DISPLAY VARS *****/
  Display.headerLog="Iniciando..";
  Display.refreshPanel=true;
}

/*****************************************************************************
** Function name:   process_eleIO
** Descriptions:    bucle de treball del modul IO
*****************************************************************************/

unsigned long sCounter = millis(); 
uint16_t headerColor = 0xFCC6;
byte coinIdx = 0; 

void process_IO(void){
  
  /*** PRINT DATA BTC EVERY 1 MIN ***/
  if(Display.refreshPanel) { 
    Display.refreshPanel=false;

    switch(coinIdx){
      case 0: printCoin(bitcoin, 0); 
              headerColor = 0xFCC6; coinIdx = 1;
              break;
      case 1: printCoin(ethereum, 1); 
              headerColor = 0x7C9D; coinIdx = 2;
              break;
      case 2: printCoin(monero, 2); 
              headerColor = 0xFB40; coinIdx = 3;
              break;
      case 3: printCoin(helium, 3); 
              headerColor = 0x3D3F; coinIdx = 0;
              break;
    }
  }

  /*** PRINT HEADER EVERY 1/2 SECOND **/
  if(millis() >= (sCounter + 500)) {  
    sCounter = millis(); 
    printHeader(Display.headerLog, TFT_BLACK, TFT_BLACK, headerColor);
  }
}

