#include <DMXSerial.h>

#define PIN 2 // Pin for the MAX485
#define CHAN 1 // DMX Channel
#define relayPin 3

int bells = 7;
int hook_button = 3;
int other_button = 4;
int chval = 0;
int No_Data_Time = 0;

const int Timeout = 500;
const int LED_Pin = 9;
const int threshold = 127;

int start = 0;
int hook_val = 0;
int other_val = 0;
int mode_state = 0;

void setup() {
  // Define the port type of the used pins
  DMXSerial.init(DMXReceiver, PIN);

  pinMode(bells, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(hook_button, INPUT_PULLUP);
  pinMode(other_button, INPUT_PULLUP);
  //End of setup
}

void loop() {

  chval = DMXSerial.read(CHAN); //Listens on Channel CHAN
  No_Data_Time = DMXSerial.noDataSince(); //Counts time since last packet was received. The time is in milliseconds

  // Handling of Modes
  other_val = digitalRead (other_button);
  if (other_val == LOW) {
    if (mode_state == 1) {
      mode_state = 0;
      digitalWrite(bells, LOW);
      delay (500);
      digitalWrite(bells, HIGH);
      delay(1000);
      start = 0;
    }
    else {
      mode_state = 1;
      digitalWrite(bells, HIGH);
      delay (100);
      digitalWrite(bells, LOW);
      delay (100);
      digitalWrite(bells, HIGH);
      delay (100);
      digitalWrite(bells, LOW);
      delay(1000);
      start = 0;
    }
  }
  // End of Mode Handling

    //Mode 1
    if (mode_state == 0) {
      hook_val = digitalRead (hook_button);
      digitalWrite(LED_Pin, LOW);
      if (start < 24) {
        if (hook_val == LOW) {
          digitalWrite(bells, HIGH);
          delay(55);
          digitalWrite(bells, LOW);
          delay(55);
          start = (start + 1);
        }
        else {
          start = 0;
        }
      } else if (start = 24) {
        delay(1800);
        start = 0;
      }
    }
    //End of Mode 1

    
    //Mode 2
    else {
      digitalWrite(LED_Pin, HIGH);
 if (No_Data_Time <= Timeout) {
      if (chval >= threshold) {
        hook_val = digitalRead (hook_button);
        if (start < 24) {
          if (hook_val == LOW) {
            digitalWrite(bells, HIGH);
            delay(55);
            digitalWrite(bells, LOW);
            delay(55);
            start = (start + 1);
          }
          else {
            start = 0;
          }
        } else if (start = 24) {
          delay(1800);
          start = 0;
        }
      } else {
        start = 0;
      }
    } else {
    start = 0;
    }
  } 
  //End of Mode 2
}
