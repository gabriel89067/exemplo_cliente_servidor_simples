#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

void logexit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int addrparse(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage)
{ //
    if (addrstr == NULL || portstr == NULL)
    {
        return -1;
    }

    uint16_t port = (uint16_t)atoi(portstr); // unsigned short
    if (port == 0)
    {
        return -1;
    }
    port = htons(port); // host to network short

    struct in_addr inaddr4; // 32-bit IP address
    if (inet_pton(AF_INET, addrstr, &inaddr4))
    {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_port = port;
        addr4->sin_addr = inaddr4;
        return 0;
    }

    struct in6_addr inaddr6; // 128-bit IPv6 address
    if (inet_pton(AF_INET6, addrstr, &inaddr6))
    {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = port;
        // addr6->sin6_addr = inaddr6
        memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
        return 0;
    }

    return -1;
}

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize)
{
    int version;
    char addrstr[INET6_ADDRSTRLEN + 1] = "";
    uint16_t port;

    if (addr->sa_family == AF_INET)
    {
        version = 4;
        struct sockaddr_in *addr4 = (struct sockaddr_in *)addr;
        if (!inet_ntop(AF_INET, &(addr4->sin_addr), addrstr,
                       INET6_ADDRSTRLEN + 1))
        {
            logexit("ntop");
        }
        port = ntohs(addr4->sin_port); // network to host short
    }
    else if (addr->sa_family == AF_INET6)
    {
        version = 6;
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)addr;
        if (!inet_ntop(AF_INET6, &(addr6->sin6_addr), addrstr,
                       INET6_ADDRSTRLEN + 1))
        {
            logexit("ntop");
        }
        port = ntohs(addr6->sin6_port); // network to host short
    }
    else
    {
        logexit("unknown protocol family.");
    }
    if (str)
    {
        snprintf(str, strsize, "IPv%d %s %hu", version, addrstr, port);
    }
}

int server_sockaddr_init(const char *proto, const char *portstr,
                         struct sockaddr_storage *storage)
{
    uint16_t port = (uint16_t)atoi(portstr); // unsigned short
    if (port == 0)
    {
        return -1;
    }
    port = htons(port); // host to network short

    memset(storage, 0, sizeof(*storage));
    if (0 == strcmp(proto, "v4"))
    {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_addr.s_addr = INADDR_ANY;
        addr4->sin_port = port;
        return 0;
    }
    else if (0 == strcmp(proto, "v6"))
    {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_addr = in6addr_any;
        addr6->sin6_port = port;
        return 0;
    }
    else
    {
        return -1;
    }
}

void name_file(char *pala_in, char *pala_set, char *name)
{
    int tam_file = 0;
    for (long unsigned int i = strlen(pala_set) + 1; i < strlen(pala_in) - 1; i++)
    { // funçao para pegar o nome do file
        name[tam_file] = pala_in[i];
        tam_file += 1;
    }
}

void read_file(char *nome_file, int *param_receb_1_num, int *param_receb_2_num, int *param_receb_3_num, int *param_receb_4_num)
{
    FILE *file;
    int file_in = 0;

    file = fopen(nome_file, "r");
    if (file == NULL)
    {
        file_in = 1;
    }
    if (file_in == 0)
    {
        fscanf(file, "%i\n %i\n %i\n %i\n", param_receb_1_num, param_receb_2_num, param_receb_3_num, param_receb_4_num);
        fclose(file);
    }
    if (file_in == 1)
    {
        *param_receb_1_num = 3141592;
    }
}

