#include <Ultrasonic.h>
#include <IRremote.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 13

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN, 24000);
IRsend irsend;

float threshold = 100;

float cmMsec = 0;
float lastcmMsec = 0;

int state = 0;
int turn = 1;

void setup()
{
  Serial.begin(115200);
}

void loop() {
  cmMsec = ultrasonic.Ranging(CM);
  Serial.print("CM ");
  Serial.print(cmMsec);
  Serial.print(", lastCM ");
  Serial.print(lastcmMsec);
  Serial.print(", state ");
  Serial.println(state);
  delay(100);

  unsigned long v = 0x44BB48B7;
  unsigned long f = 0x44BBE817;

  switch (state) {
    case 0:
      if (cmMsec < threshold && lastcmMsec > threshold) {
        state = 1;
      }
      else if (cmMsec > threshold && lastcmMsec < threshold) {
        state = 2;
      }
      break;
    case 1:
      if (cmMsec < threshold && turn == 1) {
        irsend.sendNEC(v, 32);
        Serial.println("send channel 1 ，有人靠近");
        delay(10000);
        turn = 2;
      }
      state = 0;
      break;
    case 2:
      if (cmMsec > threshold && turn == 2) {
        irsend.sendNEC(f, 32);\
        Serial.println("send channel 2 ，無人靠近");
        delay(3000);
        turn = 1;
      }
      state = 0;
      break;
  }

  lastcmMsec = cmMsec;

}
