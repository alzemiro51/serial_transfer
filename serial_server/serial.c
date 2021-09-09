#include "serial.h"

HANDLE open_serial_port(const char *device){

    HANDLE hComm;
    DWORD bytes_read;

    hComm = CreateFile(device,                      //port name
                      GENERIC_READ | GENERIC_WRITE, //Read/Write
                      0,                            // No Sharing
                      NULL,                         // No Security
                      OPEN_EXISTING,// Open existing port only
                      0,            // Non Overlapped I/O
                      NULL);        // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE){
        return INVALID_HANDLE_VALUE;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(hComm, &dcbSerialParams)) {
        return INVALID_HANDLE_VALUE;
    }
    dcbSerialParams.BaudRate=CBR_19200;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(hComm, &dcbSerialParams)){
        return INVALID_HANDLE_VALUE;
    }

    return hComm;
}
