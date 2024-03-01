#include "main.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "../include/utils.h"
extern volatile char       g_lux[5];
extern volatile char       g_mois[5];
extern volatile char       g_temp[6];

void    esp_send_data(char *g_lux, char *g_temp, char *g_mois) {
    ft_delay(F_CPU / 500);
    // uart_strx("AT+CWJAP_CUR=\"iPhone\",\"Pedro900\"\r\n");
    // uart_strx("AT+CWJAP_CUR=\"Pixel_6023\",\"92itn65rmnq5ij\"\r\n");
    uart_strx("AT+CWJAP_CUR=\"beboss\",\"petitchat\"\r\n");
    ft_delay(F_CPU / 8);
    uart_strx("AT+CIPSTATUS\r\n");
    ft_delay(F_CPU / 30);
    uart_strx("AT+CIPSTART=\"TCP\",\"87.89.194.28\",4000\r\n");
    ft_delay(F_CPU / 30);

    uint16_t body_len = 18 + strlen(g_temp) + 16 + strlen(g_mois) + 18 + strlen(g_lux) + 4; // 18 -> {\r\n\"temperature\":\"
    char cmd2[] = "\0\0\0"; // content(body) length
    ft_uitoa(body_len, cmd2, 3);
    // // char cmd2[] = "65";
    uint16_t cmd_len = 87 + 2 + 54 + body_len - 18;

    // Complete POST request example :
    // char cmd[] = "POST /api/insert HTTP/1.0\r\nHost: 87.89.194.28\r\nConnection: keep-alive\r\nContent-Length: 64\r\nContent-Type: application/json\r\n\r\n{\r\n\"temperature\":\"25\",\r\n\"humidite\":\"800\",\r\n\"luminosite\":\"500\"\r\n}";
    // char cmd[] = "GET /api/get HTTP/1.0\r\nHost: 87.89.194.28\r\n\r\n"; // GET http request example

    char cmd1[] = "POST /api/insert HTTP/1.0\r\nHost: 87.89.194.28\r\nConnection: keep-alive\r\nContent-Length: ";
    //cmd2 = body length
    char cmd3[] = "\r\nContent-Type: application/json\r\n\r\n{\r\n\"temperature\":\"";
    // char test[] = "{\r\n\"temperature\":\"";
    char cmd5[] = "\",\r\n\"humidite\":\"";
    char cmd7[] = "\",\r\n\"luminosite\":\"";
    char cmd9[] = "\"\r\n}";
    char tmp[20];
    char fcmd[250];
    sprintf(tmp, "AT+CIPSEND=%d\r\n", cmd_len);
    uart_strx(tmp);
    ft_delay(F_CPU / 50);
    
    sprintf(fcmd, "%s%s%s%s%s%s%s%s%s", cmd1, cmd2, cmd3, g_temp, cmd5, g_mois, cmd7, g_lux, cmd9);
    uart_strx(fcmd);
    
    ft_delay(F_CPU / 20);
}