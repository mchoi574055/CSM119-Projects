#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>
#include <Arduino_LSM6DSOX.h>

#define BLE_UUID_gyroscope_SERVICE "1125"
#define BLE_UUID_gyroscope_X "2139"
#define BLE_UUID_gyroscope_Y "2102"
#define BLE_UUID_gyroscope_Z "2103"

#define BLE_DEVICE_NAME "Elfo"
#define BLE_LOCAL_NAME "Elfo"

BLEService gyroscopeService(BLE_UUID_gyroscope_SERVICE);

BLEFloatCharacteristic gyroscopeCharacteristicX(BLE_UUID_gyroscope_X, BLERead | BLENotify);
BLEFloatCharacteristic gyroscopeCharacteristicY(BLE_UUID_gyroscope_Y, BLERead | BLENotify);
BLEFloatCharacteristic gyroscopeCharacteristicZ(BLE_UUID_gyroscope_Z, BLERead | BLENotify);

float x, y, z;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  Serial.print("gyroscope sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");

  // initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }

  // set advertised local name and service UUID
  BLE.setDeviceName(BLE_DEVICE_NAME);
  BLE.setLocalName(BLE_LOCAL_NAME);
  BLE.setAdvertisedService(gyroscopeService);

  gyroscopeService.addCharacteristic(gyroscopeCharacteristicX);
  gyroscopeService.addCharacteristic(gyroscopeCharacteristicY);
  gyroscopeService.addCharacteristic(gyroscopeCharacteristicZ);

  BLE.addService(gyroscopeService);

  gyroscopeCharacteristicX.writeValue(0);
 gyroscopeCharacteristicY.writeValue(0);
 gyroscopeCharacteristicZ.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE gyroscope Peripheral");
}

void loop() {
  BLEDevice central = BLE.central();

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
    Serial.println(x);
    gyroscopeCharacteristicX.writeValue(x);
   gyroscopeCharacteristicY.writeValue(y);
    gyroscopeCharacteristicZ.writeValue(z);
    delay(100);
  } 
}

