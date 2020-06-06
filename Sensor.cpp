#include "Sensor.h"

Sensor::Sensor(){

    Wire.begin();
    SPI.begin();
}

void Sensor::VoltametriaCiclica(Bluetooth::BLE& bluetooth,double passo, double tensaoIni, double tensaoFin, double tensaoInv, double velocidadeScan, int NumCiclos){

    LMP91000 pstat = LMP91000();
    MAX5481 gpot (5);

    double Step = passo;
    double voltageIni = tensaoIni;
    double voltageEnd = tensaoFin;
    double voltageInv = tensaoInv;
    double scanRate = velocidadeScan;
    double Vref;
    int cicles = NumCiclos;
    int z;

    delay(50);

    pstat.disableFET();
    pstat.setGain(6);
    pstat.setRLoad(0);
    pstat.setExtRefSource();
    pstat.setIntZ(1);
    pstat.setThreeLead();


    delay(2000); 

    // Check if the voltage is bove the limit 
    if (voltageIni/0.2 > 5000 || voltageEnd/0.2 > 5000)
    {
        Serial.println("Erro, tensão acima do máximo");
        return;
    }

    //alterar os pinos depois quando for testar com o potenciostato
    /*-----------------------------------*/
    const int oscpin = 0;
    const int outpin = 5;
    const int freq = 1000;
    const int res = 12;
    const int oscpin1 = 19;
    const int outpin1 = 34;

    
    ledcSetup(oscpin, freq, res);
    ledcAttachPin(outpin, oscpin);
    ledcWrite(oscpin, 2048);

    pinMode(outpin1, OUTPUT);
    /*-----------------------------------*/
    pstat.setBias(11);
    scanRate = (Step/scanRate)*1000;

    //Caso em que a tensão de inversão é positiva
    if(voltageInv > 0)
    {
        for (int j = 0; j < cicles; j++){

            for (double i = voltageIni; i < voltageInv; i = i + Step){
                
                // Array that will hold this iteration's data and at the end send it via bluetooth
                double data[3];

                // Variable that will hold the variable being calculated 
                double new_data;

                //Calculo da tensão de referência a ser usada no potenciostato
                Vref = (i/0.2);

                //Caso a tensão de referência for maior ou menor que |3.3|v,  será usado o potenciometro digital para fornecê-la
                if(Vref > 3300){

                    pstat.setPosBias();
                    z = Vref/5;
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // Calculate the current and send it 
                    // 
                    delay(scanRate);
                    continue;
                }

                else if(Vref < -3300){
                        
                    pstat.setNegBias();
                    z = Vref/5;
                    z = z*(-1);
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // Calculate the current and send it 
                    // 
                    delay(scanRate);
                    continue;
                }
                
                z = (((Vref)/5)*6);
                z = z + (z/6)/5;

                if(z < 0){
                        
                    z = z*(-1);
                    pstat.setNegBias();
                    ledcWrite(oscpin, z);
                    delay(1);
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.print(z);
                    Serial.print(" ; ");
                    new_data = pow(10,6)*pstat.getCurrent((z), 3300/1000.0, 12);
                    Serial.println(new_data);
                    
                }

                else{
                        
                    pstat.setPosBias();
                    ledcWrite(oscpin, z);
                    delay(1);
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.print(z);
                    Serial.print(" ; ");
                    new_data = pow(10,6)*pstat.getCurrent((z), 3300/1000.0, 12);
                    Serial.println(new_data);
                    
                }

                // Add the data points to the data array 
                data[0] = i;
                data[1] = z;
                data[2] = new_data;

                // Send the data 
                bluetooth.sendData(&new_data);
                delay(scanRate);
            }

            for (double i = voltageInv; i >= voltageEnd; i = i - Step)
            {

                // Array that will hold this iteration's data and at the end send it via bluetooth
                double data[3];

                // Variable that will hold the variable being calculated 
                double new_data;

                //Calculo da tensão de referência a ser usada no potenciostato
                Vref = (i/0.2);

                //Caso a tensão de referência for maior ou menor que |3.3|v,  será usado o potenciometro digital para fornecê-la
                if(Vref > 3300){
                        
                    pstat.setPosBias();
                    z = Vref/5;
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // Calculate the current and send it 
                    // 
                    delay(scanRate);
                    continue;
                }

                else if(Vref < -3300){
                    pstat.setNegBias();
                    z = Vref/5;
                    z = z*(-1);
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // Calculate the current and send it 
                    // 
                    delay(scanRate);
                    continue;
                }
                
                z = (((Vref)/5)*6);
                z = z + (z/6)/5;

                if(z > 0){

                    ledcWrite(oscpin, z);
                    delay(1);
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.print(z);
                    Serial.print(" ; ");
                    new_data = pow(10,6)*pstat.getCurrent((z), 3300/1000.0, 12);
                    Serial.println(new_data);
                    
                }

                else{
                        
                    z = z*(-1);
                    pstat.setNegBias();
                    ledcWrite(oscpin, z);
                    delay(1);
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.print(z);
                    Serial.print(" ; ");
                    new_data = pow(10,6)*pstat.getCurrent((z), 3300/1000.0, 12);
                    Serial.println(new_data);
                    
                }

                // Add the data points to the data array 
                data[0] = i;
                data[1] = z;
                data[2] = new_data;

                // Send the data 
                bluetooth.sendData(&new_data);
                
                // Wait the desired interval 
                delay(scanRate);
            }
        }

        pstat.setBias(0);
        return;
    }
    
    //Caso em que todas as variáveis do teste são negativas
    else if(voltageIni < 0  && voltageInv < 0 && voltageEnd < 0){

        voltageIni = voltageIni*(-1);
        voltageInv = voltageInv*(-1);
        voltageEnd = voltageEnd*(-1);
        Step = Step*(-1);
        pstat.setNegBias();

        for (int j = 0; j < cicles; j++){

            for (double i = voltageIni; i < voltageInv; i = i + Step){
                

                //Calculo da tensão de referência a ser usada no potenciostato
                Vref = (i/0.2);

                //Caso a tensão de referência for maior ou menor que |3.3|v,  será usado o potenciometro digital para fornecê-la
                if(Vref < -3300){

                    z = -Vref/5;
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // Calculate the current and send the data
                    // 
                    delay(scanRate);
                    continue;

                }
                
                z = (((Vref)/5)*6);
                z = z - (z/6)/5;
                z = z*(-1);
                ledcWrite(oscpin1, z);
                delay(1);
                Serial.print(i);
                Serial.print(" ; ");
                Serial.println(z);
                // 
                // calculate the current here, populate the array and send it
                // 
                delay(scanRate);
            }

            for (double i = voltageInv; i >= voltageEnd; i = i - Step){

                //Calculo da tensão de referência a ser usada no potenciostato
                Vref = (i/0.2);
                
                //Caso a tensão de referência for maior ou menor que |3.3|v,  será usado o potenciometro digital para fornecê-la
                if(Vref < -3300){

                    z = -Vref/5;
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // Calculate the current and send the data 
                    // 
                    delay(scanRate);
                    continue;
                }
                
                z = (((Vref)/5)*6);
                z = z - (z/6)/5;
                z = z*(-1);
                ledcWrite(oscpin1, z);
                delay(1);
                Serial.print(i);
                Serial.print(" ; ");
                Serial.println(z);
                // 
                // calculate the current here, populate the array and send it
                // 
                delay(scanRate);
            }
        }
        pstat.setBias(0);
        return;
    }
    
    //Caso em que a tensão de inversão é positiva
    else if(voltageInv < 0)
    {
        for (int j = 0; j < cicles; j++){

            for (double i = voltageIni; i > voltageInv; i = i - Step)
            {
                //Calculo da tensão de referência a ser usada no potenciostato
                Vref = (i/0.2);

                //Caso a tensão de referência for maior ou menor que |3.3|v,  será usado o potenciometro digital para fornecê-la
                if(Vref > 3300){
                        
                    pstat.setPosBias();
                    z = Vref/5;
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // Calculate the current and send the data 
                    // 
                    delay(scanRate);
                    continue;

                }

                else if(Vref < -3300){

                    pstat.setNegBias();
                    z = Vref/5;
                    z = z*(-1);
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // Calculate the current and send the data 
                    // 
                    delay(scanRate);
                    continue;

                }
                
                z = (((Vref)/5)*6);
                z = z + (z/6)/5;

                if(z < 0){

                    z = z*(-1);
                    pstat.setNegBias();
                    ledcWrite(oscpin1, z);
                    delay(1);
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // calculate the current here, populate the array 
                    // 
                }

                else{         

                    pstat.setPosBias();
                    ledcWrite(oscpin1, z);
                    delay(1);
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // calculate the current here, populate the array 
                    // 
                }

                // Send the data here 
                delay(scanRate);
            }
            
            for (double i = voltageInv; i <= voltageEnd; i = i + Step){

                //Calculo da tensão de referência a ser usada no potenciostato
                Vref = (i/0.2);
                
                //Caso a tensão de referência for maior ou menor que |3.3|v,  será usado o potenciometro digital para fornecê-la
                if(Vref > 3300){
                        
                    pstat.setPosBias();
                    z = Vref/5;
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    delay(scanRate);

                }

                else if(Vref < -3300){
                        
                    pstat.setNegBias();
                    z = Vref/5;
                    z = z*(-1);
                    gpot.setWiper(z);
                    gpot.writeWiper();
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    delay(scanRate);

                }
                
                z = (((Vref)/5)*6);
                z = z + (z/6)/5;
                
                if(z > 0){

                    pstat.setPosBias();
                    ledcWrite(oscpin1, z);
                    delay(1);
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // calculate the current here, populate the array 
                    // 
                    

                }

                else{
                        
                    z = z*(-1);
                    ledcWrite(oscpin1, z);
                    delay(1);
                    Serial.print(i);
                    Serial.print(" ; ");
                    Serial.println(z);
                    // 
                    // calculate the current here, populate the array 
                    // 
                    

                }

                // Send the data here 
                delay(scanRate);
            }
        }

        pstat.setBias(0);
        return;

    }
}

