#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#define BUFSZ 1024

struct equipamento
{
    int ID;
    int CORR;
    int TENS;
    int EFI;
    int POT;
};

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <porta do servidor>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    struct equipamento equipamentos[100];

    char mensagem[1024];
    char requ_1[9] = "INS_REQ";
    char requ_2[9] = "REM_REQ";
    char requ_3[8] = "CH_REQ";
    char requ_4[9] = "SEN_REQ";
    char requ_5[9] = "VAL_REQ";
    int param_receb_1_num;
    int param_receb_2_num;
    int param_receb_3_num;
    int param_receb_4_num;
    int QUANT_EQUI = 0;
    int ver_soc = 0;
    int csock = 0;

    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }
    //////////// socket() /////////// abertura do socket
    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
    {
        logexit("socket");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage)))
    {
        logexit("bind");
    }
    //////////// listen() /////////// fica aguardando ate 15 clientes na fila
    if (0 != listen(s, 15))
    {
        logexit("listen");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);

    while (1)
    {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);
        if (ver_soc == 0)
        {
            int csock_0 = accept(s, caddr, &caddrlen);
            if (csock == -1)
            {
                logexit("accept");
            }

            char caddrstr[BUFSZ];
            addrtostr(caddr, caddrstr, BUFSZ);

            ver_soc = 1;
            csock = csock_0;
        }

        ////////////// recv() //////////////// server recebe dado do cliente

        char buf[BUFSZ];
        memset(buf, 0, BUFSZ);
        size_t count = recv(csock, buf, BUFSZ - 1, 0); // recebi uma mensagem do client

        if (strncmp(requ_1, buf, 7) == 0)
        { ////////////// recebi INS_REQ do cliente
            printf("%s", buf);
            param_receb_1_num = 0;
            param_receb_2_num = 0;
            param_receb_3_num = 0;
            param_receb_4_num = 0;
            int exist_stru = 0;

            get_param(buf, requ_1, &param_receb_1_num, &param_receb_2_num, &param_receb_3_num, &param_receb_4_num);
            if (QUANT_EQUI != 0)
            {
                for (int i = 0; i < QUANT_EQUI; i++)
                {
                    if (equipamentos[i].ID == param_receb_1_num)
                    {
                        exist_stru = 1;
                    }
                }

                if (exist_stru == 0)
                {

                    equipamentos[QUANT_EQUI].ID = param_receb_1_num;
                    equipamentos[QUANT_EQUI].CORR = param_receb_2_num;
                    equipamentos[QUANT_EQUI].TENS = param_receb_3_num;
                    equipamentos[QUANT_EQUI].EFI = param_receb_4_num;
                    equipamentos[QUANT_EQUI].POT = param_receb_2_num * param_receb_3_num;
                    QUANT_EQUI = QUANT_EQUI + 1;

                    memset(buf, 0, BUFSZ);
                    strcpy(buf, "successful installation\n");
                    count = send(csock, buf, strlen(buf) + 1, 0);
                    if (count != strlen(buf) + 1)
                    {
                        logexit("send");
                    }
                }

                if (exist_stru == 1)
                {

                    memset(buf, 0, BUFSZ);
                    strcpy(buf, "sensor already exists\n");
                    count = send(csock, buf, strlen(buf) + 1, 0);
                    if (count != strlen(buf) + 1)
                    {
                        logexit("send");
                    }
                }
            }

            if (QUANT_EQUI == 0)
            {
                equipamentos[QUANT_EQUI].ID = param_receb_1_num;
                equipamentos[QUANT_EQUI].CORR = param_receb_2_num;
                equipamentos[QUANT_EQUI].TENS = param_receb_3_num;
                equipamentos[QUANT_EQUI].EFI = param_receb_4_num;
                equipamentos[QUANT_EQUI].POT = param_receb_2_num * param_receb_3_num;
                QUANT_EQUI = QUANT_EQUI + 1;

                memset(buf, 0, BUFSZ);
                strcpy(buf, "successful installation\n");
                count = send(csock, buf, strlen(buf) + 1, 0);
                if (count != strlen(buf) + 1)
                {
                    logexit("send");
                }
            }
            
        }

        if (strncmp(requ_2, buf, 7) == 0)
        { ////////////// recebi REM_REQ do cliente
            printf("%s", buf);
            int exist_stru = 0;
            int sens_remo = 0;
            char param_receb_1_char[50];
            memset(param_receb_1_char, 0, 50);
            int tam_param_receb = 0;

            for (long unsigned int i = strlen(requ_2) + 1; i < strlen(buf) - 1; i++)
            {
                param_receb_1_char[tam_param_receb] = buf[i];
                tam_param_receb += 1;
            }
            param_receb_1_num = atoi(param_receb_1_char);

            for (int i = 0; i < QUANT_EQUI; i++)
            {
                if (equipamentos[i].ID == param_receb_1_num)
                {
                    exist_stru = 1;
                    sens_remo = i;
                }
            }
            if (exist_stru == 0)
            {
                memset(buf, 0, BUFSZ);
                strcpy(buf, "sensor not installed\n");
                count = send(csock, buf, strlen(buf) + 1, 0);
                if (count != strlen(buf) + 1)
                {
                    logexit("send");
                }
            }

            if (exist_stru == 1)
            {
                if (sens_remo == QUANT_EQUI - 1)
                {

                    equipamentos[sens_remo].ID = 0;
                    equipamentos[sens_remo].CORR = 0;
                    equipamentos[sens_remo].TENS = 0;
                    equipamentos[sens_remo].EFI = 0;
                    equipamentos[sens_remo].POT = 0;
                    QUANT_EQUI = QUANT_EQUI - 1;

                    memset(buf, 0, BUFSZ);
                    strcpy(buf, "successful removal\n");
                    count = send(csock, buf, strlen(buf) + 1, 0);
                    if (count != strlen(buf) + 1)
                    {
                        logexit("send");
                    }
                }
                if (sens_remo < QUANT_EQUI - 1)
                {

                    equipamentos[sens_remo].ID = 0;
                    equipamentos[sens_remo].CORR = 0;
                    equipamentos[sens_remo].TENS = 0;
                    equipamentos[sens_remo].EFI = 0;
                    equipamentos[sens_remo].POT = 0;
                    QUANT_EQUI = QUANT_EQUI - 1;

                    memset(buf, 0, BUFSZ);
                    strcpy(buf, "successful removal\n");
                    count = send(csock, buf, strlen(buf) + 1, 0);
                    if (count != strlen(buf) + 1)
                    {
                        logexit("send");
                    }

                    for (int i = sens_remo; i < QUANT_EQUI; i++)
                    {
                        equipamentos[i].ID = equipamentos[i + 1].ID;
                        equipamentos[i].CORR = equipamentos[i + 1].CORR;
                        equipamentos[i].TENS = equipamentos[i + 1].TENS;
                        equipamentos[i].EFI = equipamentos[i + 1].EFI;
                        equipamentos[i].POT = equipamentos[i + 1].POT;
                    }
                }
            }
            
        }

        if (strncmp(requ_3, buf, 6) == 0)
        { ////////////// recebi CH_REQ do cliente

            printf("%s", buf);
            param_receb_1_num = 0;
            param_receb_2_num = 0;
            param_receb_3_num = 0;
            param_receb_4_num = 0;
            int exist_stru = 0;
            int sen_mod = 0;

            get_param(buf, requ_3, &param_receb_1_num, &param_receb_2_num, &param_receb_3_num, &param_receb_4_num);
            for (int i = 0; i < QUANT_EQUI; i++)
            {
                if (equipamentos[i].ID == param_receb_1_num)
                {
                    exist_stru = 1;
                    sen_mod = i;
                }
            }

            if (exist_stru == 1)
            {

                equipamentos[sen_mod].ID = param_receb_1_num;
                equipamentos[sen_mod].CORR = param_receb_2_num;
                equipamentos[sen_mod].TENS = param_receb_3_num;
                equipamentos[sen_mod].EFI = param_receb_4_num;
                equipamentos[sen_mod].POT = param_receb_2_num * param_receb_3_num;

                memset(buf, 0, BUFSZ);
                strcpy(buf, "successful change\n");
                count = send(csock, buf, strlen(buf) + 1, 0);
                if (count != strlen(buf) + 1)
                {
                    logexit("send");
                }
            }

            if (exist_stru == 0)
            {

                memset(buf, 0, BUFSZ);
                strcpy(buf, "sensor not installed\n");
                count = send(csock, buf, strlen(buf) + 1, 0);
                if (count != strlen(buf) + 1)
                {
                    logexit("send");
                }
            }
            
        }

        if (strncmp(requ_4, buf, 7) == 0)
        { ////////////// recebi SEN_REQ do cliente

            printf("%s", buf);
            int exist_stru = 0;
            int sens_remo = 0;
            char param_receb_1_char[50];
            memset(param_receb_1_char, 0, 50);
            int tam_param_receb = 0;

            for (long unsigned int i = strlen(requ_2) + 1; i < strlen(buf) - 1; i++)
            {
                param_receb_1_char[tam_param_receb] = buf[i];
                tam_param_receb += 1;
            }

            param_receb_1_num = atoi(param_receb_1_char);

            for (int i = 0; i < QUANT_EQUI; i++)
            {
                if (equipamentos[i].ID == param_receb_1_num)
                {
                    exist_stru = 1;
                    sens_remo = i;
                }
            }
            if (exist_stru == 0)
            {

                memset(buf, 0, BUFSZ);
                strcpy(buf, "sensor not installed");
                
                count = send(csock, buf, strlen(buf) + 1, 0);
                if (count != strlen(buf) + 1)
                {
                    logexit("send");
                }
            }

            if (exist_stru == 1)
            {

                memset(buf, 0, BUFSZ);
                strcpy(buf, "SEN_RES ");
                strcat(buf, param_receb_1_char);
                strcat(buf, " ");
                concatenar_char_red(buf, equipamentos[sens_remo].POT, equipamentos[sens_remo].EFI);
                
                count = send(csock, buf, strlen(buf) + 1, 0);
                if (count != strlen(buf) + 1)
                {
                    logexit("send");
                }
            }
        }

        if (strncmp(requ_5, buf, 7) == 0)
        { ////////////// recebi VAL_REQ do cliente
            printf("%s\n", buf);
            memset(mensagem, 0, BUFSZ);
            if (QUANT_EQUI == 0)
            {
                memset(mensagem, 0, BUFSZ);
                strcpy(mensagem, "no sensors");
                puts(mensagem);
                count = send(csock, mensagem, strlen(mensagem) + 1, 0);
                if (count != strlen(mensagem) + 1)
                {
                    logexit("send");
                }
            }
            if (QUANT_EQUI != 0)
            {

                strcpy(mensagem, "VAL_RES ");

                for (int i = 0; i < QUANT_EQUI; i++)
                {

                    if (i != QUANT_EQUI - 1)
                    {
                        concatenar_char_c(mensagem, equipamentos[i].ID, equipamentos[i].POT, equipamentos[i].EFI);
                        strcat(mensagem, " ");
                    }
                    if (i == QUANT_EQUI - 1)
                    {
                        concatenar_char_c(mensagem, equipamentos[i].ID, equipamentos[i].POT, equipamentos[i].EFI);
                    }
                }
                
                count = send(csock, mensagem, strlen(mensagem) + 1, 0);
                if (count != strlen(mensagem) + 1)
                {
                    logexit("send");
                }
            }
        }

        if (strncmp("close", buf, 5) == 0)
        { ////////////// recebi close do cliente
            printf("%s\n", buf);
            close(csock);
            ver_soc = 0;
        }

        if (strcmp(buf, "kill") == 0)
        { ////////////// recebi kill do cliente
            printf("%s\n", buf);
            close(csock);
            close(s);
            break;
        }
    }

    exit(EXIT_SUCCESS);
}