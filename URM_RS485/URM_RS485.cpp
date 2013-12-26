//
// URM_RS485.cpp 
// Library C++ code
// ----------------------------------
// Developed with embedXcode+ 
// http://embedXcode.weebly.com
//
// Project 		URM_RS485
//
// Created by 	qiao, 10/27/13 7:30 PM
// 				qiao
//
// Copyright 	© qiao, 2013
// License		GNU General Public License
//
// See 			URM_RS485.cpp.h and ReadMe.txt for references
//


#include "URM_RS485.h"

URM_RS485::URM_RS485(HardwareSerial& theSerial, int theRs485TriggerPin)
:SerialTransceiver(theSerial)
{
  sendingCommandStack[Header0Index]=Header0;
  sendingCommandStack[Header1Index]=Header1;
  receivedCommandStack[Header0Index]=Header0;
  receivedCommandStack[Header1Index]=Header1;

  isBusy=false;
  timeOutDuration=DefaultTimeOutDuration;
  rs485TriggerPin=theRs485TriggerPin;
  receivedCommandStackIndex=0;
  callBackState=OnNull;
}

boolean URM_RS485::begin(unsigned long theBaudrate)
{
  pinMode(rs485TriggerPin, OUTPUT);
  digitalWrite(rs485TriggerPin, LOW);
  SerialTransceiver.begin(theBaudrate);
  delay(100);
}

void URM_RS485::sendStack()
{
  byte stackLength=sendingCommandStack[LengthIndex]+5;
  
  if (stackLength>=SendingCommandStackLength) {
    return;
  }
  sendingCommandStack[stackLength]=0;
  
    //calculate the checksum
  for (int i=0; i< stackLength; i++) {
    sendingCommandStack[stackLength]+=sendingCommandStack[i];
  }
  digitalWrite(rs485TriggerPin, HIGH);
  SerialTransceiver.write((byte *)sendingCommandStack, stackLength+1);
  SerialTransceiver.flush();
  digitalWrite(rs485TriggerPin, LOW);
}

boolean URM_RS485::requestDistance(byte theAddress,unsigned long theTimeOutDuration)
{
  if (isBusy) {
    return false;
  }
  sendingCommandStack[AddressIndex]=theAddress;
  sendingCommandStack[LengthIndex]=0;
  sendingCommandStack[CommandIndex]=RequestDistanceCommand;
  sendStack();
  timeOutDuration=theTimeOutDuration;
  timeOutTimer=millis();
  isBusy=true;
  return true;
}
boolean URM_RS485::requestTemperature(byte theAddress,unsigned long theTimeOutDuration)
{
  if (isBusy) {
    return false;
  }
  sendingCommandStack[AddressIndex]=theAddress;
  sendingCommandStack[LengthIndex]=0;
  sendingCommandStack[CommandIndex]=RequestTemperatureCommand;
  sendStack();
  timeOutDuration=theTimeOutDuration;
  timeOutTimer=millis();
  isBusy=true;
  return true;
}
boolean URM_RS485::requestMaxDistance(byte theAddress,unsigned long theTimeOutDuration)
{
  if (isBusy) {
    return false;
  }
  sendingCommandStack[AddressIndex]=theAddress;
  sendingCommandStack[LengthIndex]=0;
  sendingCommandStack[CommandIndex]=RequestMaxDistanceCommand;
  sendStack();
  timeOutDuration=theTimeOutDuration;
  timeOutTimer=millis();
  isBusy=true;
  return true;
}


boolean URM_RS485::setMaxDistance(byte theAddress,int theMaxDistance, unsigned long theTimeOutDuration)
{
  if (isBusy) {
    return false;
  }
  sendingCommandStack[AddressIndex]=theAddress;
  sendingCommandStack[LengthIndex]=2;
  sendingCommandStack[CommandIndex]=SetMaxDistanceCommand;
  sendingCommandStack[Content0Index]=(byte)((theMaxDistance>>8)&0x00ff);
  sendingCommandStack[Content1Index]=(byte)(theMaxDistance&0x00ff);
  sendStack();
  timeOutDuration=theTimeOutDuration;
  timeOutTimer=millis();
  isBusy=true;
  return true;
}


boolean URM_RS485::setBaudrate(byte theAddress,unsigned long theBaudrate, unsigned long theTimeOutDuration)
{
  if (isBusy) {
    return false;
  }
  sendingCommandStack[AddressIndex]=theAddress;
  sendingCommandStack[LengthIndex]=1;
  sendingCommandStack[CommandIndex]=SetBaudrateCommand;
  switch (theBaudrate) {
    case 1200UL:
      sendingCommandStack[Content0Index]=Baudrate1200BPS;
      break;
    case 2400UL:
      sendingCommandStack[Content0Index]=Baudrate2400BPS;
      break;
    case 4800UL:
      sendingCommandStack[Content0Index]=Baudrate4800BPS;
      break;
    case 9600UL:
      sendingCommandStack[Content0Index]=Baudrate9600BPS;
      break;
    case 14400UL:
      sendingCommandStack[Content0Index]=Baudrate14400BPS;
      break;
    case 19200UL:
      sendingCommandStack[Content0Index]=Baudrate19200BPS;
      break;
    case 28800UL:
      sendingCommandStack[Content0Index]=Baudrate28800BPS;
      break;
    case 38400UL:
      sendingCommandStack[Content0Index]=Baudrate38400BPS;
      break;
    case 57600UL:
      sendingCommandStack[Content0Index]=Baudrate57600BPS;
      break;
    case 115200UL:
      sendingCommandStack[Content0Index]=Baudrate115200BPS;
      break;
    case 128000UL:
      sendingCommandStack[Content0Index]=Baudrate128000BPS;
      break;
    case 256000UL:
      sendingCommandStack[Content0Index]=Baudrate256000BPS;
      break;
      
    default:
      sendingCommandStack[Content0Index]=Baudrate19200BPS;
      break;
  }
  sendStack();
  SerialTransceiver.begin(theBaudrate);
  timeOutDuration=theTimeOutDuration;
  timeOutTimer=millis();
  isBusy=true;
  return true;
}
boolean URM_RS485::setAddress(byte theAddress, unsigned long theTimeOutDuration)
{
  if (isBusy) {
    return false;
  }
  sendingCommandStack[AddressIndex]=BroadcastAddress;
  sendingCommandStack[LengthIndex]=1;
  sendingCommandStack[CommandIndex]=SetAddressCommand;
  sendingCommandStack[Content0Index]=theAddress;
  sendStack();
  timeOutDuration=theTimeOutDuration;
  timeOutTimer=millis();
  isBusy=true;
  return true;
}

