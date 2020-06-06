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
    if(Serial.available() > 0)
    {
      sensor.VoltametriaCiclica(bluetooth,25, 25, 25, 150, 12.5, 1);
    }

    if(Serial.available() == 2)
    {
      sensor.Amperometria(0, 2, 4, 10, 0.2);
    }

  }
}
