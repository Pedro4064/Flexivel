#ifndef BLE_H
#define BLE_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include<Arduino.h>
#include<string.h>

namespace Bluetooth
{

    //callback para receber os eventos de conexão de dispositivos
    class ServerCallbacks: public BLEServerCallbacks {
    
        void onConnect(BLEServer* pServer);
    
        void onDisconnect(BLEServer* pServer);
    };

    //callback para eventos das características
    class CharacteristicCallbacks: public BLECharacteristicCallbacks {

            void onWrite(BLECharacteristic *characteristic);

    };

    //'wrapper' involta de todo o processo de inicializar e comunicar via BLE
    class BLE {

        // Private Variables
        private:

            const char* SERVICE_UUID = "ab0828b1-198e-4351-b779-901fa0e0371e";
            const char* CHARACTERISTIC_UUID_RX = "4ac8a682-9736-4e5d-932b-e9b31405049c";
            const char* CHARACTERISTIC_UUID_TX = "0972EF8C-7613-4075-AD52-756F33D4DA91";

            BLECharacteristic* characteristicTX;  // Através desse objeto iremos enviar dados para o client
            BLECharacteristic* pcharacteristic;   // BLE Characteristic para recebimento de dados
       
            std::string DeviceName = "ESP32-BLE"; // Default name for the device 


        // Public Variables 
        public:

            // Create struct that will hold all essential infromation about the parsed data 
            struct data_information{

                int array_size;
                std::string* data_array;
            };

            // Variavel static para que possa ser acessada sem ter que necessariamente instanciar a classe.
            // True se algum dispositivo estiver conectado ao BLE, false otherwise
            static bool deviceConnected;
            
            // Variable that tracks rather there are new messages sent 
            static bool new_message;

        // Private Methods 
        private:

            // Private method that takes an std::string and slices it into an array of chars 
            BLE::data_information parseData(std::string data);
            

        // Public Methods 
        public:

            // Method que será chamada no main void setUp() para inicializar todo o sistema BLE 
            void begin();

            // Method to set the name which the ESP32 will be discoverable to other deivces
            void setDeviceName(std::string name);

           
           
            // Method que irá receber e parse the incomming data into an array of boubles and return its pointer
            double* receivedDataAsDoubleArray();
           
            // Mehtod that will receive the string data, parse it and return an int array with all datapoints 
            int* receivedDataAsIntArray();
           
            // Mehtod that will receive the string data, parse it and return a float array with all datapoints 
            float* receivedDataAsFloatArray();
           
            // Method que irá receber e transformar the incomming data into a string 
            String receivedDataAsString();

           
           
            // Method template that will take the data array, convert to string, send and notify the client
            template <typename T>
            void sendDataArray(T data[], int array_size, String delimiter){

                String data_string = ""; //String variable that will hold the string verion of the Variables

                for (int i = 0; i < array_size; i++){

                    // Attack the new string formatted data to the data_string variable 
                    data_string+=String(data[i]);

                    // If its not the last number in the array, add the delimiter
                    if (i != array_size-1) data_string+=delimiter;
    
                }

                // Send the string and notify the cliente 
                std::string value (data_string.c_str());
                characteristicTX->setValue(value); //Make the string a c string(char*) to be sent via BLE 
                characteristicTX->notify();

            }

            // Method template that will send one data point via BLE 
            template <typename T>
            void sendDataPoint(T data_point){

                // Send the data and notify the cliente, setValue has many overloading 
                characteristicTX->setValue(data_point);
                characteristicTX->notify();
            }
           
            // Release memory allocated dynamically by an array back to the system
            template <typename T>
            void static releaseMemoryToSystem(T* data){

                delete[] data;
            }
    };
    
} // namespace Bluetooth
#endif
