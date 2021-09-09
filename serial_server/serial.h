#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#include <stdint.h>
#include <windows.h>

#define PKT_SIZE 1024
#define PAY_SIZE 1016

#define FILE_NAME       1
#define PAYLOAD         2
#define PAYLOAD_LAST    3
#define END_PROGRAM     4

typedef struct
{
    int id;
    union{
        int payload_size;
        int file_size;
    };
    uint8_t data[PAY_SIZE];
} Pacote;

HANDLE open_serial_port(const char *device);

#endif // SERIAL_H_INCLUDED