void get_param(char *pala_in, char *pala_set, int *param_receb_1_num, int *param_receb_2_num, int *param_receb_3_num, int *param_receb_4_num)
{
    char espaco_pala[2] = " ";
    int tot_esp = 0;
    int tam_param_receb = 0;
    int param_atual;
    char param_receb_1_char[50];
    char param_receb_2_char[50];
    char param_receb_3_char[50];
    char param_receb_4_char[50];
    memset(param_receb_1_char, 0, 50);
    memset(param_receb_2_char, 0, 50);
    memset(param_receb_3_char, 0, 50);
    memset(param_receb_4_char, 0, 50);
    param_atual = 1;

    for (long unsigned int i = strlen(pala_set) + 1; i < strlen(pala_in) - 1; i++)
    {

        if (pala_in[i] == espaco_pala[0])
        {
            tot_esp = tot_esp + 1;
        }

        if (pala_in[i] == espaco_pala[0] && param_atual == 3)
        {
            param_atual = 4;
            tam_param_receb = 0;
        }

        if (pala_in[i] == espaco_pala[0] && param_atual == 2)
        {
            param_atual = 3;
            tam_param_receb = 0;
        }

        if (pala_in[i] == espaco_pala[0] && param_atual == 1)
        {
            param_atual = 2;
            tam_param_receb = 0;
        }

        if (param_atual == 1 && pala_in[i] != espaco_pala[0])
        {
            param_receb_1_char[tam_param_receb] = pala_in[i];
            tam_param_receb += 1;
        }
        if (param_atual == 2 && pala_in[i] != espaco_pala[0])
        {
            param_receb_2_char[tam_param_receb] = pala_in[i];
            tam_param_receb += 1;
        }
        if (param_atual == 3 && pala_in[i] != espaco_pala[0])
        {
            param_receb_3_char[tam_param_receb] = pala_in[i];
            tam_param_receb += 1;
        }
        if (param_atual == 4 && pala_in[i] != espaco_pala[0])
        {
            param_receb_4_char[tam_param_receb] = pala_in[i];
            tam_param_receb += 1;
        }
    }

    if (tot_esp == 3)
    {
        *param_receb_1_num = atoi(param_receb_1_char);
        *param_receb_2_num = atoi(param_receb_2_char);
        *param_receb_3_num = atoi(param_receb_3_char);
        *param_receb_4_num = atoi(param_receb_4_char);
    }

    if (tot_esp != 3)
    {
        *param_receb_1_num = 3141592;
    }
}

