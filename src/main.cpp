#include <Arduino.h>
#include <Servo.h>

// servo setup
#define SERVO_PIN 3
Servo servo;
int servo_pos = 0;

// photoresistor
#define PHOTO_PIN 8
int val = 0; //photoresistor reading
int read_delay = 30;

// pushbutton setup
#define button 7

// scanning vars
int scan_period = 5; // scan period (s)
int scan_delay = 30; //adjustable scan delay (ms)
int strongest = 0; // strongest light position on scan
int strongest_deg = 0; //servo position of strongest photo val

void setup() {
  //initialize components
  Serial.begin(9600);

  servo.attach(SERVO_PIN);
  servo.write(0);
  delay(100);

  servo_pos = scan(); // initial scan on boot
  servo.write(servo_pos);
}

void loop() {
  // check for button press to scan
  //output to serial

  //auo scan
}
/*
TODO: LED indicator in scan function
*/
int scan (){
  Serial.println("Starting scan...");
  servo.write(0);

  for (int pos = 0; pos <= 180; pos++){
    servo.write(pos);
    // * record data * //
    delay(read_delay); //delay to read photoresistor val

    if (val < strongest){
      strongest = val;
      strongest_deg = pos; //record servo pos of currents strongest source
    }
    delay(scan_delay);
  }

  for (int pos = 180; pos >= 0; pos--){ //scan coming back
    servo.write(pos);
    // * record data * //
    delay(read_delay);

    if (val < strongest){
      strongest = val;
      strongest_deg = pos;
    }
    delay(scan_delay);
  }

  Serial.println("-Scan complete-");
  Serial.print("Strongest light source: ");
  Serial.print(strongest_deg);
  Serial.println(" deg");
  return strongest_deg;
}