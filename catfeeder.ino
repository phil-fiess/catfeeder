#include <RTClib.h>
#include <Servo.h>
#include <Wire.h>

const int servoPin = 9;
const int ledPin = 2;


//servo motor object
Servo servo;

//RealTime Clock object specific to DS1307 model
RTC_DS1307 rtc;

const int feedingTimes[4] = {9, 13, 17, 21};
const int numberDailyFeedings = 4;

void setup() {
 Serial.begin(9600);
 //checks to make sure timeclock started properly
 if (!rtc.begin()){
  Serial.print("Can't find RTC");
  while(1);
 }

 //calibrates RTC to current time on compiling if needed
  if (!rtc.isrunning()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
 
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

 //attaches signal pin to servo motor
 servo.attach(servoPin);
 pinMode(ledPin, OUTPUT);
}

void loop() {
  DateTime now = rtc.now();

  //just prints time to serial for reference
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(1000);

  if (checkFeedingTime(now)) {
    digitalWrite(ledPin, HIGH);
    servo.write(90);
    delay(1000);
    servo.write(180);
    delay(1000);
    digitalWrite(ledPin, LOW);
  }

  
}

//iterates over the feeding times (9 am, 1 pm, 5 pm, 9 pm)
//and returns true if they land on any of these hours sharp
boolean checkFeedingTime(DateTime timeNow) {
  for (int i = 0; i < numberDailyFeedings; i++) {
    if (
      timeNow.hour() == feedingTimes[i]
      &&
      timeNow.minute() == 0
      &&
      timeNow.second() == 0
    ) {
      return true; 
    }
  }
  return false;
}
