// -------------------------------
// Pin Definitions
// -------------------------------

const int PIN_MOTOR_LEFT_FORWARD   = 9;
const int PIN_MOTOR_LEFT_BACKWARD  = 10;
const int PIN_MOTOR_RIGHT_FORWARD  = 5;
const int PIN_MOTOR_RIGHT_BACKWARD = 6;

const int PIN_ULTRASONIC_TRIG = 7;
const int PIN_ULTRASONIC_ECHO = 8;

const int PIN_IR_LEFT  = 2;
const int PIN_IR_RIGHT = 3;

// -------------------------------
// Configuration Constants
// -------------------------------

const int  DISTANCE_LIMIT_CM        = 30;
const long TIMEOUT_RECOVER_MS       = 3000;
const long TIMEOUT_AVOID_MS         = 5000;

// -------------------------------
// Setup Function
// -------------------------------

void setup()
{
    pinMode(PIN_MOTOR_LEFT_FORWARD,   OUTPUT);
    pinMode(PIN_MOTOR_LEFT_BACKWARD,  OUTPUT);
    pinMode(PIN_MOTOR_RIGHT_FORWARD,  OUTPUT);
    pinMode(PIN_MOTOR_RIGHT_BACKWARD, OUTPUT);

    pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
    pinMode(PIN_ULTRASONIC_ECHO, INPUT);

    pinMode(PIN_IR_LEFT,  INPUT);
    pinMode(PIN_IR_RIGHT, INPUT);

    Serial.begin(9600);
}

// -------------------------------
// Main Loop
// -------------------------------

void loop()
{
    long distance = measureDistance();
    bool irLeft   = digitalRead(PIN_IR_LEFT);
    bool irRight  = digitalRead(PIN_IR_RIGHT);

    printSensorStatus(distance, irLeft, irRight);

    if (distance < DISTANCE_LIMIT_CM)
    {
        Serial.println("⚠️ Obstacle detected. Executing avoid routine.");
        avoidObstacle();
        return;
    }

    if (!irLeft && !irRight)
    {
        Serial.println("✅ Clear path. Moving forward.");
        moveForward();
    }
    else if (!irLeft && irRight)
    {
        Serial.println("↪ Line offset: left. Turning left.");
        turnLeft();
    }
    else if (irLeft && !irRight)
    {
        Serial.println("↩ Line offset: right. Turning right.");
        turnRight();
    }
    else
    {
        Serial.println("❓ Line lost. Initiating recovery.");
        recoverLine();
    }

    delay(100);
}

// -------------------------------
// Distance Measurement
// -------------------------------

long measureDistance()
{
    digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_ULTRASONIC_TRIG, LOW);

    long duration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH, 20000);
    return (duration == 0) ? 999 : duration * 0.034 / 2;
}

// -------------------------------
// Logging
// -------------------------------

void printSensorStatus(long distance, bool left, bool right)
{
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    Serial.print("IR Left: ");
    Serial.print(left ? "WHITE" : "BLACK");

    Serial.print(" | IR Right: ");
    Serial.println(right ? "WHITE" : "BLACK");
}

// -------------------------------
// Motion Control
// -------------------------------

void moveForward()
{
    digitalWrite(PIN_MOTOR_LEFT_FORWARD,   HIGH);
    digitalWrite(PIN_MOTOR_LEFT_BACKWARD,  LOW);
    digitalWrite(PIN_MOTOR_RIGHT_FORWARD,  HIGH);
    digitalWrite(PIN_MOTOR_RIGHT_BACKWARD, LOW);
}

void stopMotors()
{
    digitalWrite(PIN_MOTOR_LEFT_FORWARD,   LOW);
    digitalWrite(PIN_MOTOR_LEFT_BACKWARD,  LOW);
    digitalWrite(PIN_MOTOR_RIGHT_FORWARD,  LOW);
    digitalWrite(PIN_MOTOR_RIGHT_BACKWARD, LOW);
}

void turnLeft()
{
    digitalWrite(PIN_MOTOR_LEFT_FORWARD,   LOW);
    digitalWrite(PIN_MOTOR_LEFT_BACKWARD,  HIGH);
    digitalWrite(PIN_MOTOR_RIGHT_FORWARD,  HIGH);
    digitalWrite(PIN_MOTOR_RIGHT_BACKWARD, LOW);
}

void turnRight()
{
    digitalWrite(PIN_MOTOR_LEFT_FORWARD,   HIGH);
    digitalWrite(PIN_MOTOR_LEFT_BACKWARD,  LOW);
    digitalWrite(PIN_MOTOR_RIGHT_FORWARD,  LOW);
    digitalWrite(PIN_MOTOR_RIGHT_BACKWARD, HIGH);
}

// -------------------------------
// Obstacle Avoidance
// -------------------------------

void avoidObstacle()
{
    unsigned long startTime = millis();

    turnRight();
    delay(400);

    moveForward();
    unsigned long forwardStart = millis();

    while (millis() - forwardStart < 1200)
    {
        if (measureDistance() > DISTANCE_LIMIT_CM + 10)
        {
            break;
        }
        delay(50);
    }

    turnLeft();
    delay(400);

    moveForward();
    delay(500);

    recoverLine();

    if (millis() - startTime > TIMEOUT_AVOID_MS)
    {
        Serial.println("❌ Avoid failed. Proceeding anyway.");
        moveForward();
        delay(1000);
    }
}

// -------------------------------
// Line Recovery
// -------------------------------

void recoverLine()
{
    stopMotors();
    delay(100);

    digitalWrite(PIN_MOTOR_LEFT_FORWARD,   HIGH);
    digitalWrite(PIN_MOTOR_LEFT_BACKWARD,  LOW);
    digitalWrite(PIN_MOTOR_RIGHT_FORWARD,  LOW);
    digitalWrite(PIN_MOTOR_RIGHT_BACKWARD, HIGH);

    unsigned long recoveryStart = millis();

    while (digitalRead(PIN_IR_LEFT) == HIGH && digitalRead(PIN_IR_RIGHT) == HIGH)
    {
        if (millis() - recoveryStart > TIMEOUT_RECOVER_MS)
        {
            Serial.println("❌ Could not recover line.");
            stopMotors();
            return;
        }
        delay(20);
    }

    stopMotors();
    Serial.println("✅ Line successfully recovered.");
}
