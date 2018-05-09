
// Simple NeoPixel test.  Lights just a few pixels at a time so a
// 1m strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip (the end wires, if
// any, are no indication -- look instead for the data direction
// arrows printed on the strip).
 
#include <Adafruit_NeoPixel.h>
 
#define PIN      6
#define N_LEDS 119

//mpstat -A for cpu usage
//sensors for temps
//tegrastates for gpu usage, found in nvidia user folder
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
float info[46];
int it = 0;
float mode = 3.0;
int watsetime = 0;
uint16_t ledI = 0;
byte infoChanged = 0;
int fadeMagnitude = 0;
int fadeR = 85;
int fadeG = 0;
int fadeB = 0;
int fadeRotate = 0;
int fadeUpDown = 0;
  
void setup() {
  info[0] = 1.0;
  strip.begin();
  Serial.begin(57600);
  uint32_t colorIterator = 0;
}
 
void loop() {
  //checkSerial();
  mode = info[0];
  //Serial.println(mode);
  if ((millis() % 60) == 0){
    if (mode == 0.0){
      //watsetime = 0;
      turnoff();
    }
    else if (mode == 1.0) {
      flow();
    }
    else if (mode == 2.0){
       chase(strip.Color(025, 0, 0)); // Red
       chase(strip.Color(0, 025, 0)); // Green
       chase(strip.Color(0, 0, 025)); // Blue
    } else if (mode == 3.0){
      if (infoChanged){
        displayTemps();
      }
    } else if (mode == 4.0){
      if (infoChanged){
        displaySpeeds();
      }
    } else if (mode == 5.0) {
      fade();
    } else if (mode == 6.0){
      rotateGradiant();
    } else {
      turnoff();
    }
    if (ledI == 120){
      ledI = 0; 
      Serial.println(mode);
      //for (int i = 0; i < 27; i++){
      //  float currentNum = info[it];
      //  Serial.println(currentNum);
      //  Serial.println(i);
      //}
      
    }
  }
}
 
