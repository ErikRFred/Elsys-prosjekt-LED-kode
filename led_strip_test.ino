#include <Adafruit_NeoPixel.h>

// How many internal neopixels do we have? some boards have more than one!
#define NUMPIXELS    149
#define PIN1_NEO_PIXEL 33
#define PIN2_NEO_PIXEL 25

//Define input pins
int input1Pin = 32;
int input2Pin = 35;
int input3Pin = 34;

int button1State = 0;
int button2State = 0;
int button3State = 0;

Adafruit_NeoPixel pixels1(NUMPIXELS, PIN1_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel* pixels1ptr = &pixels1;
Adafruit_NeoPixel pixels2(NUMPIXELS, PIN2_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel* pixels2ptr = &pixels2;

//Forhåndsdefinerer en liste med fargene som skal brukes i regnbuefunksjonen
int colorArray[12][3] = {{255,0,0}, {255,127,0}, {255,255,0},
                         {127,255,0}, {0,255,0}, {0,255,127},
                         {0,255,255}, {0,127,255}, {0,0,255},
                         {127,0,255}, {255,0,255}, {255,0,127}};

//Lager en liste som brukes for å styre riktig farge for hver pixel i regnbuefunksjonen
int currentColor[NUMPIXELS];

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  Serial.println("-------------------");
  
  pixels1.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels1.setBrightness(50); // not so bright
  pixels2.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels2.setBrightness(50); // not so bright

  pinMode(input1Pin, INPUT);
  pinMode(input2Pin, INPUT);
  pinMode(input3Pin, INPUT);


  //Koden under brukes for å styre fargerotasjonen i regnbuefunksjonen
  int teller = 0;

  for (int i = 0; i < NUMPIXELS; i++){
    currentColor[i] = teller;

    if(teller < 11){
      teller++;
    } else{
      teller = 0;
    }
  }
}



//Definerer farger for enklere bruk senere, slik at man slipper å bruke RGB-verdier
uint32_t blank = pixels1.Color(0, 0, 0);
uint32_t green = pixels1.Color(0, 255, 0);
uint32_t red = pixels1.Color(255, 0, 0);
uint32_t blue = pixels1.Color(0, 0, 255);
uint32_t gold = pixels1.Color(255, 215, 0);
uint32_t purple = pixels1.Color(255, 0, 255);

//Lager en regnbue som roterer rundt led-strippen
void rainbow(int totalTime, Adafruit_NeoPixel* ptr){
  int delayTime = 40;
  int timeTaken = 0;
  
  while(timeTaken < totalTime){ 
     for (int pixel = 0; pixel < ptr->numPixels(); pixel++){
     int colorIndex = currentColor[pixel];
     ptr->setPixelColor(pixel, colorArray[colorIndex][0], colorArray[colorIndex][1], colorArray[colorIndex][2]);
    
     if (currentColor[pixel] < 11){
        currentColor[pixel]++;
     } else{
        currentColor[pixel] = 0;
      }
    }

  ptr->show();
  Serial.println("Rainbow updated");
  delay(delayTime);
  timeTaken += delayTime;
 }
}

void rainbowBoth(int totalTime){
  int delayTime = 80;
  int timeTaken = 0;

  while(timeTaken < totalTime){
    for (int pixel = 0; pixel < NUMPIXELS; pixel++) {
    int colorIndex = currentColor[pixel];
    pixels1.setPixelColor(pixel, colorArray[colorIndex][0], colorArray[colorIndex][1], colorArray[colorIndex][2]);
    pixels2.setPixelColor(pixel, colorArray[colorIndex][0], colorArray[colorIndex][1], colorArray[colorIndex][2]);

    if (currentColor[pixel] < 11) {
      currentColor[pixel]++;
    } else {
      currentColor[pixel] = 0;
    }
    }

    pixels1.show();
    pixels2.show();
    delay(delayTime);
    timeTaken += delayTime;
  }
}


//Fyller strippen pixel for pixel i valgt farge. Kalles med forhåndsdefinerte farger
void gradualFill(int totalTime, int color, Adafruit_NeoPixel* ptr){
  int delayTime = totalTime/(2*ptr->numPixels());

  ptr->fill((0,0,0), 1, ptr->numPixels());
  ptr->show();
  for(int i = 0; i < ptr->numPixels(); i++){
    ptr->setPixelColor(i, color);
    ptr->show();
    delay(delayTime);
  }
}

//Fyller to strips gradvis samtidig
void gradualFillBoth(int totalTime, int color) {
  int delayTime = totalTime/NUMPIXELS;

  pixels1.fill((0,0,0), 1, NUMPIXELS);
  pixels2.fill((0,0,0), 1, NUMPIXELS);
  pixels1.show();
  pixels2.show();

  for(int i = 0; i < NUMPIXELS; i++){
    pixels1.setPixelColor(i, color);
    pixels2.setPixelColor(i, color);
    pixels1.show();
    pixels2.show();
    delay(delayTime);
  }
}

void gradualFillBothInverse(int totalTime, int color) {
  int delayTime = totalTime/NUMPIXELS;

  pixels1.fill((0,0,0), 1, NUMPIXELS);
  pixels2.fill((0,0,0), 1, NUMPIXELS);
  pixels1.show();
  pixels2.show();

  for(int i = 0; i < NUMPIXELS; i++){
    pixels1.setPixelColor(i, color);
    pixels2.setPixelColor(-i, color);
    pixels1.show();
    pixels2.show();
    delay(delayTime);
  }
}

//Fyller strippen med valgt farge og fjerner fargen gradvis fra enden
void gradualRemove(int totalTime, int color, Adafruit_NeoPixel* ptr) {
  int delayTime = totalTime/(2*ptr->numPixels());

  ptr->fill(color, 1, ptr->numPixels());
  ptr->show();
  for(int i = ptr->numPixels(); i > 0; i--){
    ptr->setPixelColor(i, blank);
    ptr->show();
    delay(delayTime);
  }
}

//Fyller to stripsmed valgt farge og fjerner den, samtidig
void gradualRemoveBoth(int totalTime, int color){
  int delayTime = totalTime/NUMPIXELS;

  pixels1.fill(color, 1, NUMPIXELS);
  pixels2.fill(color, 1, NUMPIXELS);
  pixels1.show();
  pixels2.show();

  for(int i = 0; i < NUMPIXELS; i++){
    pixels1.setPixelColor(i, (0,0,0));
    pixels2.setPixelColor(i, (0,0,0));
    pixels1.show();
    pixels2.show();
    delay(delayTime);
  }
}

void gradualRemoveBothInverse(int totalTime, int color){
  int delayTime = totalTime/NUMPIXELS;

  pixels1.fill(color, 1, NUMPIXELS);
  pixels2.fill(color, 1, NUMPIXELS);
  pixels1.show();
  pixels2.show();

  for(int i = 0; i < NUMPIXELS; i++){
    pixels1.setPixelColor(i, (0,0,0));
    pixels2.setPixelColor(-i, (0,0,0));
    pixels1.show();
    pixels2.show();
    delay(delayTime);
  }
}

//Får strippen til å blinke mellom to valgte farger
void blinking(int numBlinks, int delayTime, int color1, int color2, Adafruit_NeoPixel* ptr){
  for(int i = 0;  i < numBlinks; i++){
    ptr->fill(color1, 0, ptr->numPixels());
    ptr->show();
    delay(delayTime);
    ptr->fill(color2, 0, ptr->numPixels());
    ptr->show();
    delay(delayTime);
  }
}

//Får to strips til å blinke samme farge synkront
void blinkingBoth(int numBlinks, int delayTime, int color1, int color2) {
  for(int i = 0; i < numBlinks; i++){
    pixels1.fill(color1, 0, NUMPIXELS);
    pixels2.fill(color1, 0, NUMPIXELS);
    pixels1.show();
    pixels2.show();
    delay(delayTime);
    pixels1.fill(color2, 0, NUMPIXELS);
    pixels2.fill(color2, 0, NUMPIXELS);
    pixels1.show();
    pixels2.show();
    delay(delayTime);
  }
}

//Får to strips til å blinke samme farge usynkront
void blinkingBothUnsync(int numBlinks, int delayTime, int color1, int color2) {
  for(int i = 0; i < numBlinks; i++){
    pixels1.fill(color2, 0, NUMPIXELS);
    pixels2.fill(color1, 0, NUMPIXELS);
    pixels1.show();
    pixels2.show();
    delay(delayTime);
    pixels1.fill(color1, 0, NUMPIXELS);
    pixels2.fill(color2, 0, NUMPIXELS);
    pixels1.show();
    pixels2.show();
    delay(delayTime);
  }
}

//Setter annenhver pixel til to valgte farger farger, og blinker mellom disse
void alternateBlinking(int numBlinks, int delayTime, int color1, int color2, Adafruit_NeoPixel* ptr) {
  for(int j = 0; j < numBlinks; j++){
    for(int i = 0; i < ptr->numPixels(); i += 2){
      ptr->setPixelColor(i, color1);
      ptr->setPixelColor(i+1, color2);
    }
    ptr->show();
    delay(delayTime);
    
    for(int i = 0; i < ptr->numPixels(); i+=2){
      ptr->setPixelColor(i, color2);
      ptr->setPixelColor(i+1, color1);
    }
    ptr->show();
    delay(delayTime);
  }
}

//Blinker to strips synkront med annenhver farge
void alternateBlinkingBoth(int numBlinks, int delayTime, int color1, int color2) {
  for(int j = 0; j < numBlinks; j++) {
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels1.setPixelColor(i, color1);
      pixels2.setPixelColor(i, color1);
      pixels1.setPixelColor(i+1, color2);
      pixels2.setPixelColor(i+1, color2);
    }
    pixels1.show();
    pixels2.show();
    delay(delayTime);

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels1.setPixelColor(i, color2);
      pixels2.setPixelColor(i, color2);
      pixels1.setPixelColor(i+1, color1);
      pixels2.setPixelColor(i+1, color1);
    }
    pixels1.show();
    pixels2.show();
    delay(delayTime);
  }
}

