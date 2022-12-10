#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>

const char* nameOfPeripheral = "Halo-Controller";
const char* uuidOfService = "0000181a-0000-1000-8000-00805f9b34fb";

BLEService sensorDataService(uuidOfService);
BLEByteCharacteristic pitchChar("00002AA1-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEByteCharacteristic rollChar("00002AA2-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEByteCharacteristic proxChar("00002AA4-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEByteCharacteristic gestureChar("00002AA7-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEFloatCharacteristic gyroXChar("00002AA3-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEFloatCharacteristic gyroYChar("00002AA5-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEFloatCharacteristic gyroZChar("00002AA6-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
// Sensor Data
float pitch = 0;
float roll = 0;
int gesture = -1;
float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;
float prox;

void setup() {
  // Serial Setup
  // Gyro/Accel Setup
  if (!IMU.begin()) {
    while (1);
  }
  // Proximity Sensor Setup
  if (!APDS.begin()) {
  }
  // BLE Setup
  startBLE();
  BLE.setLocalName(nameOfPeripheral);
  BLE.setAdvertisedService(sensorDataService);
  sensorDataService.addCharacteristic(pitchChar);
  sensorDataService.addCharacteristic(rollChar);
  sensorDataService.addCharacteristic(proxChar);
  sensorDataService.addCharacteristic(gyroXChar);
  sensorDataService.addCharacteristic(gyroYChar);
  sensorDataService.addCharacteristic(gyroZChar);
  sensorDataService.addCharacteristic(gestureChar);
  BLE.addService(sensorDataService);
  // Event Handlers
  BLE.setEventHandler(BLEConnected, onBLEConnected);
  BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);
  // Default Values
  proxChar.writeValue(255);
  pitchChar.writeValue(0);
  gyroXChar.writeValue(0);
  gyroYChar.writeValue(0);
  gyroZChar.writeValue(0);
  // Start Advertising
  BLE.advertise();
  // Print Perihperal Info
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice central = BLE.central();
  if(central)
  {
    while(central.connected())
    {
      
      if(APDS.proximityAvailable())
      {
        int p = APDS.readProximity();
        if((p <= 100 && prox > 100) || (p > 100 && prox <= 100))
        {
          proxChar.writeValue(p);
          //Serial.print("New Proximity: ");
          //Serial.println(p);
          prox = p;
        }
      }

      float x, y, z;
      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);

        if(gyroX != x)
        {
          gyroXChar.writeValue(x);
          gyroX = x;
        }
        if(gyroY != y)
        {
          gyroYChar.writeValue(y);
          gyroY = y;
        }
        if(gyroZ != z)
        {
          gyroZChar.writeValue(z);
          gyroZ = z;
        }
      }

      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        float currPitch = atan2(-x, z) * 180 / M_PI;
        float currRoll = atan2(-y, z) * 180 / M_PI;
        //int yaw = 180.0*atan2(magy, magx) / PI; //broken
        if(currPitch != pitch)
        {
          pitchChar.writeValue(currPitch);
          pitch = currPitch;
        }
        if(currRoll != roll)
        {
          rollChar.writeValue(currRoll);
          roll = currRoll;
        }
      }

      if (APDS.gestureAvailable()) {
        int gesture = APDS.readGesture();
        gestureChar.writeValue(gesture);
      }
    }
  }
}


void startBLE() {
  if (!BLE.begin())
  {
    while (1);
  }
}

void onBLEConnected(BLEDevice central) {
}

void onBLEDisconnected(BLEDevice central) {
}
