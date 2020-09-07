#include "BLE.h"

namespace Bluetooth{

    // Methods for the ServerCallbacks class
    void ServerCallbacks::onConnect(BLEServer* pServer){

        BLE::deviceConnected = true;
    }

    void ServerCallbacks::onDisconnect(BLEServer* pServer){

        BLE::deviceConnected = false;
    }




    // Methods for the CharacteristicCallbacks class 
    void CharacteristicCallbacks::onWrite(BLECharacteristic *characteristic){

        // Since there is a new message, set the bool variable to true 
        BLE::new_message = true;

        //retorna ponteiro para o registrador contendo o valor atual da caracteristica
        std::string rxValue = characteristic->getValue(); 

        //verifica se existe dados (tamanho maior que zero)
        if (rxValue.length() > 0){

            for (int i = 0; i < rxValue.length(); i++) {
                Serial.print(rxValue[i]);
            }

            Serial.println();
        }
    }




    // Methods and variables for the BLE wrapper 
    bool BLE::deviceConnected = false;
    bool BLE::new_message = false; 

    void BLE::begin(){

        // Create the BLE Device
        BLEDevice::init(DeviceName); // nome do dispositivo bluetooth

        // Create the BLE Server
        pserver = BLEDevice::createServer(); //cria um BLE server 

        // Set the server's callback
        pserver->setCallbacks(new ServerCallbacks());

        // Create the BLE Service
        pservice = pserver->createService(SERVICE_UUID);


        // * Iterate though the map containing all the characteristics informations and initialize them 
        for (auto& characteristic : characteristics_info){

            // Create a BLE Characteristic 
            BLECharacteristic* pcharacteristic = pservice->createCharacteristic(characteristic.second, BLECharacteristic::PROPERTY_NOTIFY|
                                                                                    BLECharacteristic::PROPERTY_READ |
                                                                                    BLECharacteristic::PROPERTY_WRITE);
                                                                                    
            pcharacteristic->addDescriptor(new BLE2902());
            pcharacteristic->setCallbacks(new CharacteristicCallbacks());

            // add it to the map 
            characteristics_list[characteristic.first] = pcharacteristic;

        }
        

        // Start the service
        pservice->start();

        // Start advertising (descoberta do ESP32)
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(false);
        pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
        BLEDevice::startAdvertising();
    }

    void BLE::setDeviceName(std::string name){

        // set the device name 
        DeviceName = name;
    }
   
    void BLE::use_characteristic(String characteristic_name){
        target_characteristic = characteristic_name;
    }

    void BLE::add_characteristic(String characteristic_name , const char* uuid){

            // Add the characteristic to the map 
            characteristics_info[characteristic_name] = uuid;

    }

    BLE::data_information BLE::parseData(std::string data){


        // Instantiate the struct that will hold all data info 
        BLE::data_information data_info;

        // iterate through the string, check how many ","  there are
        int comma_counter = 0;
        for (char& character : data){

            // count the number of ,
            if (character == ';')comma_counter++;
        }


        // Create and populate an array that will contain the position in which the , are in the string
        int* comma_position = new int[comma_counter];
        int iteration_counter = 0;
        int array_empty_spot = 0;

        // iterate through the string and add the position in which the , appears 
        for (char& character : data){

            // count the number of ,
            if (character == ';'){

                comma_position[array_empty_spot] = iteration_counter;
                array_empty_spot++;
            }

            iteration_counter++;
        }
        
       
        // Based on the number of commas separating the datapoints, calculate the array size 
        data_info.array_size = comma_counter+1;

        // Allocate the memory dynamically for the dataset 
        data_info.data_array = new std::string[data_info.array_size];
       
       
        // If there are no , it means that there is only one datapoint, so add it to the struct and return it
        if (comma_counter == 0){

            // Add the data to the struct 
            data_info.data_array[0] = data;

            // Release the memory used in the helper arrays back to the system 
            BLE::releaseMemoryToSystem(comma_position);

            // return the struct 
            return data_info;
        }

       
        // Create the substrings based on the comma positions for each data point that will be stored in the array 
        for (int i = 0; i < data_info.array_size; i++){

            // For the first data point, start from 0 up to comma_position[0] - 1 ----> -1 so it does not include the comma 
            if (i == 0){
                data_info.data_array[i] = data.substr(0,comma_position[0] - 1);
            }
            
            // for the last data point in the string  -----> Create a substring from the position of the last comma +1 (so it does not include the ,) up to the end of the string 
            else if (i == comma_counter){

                data_info.data_array[i] = data.substr(comma_position[i-1]+1);
            }

            // Create a substring from the position of the privious comma +1 (so it does not include the comma) up to the number of characters 
            // between commas -1 (so it does not include the last comma)
            else{
                data_info.data_array[i] = data.substr(comma_position[i-1]+1,((comma_position[i] - comma_position[i-1] - 1)));
            }

        }

        // Release the memory used by helper arrays back to the system 
        BLE::releaseMemoryToSystem(comma_position);

        // Return the struct 
        return data_info;

    }