//Blinker to strips usynkront med annenhver farge
void alternateBlinkingBothUnsync(int numBlinks, int delayTime, int color1, int color2) {
  for(int j = 0; j < numBlinks; j++) {
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels1.setPixelColor(i, color1);
      pixels2.setPixelColor(i, color2);
      pixels1.setPixelColor(i+1, color2);
      pixels2.setPixelColor(i+1, color1);
    }
    pixels1.show();
    pixels2.show();
    delay(delayTime);

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels1.setPixelColor(i, color2);
      pixels2.setPixelColor(i, color1);
      pixels1.setPixelColor(i+1, color1);
      pixels2.setPixelColor(i+1, color2);
    }
    pixels1.show();
    pixels2.show();
    delay(delayTime);
  }
}

//Funksjonen som spilles når man venter på input
void idle(Adafruit_NeoPixel* ptr){ //stripNmbr bør være samme tall som er i navnet til strippen ptr peker på
  ptr->setBrightness(67);
  rainbow(1, ptr);
}

void idleBoth() {
  pixels1.setBrightness(67);
  pixels2.setBrightness(67);
  rainbowBoth(1);
}

void wrongInput(Adafruit_NeoPixel* ptr) {
  ptr->setBrightness(50);
  blinking(10, 20, red, blank, ptr);
}

