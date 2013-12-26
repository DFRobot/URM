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
  1.Date        : 2013/12/
    Author      : Leo Yan
    Modification: Created file

******************************************************************************/

#include "Arduino.h"
#include "URM_UART.h"
     
/*todo: set actual parameters*/
const unsigned long URM_Baudrate = 19200;  
const byte URM1_Address = 0x11;          //0xAB is broadcast address
const byte URM2_Address = 22;  

URM_UART urm(Serial);  //select the Serial port for communication with Urm sensor

/*inner define*/
#define CustomizedTimeOutDuration 300   //Time Out Duration can be Customized in "ms" unit. Default value is 1000ms.


void setup()
{     
  urm.begin(URM_Baudrate);
}

void loop() 
{
  commandProcess();  

  /*read module 1*/  
  while(!urm.requestDistance(URM1_Address, CustomizedTimeOutDuration)) {
    commandProcess();
  }   

  while(!urm.requestTemperature(URM1_Address, CustomizedTimeOutDuration)) {
    commandProcess();
  }    

  /*read module 2*/
  while(!urm.requestDistance(URM2_Address, CustomizedTimeOutDuration)) {
    commandProcess();
  }     
   
  delay(CustomizedTimeOutDuration >> 1);
}


/*  */
void onTimeOut()
{
  //If there is no reply from Urm lasting for 'CustomizedTimeOutDuration' second, this function will run. The time duration can be Customized
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












