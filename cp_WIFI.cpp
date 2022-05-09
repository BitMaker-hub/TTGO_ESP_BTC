/*
   Periferic BLE
    Created on: 08/04/2020
        Author: Roger Colet
*/
#include <Arduino.h>
#include <stdint.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include "funcs.h"
#include "cp_WIFI.h"
#include "cp_IO.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

/************** VARS *************************************************/
extern sConfig Config; 
extern sDisplay Display;

enum wifiState {
  WFinit,
  WFconnecting,
  WFconnected,
  WFerror
};

//Your Domain name with URL path or IP address with path
const char* ApiCall= "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin,ethereum,monero,helium&vs_currencies=usd&include_24hr_change=true";

wifiState statusWifi = WFinit;
int wifiCicles=0;

/*****************************************************************************
** Function name:   Init_eleWIFI
** Descriptions:    starts Wifi net and loop values
*****************************************************************************/
void Init_WIFI(void){

  /******** SETUP INIT CONFIG VARS *****/
  Config.SSID="MIWIFI_JG7Y";
  Config.password="GPsPhJTr";
  Config.StaticIP=Config.StaticNM=Config.StaticGW="";
  
  Serial.print("Connecting to WiFi net [");
  Serial.print(Config.SSID); 
  Serial.print("] with password: "); 
  Serial.println(Config.password);

  statusWifi = WFconnecting;
}

/*****************************************************************************
** Function name:   connectingWiFi
** Descriptions:    connects to configured WiFi Network
*****************************************************************************/
void connectingWiFi(void){

  /********** Start Connecting sequence ********/
  if(wifiCicles==0){
    char ssid[Config.SSID.length()+1];
    Config.SSID.toCharArray(ssid,sizeof(ssid));

    if((Config.StaticIP != "")&&(Config.StaticNM != "")&&(Config.StaticGW != "")){
      IPAddress ip; 
      ip.fromString(Config.StaticIP);
      IPAddress netmask; 
      netmask.fromString(Config.StaticNM);
      IPAddress gateway; 
      gateway.fromString(Config.StaticGW);
      WiFi.config(ip, gateway, netmask);
    }

    if(Config.password=="") WiFi.begin(ssid);
    else {
      char password[Config.password.length()+1];
      Config.password.toCharArray(password,sizeof(password));
      WiFi.begin(ssid, password);
    }
  }
  if(wifiCicles++>(ONE_SEC_T*10)){ statusWifi = WFerror; wifiCicles = 0; return; }

  /********** Wait Connecion ********/
  Display.headerLog="Connecting";
  Display.headerTextColor = TFT_DARKGREY;
  for(byte i=0; i<wifiCicles%5; i++) Display.headerLog+=".";
  if (WiFi.status() == WL_CONNECTED) {
      Display.headerLog=WiFi.localIP().toString();
      Serial.println("");
      Serial.println("WiFi connected!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.print("ESP Mac Address: ");
      Serial.println(WiFi.macAddress());
      Serial.print("Subnet Mask: ");
      Serial.println(WiFi.subnetMask());
      Serial.print("Gateway IP: ");
      Serial.println(WiFi.gatewayIP());
      Serial.print("DNS: ");
      Serial.println(WiFi.dnsIP());
      statusWifi = WFconnected;
      wifiCicles = 0;
  }                     
}

/*****************************************************************************
** Function name:   GET_apiCALL
** Descriptions:    api call to Coingecko
*****************************************************************************/
void getData(JSONVar myObject, String coin, String& price, String& profit){
  price = JSON.stringify(myObject[coin]["usd"]) + "$";
  String temp = JSON.stringify(myObject[coin]["usd_24h_change"]);
  if (temp.indexOf("-") == -1) temp = "+" + temp;
  profit = temp.substring(0,5) + "%"; 
}

String response;
unsigned long tCall_api = 0;
void GET_apiCALL(void){

    if(millis() <= tCall_api) return; 
    tCall_api = millis() + (1000 * 10); //Task only every 10 seconds

    response = httpGETRequest(ApiCall);
    Serial.println(response);
    JSONVar myObject = JSON.parse(response);

    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
  
    Serial.print("JSON object = ");
    Serial.println(myObject);

    Serial.print("Bitcoin: ");
    Serial.println(myObject["bitcoin"]["usd"]);

    getData(myObject, "bitcoin", Display.coinPrice[0], Display.coinChange[0]);
    getData(myObject, "ethereum", Display.coinPrice[1], Display.coinChange[1]);
    getData(myObject, "monero", Display.coinPrice[2], Display.coinChange[2]);
    getData(myObject, "helium", Display.coinPrice[3], Display.coinChange[3]);
    Display.refreshPanel = true;
}


/*****************************************************************************
** Function name:   process_eleWIFI
** Descriptions:    Loop where operates with WiFi connection 
*****************************************************************************/

void process_WIFI(void){

  switch(statusWifi){
    case WFconnecting : connectingWiFi(); break;
                        break;
    case WFconnected:   if (WiFi.status() != WL_CONNECTED){ statusWifi = WFerror; wifiCicles = 0;}
                        GET_apiCALL();
                        break;
    case WFerror:       if (WiFi.status() == WL_CONNECT_FAILED) Display.headerLog = "Connection failed"; 
                        else if (WiFi.status() == WL_CONNECTION_LOST) Display.headerLog = "Connection lost";
                        else if (WiFi.status() == WL_DISCONNECTED) Display.headerLog = "Wrong password";
                        else if (WiFi.status() == WL_NO_SSID_AVAIL) Display.headerLog = "SSID not found";
                        else if (WiFi.status() == WL_NO_SHIELD) Display.headerLog = "No WiFi shield";
                        else if (WiFi.status() != WL_IDLE_STATUS) Display.headerLog = "Undefined error";
                        Display.headerTextColor = TFT_RED; 
                        if(wifiCicles++ > ONE_SEC_T*2){ statusWifi = WFconnecting; wifiCicles = 0;}
                        break;
  }
}




