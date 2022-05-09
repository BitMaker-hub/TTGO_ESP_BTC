#include "cp_IO.h"
#include "cp_WIFI.h"

sConfig Config; 
sDisplay Display;

void setup() {
  
  Serial.begin(115200);
  Init_IO();
  Init_WIFI();
}

void loop() {
  process_WIFI();
  process_IO();
  delay(DELAY_T);
}
