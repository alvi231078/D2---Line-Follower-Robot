// pins for the motor, not 100% sure if enablePin needs PWM but I think yes
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;  // connects to ENA on L298N

// ultrasonic sensor stuff (HC-SR04 thingy)
const int trigPin = 6;  // sends the signal
const int echoPin = 7;  // gets the bounce-back

// speeds – I think 255 is max for analogWrite?
const int forwardSpeed = 180;
const int reverseSpeed = 160;  // not using this yet but might later

// if something is closer than this we stop (in cm, I think?)
const int distanceThreshold = 15;

void setup() {
  setupMotorPins();      // set motor pins as output
  setupSensorPins();     // same for ultrasonic sensor
  Serial.begin(9600);    // so we can see what’s happening
}

void loop() {
  int distance = getDistance();  // get how far the thing is

  showDistance(distance);        // print it to serial, for debugging or whatever

  controlMotor(distance);        // run or stop based on distance

  delay(500);  // maybe make this faster later?
}

// this sets the pins for motor – basic stuff
void setupMotorPins() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);  // for speed control I guess
}

// sets up sensor pins – trig is out, echo is in
void setupSensorPins() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// send signal and measure how long it takes to bounce back
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // not sure if this is necessary, but tutorials say yes

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);  // 10 microseconds pulse, pretty standard
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);  // wait for echo
  int distance = duration * 0.034 / 2;     // convert to cm (found this formula online)

  return distance;  // hope this works lol
}

// just prints the distance – useful to see if the sensor works
void showDistance(int distance) {
  Serial.print("Object is ");
  Serial.print(distance);
  Serial.println(" cm away");
}

// decides what to do with motor based on distance
void controlMotor(int distance) {
  if (distance > distanceThreshold) {
    moveForward();  // looks clear, go!
  } else {
    stopMotor();    // whoa! too close
  }
}

// moves motor forward (just basic logic)
void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, forwardSpeed);  // make it go
}

// stops motor, probably could brake it harder but this works
void stopMotor() {
  analogWrite(enablePin, 0);
}
