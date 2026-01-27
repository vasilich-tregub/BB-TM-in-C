/* TERMS OF USE
 * This source code is subject to the terms of the MIT License.
 * Copyright(c) 2026 Vladimir Vasilich Tregub
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "cc-BB.h"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080

void build_http_response(int client_socket, char* file_name) {

    char response[1024];

    char fullpath[255];
    FILE* file = NULL;
    int res = 0;

    if (strlen(file_name) == 0) {
        strcpy_s(fullpath, 11, "index.html");
    }
    else {
        res = fopen_s(&file, file_name, "rb");
        if (res != 0) {
            memcpy(fullpath, file_name, strlen(file_name));
            if (file_name[strlen(file_name)] == 0x2F)
                memcpy(fullpath + strlen(file_name), "index.html", 11);
            else
                memcpy(fullpath + strlen(file_name), "/index.html", 12);
        }
        else
            memcpy(fullpath, file_name, strlen(file_name) + 1);
    }
    res = fopen_s(&file, fullpath, "rb");
    if (res != 0) {
        // File not found, send 404 response
        sprintf_s(response, 27, "HTTP/1.1 404 Not Found\r\n\r\n");
        send(client_socket, response, (int)strlen(response), 0);
    }
    else {
        // File found, send 200 OK response
        int notWasm = -1;
        char ext[7];
        if (strlen(file_name) > 6) {
            memcpy(ext, file_name + (int)strlen(file_name) - 5, 6);
            notWasm = strcmp(ext, ".wasm");
        }
        if (notWasm) {
            sprintf_s(response, 20, "HTTP/1.1 200 OK\r\n\r\n");
            send(client_socket, response, (int)strlen(response), 0);
        }
        else {
            sprintf_s(response, 73, "HTTP/1.1 200 OK\r\nContent-Type: application/wasm\r\nContent-Length: 258\r\n\r\n");
            send(client_socket, response, (int)strlen(response), 0);
        }
        // Send the file content
        char file_buffer[1024];
        int bytes_read = 0;
        int total_sent = 0;
        do {
            bytes_read = (int)fread_s(file_buffer, 1024, sizeof(char), 1024, file);
            int sent = send(client_socket, file_buffer, bytes_read, 0);
            total_sent += sent;
        } while (bytes_read == 1024);

        fclose(file);
    }
}

void build_xhr_response(int client_socket, char* query_string) {
    char sztmrules[37];
    int MIN_POS, MAX_POS;
    strcpy_s(sztmrules, 37, query_string + 10);
    int ret = cc_BB(sztmrules);
    int iter = 0;
    while (TAPE[iter] == 0 && iter < TAPE_LEN) {
        iter++;
    }
    MIN_POS = iter;
    while (TAPE[iter] != 0 && iter < TAPE_LEN) {
        iter++;
    }
    MAX_POS = iter;

    uint8_t* tapedisplay = malloc((MAX_POS - MIN_POS + 3) * sizeof(char));
    int tdix = 0;
    int ZEROPOS = TAPE_LEN / 2;
    if (POS < ZEROPOS) {
        for (int i = MIN_POS; i <= POS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
        tapedisplay[tdix++] = '.';
        for (int i = POS + 1; i < ZEROPOS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
        tapedisplay[tdix++] = '^';
        for (int i = ZEROPOS; i <= MAX_POS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
    }
    else {
        for (int i = MIN_POS; i < ZEROPOS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
        tapedisplay[tdix++] = '^';
        for (int i = ZEROPOS; i <= POS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
        tapedisplay[tdix++] = '.';
        for (int i = POS + 1; i <= MAX_POS; ++i) {
            tapedisplay[tdix++] = TAPE[i];
        }
    }
    tapedisplay[MAX_POS - MIN_POS + 2] = 0;
    //int shifts = (ret >> 16);
    //int POS = ret & 0xFFFF;
    //printf("shifts: %d, POS: %d\n\n", shifts, POS);
    char response[0x10100];
    sprintf_s(response, 20, "HTTP/1.1 200 OK\r\n\r\n");
    send(client_socket, response, (int)strlen(response), 0);
    sprintf_s(response, 0x10100, "%f§%d§%s", execTime, shifts, tapedisplay);
    free(tapedisplay);
    int sent = send(client_socket, response, strlen(response), 0);
}

char* parse_request(char* request, int client_socket) {
    // Extract the file name from the request
    char* query_string = strtok(request, " ");
    char* file_name;
    query_string = strtok(NULL, " ");
    if (query_string[1] == 0x3F)
    {
        build_xhr_response(client_socket, query_string);
        return NULL;
    }
 
    file_name = strtok(query_string, " ");
    return file_name + 1; // Skip the leading 
}

void handle_client(int client_socket) {
    char buffer[1024] = { 0 };
    int byte_received = 0;
    byte_received = recv(client_socket, buffer, 1024, 0);
    if (byte_received > 0)
    {
        printf("Bytes received: %d\n", byte_received);
        // Extract the requested file name from the request
        char* file_name = parse_request(buffer, client_socket);
        if (file_name == NULL) return;
        char response[1024];
        if (file_name[0] == 0x3F)
        {
            sprintf_s(response, 20, "HTTP/1.1 200 OK\r\n\r\n");
            send(client_socket, response, (int)strlen(response), 0);
            return;
        }
        // Build and send the HTTP response
        build_http_response(client_socket, file_name);
    }
    else if (byte_received == 0) {
        printf("Connection closed\n");
    }
    else
        printf("recv failed: %d\n", WSAGetLastError());
}

int main() {

    TAPE = malloc(TAPE_LEN * sizeof(uint8_t));

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0)
        return 1;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // Binding the socket to the port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");// INADDR_LOOPBACK;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        perror("bind failed");
        closesocket(server_fd);
        exit(EXIT_FAILURE);
    }
    // Listening for incoming connections
    if (listen(server_fd, 10) == SOCKET_ERROR) {
        perror("listen");
        closesocket(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, NULL/*(struct sockaddr*)&address*/, NULL/*(socklen_t*)&addrlen*/)) < 0) {
            perror("accept");
            closesocket(server_fd);
            exit(EXIT_FAILURE);
        }
        // Handle the client's request in a separate function
        handle_client(new_socket);
        closesocket(new_socket);
    }

    WSACleanup();

    free(TAPE);

    return 0;
}


