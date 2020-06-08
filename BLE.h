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

            BLECharacteristic* characteristicTX; //através desse objeto iremos enviar dados para o client
            BLECharacteristic* pcharacteristic; // BLE Characteristic para recebimento de dados
       
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

        // Private Methods 
        private:

            // Private method that takes an std::string and slices it into an array of chars 
            BLE::data_information parseData(std::string data);
            

        // Public Methods 
        public:

            // Method que será chamada no main void setUp() para inicializar todo o sistema BLE 
            void begin();

            // Method that will take the data array, convert to string, send and notify the client
            void sendData(double data[]);

            // Method overload que irá receber e parse the incomming data into an array of boubles and return its pointer
            double* receivedData();

            // // Method overload que irá receber e parse the incomming data into an array of ints and return its pointer
            // int* receivedData();

            // // Method overload que irá receber e parse the incomming data into an array of ints and return its pointer
            // float* receivedData();

            template <typename T>
            static void releaseMemoryToSystem(T* data);
    };
    
} // namespace Bluetooth
#endif
