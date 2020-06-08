#ifndef SENSOR_H
#define SENSOR_H

#include "BLE.h"
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <MAX5481.h>
#include <LMP91000.h>


class Sensor{

    private:
        
        signed char determineLMP91000Bias(double Voltage);

    public: 
        Sensor();
        void VoltametriaCiclica(Bluetooth::BLE& bluetooth,double passo, double tensaoIni, double tensaoFin, double tensaoInv, double velocidadeScan, int NumCiclos);
        void Amperometria(Bluetooth::BLE& bluetooth, double tensaoIni, double tensao, int tempo, int intervalo, double frequencia);
};

#endif