void Sensor::Amperometria(double tensaoIni, double tensao, int tempo, int intervalo, double frequencia)
{
  int16_t voltage = tensao;
  const double voltageIni = tensaoIni;
  double frequence = frequencia;
  unsigned long timeArray[2] = {tempo*1000, intervalo*1000};

  /*pstat.disableFET();
  pstat.setGain(6);
  pstat.setRLoad(0);
  pstat.setIntRefSource();
  pstat.setIntZ(1);
  pstat.setThreeLead();
  pstat.setBias(0);
  delay(2000); //warm-up time for the gas sensor

  for(int i = 0; i < 2; i++)
  { 
    //For pre-step voltage
    voltage = determineLMP91000Bias(voltage);
    if(voltage < 0) 
    {
      pstat.setNegBias();
    }
    else 
    {
      pstat.setPosBias();
    }
    unsigned long startTime = millis();
    pstat.setBias(abs(voltage));
    
    while(millis() - startTime < timeArray[i])
    {
      Serial.println(pstat.getOutput(A0) + 16);
      Serial.print(" ; ");
      Serial.println(pow(10,6)*pstat.getCurrent((pstat.getOutput(A0) + 16), 5000/1000.0, 10));
      delay(frequence);
    }
  }

  //End at 0V
  pstat.setBias(0);*/
}

signed char Sensor::determineLMP91000Bias(double Voltage)
{
  signed char polarity = 0;
  if(Voltage < 0) polarity = -1;
  else polarity = 1;
  
  int16_t v1 = 0;
  int16_t v2 = 0;
  
  Voltage = abs(Voltage);

  if(Voltage == 0) return 0;
  
  for(int i = 0; i < NUM_TIA_BIAS-1; i++){

    v1 = 5000*TIA_BIAS[i];
    v2 = 5000*TIA_BIAS[i+1];

    if(Voltage == v1) return polarity*i;
    else if(Voltage > v1 && Voltage < v2){
        
      if(abs(Voltage-v1) < abs(Voltage-v2)) return polarity*i;
      else return polarity*(i+1);

    }
  }

  return 0;
}
