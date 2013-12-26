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

/*Manual：
1）Connect only ONE URM that you want to get baudrate to an Arduino control board. 
2）Upload the program.
3）Please open serial COM at 9600bps after uploaded the program. You can see the baudrate of the URM after a while.

Example of Serial COM output (There will be some magic characters if searching failed):
Try searching baudrate begin...ófffæ<ûðüÿSearch failed!
Try searching baudrate begin...ófffæ<ûðüÿSearch failed!
Try searching baudrate begin...ófffæ<ûðüÿSearch failed!
Try searching baudrate begin...ófffæ<ûðüÿSearch success, Address:10  Baud=256000
Search success, Address:10  Baud=256000
Search success, Address:10  Baud=256000

*/

//TODO: Define URM, please using the actual SERIAL
#define URM_SERIAL Serial  //select the Serial port for communication with URM sensor


/*inner definition*/      
#define CustomizedTimeOutDuration 300   //Time Out Duration can be Customized in "ms" unit
const byte URM_Address = 0xAB;          //if there is only one URM, you can use broadcast address 0xAB
const unsigned long a_Baudrate[] = {4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200, 128000, 256000};

URM_UART urm(URM_SERIAL);  
static boolean s_findFlag = false;
static unsigned long s_baudRate = 0;
static byte s_Address = 0;


void setup()
{     

}

void loop() 
{
  
  if ( true == s_findFlag )
  {
      Serial.end();
      Serial.begin( 9600 );
      Serial.print("Search success, ");
      Serial.print("Address:");
      Serial.print(s_Address);
      Serial.print("  Baud=");
      Serial.println(s_baudRate);
  }
  else
  {
      Serial.end();
      Serial.begin( 9600 );
      Serial.println("Try searching baudrate begin...");
      
      for ( byte i = 0; i < (sizeof(a_Baudrate)/sizeof(a_Baudrate[0])); i++ )
      {
          s_baudRate = a_Baudrate[i];
          URM_SERIAL.end();
          urm.begin(s_baudRate);
          
          while(!urm.requestDistance(URM_Address, CustomizedTimeOutDuration)) 
          {
              commandProcess();
          }
          
          for ( byte j = 0; j < 4; j++ );
          {
              delay( CustomizedTimeOutDuration/2 );  
              commandProcess();      
          }
          
          if ( true == s_findFlag ) break;
      }
      
      if ( false == s_findFlag )
      {     
          Serial.end();
          Serial.begin( 9600 );
          Serial.println("Search failed!");
      }
      
  }
  
  delay(300);
}


/*  */
void onTimeOut()
{
  //If there is no reply from Urm_485 lasting for 1 second, this function will run. The time duration can be Customized
  //TODO write your code here:
  //Serial.println("onTimeOut");

}
void onRequestDistance(byte theAddress, int theDistance)
{
  //If received Distance reply, this function will run.
  //theDistance is in "mm" unit
  //TODO write your code here:
  s_findFlag = true; 
  s_Address = theAddress;
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
  //Serial.println("WrongStack");
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












