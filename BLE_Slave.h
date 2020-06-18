#ifndef BLE_SLAVE_H
#define BLE_SLAVE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <BLEAdvertisedDevice.h>


#include<Arduino.h>
#include<string.h>

#define log(x) Serial.println(x)

namespace Bluetooth_slave
{

    class BLE_Slave{

        // private variable
        private:
            BLEClient* bluetooth_client;
            BLERemoteService* bluetooth_service;
            BLERemoteCharacteristic* bluetooth_target_characteristc;

            const char* service_uuid;
            const char* characteristic_uuid;


        // public variables 
        public:

            // Create struct that will hold all essential infromation about the parsed data 
            struct data_information{

                int array_size;
                std::string* data_array;
            };

            // Variable that will hold the name of the target device 
            std::string target_device_name; 

            // Variable that will hold the address of the target device
            BLEAddress* target_device_address;

            // Scan object
            BLEScan* scan;


            // Last data passed in the notification 
            static data_information parsed_data_info;
            static bool message_read;


            // Bool variable to keep track rather we are connected to the server 
            bool connected_to_server = false;


        // Public methods 
        public:

            // Constructor for the class, it takes the name of the target deivce, the service uuid and the characteristic_uuid
            BLE_Slave(std::string t_name, const char* s_uuid, const char* c_uuid);

            // void method to connect to the server 
            void connect_to_server();

            // Method is called whenever the server issues a notification, it receives the data, parses it and sets the message_read to false
            static void notifyCallback(BLERemoteCharacteristic* r_characteristic ,uint8_t* pData,size_t length, bool isNotify){

                // Create an std::string from the received data it 
                std::string string_data((char*)pData);

                // Parse the data and receive the struct with the data and information related to it
                parsed_data_info = BLE_Slave::parseData(string_data);

                // Set the message read to false 
                Bluetooth_slave::BLE_Slave::message_read = false;
            }



            // Private method that takes an std::string and slices it into an array of chars 
            static BLE_Slave::data_information parseData(std::string data);

            // Mehtod that will receive the string data, parse it and return an int array with all datapoints 
            int* receivedDataAsIntArray();
            // Mehtod that will receive the string data, parse it and return a float array with all datapoints 
            float* receivedDataAsFloarArray();

            // Release memory allocated dynamically by an array back to the system
            template <typename T>
            void static releaseMemoryToSystem(T* data){

                delete[] data;
            }
    
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