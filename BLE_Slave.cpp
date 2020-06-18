#include "BLE_Slave.h"

namespace Bluetooth_slave
{

    //////////////////////////// MyCallbacks Methods ////////////////////////////

    MyCallbacks::MyCallbacks(BLE_Slave* ble_module){
        bluetooth_module = ble_module;
    }

    void MyCallbacks::onResult(BLEAdvertisedDevice device){

        // Log the address of all devices found 
        log(device.getAddress().toString().c_str());
        
        // This method is called whenever a device is found while scanning
        // If the device found has a te same name as the target device, save the address 
        if (device.getName() == bluetooth_module->target_device_name){

            log("[BLE_SCAN] Found the target server");
            log(device.getName().c_str());
            log(device.getAddress().toString().c_str());
            
            log("");
            log("");
            log("");
            
            *bluetooth_module->target_device_address = device.getAddress();
            bluetooth_module->scan->stop();

            // connect to the server 
            bluetooth_module->connect_to_server();
        }
    }
    



    //////////////////////////// BLE_Slave variables ////////////////////////////
    
    bool BLE_Slave::message_read = false;
    BLE_Slave::data_information BLE_Slave::parsed_data_info;


    //////////////////////////// BLE_Slave methdos ////////////////////////////
    
    BLE_Slave::BLE_Slave(std::string t_name, const char* s_uuid, const char* c_uuid){

        // Set the target's name the one passed as argument 
        target_device_name = t_name;
        characteristic_uuid = c_uuid;
        service_uuid = s_uuid;

        //////// Start sequence ////////

        //init the ble client 
        log("[BLE_Slave] Initializing BLE client...");
        BLEDevice::init(""); 

        //Get a pointer to the scan object 
        log("[BLE_Slave] Setting up scan...");
        scan = BLEDevice::getScan(); 

        //Set the callback function and pass a pointer to this instance of the BLE_Slave class
        scan->setAdvertisedDeviceCallbacks(new MyCallbacks(this)); 

        // start the scan for 60 seconds
        log("[BLE_Slave] Scanning for 60 s...");
        scan->start(60);
    }

    void BLE_Slave::connect_to_server(){

        log("[BLE_Slave] Connecting to server ...");
        // create a client 
        bluetooth_client = BLEDevice::createClient();

        // Connect to the device and pass (and dereference) the target's address
        bluetooth_client->connect(*target_device_address);
        connected_to_server = true;
        log("[BLE_Slave] Connected to the server !!!");

        // Request a reference for the service (so we know which one and can read/write for that service)
        bluetooth_service = bluetooth_client->getService(service_uuid);

        // Ask for the reference for the characterist we are interested in 
        bluetooth_target_characteristc = bluetooth_service->getCharacteristic(characteristic_uuid);

        // Tell the server we want to receive notifications and point to the callback method 
        bluetooth_target_characteristc->registerForNotify(notifyCallback);

    }

    BLE_Slave::data_information BLE_Slave::parseData(std::string data){


        // Instantiate the struct that will hold all data info 
        BLE_Slave::data_information data_info;

        // iterate through the string, check how many ; there are
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
            BLE_Slave::releaseMemoryToSystem(comma_position);

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
        BLE_Slave::releaseMemoryToSystem(comma_position);

        // Return the struct 
        return data_info;

    }


    int* BLE_Slave::receivedDataAsIntArray(){

            // Create an array of type double to hold the values as actual numbers 
            int* data = new int[parsed_data_info.array_size];

            for (int number = 0; number < parsed_data_info.array_size; number++){

                // transfor std::string to char* 
                String string_data = parsed_data_info.data_array[number].c_str();

                // Populate the new double array with the double values converted from the Strings values
                data[number] = string_data.toInt();
            }

            // Release memory back to the system 
            BLE_Slave::releaseMemoryToSystem(parsed_data_info.data_array);

            // return the pointer to the array 
            return data;

    }

    float* BLE_Slave::receivedDataAsFloarArray(){

            // Create an array of type double to hold the values as actual numbers 
            float* data = new float[parsed_data_info.array_size];

            for (int number = 0; number < parsed_data_info.array_size; number++){

                // transfor std::string to char* 
                String string_data = parsed_data_info.data_array[number].c_str();

                // Populate the new double array with the double values converted from the Strings values
                data[number] = string_data.toFloat();
            }

            // Release memory back to the system 
            BLE_Slave::releaseMemoryToSystem(parsed_data_info.data_array);

            // return the pointer to the array 
            return data;

    }

} // namespace Bluetooth_slave