void serialEvent(){
  //it = 0;
  //Serial.println(it);
  while (Serial.available() > 0)
  {
    //Serial.println("Hello");
    //Serial.readBytesUntil('\t', info, 48);
    //info[i] = Serial.parseFloat();
    char tempBytes[4];
    Serial.readBytesUntil((char) 0xFF, tempBytes, 4);
    //float currentNum = (float) tempBytes[0];
    memcpy(info + it, tempBytes, sizeof(float));
    float currentNum = info[it];
    Serial.println(currentNum);
    Serial.println(it);
    if (it == 0){
      //if (currentNum < -100.0 || currentNum > 100.0)
      if (currentNum == -1000.0){
        it = 0;
      } else {
        it++;
      }
    } else {
    //float currentNum = info[i];
    it++;
    }
    fadeMagnitude = 0;
    fadeR = 85;
    fadeG = 0;
    fadeB = 0;
    fadeRotate = 0;
    fadeUpDown = 0;
    //Serial.println(currentNum);
    //Serial.print(tempBytes[0]);
    //Serial.print(tempBytes[1]);
    //Serial.print(tempBytes[2]);
    //Serial.println(tempBytes[3]);
    //Serial.println(it);
    //if (it == 12){
    ////  for (int j = 0; j < it; j++){
    ////    Serial.println(info[j]);
    ////    Serial.print(j);
    ////    Serial.print(" ");
    ////    Serial.println(it);
    ////  }
      //it = 0;
    ////}
    //if (it == 49){
    //  it = 0;
    //}
    
  }
  it = 0;
  Serial.read();
  infoChanged = 1;
  /*Serial.print(info[0]);
  Serial.print(info[1]); + 40
  Serial.print(info[2]);
  Serial.print(info[3]);
  Serial.print(info[4]);
  Serial.print(info[5]);
  Serial.print(info[6]);
  Serial.print(info[7]);
  Serial.print(info[9]);
  Serial.println(info[10]);*/
}
static void chase(uint32_t c) {
  //for(uint16_t i=0; i<60; i++) {//strip.numPixels()+30
      strip.setPixelColor(ledI  , c); // Draw new pixel
      strip.setPixelColor(ledI-30, 0); // Erase pixel a few steps back
      strip.show();
      ledI++;
      //while ((millis() % 25) != 0){
      //  watsetime++;
      //}
  //}
}
static void flow() {
  //for (uint16_t i=0; i < 60; i++){
   /*strip.setPixelColor(ledI  , strip.Color(255, 000, 000)); // Draw new pixel
   strip.setPixelColor((ledI+10)%60  , strip.Color(155, 155, 000));
   strip.setPixelColor((ledI+20)%60  , strip.Color(000, 255, 000));
   strip.setPixelColor((ledI+30)%60  , strip.Color(000, 155, 155));
   strip.setPixelColor((ledI+40)%60  , strip.Color(000, 000, 255));
   strip.setPixelColor((ledI+50)%60  , strip.Color(155, 000, 155));
   
   strip.setPixelColor(ledI + 60  , strip.Color(255, 000, 000)); // Draw new pixel
   strip.setPixelColor((ledI+10)%60 + 60  , strip.Color(155, 155, 000));
   strip.setPixelColor((ledI+20)%60 + 60  , strip.Color(000, 255, 000));
   strip.setPixelColor((ledI+30)%60 + 60  , strip.Color(000, 155, 155));
   strip.setPixelColor((ledI+40)%60 + 60  , strip.Color(000, 000, 255));
   strip.setPixelColor((ledI+50)%60 + 60  , strip.Color(155, 000, 155));*/
//   colorIterator++;
   strip.setPixelColor((ledI) % 40  , strip.Color(255, 000, 000)); // Draw new pixel
   strip.setPixelColor((ledI+7)%40  , strip.Color(155, 155, 000));
   strip.setPixelColor((ledI+14)%40  , strip.Color(000, 255, 000));
   strip.setPixelColor((ledI+21)%40  , strip.Color(000, 155, 155));
   strip.setPixelColor((ledI+28)%40  , strip.Color(000, 000, 255));
   strip.setPixelColor((ledI+35)%40  , strip.Color(155, 000, 155));
   
   strip.setPixelColor((ledI) % 40 + 40  , strip.Color(255, 000, 000)); // Draw new pixel
   strip.setPixelColor((ledI+7)%40 + 40  , strip.Color(155, 155, 000));
   strip.setPixelColor((ledI+14)%40 + 40  , strip.Color(000, 255, 000));
   strip.setPixelColor((ledI+21)%40 + 40  , strip.Color(000, 155, 155));
   strip.setPixelColor((ledI+28)%40 + 40  , strip.Color(000, 000, 255));
   strip.setPixelColor((ledI+35)%40 + 40  , strip.Color(155, 000, 155));
   
   strip.setPixelColor((ledI) % 40 + 80  , strip.Color(255, 000, 000)); // Draw new pixel
   strip.setPixelColor((ledI+7)%40 + 80  , strip.Color(155, 155, 000));
   strip.setPixelColor((ledI+14)%40 + 80  , strip.Color(000, 255, 000));
   strip.setPixelColor((ledI+21)%40 + 80  , strip.Color(000, 155, 155));
   strip.setPixelColor((ledI+28)%40 + 80  , strip.Color(000, 000, 255));
   strip.setPixelColor((ledI+35)%40 + 80  , strip.Color(155, 000, 155));
   strip.show();
   ledI++;
   //if (ledI++ == 40) {
   //  ledI = 0;
   //}
   //delay(35);
  //}
}

static void fade(){
  if (fadeRotate == 0){
    for (int k = 0; k < 120; k++){
      strip.setPixelColor(k  , strip.Color(12*fadeMagnitude, 000, 000));
    } 
  } else if (fadeRotate == 1){
    for (int k = 0; k < 120; k++){
      strip.setPixelColor(k  , strip.Color(7*fadeMagnitude, 7*fadeMagnitude, 000));
    } 
  } else if (fadeRotate == 2){
    for (int k = 0; k < 120; k++){
      strip.setPixelColor(k  , strip.Color(000, 12*fadeMagnitude, 000));
    } 
  } else if (fadeRotate == 3){
    for (int k = 0; k < 120; k++){
      strip.setPixelColor(k  , strip.Color(000, 7*fadeMagnitude, 7*fadeMagnitude));
    } 
  } else if (fadeRotate == 4){
    for (int k = 0; k < 120; k++){
      strip.setPixelColor(k  , strip.Color(000, 000, 12*fadeMagnitude));
    } 
  } else if (fadeRotate == 5){
    for (int k = 0; k < 120; k++){
      strip.setPixelColor(k  , strip.Color(7*fadeMagnitude, 000, 7*fadeMagnitude));
    }
  }

  if (fadeUpDown){
    fadeMagnitude--;
  } else {
    fadeMagnitude++;
  }
  
  if (fadeMagnitude == 20){
    fadeUpDown = 1;
  } else if (fadeMagnitude == 0){
    fadeUpDown = 0;
    fadeRotate++;
  }
  if (fadeRotate == 6){
   fadeRotate = 0; 
  }
  strip.show();
}

