# BLE Documentation 

## Overview 
• A wrapper around the Bluetooth Low Energy protocol for the ESP32 microcontroller.

## Topics:
•[Public Methods](#public-methods)
•[Public Variables](#public-variables)
•[Private Methods](#private-methods)
•[Private Variables ](#private-variables)
•[Static Members](#static-members)
•[Examples](#examples)



## Public Methods
1. [setDeviceName](#1-setdevicename)
2. [begin](#2-begin)
3. [receivedDataAsIntArray](#3-receiveddataasintarray)
4. [receivedDataAsDoubleArray](#4-receiveddataasdoublearray)
5. [receivedDataAsFloatArray](#5-receiveddataasfloatarray)
6. [receivedDataAsString](#6-receiveddataasstring)
7. [sendDataArray](#7-senddataarray)
8. [sendDataPoint](#8-senddatapoint)
9. [releaseMemoryToSystem](#9-releasememorytosystem)



### 1. setDeviceName

##### Method Definition
```c++
void setDeviceName(std::string name);
```
##### Method Description 
• It takes an `std::string` and sets it as the name which the bluetooth device will be discoverable by other devices.



### 2. begin
##### Method Definition
```c++
void begin();
```
##### Method Description 
• It sets up and initializes the whole Bluetooth low energy server and characteristic callbacks with the following UUIDs:

|data type| variable | value |
|---------|----------|-------|
|`const char*`|SERVICE_UUID|`ab0828b1-198e-4351-b779-901fa0e0371e`|
|`const char*`|CHARACTERISTIC_UUID_RX|`4ac8a682-9736-4e5d-932b-e9b31405049c`|
|`const char*`|CHARACTERISTIC_UUID_TX|`0972EF8C-7613-4075-AD52-756F33D4DA91`|



### 3. receivedDataAsIntArray
##### Method Definition
```c++
int* receivedDataAsIntArray();
```
##### Method Description 
• Retrieves the last data passed by the other device as a pointer to an int array.
• If there is no data to be processed it will return a `NULL` pointer*.
• It is highly recommended that you check rather the response was a null pointer before doing any data processing, to avoid errors and exceptions. 



### 4. receivedDataAsDoubleArray
##### Method Definition
```c++
float* receivedDataAsDoubleArray();
```
##### Method Description 
• Retrieves the last data passed by the other device as a pointer to an array of doubles.
• If there is no data to be processed it will return a `NULL` pointer*.
• It is highly recommended that you check rather the response was a null pointer before doing any data processing, to avoid errors and exceptions.





### 5. receivedDataAsFloatArray
##### Method Definition
```c++
float* receivedDataAsFloatArray();
```
##### Method Description 
• Retrieves the last data passed by the other device as a pointer to a float array.
• If there is no data to be processed it will return a `NULL` pointer*.
• It is highly recommended that you check rather the response was a null pointer before doing any data processing, to avoid errors and exceptions.




### 6. receivedDataAsString
##### Method Definition
```c++
String receivedDataAsString();
```
##### Method Description 
• Retrieves the last data passed by the other device as a `String`.



### 7. sendDataArray
##### Template Method Definition
```c++
template <typename T>
void sendDataArray(T data[], int array_size){...}
```
##### Method Description 
• A template method that converts an array of any data type into a CSV formatted c string(`char*`) and sends it via BLE.




### 8. sendDataPoint
##### Template Method Definition
```c++
template <typename T>
void sendDataPoint(T data_point){...}
```
##### Method Description 
• A template method that converts a value of any data type into a c string(`char*`) and sends it via BLE.




### 9. releaseMemoryToSystem
##### Template Method Definition
```c++
template <typename T>
void static releaseMemoryToSystem(T* data){...}
```
##### Method Description 
• A template method that takes a pointer to an array that was allocated dynamically, and releases the memory back to the system.




## Public Variables
• The only public variable is a custom datatype(struct) used to store all the information related to the received data, that will be used to parse and convert it.
```c++
// Create struct that will hold all essential infromation about the parsed data 
struct data_information{

    int array_size;
    std::string* data_array;

};
```


## Private Methods

•The only private method is the parsing algorithm. It takes the incoming `std::string` data in a CSV format and splits the data points into an array of `std::strings`. It return the struct containing the size of the array and the array itself.
•Since this method is private it cannot be accessed by the end user, and it's used inside other methods (e.g [receiveDataAsIntArray](#3-receiveddataasintarray))

#### Method definition 
```c++
BLE::data_information parseData(std::string data);
```



## Private Variables 
•Internal variables that are accessible only by methods:

|Data Type|Variable Name|Default value|
|---------|-------------|-------------|
|`const char*`|SERVICE_UUID|`ab0828b1-198e-4351-b779-901fa0e0371e`|
|`const char*`|CHARACTERISTIC_UUID_RX|`4ac8a682-9736-4e5d-932b-e9b31405049c`|
|`const char*`|CHARACTERISTIC_UUID_TX|`0972EF8C-7613-4075-AD52-756F33D4DA91`|
|`std::string`|DeviceName|`"ESP32-BLE"`|


## Static members 
• A static member is a variable or a method that can be accessed without having the need to instantiate the class nor use an object. Furthermore, it is common to all instances of the class (i.e They share the same variable).
#### 1. deviceConnected 
• `bool` variable that is `true` when there is a device connected via BLE, or `false` when there is no device.

##### Using it
```c++
// If there is a device connected 
if (Bluetooth::BLE::deviceConnected){...}

// If there is no device...
else if (!Bluetooth::BLE::deviceConnected){...}
```

## Examples

```c++
// Include the .h file 
#inlude "BLE.h"

// instantiate an object 
Bluetooth::BLE bluetooth;

void setUp(){
    
    // set the device name
    bluetooth.setDeviceName("Weather_Station");
    // Start the BLE server
    bluetooth.begin();
}

void loop(){

    // Check to see if a device is connected, if it is send the sensor data
    if (Bluetooth::BLE::deviceConnected){

        //Read the sensor on pin 10
        int sensor_data = analogRead(10); 

        // Send the data 
        bluetooth.sendDataPoint(sensor_data);
    }
}
```