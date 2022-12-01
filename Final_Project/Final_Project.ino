#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
#include <Arduino_APDS9960.h>

const char* nameOfPeripheral = "Halo-Controller";
const char* uuidOfService = "0000181a-0000-1000-8000-00805f9b34fb";

BLEService sensorDataService(uuidOfService);
BLEByteCharacteristic pitchChar("00002AA1-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEByteCharacteristic rollChar("00002AA2-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
BLEByteCharacteristic proxChar("00002AA4-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
// Sensor Data
float pitch, roll;
float prox;

void setup() {
  // Serial Setup
  Serial.begin(9600);
  while(!Serial);
  // Gyro Setup
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  // Proximity Sensor Setup
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
  }
  // BLE Setup
  startBLE();
  BLE.setLocalName(nameOfPeripheral);
  BLE.setAdvertisedService(sensorDataService);
  sensorDataService.addCharacteristic(pitchChar);
  sensorDataService.addCharacteristic(rollChar);
  sensorDataService.addCharacteristic(proxChar);
  BLE.addService(sensorDataService);
  
  BLE.setEventHandler(BLEConnected, onBLEConnected);
  BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);

  proxChar.writeValue(255);
  BLE.advertise();

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
      if (IMU.accelerationAvailable()) 
      {
        IMU.readAcceleration(x, y, z);
        pitch = atan2(-x, z) * 180 / M_PI;
        roll = atan2(-y, z) * 180 / M_PI;
        //int yaw = 180.0*atan2(magy, magx) / PI; //broken
        Serial.print(pitch);
        Serial.print('\t');
        Serial.println(roll);
    }
    }
  }
  /*
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    int pitch = atan2(-x, z) * 180 / M_PI;
    int roll = atan2(-y, z) * 180 / M_PI;
    int yaw = 180.0*atan2(magy, magx) / PI; //broken
    Serial.print(pitch);
    Serial.print('\t');
    Serial.println(roll);
  }
  */
  /*
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }
  */
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