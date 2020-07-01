#include "BLE.h"
#include "Sensor.h"
#define log(x) Serial.println(x);

Bluetooth::BLE bluetooth;
Sensor sensor;


void setup()
{

  Serial.begin(115200);
  bluetooth.begin();

}
  
void loop() {
  
  delay(1000);
  
  //se existe algum dispositivo conectado
  if(Bluetooth::BLE::deviceConnected)
  {
    // Receive the data as an array of doubles 
    double* incoming_data = bluetooth.receivedDataAsDoubleArray();

    // Check if there is actual data or if it's null 
    if (incoming_data != NULL){

      log(incoming_data[0]);
      
      // If the data collection type is set to 0, it's VoltametriaCiclica
      if(incoming_data[0] == 0)
      {
        sensor.VoltametriaCiclica(bluetooth,incoming_data);
      }

      // If the data collection type is set to 1, it's Amperometria
      else if(incoming_data[0] == 1)
      {
        sensor.Amperometria(bluetooth,0, 2, 4, 10, 0.2);
      }


      // After using the data in the BLE call, release the memory back to the system to avoid memory leaking
      Bluetooth::BLE::releaseMemoryToSystem(incoming_data);
    }
  }
}
