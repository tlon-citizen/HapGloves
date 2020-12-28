/* Haptics Micro-controller Library
 * UHH HCI 2015
 * ariza@informatik.uni-hamburg.de
 */

#ifndef __HCI_HAPTICS_CONTROLLER_H__
#define __HCI_HAPTICS_CONTROLLER_H__

#include "BLEDevice.h"
#include "GattAttribute.h"
#include "Adafruit_PWMServoDriver.h"

Serial pc(USBTX, USBRX);
DigitalOut led1(LED1);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(P0_4, P0_5);

class HCIHapticService
{
private:
    float mapValue(float x, float in_min, float in_max, float out_min, float out_max)
    {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    
    virtual void onDataWritten(const GattCharacteristicWriteCBParams *params) 
    {
        //((float)WritableCharValue0[0] >= 32.0f) ? led1 = 0 : led1 = 1;
        //pc.printf("%d \n", count);
        
        if(!pwmStarted)
        {
            pwm.begin();
            pwm.setPWMFreq(360);  // Analog servos run at ~60 Hz updates    
            pwmStarted = true;
        }

        for(int i = 0; i < 16; i++)
        {
            if (params->charHandle == handleC[i]) 
            {
                WritableCharValue[0] = (*params->data);
                float value = mapValue((float)WritableCharValue[0], 0.0f, 255.0f, 0.0f, 1024.0f * 3.0f);
                pwm.setPWM(i, 0, value);
            }
        }
    }

public:
    HCIHapticService
    (
        BLEDevice  &_ble,
        const char *manufacturersName = NULL,
        const char *modelNumber       = NULL,
        const char *serialNumber      = NULL,
        const char *hardwareRevision  = NULL,
        const char *firmwareRevision  = NULL,
        const char *softwareRevision  = NULL
    )   :
        ble(_ble),
        manufacturersNameStringCharacteristic
        (
            GattCharacteristic::UUID_MANUFACTURER_NAME_STRING_CHAR,
            (uint8_t *)manufacturersName,
            (manufacturersName != NULL) ? strlen(manufacturersName) : 0, /* minLength */
            (manufacturersName != NULL) ? strlen(manufacturersName) : 0, /* maxLength */
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        ),
        modelNumberStringCharacteristic
        (
            GattCharacteristic::UUID_MODEL_NUMBER_STRING_CHAR,
            (uint8_t *)modelNumber,
            (modelNumber != NULL) ? strlen(modelNumber) : 0, /* minLength */
            (modelNumber != NULL) ? strlen(modelNumber) : 0, /* maxLength */
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        ),
        serialNumberStringCharacteristic
        (
            GattCharacteristic::UUID_SERIAL_NUMBER_STRING_CHAR,
            (uint8_t *)serialNumber,
            (serialNumber != NULL) ? strlen(serialNumber) : 0, /* minLength */
            (serialNumber != NULL) ? strlen(serialNumber) : 0, /* maxLength */
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        ),
        hardwareRevisionStringCharacteristic
        (
            GattCharacteristic::UUID_HARDWARE_REVISION_STRING_CHAR,
            (uint8_t *)hardwareRevision,
            (hardwareRevision != NULL) ? strlen(hardwareRevision) : 0, /* minLength */
            (hardwareRevision != NULL) ? strlen(hardwareRevision) : 0, /* maxLength */
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        ),
        firmwareRevisionStringCharacteristic
        (
            GattCharacteristic::UUID_FIRMWARE_REVISION_STRING_CHAR,
            (uint8_t *)firmwareRevision,
            (firmwareRevision != NULL) ? strlen(firmwareRevision) : 0, /* minLength */
            (firmwareRevision != NULL) ? strlen(firmwareRevision) : 0, /* maxLength */
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        ),
        softwareRevisionStringCharacteristic
        (
            GattCharacteristic::UUID_SOFTWARE_REVISION_STRING_CHAR,
            (uint8_t *)softwareRevision,
            (softwareRevision != NULL) ? strlen(softwareRevision) : 0, /* minLength */
            (softwareRevision != NULL) ? strlen(softwareRevision) : 0, /* maxLength */
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
        )
    {
        for(int i = 0; i < 16; i++)
        {
            WritableCharValue[i] = 0;
            WritableChar[i] = new GattCharacteristic(0xFFAA + i, (uint8_t *)&WritableCharValue[i], sizeof(WritableCharValue[i]), sizeof(WritableCharValue[i]), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);
        }
        
        pc.baud(9600);
        
        led1 = 1;
        pwmStarted = false;

        setupService();
    }
    
    void processInputs() 
    {
        //
    }
    
private:
    void setupService(void) 
    {
        static bool serviceAdded = false; 
        if (serviceAdded) 
        {
            return;
        }

        GattCharacteristic *charTable[22];
        
        charTable[0] = &manufacturersNameStringCharacteristic;
        charTable[1] = &modelNumberStringCharacteristic;
        charTable[2] = &serialNumberStringCharacteristic;
        charTable[3] = &hardwareRevisionStringCharacteristic;
        charTable[4] = &firmwareRevisionStringCharacteristic;
        charTable[5] = &softwareRevisionStringCharacteristic;
        for(int i = 0; i < 16; i++)
        {
            charTable[6+i] = WritableChar[i];
        }
        
        GattService service
        (  
            GattService::UUID_DEVICE_INFORMATION_SERVICE, 
            charTable,
            sizeof(charTable) / sizeof(GattCharacteristic *)
        );

        ble.addService(service);

        serviceAdded = true;
        
        ble.onDataWritten(this, &HCIHapticService::onDataWritten);
        
        for(int i = 0; i < 16; i++)
        {
            handleC[i] = WritableChar[i]->getValueAttribute().getHandle();
        }
    }

private:
    BLEDevice &ble;
    
    GattCharacteristic  manufacturersNameStringCharacteristic;
    GattCharacteristic  modelNumberStringCharacteristic;
    GattCharacteristic  serialNumberStringCharacteristic;
    GattCharacteristic  hardwareRevisionStringCharacteristic;
    GattCharacteristic  firmwareRevisionStringCharacteristic;
    GattCharacteristic  softwareRevisionStringCharacteristic;
    
    bool pwmStarted;
    
    uint8_t WritableCharValue[16];
    GattCharacteristic* WritableChar[16];
    uint16_t handleC[16];
};

#endif /* #ifndef __HCI_HAPTICS_CONTROLLER_H__*/
