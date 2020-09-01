#include "Sensor.h"
#include<Arduino.h>

#define log(x) Serial.println(x);


Sensor sensor;


void setup()
{

  Serial.begin(115200);

}
  
void loop() {
  
  log("Iniciando Medições ...");

  // O array que contém os dados na seguinte ordem : 
  /* We will be getting the array in the following format
        
        0;0.0;0.0;0.0;0.0;100;100;500;100;100;2
        
        where:
            array[0] - will determine the measuring type 
            array[1-4] - Pré-tratamento 
            array[5-10] - arguments : voltageIni: 100mV
                                      voltageEnd: 100mV
                                      voltageInv: 500mV
                                      ScanRate: 100mV/s
                                      step: 100mV
                                      cicles: 2
    */


    double incoming_data[] = {0,0.0,0.0,0.0,0.0,100,100,500,100,100};
    log(incoming_data[0]);

    sensor.VoltametriaCiclica(incoming_data);
  


}