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
        inline double currentCalculation(LMP91000& pstat,int z);
        inline double negativeMinVoltageCurrentCalculator(LMP91000& pstat, MAX5481& gpot, double Vref);
        inline double positiveMaxVoltageCurrentCalculator(LMP91000& pstat, MAX5481& gpot, double Vref);
        inline int calculateZ(double Vref);

        // Method that calculates and sends the current for each step in the measuring scenario in which the inversion voltage in positive 
        void positiveVoltageCurrentProcessing(Bluetooth::BLE& bluetooth, LMP91000& pstat, MAX5481& gpot, double& i);
    
    public: 
        Sensor();
        void VoltametriaCiclica(double data_array[]);
        void Amperometria(Bluetooth::BLE& bluetooth, double tensaoIni, double tensao, int tempo, int intervalo, double frequencia);
};

#endif