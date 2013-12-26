
#ifndef URM_UART_RELEASE
#define URM_UART_RELEASE 100
#include "Arduino.h"


#define ReceivedCommandStackLength 10
#define SendingCommandStackLength 10

#define DefaultTimeOutDuration 1000

#define Header0 0x55
#define Header1 0xaa

#define Header0Index 0
#define Header1Index 1
#define AddressIndex 2
#define LengthIndex 3
#define CommandIndex 4
#define Content0Index 5
#define Content1Index 6

#define RequestDistanceCommand 0x02            //distance command
#define RequestTemperatureCommand 0x03           //temperature command
#define RequestMaxDistanceCommand 0x05         //distance threshold

#define SetMaxDistanceCommand 0x04
#define SetBaudrateCommand 0x08
#define SetAddressCommand 0x55

#define BroadcastAddress 0xab

#define OperationSuccess 0xcc
#define OperationFailure 0xee

#define Baudrate1200BPS 0x00
#define Baudrate2400BPS 0x01
#define Baudrate4800BPS 0x02
#define Baudrate9600BPS 0x03
#define Baudrate14400BPS 0x04
#define Baudrate19200BPS 0x05
#define Baudrate28800BPS 0x06
#define Baudrate38400BPS 0x07
#define Baudrate57600BPS 0x08
#define Baudrate115200BPS 0x09
#define Baudrate128000BPS 0x0A
#define Baudrate256000BPS 0x0B

class URM_UART {
    
private:
    HardwareSerial& SerialTransceiver;      //serial port to communicate with
    unsigned long timeOutTimer;
    unsigned long timeOutDuration;
    byte receivedCommandStack[ReceivedCommandStackLength];
    byte sendingCommandStack[SendingCommandStackLength];
    
    byte receivedCommandStackIndex;
    //  boolean isReceiving;
    
    void sendStack();
    void parseStack();
    boolean validateStack();
    
public:
    URM_UART(HardwareSerial& theSerial);
    
    enum CallBackState {
        OnNull,
        OnTimeOut,
        OnRequestDistance,
        OnRequestTemperature,
        OnRequestMaxDistance,
        OnSetMaxDistance,
        OnSetBaudrate,
        OnSetAddress,
        OnWrongStack
    } callBackState;
    
    boolean available();
    
    boolean isBusy;
    
    boolean begin(unsigned long theBaudrate);
    
    boolean requestDistance(byte theAddress,unsigned long theTimeOutDuration=DefaultTimeOutDuration);
    boolean requestTemperature(byte theAddress,unsigned long theTimeOutDuration=DefaultTimeOutDuration);
    boolean requestMaxDistance(byte theAddress,unsigned long theTimeOutDuration=DefaultTimeOutDuration);
    
    
    boolean setMaxDistance(byte theAddress,int theMaxDistance,unsigned long theTimeOutDuration=DefaultTimeOutDuration);
    boolean setBaudrate(byte theAddress,unsigned long theBaudrate,unsigned long theTimeOutDuration=DefaultTimeOutDuration);
    boolean setAddress(byte theAddress,unsigned long theTimeOutDuration=DefaultTimeOutDuration);
    
    byte receivedAddress;
    int receivedContent;
    
};

#endif
