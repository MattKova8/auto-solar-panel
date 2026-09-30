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
#define BUTTON 2

// RGB LED setup
#define BLUE_PIN 9
#define RED_PIN 11
#define GREEN_PIN 10

// scanning vars
int scan_delay = 10; //adjustable scan delay (ms)
int strongest = 1023; // strongest light position on scan
int strongest_deg; //servo position of strongest photo val

// timer vars
unsigned long previousMillis = 0;
unsigned long scan_period = 60000; // default 60 sec auto scan

void scan (){
  Serial.println("Starting scan...");
  
  // led red
  digitalWrite(RED_PIN, 255);
  digitalWrite(BLUE_PIN, 0);
  digitalWrite(GREEN_PIN, 0);

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

  // led green
  digitalWrite(RED_PIN, 0);
  digitalWrite(BLUE_PIN, 0);
  digitalWrite(GREEN_PIN, 255);
}

void setup() {
  Serial.println("- Starting System -");

  //initialize components
  Serial.begin(9600);

  pinMode(PHOTO_PIN, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  servo.attach(SERVO_PIN);
  servo.write(0);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

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

          // flash blue for set interval
          for (int i=0; i < 4; i++){
            digitalWrite(RED_PIN, 0);
            digitalWrite(BLUE_PIN, 255);
            digitalWrite(GREEN_PIN, 0);
            delay(100);
            digitalWrite(RED_PIN, 0);
            digitalWrite(BLUE_PIN, 0);
            digitalWrite(GREEN_PIN, 0);
            delay(100);
          }
          digitalWrite(GREEN_PIN, 255);

          Serial.print("Interval set to ");
          Serial.print(seconds);
          Serial.println(" seconds.");
      }
  }

  // manual scan on button press
  if (digitalRead(BUTTON) == LOW){
    Serial.println("Starting manual scan...");
    scan();
  }

  //auto scan
  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= scan_period) {
        previousMillis = currentMillis;

        Serial.println("Autoscan activated...");
        scan();
    }
}
