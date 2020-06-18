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

            // connect to the server 
            bluetooth_module->connect_to_server();
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

    void BLE_Slave::connect_to_server(){

        // create a client 
        bluetooth_client = BLEDevice::createClient();

        // Connect to the device and pass (and dereference) the target's address
        bluetooth_client->connect(*target_device_address);

        // Request a reference for the service (so we know which one and can read/write for that service)
        bluetooth_service = bluetooth_client->getService(*service_uuid);

        // Ask for the reference for the characterist we are interested in 
        bluetooth_target_characteristc = bluetooth_service->getCharacteristic(*characteristic_uuid);

    }
} // namespace Bluetooth_slave
