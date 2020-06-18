#ifndef BLE_SLAVE_H
#define BLE_SLAVE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <BLEAdvertisedDevice.h>


#include<Arduino.h>
#include<string.h>

namespace Bluetooth_slave
{

    class BLE_Slave{

        public:
            // Variable that will hold the name of the target device 
            std::string target_device_name; 

            // Variable that will hold the address of the target device
            BLEAddress* target_device_address;

            // Scan object
            BLEScan* scan;
            
            // Constructor for the class, it takes the name of the target deivce as an std::string 
            BLE_Slave(std::string t_name);
    };

    class MyCallbacks : public BLEAdvertisedDeviceCallbacks{

        private:
            // reference to the ble class so we can change the variables and stop the scanning 
            BLE_Slave* bluetooth_module;

        public: 

            // Constructor that takes a reference to an BLE_Slave object
            MyCallbacks(BLE_Slave* ble_module);

            // Method called whenever a device is found while scanning, if the device name is  == the target's, stop the scan 
            // and save it's id
            void onResult(BLEAdvertisedDevice device);
    };
} // namespace Bluetooth_slave

#endif