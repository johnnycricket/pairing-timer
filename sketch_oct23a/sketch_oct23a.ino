#include <Bounce.h>

#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_7segment timerDisplay = Adafruit_7segment();

const int startPin = 2;
const int incPin = 3;
const int stopPin = 1;

Bounce pushStart = Bounce(startPin, 10);
Bounce pushInc = Bounce(incPin, 100);
Bounce pushStop = Bounce(stopPin, 10);

int minutes = 0;
int seconds = 0;
bool decriment = false;

void stop() {
    decriment = false;
    minutes = 0;
    seconds = 0;
}

void start() {
    if(minutes > 0) {
        decriment = !decriment;
    }
}

void setLimit() {
    //increment by minutes only?
    if(minutes <= 99){ 
        minutes += 5;
    }
}

void its() {
    if(decriment){
        delay(1000);
        if(seconds == 0 && minutes != 0) {
            minutes--;
            seconds = 60;
        }
        seconds--;
    }
}

void setDis(int x, int y) {
    int t = x*100 + y;
    timerDisplay.print(t, DEC);
    timerDisplay.drawColon(true);
    timerDisplay.writeDisplay();
}

void setup() {
    Serial.begin(38400);
    timerDisplay.begin(0x70);
    timerDisplay.setBrightness(1);
    pinMode(1, INPUT); //stop reset
    pinMode(2, INPUT); //start
    pinMode(3, INPUT); //increment
}

byte previousState = HIGH;

void loop() {
    if(pushStop.update()) {
      Serial.println("red button");
      stop();        
    }
    if(pushStart.update()) {
      if(pushStart.fallingEdge()) {
        Serial.println("green");
        start();
      }  
    }
    if(pushInc.update()) {
      if(pushInc.fallingEdge()) {
        Serial.println("yellow button");
        setLimit(); 
      }
    }
    its();
    setDis(minutes, seconds);
}
