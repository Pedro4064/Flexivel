#include "BLE_Slave.h"

std::string target_name = "ESP-32";
#define service_uuid "ab0828b1-198e-4351-b779-901fa0e0371e"
#define characteristic_uuid "4ac8a682-9736-4e5d-932b-e9b31405049c"


// Creat the instances of the classes and start the ble communication 
Bluetooth_slave::BLE_Slave ble_module(target_name, service_uuid, characteristic_uuid);

void setup(){

    Serial.begin(9600);
}

void loop(){

    // If we are connected to a server, continue to read sensor data 
    if (ble_module.connected_to_server){

        // Check to see if there are any new notifications 
        if (!ble_module.message_read){

            log("[test.ino] Data in the server notify:");
            // print all the data points in the notify 
            for (int i = 0; i < ble_module.parsed_data_info.array_size; i++){
                log(ble_module.parsed_data_info.data_array[i].c_str());

            }

            // If you want to receive the data, parse it and turn it into an int array you can:
            int* int_data = ble_module.receivedDataAsIntArray();
            // But remember to release memory back to the system 
            Bluetooth_slave::BLE_Slave::releaseMemoryToSystem<int>(int_data);
            
            // since we read the data, change the message_read value to true, so we don't read the same data twice
            ble_module.message_read = true;

        }
    }
}
