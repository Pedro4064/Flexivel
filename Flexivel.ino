#include "BLE.h"
#include "Sensor.h"


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
    // Receive the data 
    double* incoming_data = bluetooth.receivedData();

    // Check if there is actual data or if it's null 
    if (incoming_data != NULL){

      // If the data collection type is set to 0, it's VoltametriaCiclica
      if(incoming_data[0] = 0)
      {
        sensor.VoltametriaCiclica(bluetooth,25, 25, 25, 150, 12.5, 1);
      }

      // If the data collection type is set to 1, it's Amperometria
      else if(incoming_data[0] = 1)
      {
        sensor.Amperometria(bluetooth,0, 2, 4, 10, 0.2);
      }


      // After using the data in the BLE call, release the memory back to the system to avoid memory leaking
      Bluetooth::BLE::releaseMemoryToSystem(incoming_data);
    }
  }
}
