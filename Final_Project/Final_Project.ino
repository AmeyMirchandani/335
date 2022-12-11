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
int pitch = 0;
int roll = 0;
int gesture = -1;
float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;
float prox;

void setup() {
  // Serial Setup
  Serial.begin(9600);
  while(!Serial);
  // Gyro/Accel Setup
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  // Proximity Sensor Setup
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
  }
  // BLE Setup -> https://create.arduino.cc/projecthub/sridhar-rajagopal/control-arduino-nano-ble-with-bluetooth-python-331e33?ref=part&ref_id=108462&offset=11
  //              Lot of code from ^^
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
  Serial.println("Peripheral advertising info: ");
  Serial.print("Name: ");
  Serial.println(nameOfPeripheral);
  Serial.print("MAC: ");
  Serial.println(BLE.address());
  Serial.print("Service UUID: ");
  Serial.println(sensorDataService.uuid());
  Serial.print("pitchChar UUID: ");
  Serial.println(pitchChar.uuid());
  Serial.print("rollChar UUID: ");
  Serial.println(rollChar.uuid());
  Serial.print("proxChar UUID: ");
  Serial.println(proxChar.uuid());
  
  Serial.println("\nBluetooth device active, waiting for connections...");
}

void loop() {
  // https://rootsaid.com/arduino-ble-example/
  // basic central connection setup code ^
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
        int currPitch = atan2(-x, z) * 180 / M_PI;
        int currRoll = atan2(-y, z) * 180 / M_PI;
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
          Serial.println(roll);
        }
      }

      if (APDS.gestureAvailable()) {
        int gesture = APDS.readGesture();
        gestureChar.writeValue(gesture);
        /*
        switch (gesture){
          case GESTURE_UP:
            if(gesture != 0) { gestureChar.writeValue(0); }
            gesture = 0;
            break;

          case GESTURE_DOWN:
            if(gesture != 1) { gestureChar.writeValue(1); }
            gesture = 1;
            break;

          case GESTURE_LEFT:
            if(gesture != 2) { gestureChar.writeValue(2); }
            gesture = 2;
            break;

          case GESTURE_RIGHT:
            if(gesture != 3) { gestureChar.writeValue(3); }
            gesture = 3;
            break;

          default:
            // ignore
            break;
        }
        */
      }
    }
  }
}


void startBLE() {
  if (!BLE.begin())
  {
    Serial.println("starting BLE failed!");
    while (1);
  }
}

void onBLEConnected(BLEDevice central) {
  Serial.print("Connected device, central: ");
  Serial.println(central.address());
}

void onBLEDisconnected(BLEDevice central) {
  Serial.print("Disconnected device, central: ");
  Serial.println(central.address());
}