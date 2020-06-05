#include "BLE.h"

namespace Bluetooth{

    // Methods for the ServerCallbacks class
    void ServerCallbacks::onConnect(BLEServer* pServer){

        BLE.deviceConnected = true;
    }

    void ServerCallbacks::onDisconnect(BLEServer* pServer){

        BLE.deviceConnected = false;
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
    BLE::deviceConnected = false;

    void BLE::begin(){

        // Create the BLE Device
        BLEDevice::init("ESP32-BLE"); // nome do dispositivo bluetooth

        // Create the BLE Server
        BLEServer *server = BLEDevice::createServer(); //cria um BLE server 

        // Set the server's callback
        server->setCallbacks(new ServerCallbacks());

        // Create the BLE Service
        BLEService *pservice = pserver->createService(SERVICE_UUID);

        // Create a BLE Characteristic para envio de dados
        characteristicTX = pservice->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
        characteristicTX->addDescriptor(new BLE2902());

        // Create a BLE Characteristic para recebimento de dados
        BLECharacteristic *pcharacteristic = pservice->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
        pcharacteristic->setCallbacks(new CharacteristicCallbacks());

        // Start the service
        pservice->start();

        // Start advertising (descoberta do ESP32)
        pserver->getAdvertising()->start();
    }
}