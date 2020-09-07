#include "BLE.h"
#include "Sensor.h"
#define log(x) Serial.println(x);

Bluetooth::BLE bluetooth;
// Sensor sensor;




void setup()
{

  // initialize the serial monitor 
  Serial.begin(115200);

  // Add the characteristics that will be used 
  bluetooth.add_characteristic("Potato", "12312312-31234343-43232342");
  bluetooth.begin();

}
  
void loop() {
  
  delay(1000);
  
  //se existe algum dispositivo conectado e nova mensagem 
  if(Bluetooth::BLE::deviceConnected && Bluetooth::BLE::new_message)
  {
    // Receive the data as an array of doubles from the  "Potato" characteristic 
    bluetooth.use_characteristic("Potato"); // Change to the specific characteristic 
    double* incoming_data = bluetooth.receivedDataAsDoubleArray();

    // Mark message as read
    Bluetooth::BLE::new_message = false;

    // Check if there is actual data or if it's null 
    if (incoming_data != NULL){

      log(incoming_data[0]);
      int monitoring_type = incoming_data[0]; 

      switch (monitoring_type)  
      {
      case 0:
        /* call voltometria cíclica */
        break;
      
      case 1:
        /*call Amperometria*/
        break;
      }


      // After using the data in the BLE call, release the memory back to the system to avoid memory leaking
      Bluetooth::BLE::releaseMemoryToSystem(incoming_data);
    }
    
  }
}
