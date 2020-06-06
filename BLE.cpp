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
 
    template<typename T>
    void BLE::sendData(T data[]){

        // Create char buffer that will hold the string values 
        char buffer[5];

        // Format the string
        sprintf(buffer,"%g,%g,%g", data[0],data[1],data[2]);

        // Send the string and notify the cliente 
        characteristicTX->setValue(buffer);
        characteristicTX->notify();

    }
}
