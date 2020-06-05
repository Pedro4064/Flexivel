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
  // put your main code here, to run repeatedly:
  delay(1000);
  //se existe algum dispositivo conectado
  if(Bluetooth::BLE::deviceConnected)
  {
    if(Serial.available() > 0)
    {
      sensor.VoltametriaCiclica(25, 25, 25, 150, 12.5, 1);
    }

    if(Serial.available() == 2)
    {
      sensor.Amperometria(0, 2, 4, 10, 0.2);
    }

    /*if(Serial.available() == 3)
    {
      VoltametriaDiferencial();
    }

    if(Serial.available() == 4)
    {
      VoltametriaQuadrada();
    }

    if(Serial.available() == 5)
    {
      Potenciometria();
    }*/
  }
}
