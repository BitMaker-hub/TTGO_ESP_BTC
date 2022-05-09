/*
 *  Functions
 *      Author: Cryptopasivo
 */


/********** PRINT SCREEN FUNCTIONS *******/
 void printHeader(String newHeaderText, uint16_t TextColor, uint16_t LineColor, uint16_t backGroundColor);
 void draw_button(byte button, int buttonColor, String text, int textColor, int borderColor);
 void printCoin(const unsigned short * imageCoin, int coinIDX);
 void printInitScreen(void);

/********** HTTP FUNCTIONS ************/
String httpGETRequest(const char* serverName);
