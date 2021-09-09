#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <stdlib.h>
#include <dirent.h>

#include "serial.h"

int main()
{
    FILE *file_ptr;
    int file_size;
    int num_packet;
    int remaining_bytes;
    int i;

    Pacote pkt;

    //Manipulação da serial
    HANDLE hComm;
    DWORD bytes_writen;
    hComm = open_serial_port("\\\\.\\COM3");
    if (hComm == INVALID_HANDLE_VALUE){
        printf("Problema ao conectar com a serial\n");
        return -1;
    }
    else
        printf("Serial aberta\n");

    //Manipulação do diretório
    DIR *dir;
    struct dirent *lsdir;
    dir = opendir("c:\\temp\\cliente\\files\\");

    while ((lsdir = readdir(dir)) != NULL){
        char filename[282];
        sprintf(filename, "c:\\temp\\cliente\\files\\%s", lsdir->d_name);

        if(GetFileAttributesA(filename) & FILE_ATTRIBUTE_ARCHIVE){
            printf ("%s\n", lsdir->d_name);
            file_ptr = fopen(filename, "rb");

            if (file_ptr != NULL){
                printf("Arquivo aberto com sucesso\n");

                fseek(file_ptr, 0L, SEEK_END);
                file_size = ftell(file_ptr);
                fseek(file_ptr, 0L, SEEK_SET);
                num_packet = file_size/PAY_SIZE;
                remaining_bytes = file_size - PAY_SIZE*num_packet;
                printf("num_packet: %d\n", num_packet);
                printf("remaining_bytes: %d\n", remaining_bytes);

                //cabeçalho
                pkt.id = FILE_NAME;
                pkt.file_size = file_size;
                sprintf((char*)pkt.data, "%s", lsdir->d_name);
                WriteFile(hComm, &pkt, sizeof(pkt), &bytes_writen, NULL);

                //le arquivo e envia pela serial
                for(i=0; i<num_packet; i++){
                    if(i == num_packet-1 && remaining_bytes == 0)
                        pkt.id = PAYLOAD_LAST;
                    else
                        pkt.id = PAYLOAD;
                    pkt.payload_size = PAY_SIZE;
                    fread(pkt.data, sizeof(uint8_t), PAY_SIZE, file_ptr);
                    WriteFile(hComm, &pkt, PKT_SIZE, &bytes_writen, NULL);
                }
                //ultimo pacote
                if (remaining_bytes){
                    pkt.id = PAYLOAD_LAST;
                    pkt.payload_size = remaining_bytes;
                    fread(pkt.data, sizeof(uint8_t), remaining_bytes, file_ptr);
                    WriteFile(hComm, &pkt, PKT_SIZE, &bytes_writen, NULL);
                }

                fclose(file_ptr);
            }
            else
                printf("Arquivo não pode ser aberto\n");
        }
    }

    //Pacote informando fim da transferencia
    pkt.id = END_PROGRAM;
    pkt.payload_size = 5;
    sprintf((char*)pkt.data, "exit");
    WriteFile(hComm, &pkt, sizeof(pkt), &bytes_writen, NULL);

    closedir(dir);
    CloseHandle(hComm);

    return 0;
}
