#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>
// Sensor Data
float pitch, roll;
float prox;

int pirPin = 2;   // PIR Out pin
int pirStat = 0;  // PIR status

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
  }
}

void loop() {


  float x, y, z;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
    Serial.print("Gyro");
    Serial.print('\t');
    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    int pitch = atan2(-x, z) * 180 / M_PI;
    int roll = atan2(-y, z) * 180 / M_PI;
    int yaw = atan2(z,  / sqrt(accelerationX * accelerationX + accelerationZ * accelerationZ)) / M_PI;

    //int yaw = 180.0*atan2(magy, magx) / PI; //broken
    Serial.print("pitchRoll");
    Serial.print('\t');
    Serial.print(pitch);
    Serial.print('\t');
    Serial.println(roll);
  }

  if (APDS.gestureAvailable()) {
    // a gesture was detected, read and print to Serial Monitor
    int gesture = APDS.readGesture();

    switch (gesture) {
      case GESTURE_UP:
        Serial.println("UP");
        break;
      case GESTURE_DOWN:
        Serial.println("DOWN");
        break;
      case GESTURE_LEFT:
        Serial.println("LEFT");
        break;
      case GESTURE_RIGHT:
        Serial.println("RIGHT");
        break;
      default:
        break;
    }
  }

}