void concatenar_char(char *equipament, int param_receb_1_num, int param_receb_2_num, int param_receb_3_num, int param_receb_4_num)
{
    char espaco_pala[2] = " ";
    char pala[1024];

    sprintf(pala, "%d", param_receb_1_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);

    sprintf(pala, "%d", param_receb_2_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);

    sprintf(pala, "%d", param_receb_3_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);

    sprintf(pala, "%d", param_receb_4_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
}

void concatenar_char_c_pare(char *equipament, int param_receb_1_num, int param_receb_2_num, int param_receb_3_num)
{
    char espaco_pala[2] = " ";
    char pala[1024];

    sprintf(pala, "%d", param_receb_1_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);

    strcat(equipament, "(");
    sprintf(pala, "%d", param_receb_2_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);

    sprintf(pala, "%d", param_receb_3_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, ")");
}

void concatenar_char_red(char *equipament, int param_receb_2_num, int param_receb_3_num)
{
    char espaco_pala[2] = " ";
    char pala[1024];

    sprintf(pala, "%d", param_receb_2_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);

    sprintf(pala, "%d", param_receb_3_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);
}

void verif_param(int *param_receb_1_num, int *param_receb_2_num, int *param_receb_3_num, int *param_receb_4_num)
{

    if (*param_receb_2_num < 0 || *param_receb_2_num > 10)
    {
        *param_receb_1_num = 3141592;
    }

    if (*param_receb_3_num < 0 || *param_receb_3_num > 150)
    {
        *param_receb_1_num = 3141592;
    }

    if (*param_receb_4_num < 0 || *param_receb_4_num > 100)
    {
        *param_receb_1_num = 3141592;
    }
}

void concatenar_char_c(char *equipament, int param_receb_1_num, int param_receb_2_num, int param_receb_3_num)
{
    char espaco_pala[2] = " ";
    char pala[1024];

    sprintf(pala, "%d", param_receb_1_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);

    sprintf(pala, "%d", param_receb_2_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
    strcat(equipament, espaco_pala);

    sprintf(pala, "%d", param_receb_3_num);
    strcat(equipament, pala);
    memset(pala, 0, 1024);
}

void get_sen_res(char *retorno, char *pala_in, char *pala_set)
{

    char espaco_pala[2] = " ";
    int tot_esp = 0;
    int tam_param_receb = 0;
    int param_atual;
    char param_receb_1_char[50];
    char param_receb_2_char[50];
    char param_receb_3_char[50];
    memset(param_receb_1_char, 0, 50);
    memset(param_receb_2_char, 0, 50);
    memset(param_receb_3_char, 0, 50);
    param_atual = 1;

    for (long unsigned int i = strlen(pala_set); i < strlen(pala_in) - 1; i++)
    {

        if (pala_in[i] == espaco_pala[0])
        {
            tot_esp = tot_esp + 1;
        }
        if (pala_in[i] == espaco_pala[0] && param_atual == 2)
        {
            param_atual = 3;
            tam_param_receb = 0;
        }

        if (pala_in[i] == espaco_pala[0] && param_atual == 1)
        {
            param_atual = 2;
            tam_param_receb = 0;
        }

        if (param_atual == 1 && pala_in[i] != espaco_pala[0])
        {
            param_receb_1_char[tam_param_receb] = pala_in[i];
            tam_param_receb += 1;
        }
        if (param_atual == 2 && pala_in[i] != espaco_pala[0])
        {
            param_receb_2_char[tam_param_receb] = pala_in[i];
            tam_param_receb += 1;
        }
        if (param_atual == 3 && pala_in[i] != espaco_pala[0])
        {
            param_receb_3_char[tam_param_receb] = pala_in[i];
            tam_param_receb += 1;
        }
    }

    if (tot_esp == 2)
    {
        strcpy(retorno, "sensor ");
        strcat(retorno, param_receb_1_char);
        strcat(retorno, ": ");
        strcat(retorno, param_receb_2_char);
        strcat(retorno, " ");
        strcat(retorno, param_receb_3_char);
    }
}

void get_val_res(char *retorno, char *pala_in, char *pala_set)
{

    int entrou_para = 0;
    char espaco_pala[2] = " ";
    int inici;
    int param_atual;
    char mens_ret[1024];
    memset(mens_ret, 0, 1024);

    strcpy(mens_ret, "sensors: ");

    param_atual = 1;

    for (long unsigned int i = strlen(pala_set); i < strlen(pala_in); i++)
    {

        inici = 1;
        if (pala_in[i] == espaco_pala[0] && param_atual == 3)
        {
            strcat(mens_ret, ")");
            strcat(mens_ret, " ");
            param_atual = 1;
            entrou_para = 0;
            inici = 0;
        }

        if (pala_in[i] == espaco_pala[0] && param_atual == 2)
        {
            strcat(mens_ret, " ");
            param_atual = 3;
            entrou_para = 0;
        }

        if (pala_in[i] == espaco_pala[0] && param_atual == 1 && inici == 1)
        {
            param_atual = 2;
            strcat(mens_ret, " ");
        }

        if (param_atual == 1 && pala_in[i] != espaco_pala[0])
        {
            mens_ret[strlen(mens_ret)] = pala_in[i];
            inici = 0;
        }
        if (param_atual == 2 && pala_in[i] != espaco_pala[0])
        {
            if (entrou_para == 0)
            {
                strcat(mens_ret, "(");
                mens_ret[strlen(mens_ret)] = pala_in[i];
                entrou_para = 1;
            }
            else
            {
                mens_ret[strlen(mens_ret)] = pala_in[i];
            }
        }
        if (param_atual == 3 && pala_in[i] != espaco_pala[0])
        {
            mens_ret[strlen(mens_ret)] = pala_in[i];
        }
    }
    strcat(mens_ret, ")");
    strcpy(retorno, mens_ret);
}