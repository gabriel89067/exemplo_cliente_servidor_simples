#pragma once

#include <stdlib.h>
#include <arpa/inet.h>

void logexit(const char *msg);

int addrparse(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage);

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize);

int server_sockaddr_init(const char *proto, const char *portstr,
                         struct sockaddr_storage *storage);

void name_file(char *pala_in, char *pala_set, char *name);

void read_file(char *nome_file, int *param_receb_1_num, int *param_receb_2_num, int *param_receb_3_num, int *param_receb_4_num);

void get_param(char *pala_in, char *pala_set, int *param_receb_1_num, int *param_receb_2_num, int *param_receb_3_num, int *param_receb_4_num);

void concatenar_char(char *equipamento, int param_receb_1_num, int param_receb_2_num, int param_receb_3_num, int param_receb_4_num);

void concatenar_char_c_pare(char *equipamento, int param_receb_1_num, int param_receb_2_num, int param_receb_3_num);

void concatenar_char_red(char *equipamento, int param_receb_2_num, int param_receb_3_num);

void verif_param(int *param_receb_1_num, int *param_receb_2_num, int *param_receb_3_num, int *param_receb_4_num);

void concatenar_char_c(char *equipament, int param_receb_1_num, int param_receb_2_num, int param_receb_3_num);

void get_sen_res(char *retorno, char *pala_in, char *pala_set);

void get_val_res(char *retorno, char *pala_in, char *pala_set);