    double* BLE::receivedDataAsDoubleArray(){

        // Get the data 
        std::string rxValue = characteristics_list[target_characteristic]->getValue(); 

        // If the data is empty, return a null pointer 
        if (rxValue.length() <= 0){

            return NULL;
        }
    
        // Else, parse the data and create an array with the correct data type from the array of string returned from the parser function
        else{

            // parse the data 
            BLE::data_information parsed_data_info = BLE::parseData(rxValue);

            // Create an array of type double to hold the values as actual numbers 
            double* data = new double[parsed_data_info.array_size];

            for (int number = 0; number < parsed_data_info.array_size; number++){

                // transfor std::string to char* 
                String string_data = parsed_data_info.data_array[number].c_str();

                // Populate the new double array with the double values converted from the Strings values
                data[number] = string_data.toDouble();
            }

            // Release memory back to the system 
            BLE::releaseMemoryToSystem(parsed_data_info.data_array);

            // return the pointer to the array 
            return data;
        }
    }
    
    int* BLE::receivedDataAsIntArray(){

        // Get the data 
        std::string rxValue = characteristics_list[target_characteristic]->getValue(); 

        // If the data is empty, return a null pointer 
        if (rxValue.length() <= 0){

            return NULL;
        }
    
        // Else, parse the data and create an array with the correct data type from the array of string returned from the parser function
        else{

            // parse the data 
            BLE::data_information parsed_data_info = BLE::parseData(rxValue);

            // Create an array of type double to hold the values as actual numbers 
            int* data = new int[parsed_data_info.array_size];

            for (int number = 0; number < parsed_data_info.array_size; number++){

                // transfor std::string to char* 
                String string_data = parsed_data_info.data_array[number].c_str();

                // Populate the new double array with the double values converted from the Strings values
                data[number] = string_data.toInt();
            }

            // Release memory back to the system 
            BLE::releaseMemoryToSystem(parsed_data_info.data_array);

            // return the pointer to the array 
            return data;
        }

    }

    float* BLE::receivedDataAsFloatArray(){

        // Get the data 
        std::string rxValue = characteristics_list[target_characteristic]->getValue(); 

        // If the data is empty, return a null pointer 
        if (rxValue.length() <= 0){

            return NULL;
        }
    
        // Else, parse the data and create an array with the correct data type from the array of string returned from the parser function
        else{

            // parse the data 
            BLE::data_information parsed_data_info = BLE::parseData(rxValue);

            // Create an array of type double to hold the values as actual numbers 
            float* data = new float[parsed_data_info.array_size];

            for (int number = 0; number < parsed_data_info.array_size; number++){

                // transfor std::string to char* 
                String string_data = parsed_data_info.data_array[number].c_str();

                // Populate the new double array with the double values converted from the Strings values
                data[number] = string_data.toFloat();
            }

            // Release memory back to the system 
            BLE::releaseMemoryToSystem(parsed_data_info.data_array);

            // return the pointer to the array 
            return data;
        }
        
    }
   
    String BLE::receivedDataAsString(){

        // Get the data 
        std::string rxValue = characteristics_list[target_characteristic]->getValue(); 

        // If the data is empty, return a null pointer 
        if (rxValue.length() <= 0){

            return "";
        }
    
        else{

            String data_string = rxValue.c_str();
            return data_string;
        }
    }
}
