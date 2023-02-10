#include <Adafruit_NeoPixel.h>

#define BUFFER 6
#define DELAY 10
#define RED_INPUT A7
#define GREEN_INPUT A1
#define BLUE_INPUT A2
#define BRIGHT_INPUT A3 
#define LED_COUNT 8
#define BUTTON_PIN 12
#define LED_PIN 2

int lastState = HIGH;
int currentState;
int r =0;
int rB4;
int g = 0;
int gB4;
int b =0;
int bB4;
int w =50;
int wB4;
int saved[] = {0,0,0,0};

long stTime;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();           // initializes NeoPixel strip object
  strip.show(); 
  strip.setBrightness(50);         
  theaterChaseRainbow(50);

}

void loop() {

  //reads state of button
  currentState = digitalRead(BUTTON_PIN);

  if(lastState == HIGH && currentState == LOW){
    //time starts to determine how long button was pressed
    stTime = millis();
    //sets color to whatever color was perviously saved, or "off" if none saved yet
    colorWipe(strip.Color(saved[0],saved[1],saved[2]),15);
  }

  //checks if the button has been pressed for at least 5 seconds
  if(lastState == LOW && currentState == LOW && millis()-stTime>= 5000){
    //sets the color to the inputs from the knobs and saves that color for the next time the button is pressed
    colorWipe(strip.Color(r,g,b),10);
    saved[0] = r;
    saved[1] = g;
    saved[2] = b;
    saved[3] = w;
  }

  //reads the analog value of the read input knob and divides by 4 to find number betwwen 0 and 255
  r = analogRead(RED_INPUT)/4;
  updateCol(r, rB4, DELAY)


  g = analogRead(GREEN_INPUT)/4;
  updateCol(g, gB4, DELAY)


  b = analogRead(BLUE_INPUT)/4;
  updateCol(b, bB4, DELAY)


  w = analogRead(BRIGHT_INPUT)/4;
  if (wB4 != w && abs(w-wB4)>=BUFFER){
    //if the brightness value is less than or greater than 4 turn off the entire strip
    //(i set it to less than or equal to 4 because this is a gift for my mother and i know she would prefer it this way)
    if(w <= 4){
      colorWipe(strip.Color(0, 0, 0), DELAY/2);
    }else{
      strip.setBrightness(w);
    }      
  }else {
    w = wB4;
  }

  //set all prevous values to current values for next loop
  lastState = currentState;
  rB4 = r;
  gB4 = g;
  bB4 = b;
  wB4 = w;

}

//takes in value for color, the color beforehand, and the delay for colorWipe
//checks if the new value is different enough from the first value to change the color to pevent flickering
void updateCol(int color, int colorB4, int delay){
  if (colorB4 != color && abs(color-colorB4)>=BUFFER){
    colorWipe(strip.Color(r, g, b), delay);
  }else {
    color = colorB4;
  }

}

//creates a rainbow chase ran on start up
//method from AdaFruit NeoPixel "strandtest.ino" example, author not given 
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

//sets the given color for each pixel gradually to create a cool effect
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);         
    strip.show();                          
    delay(wait);                           
  }
}