#include "BLE_Slave.h"

namespace Bluetooth_slave
{
    
    MyCallbacks::MyCallbacks(BLE_Slave* ble_module){
        bluetooth_module = ble_module;
    }

    void MyCallbacks::onResult(BLEAdvertisedDevice device){

        // This method is called whenever a device is found while scanning
        // If the device found has a te same name as the target device, save the address 
        if (device.getName() == bluetooth_module->target_device_name){

            *bluetooth_module->target_device_address = device.getAddress();
            bluetooth_module->scan->stop();
        }
    }
    

    BLE_Slave::BLE_Slave(std::string t_name){

        // Set the target's name the one passed as argument 
        target_device_name = t_name;

        //////// Start sequence ////////

        //Set the device name to table
        BLEDevice::init("Table"); 
        //Get a pointer to the scan object 
        scan = BLEDevice::getScan(); 
        //Set the callback function and pass a pointer to this instance of the BLE_Slave class
        scan->setAdvertisedDeviceCallbacks(new MyCallbacks(this)); 

        // start the scan for 60 seconds
        scan->start(60);
    }
} // namespace Bluetooth_slave