static void rotateGradiant(){
  for (int k = 0; k < 120; k++){
    strip.setPixelColor(k  , strip.Color(3*fadeR, 3*fadeG, 3*fadeB));
  } 

  if (fadeRotate == 0){
    fadeG++;
    fadeR--;
    if (fadeR == 0){
      fadeRotate = 1;
    }
  } else if (fadeRotate == 1){
    fadeB++;
    fadeG--;
    if (fadeG == 0){
      fadeRotate = 2;
    }
  } else if (fadeRotate == 2){
    fadeR++;
    fadeB--;
    if (fadeB == 0){
      Serial.println(fadeR);
      Serial.println(fadeG);
      Serial.println(fadeB);
      fadeRotate = 0;
    }
  }
  
 
  strip.show();
}

static void turnoff() {
 // for (uint16_t i=0; i < 60; i++){
    strip.setPixelColor(ledI, strip.Color(000, 000, 000));
    strip.show();
    ledI++;
    //checkSerial();
    //delay(0);
 // }
}

static void displayTemps() {
  float hdcputp = (info[0] - 60.0) * 100;
  Serial.println("temps");
  for (int i = 0; i < 5; i++){
    Serial.println(info[1 + i*2]);
    if (info[1 + i*2] < 34){
      strip.setPixelColor(0 + i*8, strip.Color(0, 0, 255));
      strip.setPixelColor(1 + i*8, strip.Color(0, 0, 255));
      strip.setPixelColor(2 + i*8, strip.Color(0, 0, 255));
      strip.setPixelColor(3 + i*8, strip.Color(0, 0, 255));
      strip.setPixelColor(0 + i*8 + 40-1, strip.Color(0, 0, 255));
      strip.setPixelColor(1 + i*8 + 40-1, strip.Color(0, 0, 255));
      strip.setPixelColor(2 + i*8 + 40-1, strip.Color(0, 0, 255));
      strip.setPixelColor(3 + i*8 + 40-1, strip.Color(0, 0, 255));
      strip.setPixelColor(0 + i*8 + 80-1, strip.Color(0, 0, 255));
      strip.setPixelColor(1 + i*8 + 80-1, strip.Color(0, 0, 255));
      strip.setPixelColor(2 + i*8 + 80-1, strip.Color(0, 0, 255));
      strip.setPixelColor(3 + i*8 + 80-1, strip.Color(0, 0, 255));
    } else if (info[1 + i*2] < 36){
      strip.setPixelColor(0 + i*8, strip.Color(0, 255, 0));
      strip.setPixelColor(1 + i*8, strip.Color(0, 255, 0));
      strip.setPixelColor(2 + i*8, strip.Color(0, 255, 0));
      strip.setPixelColor(3 + i*8, strip.Color(0, 255, 0));
      strip.setPixelColor(0 + i*8 + 40-1, strip.Color(0, 255, 0));
      strip.setPixelColor(1 + i*8 + 40-1, strip.Color(0, 255, 0));
      strip.setPixelColor(2 + i*8 + 40-1, strip.Color(0, 255, 0));
      strip.setPixelColor(3 + i*8 + 40-1, strip.Color(0, 255, 0));
      strip.setPixelColor(0 + i*8 + 80-1, strip.Color(0, 255, 0));
      strip.setPixelColor(1 + i*8 + 80-1, strip.Color(0, 255, 0));
      strip.setPixelColor(2 + i*8 + 80-1, strip.Color(0, 255, 0));
      strip.setPixelColor(3 + i*8 + 80-1, strip.Color(0, 255, 0));
    } else if (info[1 + i*2] < 37){
      strip.setPixelColor(0 + i*8, strip.Color(155, 155, 0));
      strip.setPixelColor(1 + i*8, strip.Color(155, 155, 0));
      strip.setPixelColor(2 + i*8, strip.Color(155, 155, 0));
      strip.setPixelColor(3 + i*8, strip.Color(155, 155, 0));
      strip.setPixelColor(0 + i*8 + 40-1, strip.Color(155, 155, 0));
      strip.setPixelColor(1 + i*8 + 40-1, strip.Color(155, 155, 0));
      strip.setPixelColor(2 + i*8 + 40-1, strip.Color(155, 155, 0));
      strip.setPixelColor(3 + i*8 + 40-1, strip.Color(155, 155, 0));
      strip.setPixelColor(0 + i*8 + 80-1, strip.Color(155, 155, 0));
      strip.setPixelColor(1 + i*8 + 80-1, strip.Color(155, 155, 0));
      strip.setPixelColor(2 + i*8 + 80-1, strip.Color(155, 155, 0));
      strip.setPixelColor(3 + i*8 + 80-1, strip.Color(155, 155, 0));
    } else if (info[1 + i*2] < 50){
      strip.setPixelColor(0 + i*8, strip.Color(160, 80, 0));
      strip.setPixelColor(1 + i*8, strip.Color(160, 80, 0));
      strip.setPixelColor(2 + i*8, strip.Color(160, 80, 0));
      strip.setPixelColor(3 + i*8, strip.Color(160, 80, 0));
      strip.setPixelColor(0 + i*8 + 40-1, strip.Color(160, 80, 0));
      strip.setPixelColor(1 + i*8 + 40-1, strip.Color(160, 80, 0));
      strip.setPixelColor(2 + i*8 + 40-1, strip.Color(160, 80, 0));
      strip.setPixelColor(3 + i*8 + 40-1, strip.Color(160, 80, 0));
      strip.setPixelColor(0 + i*8 + 80-1, strip.Color(160, 80, 0));
      strip.setPixelColor(1 + i*8 + 80-1, strip.Color(160, 80, 0));
      strip.setPixelColor(2 + i*8 + 80-1, strip.Color(160, 80, 0));
      strip.setPixelColor(3 + i*8 + 80-1, strip.Color(160, 80, 0));
    } else {
      strip.setPixelColor(0 + i*8, strip.Color(255, 0, 0));
      strip.setPixelColor(1 + i*8, strip.Color(255, 0, 0));
      strip.setPixelColor(2 + i*8, strip.Color(255, 0, 0));
      strip.setPixelColor(3 + i*8, strip.Color(255, 0, 0));
      strip.setPixelColor(0 + i*8 + 40-1, strip.Color(255, 0, 0));
      strip.setPixelColor(1 + i*8 + 40-1, strip.Color(255, 0, 0));
      strip.setPixelColor(2 + i*8 + 40-1, strip.Color(255, 0, 0));
      strip.setPixelColor(3 + i*8 + 40-1, strip.Color(255, 0, 0));
      strip.setPixelColor(0 + i*8 + 80-1, strip.Color(255, 0, 0));
      strip.setPixelColor(1 + i*8 + 80-1, strip.Color(255, 0, 0));
      strip.setPixelColor(2 + i*8 + 80-1, strip.Color(255, 0, 0));
      strip.setPixelColor(3 + i*8 + 80-1, strip.Color(255, 0, 0));
    }
  }
  for (int i = 0; i < 5; i++){
    Serial.println(info[1 + i*2]);
    if (info[2 + i*2] < 34){
      strip.setPixelColor(4 + i*8, strip.Color(0, 0, 255));
      strip.setPixelColor(5 + i*8, strip.Color(0, 0, 255));
      strip.setPixelColor(6 + i*8, strip.Color(0, 0, 255));
      strip.setPixelColor(7 + i*8, strip.Color(0, 0, 255));
      strip.setPixelColor(4 + i*8 + 40-1, strip.Color(0, 0, 255));
      strip.setPixelColor(5 + i*8 + 40-1, strip.Color(0, 0, 255));
      strip.setPixelColor(6 + i*8 + 40-1, strip.Color(0, 0, 255));
      strip.setPixelColor(7 + i*8 + 40-1, strip.Color(0, 0, 255));
      strip.setPixelColor(4 + i*8 + 80-1, strip.Color(0, 0, 255));
      strip.setPixelColor(5 + i*8 + 80-1, strip.Color(0, 0, 255));
      strip.setPixelColor(6 + i*8 + 80-1, strip.Color(0, 0, 255));
      strip.setPixelColor(7 + i*8 + 80-1, strip.Color(0, 0, 255));
    } else if (info[2 + i*2] < 36){
      strip.setPixelColor(4 + i*8, strip.Color(0, 255, 0));
      strip.setPixelColor(5 + i*8, strip.Color(0, 255, 0));
      strip.setPixelColor(6 + i*8, strip.Color(0, 255, 0));
      strip.setPixelColor(7 + i*8, strip.Color(0, 255, 0));
      strip.setPixelColor(4 + i*8 + 40-1, strip.Color(0, 255, 0));
      strip.setPixelColor(5 + i*8 + 40-1, strip.Color(0, 255, 0));
      strip.setPixelColor(6 + i*8 + 40-1, strip.Color(0, 255, 0));
      strip.setPixelColor(7 + i*8 + 40-1, strip.Color(0, 255, 0));
      strip.setPixelColor(4 + i*8 + 80-1, strip.Color(0, 255, 0));
      strip.setPixelColor(5 + i*8 + 80-1, strip.Color(0, 255, 0));
      strip.setPixelColor(6 + i*8 + 80-1, strip.Color(0, 255, 0));
      strip.setPixelColor(7 + i*8 + 80-1, strip.Color(0, 255, 0));
    } else if (info[2 + i*2] < 37){
      strip.setPixelColor(4 + i*8, strip.Color(155, 155, 0));
      strip.setPixelColor(5 + i*8, strip.Color(155, 155, 0));
      strip.setPixelColor(6 + i*8, strip.Color(155, 155, 0));
      strip.setPixelColor(7 + i*8, strip.Color(155, 155, 0));
      strip.setPixelColor(4 + i*8 + 40-1, strip.Color(155, 155, 0));
      strip.setPixelColor(5 + i*8 + 40-1, strip.Color(155, 155, 0));
      strip.setPixelColor(6 + i*8 + 40-1, strip.Color(155, 155, 0));
      strip.setPixelColor(7 + i*8 + 40-1, strip.Color(155, 155, 0));
      strip.setPixelColor(4 + i*8 + 80-1, strip.Color(155, 155, 0));
      strip.setPixelColor(5 + i*8 + 80-1, strip.Color(155, 155, 0));
      strip.setPixelColor(6 + i*8 + 80-1, strip.Color(155, 155, 0));
      strip.setPixelColor(7 + i*8 + 80-1, strip.Color(155, 155, 0));
    } else if (info[2 + i*2] < 50){
      strip.setPixelColor(4 + i*8, strip.Color(160, 80, 0));
      strip.setPixelColor(5 + i*8, strip.Color(160, 80, 0));
      strip.setPixelColor(6 + i*8, strip.Color(160, 80, 0));
      strip.setPixelColor(7 + i*8, strip.Color(160, 80, 0));
      strip.setPixelColor(4 + i*8 + 40-1, strip.Color(160, 80, 0));
      strip.setPixelColor(5 + i*8 + 40-1, strip.Color(160, 80, 0));
      strip.setPixelColor(6 + i*8 + 40-1, strip.Color(160, 80, 0));
      strip.setPixelColor(7 + i*8 + 40-1, strip.Color(160, 80, 0));
      strip.setPixelColor(4 + i*8 + 80-1, strip.Color(160, 80, 0));
      strip.setPixelColor(5 + i*8 + 80-1, strip.Color(160, 80, 0));
      strip.setPixelColor(6 + i*8 + 80-1, strip.Color(160, 80, 0));
      strip.setPixelColor(7 + i*8 + 80-1, strip.Color(160, 80, 0));
    } else {
      strip.setPixelColor(4 + i*8, strip.Color(255, 0, 0));
      strip.setPixelColor(5 + i*8, strip.Color(255, 0, 0));
      strip.setPixelColor(6 + i*8, strip.Color(255, 0, 0));
      strip.setPixelColor(7 + i*8, strip.Color(255, 0, 0));
      strip.setPixelColor(4 + i*8 + 40-1, strip.Color(255, 0, 0));
      strip.setPixelColor(5 + i*8 + 40-1, strip.Color(255, 0, 0));
      strip.setPixelColor(6 + i*8 + 40-1, strip.Color(255, 0, 0));
      strip.setPixelColor(7 + i*8 + 40-1, strip.Color(255, 0, 0));
      strip.setPixelColor(4 + i*8 + 80-1, strip.Color(255, 0, 0));
      strip.setPixelColor(5 + i*8 + 80-1, strip.Color(255, 0, 0));
      strip.setPixelColor(6 + i*8 + 80-1, strip.Color(255, 0, 0));
      strip.setPixelColor(7 + i*8 + 80-1, strip.Color(255, 0, 0));
    }
  }
  strip.show();
  infoChanged = 0;
}

