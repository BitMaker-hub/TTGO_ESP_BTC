/*
 * Functions
 *  Created on: 08/04/2020
 *      Author: Roger Colet
 */
#include <Arduino.h>
#include "cp_IO.h"
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <WiFi.h>
#include "cp_WIFI.h"
#include "funcs.h"
#include <HTTPClient.h>


/************** VARS *************************************************/
extern TFT_eSPI tft;  
extern const char* ApiCall;
extern sDisplay Display;

/*****************************************************************************
** Function name:  printHeader
** printHeader:    imprime un Header en la pantalla
** printWifiLevel: imprime el estado de la red Wifi
*****************************************************************************/

void printWifiLevel(byte wifiState, uint16_t lineColor, uint16_t backGroundColor){
  const byte headerHeigth = 25;
  uint16_t xPos=D_ANCHO - 30; 
  uint16_t yPos=HEADER_HEIGHT-10;

  tft.fillRect(xPos, 4, 30, HEADER_HEIGHT-6, backGroundColor); //Delete icon area 
  if(wifiState==0) lineColor=TFT_RED;
  for(byte i=1; i<5; i++){
    if(i<=wifiState) tft.fillRect(xPos, yPos, 4, (i*4), lineColor);
    else tft.drawRect(xPos, yPos, 4, (i*4), lineColor);
    xPos+=7; yPos-=4;
  }
  if(wifiState==0) {
    tft.drawLine(xPos, yPos+4, xPos+25, yPos-16, TFT_RED);
    tft.drawLine(xPos, yPos-16, xPos+25, yPos+4, TFT_RED);
  }
}
/****************************************************************************/
String headerText = ""; 
int lastWifiLevel = 10;
int lastBatLevel = 10;
void printHeader(String newHeaderText, uint16_t TextColor, uint16_t LineColor, uint16_t backGroundColor)
{
  /******** Pintar header text ***********/
  //if(newHeaderText != headerText){
    headerText = newHeaderText;
    tft.setTextSize(1);
    tft.fillRect(0, 0, D_ANCHO, HEADER_HEIGHT-1, backGroundColor);//Clear header text area
    //tft.drawFastHLine(0,HEADER_HEIGHT, D_ANCHO, LineColor); //(int32_t x, int32_t y, int32_t w, uint32_t color);
    tft.setTextColor(TextColor, backGroundColor);
    tft.setTextDatum(ML_DATUM); //Midle left
    tft.setFreeFont(FMB9);
    tft.drawString(headerText, 2, (HEADER_HEIGHT/2) - 1, GFXFF); //drawString(const String& string, int32_t poX, int32_t poY, uint8_t font);
  //}
  
  
  /******** Pintar WIFI bars ***********/
  int newWifiLevel=0;
  if(!WiFi.isConnected()) newWifiLevel=0;
  else{
    int rssi = WiFi.RSSI(); 
    //Serial.println("RSSI level - " + String(rssi));
    if(rssi>-40) newWifiLevel = 4;
    else if(rssi>-60) newWifiLevel = 3;
    else if(rssi>-70) newWifiLevel = 2;
    else if(rssi>-85) newWifiLevel = 1;
    else newWifiLevel = 0;
  }
  //if(lastWifiLevel!=newWifiLevel){
    printWifiLevel(newWifiLevel, LineColor, backGroundColor);
    lastWifiLevel = newWifiLevel;
  //}
  
}
/*****************************************************************************
** Function name:  printCoin
** Imprime la moneda seleccionada en pantalla
*****************************************************************************/
void printCoin(const unsigned short * imageCoin, int coinIDX){

    //Print Image
    int imageX = 20;
    int imageY = 50; 
    tft.fillRect(imageX + coins_Width, HEADER_HEIGHT+1, D_ANCHO, D_ALTO - HEADER_HEIGHT, TFT_BLACK);
    tft.setSwapBytes(true);
    tft.pushImage(imageX, imageY, coins_Width, coins_Height, imageCoin); //bitcoin);

    //Check Price lenght, round and print
    byte len = Display.coinPrice[coinIDX].length();
    if(len > 6) {
      int priceRound =  Display.coinPrice[coinIDX].substring(0, len-1).toInt();
      Display.coinPrice[coinIDX] = String(priceRound) + "$";
    }
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(ML_DATUM); //MIDDLE CENTER - MC_DATUM / TOP CENTER - TC_DATUM
    tft.setFreeFont(FMBO18);
    tft.drawString(Display.coinPrice[coinIDX], imageX + coins_Width + 12 , imageY+15, GFXFF); 

    //Print 24hChange
    uint16_t TextColor = TFT_GREEN;
    if(Display.coinChange[coinIDX].toFloat() < 0)  TextColor = TFT_RED;  
    tft.setTextColor(TextColor);
    tft.setFreeFont(FMB12);
    tft.drawString(Display.coinChange[coinIDX], imageX + coins_Width + 12 , imageY+45, GFXFF); 
}
/*****************************************************************************
** Function name:  printInitScreen
   Imprime un la pantalla inicial
*****************************************************************************/
 void printInitScreen(void){
  tft.fillScreen(TFT_BLACK);

  // Swap the colour byte order when rendering
  tft.setSwapBytes(true);
  tft.pushImage(20, 35, logoWidth, logoHeight, logo);
 }

/*****************************************************************************
** Function name:   httpGETRequest
** Descriptions:    client to make a GETrequest
*****************************************************************************/
String httpGETRequest(const char* serverName){
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  Serial.print(" -> Starting request: "); Serial.println(ApiCall);
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

