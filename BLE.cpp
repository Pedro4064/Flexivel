#include "BLE.h"

namespace Bluetooth{

    // Methods for the ServerCallbacks class
    void ServerCallbacks::onConnect(BLEServer* pServer){

        BLE::deviceConnected = true;
    }

    void ServerCallbacks::onDisconnect(BLEServer* pServer){

        BLE::deviceConnected = false;
    }




    // Methods for the CharacteristicCallbacks class 
    void CharacteristicCallbacks::onWrite(BLECharacteristic *characteristic){


        //retorna ponteiro para o registrador contendo o valor atual da caracteristica
        std::string rxValue = characteristic->getValue(); 

        //verifica se existe dados (tamanho maior que zero)
        if (rxValue.length() > 0){

            for (int i = 0; i < rxValue.length(); i++) {
                Serial.print(rxValue[i]);
            }

            Serial.println();
        }
    }



    // Methods and variables for the BLE wrapper 
    bool BLE::deviceConnected = false;

    void BLE::begin(){

        // Create the BLE Device
        BLEDevice::init("ESP32-BLE"); // nome do dispositivo bluetooth

        // Create the BLE Server
        BLEServer *pserver = BLEDevice::createServer(); //cria um BLE server 

        // Set the server's callback
        pserver->setCallbacks(new ServerCallbacks());

        // Create the BLE Service
        BLEService *pservice = pserver->createService(SERVICE_UUID);

        // Create a BLE Characteristic para envio de dados
        characteristicTX = pservice->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
        characteristicTX->addDescriptor(new BLE2902());

        // Create a BLE Characteristic para recebimento de dados
        pcharacteristic = pservice->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
        pcharacteristic->setCallbacks(new CharacteristicCallbacks());

        // Start the service
        pservice->start();

        // Start advertising (descoberta do ESP32)
        pserver->getAdvertising()->start();
    }
    
    void BLE::sendData(double data[]){

        // Create char buffer that will hold the string values 
        char buffer[5];

        // Format the string
        sprintf(buffer,"%g,%g,%g", data[0],data[1],data[2]);

        // Send the string and notify the cliente 
        characteristicTX->setValue(buffer);
        characteristicTX->notify();

    }

    char* BLE::parseData(std::string data){

        // pointer to char array that will store only the values for the important data (it will ignore , separating the data)
        // (data.length() - 1) / 2  ---- It calculates the number of data points in the string (subtracting the , separating them)
        char* data_characters = new char[(data.length() - 1) / 2]; 

        // iterate through the string, add the characters and ignore the ,
        int counter = 0;
        for (char& character : data){

            // Ignores the ,
            if (character == ',')continue;

            // Add the char to the array 
            data_characters[counter] = character;
            counter++;
        }

        return data_characters;

    }
    
    double* BLE::receivedData(){

        // Get the data 
        std::string rxValue = pcharacteristic->getValue(); 

        // If the data is empty, return a null pointer 
        if (rxValue.length() <= 0){

            return NULL;
        }
    
        else{

            // parse the data 
            char* parsed_data = BLE::parseData(rxValue);

        }
    }
}
