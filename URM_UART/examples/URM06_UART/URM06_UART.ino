#include "Arduino.h"

// Include application, user and local libraries
#include "URM_UART.h"


#define DefaultBaudrate 19200UL         //the Default Baudrate for the Urm06_UART
#define DefaultAddress 0x11             //the Default Address for the Urm06_UART
#define DefaultMaxDistance (-1)         //the Default Max Distance for the Urm06_UART. "-1" means there is no Max Distance limitation. Set the Max Distance can limit the Distance range and speed up the detection.

#define CustomizedTimeOutDuration 500   //Time Out Duration can be Customized in "ms" unit

// Define variables and constants
//URM_UART urm(Serial);  //select the Serial port for communication with Urm_UART sensor
URM_UART urm(Serial);  //select the Serial port for communication with Urm_UART sensor

void onTimeOut()
{
    //If there is no reply from Urm_UART lasting for 1 second, this function will run. The time duration can be Customized
    //TODO write your code here:
    Serial.println("onTimeOut");
    
}
void onRequestDistance(byte theAddress, int theDistance)
{
    //If received Distance reply, this function will run.
    //theDistance is in "mm" unit
    //TODO write your code here:
    Serial.print("Address:");
    Serial.println(theAddress);
    Serial.print("Distance:");
    Serial.print(theDistance);
    Serial.println("mm");
    
}
void onRequestTemperature(byte theAddress, float theTemperature)
{
    //If received Temperature reply, this function will run.
    //theTemperature is in "°C" unit
    //TODO write your code here:
    Serial.print("Address:");
    Serial.println(theAddress);
    Serial.print("Temperature:");
    Serial.print(theTemperature);
    Serial.println(" C");
    
}
void onRequestMaxDistance(byte theAddress, int theMaxDistance)
{
    //If received Max Distance reply, this function will run.
    //theMaxDistance is in "mm" unit
    //TODO write your code here:
    Serial.print("Address:");
    Serial.println(theAddress);
    Serial.print("MaxDistance:");
    Serial.print(theMaxDistance);
    Serial.println("mm");
}
void onSetMaxDistance(byte theAddress, boolean isOperationSuccess)
{
    //After setting the Max Distance and getting a reply, this function will run.
    //Set the Max Distance can limit the Distance range and speed up the detection.
    //TODO write your code here:
    Serial.print("Address:");
    Serial.println(theAddress);
    Serial.print("SetMaxDistance:");
    if (isOperationSuccess) {
        Serial.println("Success");
    }
    else{
        Serial.println("Failure");
    }
}
void onSetBaudrate(byte theAddress, boolean isOperationSuccess)
{
    //After setting the Baudrate and getting a reply, this function will run.
    //TODO write your code here:
    Serial.print("Address:");
    Serial.println(theAddress);
    Serial.print("SetBaudrate:");
    if (isOperationSuccess) {
        Serial.println("Success");
    }
    else{
        Serial.println("Failure");
    }
}
void onSetAddress(byte theAddress, boolean isOperationSuccess)
{
    //After setting the Address and getting a reply, this function will run.
    //TODO write your code here:
    Serial.print("Address:");
    Serial.println(theAddress);
    Serial.print("SetAddress:");
    if (isOperationSuccess) {
        Serial.println("Success");
    }
    else{
        Serial.println("Failure");
    }
}
void onWrongStack()
{
    //If received wrong command, this function will run.
    //TODO write your code here:
    Serial.println("WrongStack");
}

//Run the proper function based on the different kinds of states
void commandProcess()
{
    if (urm.available()) {
        switch (urm.callBackState) {
            case URM_UART::OnTimeOut:
                onTimeOut();
                break;
            case URM_UART::OnRequestDistance:
                onRequestDistance(urm.receivedAddress, urm.receivedContent);
                break;
            case URM_UART::OnRequestTemperature:
                onRequestTemperature(urm.receivedAddress, urm.receivedContent/10.0);
                break;
            case URM_UART::OnRequestMaxDistance:
                onRequestMaxDistance(urm.receivedAddress, urm.receivedContent);
                break;
            case URM_UART::OnSetMaxDistance:
                onSetMaxDistance(urm.receivedAddress, urm.receivedContent);
                break;
            case URM_UART::OnSetBaudrate:
                onSetBaudrate(urm.receivedAddress, urm.receivedContent);
                break;
            case URM_UART::OnSetAddress:
                onSetAddress(urm.receivedAddress, urm.receivedContent);
                break;
            case URM_UART::OnWrongStack:
                onWrongStack();
                break;
            default:
                break;
        }
    }
}

// Add setup code
void setup()
{
    urm.begin(DefaultBaudrate);
}

// Add loop code
void loop()
{
    commandProcess();
    
    static unsigned long sendingTimer=millis();
    if (millis()-sendingTimer>=1000) {
        sendingTimer=millis();
        
        //Each function below from URM_UART returns the state whether UART Bus is busy or not.
        //If the Bus is busy, wait until the bus is released.
        while(!urm.requestDistance(DefaultAddress)) {
            commandProcess();
        }
        //
        //    while(!urm.requestTemperature(DefaultAddress)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.requestMaxDistance(DefaultAddress)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.setAddress(DefaultAddress)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.setBaudrate(DefaultAddress, DefaultBaudrate)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.setMaxDistance(DefaultAddress, DefaultMaxDistance)) {
        //      commandProcess();
        //    }
        //
        
        
        //Time Out Duration can be Customized
        //    while(!urm.requestDistance(DefaultAddress, CustomizedTimeOutDuration)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.requestTemperature(DefaultAddress, CustomizedTimeOutDuration)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.requestMaxDistance(DefaultAddress, CustomizedTimeOutDuration)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.setAddress(DefaultAddress, CustomizedTimeOutDuration)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.setBaudrate(DefaultAddress, DefaultBaudrate, CustomizedTimeOutDuration)) {
        //      commandProcess();
        //    }
        //
        //    while(!urm.setMaxDistance(DefaultAddress, DefaultMaxDistance, CustomizedTimeOutDuration)) {
        //      commandProcess();
        //    }
        
    }
}























