#include <Arduino.h>
#include <Servo.h>

// servo setup
#define SERVO_PIN 3
Servo servo;
int servo_pos = 0;

// photoresistor
#define PHOTO_PIN A0
int val; //photoresistor reading (high val so that any read val is lower)

// pushbutton setup
#define button 7

// scanning vars
int scan_delay = 10; //adjustable scan delay (ms)
int strongest = 1023; // strongest light position on scan
int strongest_deg; //servo position of strongest photo val

// timer vars
unsigned long previousMillis = 0;
unsigned long scan_period = 30000; // default 30 sec auto scan

/*
TODO: LED indicator in scan function
*/
void scan (){
  Serial.println("Starting scan...");
  servo.write(0);

  for (int pos = 0; pos <= 180; pos++){
    servo.write(pos);
    delay(scan_delay); //delay to read photoresistor val

    // * record data * //
    val = analogRead(PHOTO_PIN) ;
    //Serial.println(val);

    if (val < strongest){ 
      strongest = val;
      strongest_deg = pos; //record servo pos of current strongest source
    }
    delay(scan_delay);
  }

  servo.write(0);

  Serial.println();
  Serial.println("-Scan complete-");
  Serial.print("Strongest light source: ");
  Serial.print(strongest_deg);
  Serial.println(" deg");

  // move servo to strongest light source
  servo.write(strongest_deg);
  delay(scan_delay);
}

void setup() {
  Serial.println("- Starting System -");

  //initialize components
  Serial.begin(9600);
  pinMode(PHOTO_PIN, INPUT);
  servo.attach(SERVO_PIN);
  servo.write(0);
  delay(300);

  scan(); // initial scan on boot

  previousMillis = millis();
}

void loop() {
  // Check for user input
  if (Serial.available() > 0) {
      int seconds = Serial.parseInt();

      if (seconds > 0) {
          scan_period = (unsigned long)seconds * 1000;

          Serial.print("Interval set to ");
          Serial.print(seconds);
          Serial.println(" seconds.");
      }
  }

  //auto scan
  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= scan_period) {
        previousMillis = currentMillis;

        Serial.println("Autoscan activated...");
        scan();
    }

  // check for button press to scan
  //output to serial
}
