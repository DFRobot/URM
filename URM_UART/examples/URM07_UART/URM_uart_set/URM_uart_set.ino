/******************************************************************************
                        URM07 UART Software Example
  
  Copyright (C) <2013>  <www.dfrobot.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 

 ******************************************************************************
  Author        : Leo Yan
  Created       : 2013/12
  History       :
  1.Date        : 2013/12
    Author      : Leo Yan
    Modification: Created file

******************************************************************************/
#include "Arduino.h"
#include "URM_UART.h"

/**TODO: set parameters**/
#define PRE_BAUDRATE 19200    //set the actual baudrate       
const byte URM_Address = 22;  //set new address  (0xAB is broadcast address)
/* Baudrate should be: 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200, 128000, 256000.  If others, it will use 19200*/
const unsigned long URM_Baudrate = 19200;  //set new baudrate

URM_UART urm(Serial);  //select the Serial port for communication with Urm_485 sensor


/**inner definition**/
#define CustomizedTimeOutDuration 500   //Time Out Duration can be Customized in "ms" unit


void setup()
{     
  urm.begin(PRE_BAUDRATE);

  while(!urm.setAddress(URM_Address, CustomizedTimeOutDuration)) {
    commandProcess();
  }
  delay( CustomizedTimeOutDuration );
  commandProcess();

  while(!urm.setBaudrate(URM_Address, URM_Baudrate, CustomizedTimeOutDuration)) {
    commandProcess();
  }
  
  delay( CustomizedTimeOutDuration );
  commandProcess();
}

void loop() 
{
  commandProcess();  
    
  while(!urm.requestDistance(URM_Address, CustomizedTimeOutDuration)) {
    commandProcess();
  }   

  while(!urm.requestTemperature(URM_Address, CustomizedTimeOutDuration)) {
    commandProcess();
  }    

    
   delay(CustomizedTimeOutDuration >> 1);
}


/*  */
void onTimeOut()
{
  //If there is no reply from Urm_485 lasting for 1 second, this function will run. The time duration can be Customized
  //TODO write your code here:
  Serial.println("onTimeOut");

}
void onRequestDistance(byte theAddress, int theDistance)
{
  //If received Distance reply, this function will run.
  //theDistance is in "mm" unit
  //TODO write your code here:
  Serial.print("Address-");
  Serial.print(theAddress);
  Serial.print(":Distance=");
  Serial.print(theDistance);
  Serial.println("mm");

}
void onRequestTemperature(byte theAddress, float theTemperature)
{
  //If received Temperature reply, this function will run.
  //theTemperature is in "°C" unit
  //TODO write your code here:
  Serial.print("Address-");
  Serial.print(theAddress);
  Serial.print(":Temperature=");
  Serial.print(theTemperature);
  Serial.println(" C");

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












