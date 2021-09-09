#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <stdlib.h>
#include <dirent.h>

#include "serial.h"

int main()
{
    FILE *file_ptr;

    Pacote pkt;

    //Manipulação da serial
    HANDLE hComm;
    DWORD bytes_read;
    hComm = open_serial_port("\\\\.\\COM4");
    if (hComm == INVALID_HANDLE_VALUE){
        printf("Problema ao conectar com a serial\n");
        return -1;
    }
    else
        printf("Serial aberta\n");


    while(1){
        ReadFile(hComm, &pkt, sizeof(pkt), &bytes_read, NULL);

        if(pkt.id == END_PROGRAM) break;

        if(pkt.id == FILE_NAME){
            file_ptr = fopen((char*)pkt.data, "wb");
            printf("%s\n", pkt.data);

            if (file_ptr != NULL)
                printf("Arquivo aberto com sucesso\n");
            else
                printf("Arquivo não pode ser aberto\n");
        }

        if(pkt.id == PAYLOAD){
            fwrite(&pkt.data, sizeof (uint8_t), PAY_SIZE, file_ptr);
        }

        if(pkt.id == PAYLOAD_LAST){
            fwrite(&pkt.data, sizeof (uint8_t), pkt.payload_size, file_ptr);
            fclose(file_ptr);
        }
    }

    CloseHandle(hComm);//Closing the Serial Port
    return 0;
}