void wrongInputBoth(){
  pixels1.setBrightness(50);
  pixels2.setBrightness(50);
  blinkingBoth(10, 100, red, blank);
}

void rightInput(Adafruit_NeoPixel* ptr) {
  ptr->setBrightness(50);
  blinking(10, 20, green, blank, ptr);
}

void rightInputBoth(){
  pixels1.setBrightness(50);
  pixels2.setBrightness(50);
  blinkingBothUnsync(10, 100, green, blank);
}

void premiumInput(Adafruit_NeoPixel* ptr) {
  ptr->setBrightness(255);
  gradualFill(1000, gold, ptr);
  alternateBlinking(6, 30, gold, purple, ptr);
  gradualFill(500, gold, ptr);
}

void premiumInputBoth() {
  pixels1.setBrightness(50);
  pixels2.setBrightness(50);
  gradualRemoveBothInverse(500, gold);
  alternateBlinkingBothUnsync(6, 75, gold, purple);
  gradualFillBoth(500, gold);
}

// the loop routine runs over and over again forever:
void loop() {
  // say hi
  Serial.println("Hello!");

  button1State = digitalRead(input1Pin);
  button2State = digitalRead(input2Pin);
  button3State = digitalRead(input3Pin);

  if(button1State == HIGH){
    rightInputBoth();
  } else if(button2State == HIGH) {
    wrongInputBoth();
  }else if(button3State == HIGH) {
    premiumInputBoth();
  } else {
    idleBoth();
  }

}