static void displaySpeeds() {
  for (int i = 0; i < 5; i++){
    Serial.println(info[1 + i*7]);
    int magnitude = (int) info[1 + i*7] * 0.12;
    strip.setPixelColor(0 + i*8, strip.Color(0, 0, magnitude));
    magnitude = (int) info[2 + i*7] * 0.12;
    strip.setPixelColor(1 + i*8, strip.Color(0, 0, magnitude));
    magnitude = (int) info[3 + i*7] * 0.12;
    strip.setPixelColor(2 + i*8, strip.Color(0, 0, magnitude));
    magnitude = (int) info[4 + i*7] * 0.12;
    strip.setPixelColor(3 + i*8, strip.Color(0, 0, magnitude));
    magnitude = (int) info[5 + i*7] * 0.12;
    strip.setPixelColor(4 + i*8, strip.Color(0, 0, magnitude));
    magnitude = (int) info[6 + i*7] * 0.12;
    strip.setPixelColor(5 + i*8, strip.Color(0, 0, magnitude));
    magnitude = (int) info[7 + i*7] * 0.19;
    strip.setPixelColor(6 + i*8, strip.Color(0, magnitude,0));
    strip.setPixelColor(7 + i*8, strip.Color(0, magnitude,0));
    
    strip.setPixelColor(0 + i*8 + 40, strip.Color(0, 0, magnitude));
    magnitude = (int) info[2 + i*7] * 0.12;
    strip.setPixelColor(1 + i*8 + 40, strip.Color(0, 0, magnitude));
    magnitude = (int) info[3 + i*7] * 0.12;
    strip.setPixelColor(2 + i*8 + 40, strip.Color(0, 0, magnitude));
    magnitude = (int) info[4 + i*7] * 0.12;
    strip.setPixelColor(3 + i*8 + 40, strip.Color(0, 0, magnitude));
    magnitude = (int) info[5 + i*7] * 0.12;
    strip.setPixelColor(4 + i*8 + 40, strip.Color(0, 0, magnitude));
    magnitude = (int) info[6 + i*7] * 0.12;
    strip.setPixelColor(5 + i*8 + 40, strip.Color(0, 0, magnitude));
    magnitude = (int) info[7 + i*7] * 0.19;
    strip.setPixelColor(6 + i*8 + 40, strip.Color(0, magnitude,0));
    strip.setPixelColor(7 + i*8 + 40, strip.Color(0, magnitude,0));
    
    strip.setPixelColor(0 + i*8 + 80, strip.Color(0, 0, magnitude));
    magnitude = (int) info[2 + i*7] * 0.12;
    strip.setPixelColor(1 + i*8 + 80, strip.Color(0, 0, magnitude));
    magnitude = (int) info[3 + i*7] * 0.12;
    strip.setPixelColor(2 + i*8 + 80, strip.Color(0, 0, magnitude));
    magnitude = (int) info[4 + i*7] * 0.12;
    strip.setPixelColor(3 + i*8 + 80, strip.Color(0, 0, magnitude));
    magnitude = (int) info[5 + i*7] * 0.12;
    strip.setPixelColor(4 + i*8 + 80, strip.Color(0, 0, magnitude));
    magnitude = (int) info[6 + i*7] * 0.12;
    strip.setPixelColor(5 + i*8 + 80, strip.Color(0, 0, magnitude));
    magnitude = (int) info[7 + i*7] * 0.19;
    strip.setPixelColor(6 + i*8 + 80, strip.Color(0, magnitude,0));
    strip.setPixelColor(7 + i*8 + 80, strip.Color(0, magnitude,0));
    
  }
  strip.show();
  infoChanged = 0;
}
