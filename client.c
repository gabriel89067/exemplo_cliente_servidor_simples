#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFSZ 1024 // tamanho do buffer

void usage(int argc, char **argv)
{
    printf("use: %s <IP do servidor> <porta do servidor>", argv[0]);
    printf("exemplo: %s 127.0.0.1 5151", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{

    char requ_1[9] = "INS_REQ";
    char requ_2[9] = "REM_REQ";
    char requ_3[8] = "CH_REQ";
    char requ_4[9] = "SEN_REQ";
    char requ_5[9] = "VAL_REQ";
    char pala_1_1[13] = "install file";
    char pala_1_2[14] = "install param";
    char pala_2[7] = "remove";
    char pala_3_1[13] = "change param";
    char pala_3_2[12] = "change file";
    char pala_4_1[12] = "show value ";
    char pala_4_2[12] = "show values";
    char equipament[1024];
    char mensagem[1024];
    char nome_file[50];
    int param_receb_1_num;
    int param_receb_2_num;
    int param_receb_3_num;
    int param_receb_4_num;
    int pala_conhecid; // se a palavra existir = 1
    char pala_in[50];

    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != addrparse(argv[1], argv[2], &storage))
    { // uso o endereço no storage
        usage(argc, argv);
    }

    //////////// socket() /////////// abertura do socket

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
    { // se o socket da erro gera -1
        logexit("socket");
    }

    //////////// connect() /////////// cliente esta conectando com o server
    struct sockaddr *addr = (struct sockaddr *)(&storage); // converto o storage para o endereço certo
    if (0 != connect(s, addr, sizeof(storage)))
    {
        logexit("connect");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ); // funçao que converte endereço para string

    ////////////////////////////////////// loop ///////////////////////////////
    while (1)
    {

        memset(pala_in, 0, 50);
        fgets(pala_in, 50, stdin);
        pala_conhecid = 0;

        if (strncmp(pala_1_1, pala_in, 12) == 0)
        { // digitei install file
            param_receb_1_num = 0;
            param_receb_2_num = 0;
            param_receb_3_num = 0;
            param_receb_4_num = 0;
            pala_conhecid = 1;
            memset(nome_file, 0, 50);

            name_file(pala_in, pala_1_1, nome_file);                                                              // funçao para pegar o nome do file /
            read_file(nome_file, &param_receb_1_num, &param_receb_2_num, &param_receb_3_num, &param_receb_4_num); // funçao para pegar os parametros do file/

            memset(equipament, 0, 1024);
            memset(mensagem, 0, 1024);
            concatenar_char(equipament, param_receb_1_num, param_receb_2_num, param_receb_3_num, param_receb_4_num);
            verif_param(&param_receb_1_num, &param_receb_2_num, &param_receb_3_num, &param_receb_4_num);

            if (param_receb_1_num != 3141592)
            { // se o parametro digitado estiver correto ele segue aqui

                strcat(mensagem, requ_1);
                strcat(mensagem, " ");
                strcat(mensagem, equipament);
                strcat(mensagem, "\n");

                /////////// mensagem = INS_REQ 0 5 100 30  (exemplo) //////////////////

                size_t count = send(s, mensagem, strlen(mensagem) + 1, 0);
                if (count != strlen(mensagem) + 1)
                {
                    logexit("send");
                }

                memset(mensagem, 0, 1024);

                count = recv(s, mensagem, BUFSZ - 1, 0);

                printf("%s", mensagem);
            }
            else
            {
                printf("invalid sensor\n");
            }
        }
        if (strncmp(pala_1_2, pala_in, 13) == 0)
        { // digitei install param
            param_receb_1_num = 0;
            param_receb_2_num = 0;
            param_receb_3_num = 0;
            param_receb_4_num = 0;
            pala_conhecid = 1;
            ///////////// install param 0 5 100 30 (exemplo)  ////////////////////
            get_param(pala_in, pala_1_2, &param_receb_1_num, &param_receb_2_num, &param_receb_3_num, &param_receb_4_num); // funçao para pegar os parametros do get da tela /

            memset(equipament, 0, 1024);
            memset(mensagem, 0, 1024);

            concatenar_char(equipament, param_receb_1_num, param_receb_2_num, param_receb_3_num, param_receb_4_num);
            verif_param(&param_receb_1_num, &param_receb_2_num, &param_receb_3_num, &param_receb_4_num);

            if (param_receb_1_num != 3141592)
            { // se o parametro digitado estiver correto ele segue aqui

                strcat(mensagem, requ_1);
                strcat(mensagem, " ");
                strcat(mensagem, equipament);
                strcat(mensagem, "\n");

                /////////// mensagem = INS_REQ 0 5 100 30  (exemplo) //////////////////

                size_t count = send(s, mensagem, strlen(mensagem) + 1, 0);
                if (count != strlen(mensagem) + 1)
                {
                    logexit("send");
                }

                memset(mensagem, 0, 1024);

                count = recv(s, mensagem, BUFSZ - 1, 0);

                printf("%s", mensagem);
            }
            else
            {
                printf("invalid sensor\n");
            }
        }
        if (strncmp(pala_2, pala_in, 6) == 0)
        { // digitei remove
            pala_conhecid = 1;
            char param_receb_1_char[50];
            memset(param_receb_1_char, 0, 50);
            memset(mensagem, 0, 1024);
            int tam_param_receb = 0;

            if (!(strlen(pala_2) + 1 == strlen(pala_in)))
            {
                for (long unsigned int i = strlen(pala_2) + 1; i < strlen(pala_in) - 1; i++)
                {
                    param_receb_1_char[tam_param_receb] = pala_in[i];
                    tam_param_receb += 1;
                }

                strcpy(mensagem, requ_2);
                strcat(mensagem, " ");
                strcat(mensagem, param_receb_1_char);
                strcat(mensagem, "\n");

                size_t count = send(s, mensagem, strlen(mensagem) + 1, 0);
                if (count != strlen(mensagem) + 1)
                {
                    logexit("send");
                }

                count = recv(s, mensagem, BUFSZ - 1, 0);

                printf("%s", mensagem);
            }
            else
            {
                printf("invalid sensor\n");
            }
        }
        if (strncmp(pala_3_1, pala_in, 12) == 0)
        { // digitei change param
            param_receb_1_num = 0;
            param_receb_2_num = 0;
            param_receb_3_num = 0;
            param_receb_4_num = 0;
            pala_conhecid = 1;
            memset(mensagem, 0, 1024);

            get_param(pala_in, pala_3_1, &param_receb_1_num, &param_receb_2_num, &param_receb_3_num, &param_receb_4_num); // funçao para pegar os parametros do get da tela /

            strcat(mensagem, requ_3);
            strcat(mensagem, " ");
            concatenar_char(mensagem, param_receb_1_num, param_receb_2_num, param_receb_3_num, param_receb_4_num);
            strcat(mensagem, "\n");
            if (param_receb_1_num != 3141592)
            { // se o parametro digitado estiver correto ele segue aqui
                size_t count = send(s, mensagem, strlen(mensagem) + 1, 0);
                if (count != strlen(mensagem) + 1)
                {
                    logexit("send");
                }

                memset(mensagem, 0, 1024);

                count = recv(s, mensagem, BUFSZ - 1, 0);

                printf("%s", mensagem);
            }
            else
            {
                printf("invalid sensor\n");
            }
        }
        if (strncmp(pala_3_2, pala_in, 11) == 0)
        { // digitei change file
            param_receb_1_num = 0;
            param_receb_2_num = 0;
            param_receb_3_num = 0;
            param_receb_4_num = 0;
            pala_conhecid = 1;
            memset(nome_file, 0, 50);
            memset(mensagem, 0, 1024);

            name_file(pala_in, pala_3_2, nome_file);
            read_file(nome_file, &param_receb_1_num, &param_receb_2_num, &param_receb_3_num, &param_receb_4_num);

            strcat(mensagem, requ_3);
            strcat(mensagem, " ");
            concatenar_char(mensagem, param_receb_1_num, param_receb_2_num, param_receb_3_num, param_receb_4_num);
            strcat(mensagem, "\n");
            if (param_receb_1_num != 3141592)
            { // se o parametro digitado estiver correto ele segue aqui
                size_t count = send(s, mensagem, strlen(mensagem) + 1, 0);
                if (count != strlen(mensagem) + 1)
                {
                    logexit("send");
                }

                memset(mensagem, 0, 1024);

                count = recv(s, mensagem, BUFSZ - 1, 0);

                printf("%s", mensagem);
            }
            else
            {
                printf("invalid sensor\n");
            }
        }
        if (strncmp(pala_4_1, pala_in, 11) == 0)
        { // digitei show value
            char param_req[1024];
            char buf[1024];
            pala_conhecid = 1;
            char param_receb_1_char[50];
            char mens_m[1024];
            memset(mens_m, 0, 1024);
            memset(param_req, 0, 1024);
            memset(param_receb_1_char, 0, 50);
            memset(mensagem, 0, 1024);
            int tam_param_receb = 0;

            for (long unsigned int i = strlen(pala_4_1); i < strlen(pala_in) - 1; i++)
            {
                param_receb_1_char[tam_param_receb] = pala_in[i];
                tam_param_receb += 1;
            }

            strcpy(mensagem, requ_4);
            strcat(mensagem, " ");
            strcat(mensagem, param_receb_1_char);
            strcat(mensagem, "\n");

            size_t count = send(s, mensagem, strlen(mensagem) + 1, 0);
            if (count != strlen(mensagem) + 1)
            {
                logexit("send");
            }

            count = recv(s, mensagem, BUFSZ - 1, 0);
            if (strncmp(mensagem, "sensor not installed", strlen("sensor not installed")) == 0)
            {
                puts(mensagem);
            }
            else
            {
                memset(buf, 0, 1024);
                get_sen_res(buf, mensagem, "SEN_RES ");
                printf("%s\n", buf);
            }
        }
        if (strncmp(pala_4_2, pala_in, 11) == 0)
        { // digitei show values
            pala_conhecid = 1;
            char buf[1024];
            char param_req[1024];
            memset(param_req, 0, 1024);

            if (strcmp("show values\n", pala_in) == 0)
            {

                memset(mensagem, 0, 1024);

                strcat(mensagem, requ_5);

                size_t count = send(s, mensagem, strlen(mensagem) + 1, 0);
                if (count != strlen(mensagem) + 1)
                {
                    logexit("send");
                }

                memset(mensagem, 0, 1024);
                count = recv(s, mensagem, BUFSZ - 1, 0);

                if (strncmp(mensagem, "no sensors", strlen("no sensors")) == 0)
                {
                    puts(mensagem);
                }
                else
                {
                    memset(buf, 0, 1024);
                    get_val_res(buf, mensagem, "SEN_RES ");
                    printf("%s\n", buf);
                }
            }
            else
            {
                printf("invalid param\n");
            }
        }
        if (strncmp("kill", pala_in, 4) == 0)
        {
            pala_conhecid = 1;
            size_t count = send(s, "kill", strlen("kill") + 1, 0);
            if (count != strlen("kill") + 1)
            {
                logexit("send");
            }
            close(s);
            break;
        }

        if (pala_conhecid == 0)
        { ////////////// close() //////////////// clienet encerra conexao

            size_t count = send(s, "close", strlen("close") + 1, 0);
            if (count != strlen("close") + 1)
            {
                logexit("send");
            }
            close(s);
            break;
        }
    }

    exit(EXIT_SUCCESS);
}