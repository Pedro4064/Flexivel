#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include<Arduino.h>

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

        private:

            const char* SERVICE_UUID = "ab0828b1-198e-4351-b779-901fa0e0371e";
            const char* CHARACTERISTIC_UUID_RX = "4ac8a682-9736-4e5d-932b-e9b31405049c";
            const char* CHARACTERISTIC_UUID_TX = "0972EF8C-7613-4075-AD52-756F33D4DA91";

            BLECharacteristic* characteristicTX; //através desse objeto iremos enviar dados para o client
        
        public:

            // Variavel static para que possa ser acessada sem ter que necessariamente instanciar a classe 
            static bool deviceConnected;

            // Method que será chamada no main void setUp() para inicializar todo o sistema BLE 
            void begin();
    };
    
} // namespace Bluetooth