void URM_RS485::parseStack()
{
  switch (receivedCommandStack[CommandIndex]) {
    case RequestDistanceCommand:
      callBackState=OnRequestDistance;
      receivedContent=receivedCommandStack[Content0Index];
      receivedContent<<=8;
      receivedContent&=0xff00;
      receivedContent|=receivedCommandStack[Content1Index];
      receivedAddress=receivedCommandStack[AddressIndex];
      break;
    case RequestTemperatureCommand:
      callBackState=OnRequestTemperature;
      receivedContent=receivedCommandStack[Content0Index];
      receivedContent<<=8;
      receivedContent&=0xff00;
      receivedContent|=receivedCommandStack[Content1Index];
      receivedAddress=receivedCommandStack[AddressIndex];
      break;
    case RequestMaxDistanceCommand:
      callBackState=OnRequestMaxDistance;
      receivedContent=receivedCommandStack[Content0Index];
      receivedContent<<=8;
      receivedContent&=0xff00;
      receivedContent|=receivedCommandStack[Content1Index];
      receivedAddress=receivedCommandStack[AddressIndex];
      break;
    case SetAddressCommand:
      callBackState=OnSetAddress;
      if (receivedCommandStack[Content0Index]==OperationSuccess) {
        receivedContent=true;
      }
      else if (receivedCommandStack[Content0Index]==OperationFailure){
        receivedContent=false;
      }
      else{
        callBackState=OnWrongStack;
      }
      receivedAddress=receivedCommandStack[AddressIndex];
      break;
    case SetBaudrateCommand:
      callBackState=OnSetBaudrate;
      if (receivedCommandStack[Content0Index]==OperationSuccess) {
        receivedContent=true;
      }
      else if (receivedCommandStack[Content0Index]==OperationFailure){
        receivedContent=false;
      }
      else{
        callBackState=OnWrongStack;
      }
      receivedAddress=receivedCommandStack[AddressIndex];
      break;
    case SetMaxDistanceCommand:
      callBackState=OnSetMaxDistance;
      if (receivedCommandStack[Content0Index]==OperationSuccess) {
        receivedContent=true;
      }
      else if (receivedCommandStack[Content0Index]==OperationFailure){
        receivedContent=false;
      }
      else{
        callBackState=OnWrongStack;
      }
      receivedAddress=receivedCommandStack[AddressIndex];
      break;
      
    default:
      callBackState=OnWrongStack;
      break;
  }
}

boolean URM_RS485::validateStack()
{
  byte checkSum=0x00;
  byte checkLength=receivedCommandStack[LengthIndex]+5;
  for (int i=0; i<checkLength; i++) {
    checkSum+=receivedCommandStack[i];
  }
  return (receivedCommandStack[checkLength]==checkSum);
}


boolean URM_RS485::available()
{
   Serial.print("isBusy=");
   Serial.println(isBusy);
   
    
  if (isBusy) {
    if (millis()-timeOutTimer>=timeOutDuration) {
      callBackState=OnTimeOut;
      receivedCommandStackIndex=0;
      isBusy=false;
      return true;
    }
  }
  
  while (SerialTransceiver.available()) {
    Serial.println(receivedCommandStackIndex);
    if (receivedCommandStackIndex) {
        //is receiving
      if (receivedCommandStackIndex==Header1Index) {
        if (SerialTransceiver.peek()!=Header1) {
          callBackState=OnWrongStack;
          receivedCommandStackIndex=0;
          isBusy=false;
          return true;
        }
      }
      
      if (receivedCommandStackIndex<=LengthIndex) {
        receivedCommandStack[receivedCommandStackIndex++]=SerialTransceiver.read();
      }
      else{
        if (receivedCommandStack[LengthIndex]<=(ReceivedCommandStackLength-5)) {
          receivedCommandStack[receivedCommandStackIndex++]=SerialTransceiver.read();
          if (receivedCommandStackIndex==receivedCommandStack[LengthIndex]+5+1) {
            if (validateStack()) {
              parseStack();
              receivedCommandStackIndex=0;
              isBusy=false;
              return true;
            }
            else{
              callBackState=OnWrongStack;
              receivedCommandStackIndex=0;
              isBusy=false;
              return true;
            }
          }
        }
        else{
          callBackState=OnWrongStack;
          receivedCommandStackIndex=0;
          isBusy=false;
          return true;
        }
      }
    }
    else{
        //is not receiving
      if (SerialTransceiver.read()==Header0) {
        receivedCommandStackIndex++;
      }
    }
  }

  return false;
}




















