﻿// BluetoothTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <windows.h>
#include <ws2bth.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup function failed with error: %d\n", iResult);
        return 1;
    }
    // Ops�tning af socket
    SOCKET s;
    s = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
    if (s == INVALID_SOCKET) {
        wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //Ops�tning af bth socket adresse
    _SOCKADDR_BTH sbth;
    //memset(&sbth, 0, sizeof(SOCKADDR_BTH));
    sbth.addressFamily = AF_BTH;
    sbth.serviceClassId = RFCOMM_PROTOCOL_UUID;
    sbth.btAddr = BTH_ADDR(0x00066619FD59);
    sbth.port = 0;

    // Tilslutning til bluetooth module
    iResult = connect(s, (sockaddr*)&sbth, sizeof(sbth));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
        iResult = closesocket(s);
        if (iResult == SOCKET_ERROR)
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //Modtagelse af data 
    char recvbuf[200];
    char sendbuf[1];
    do {

        iResult = recv(s, recvbuf, sizeof(recvbuf), 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            for (int i = 0; i < iResult; i++) {
                cout << recvbuf[i];
            }
            cout << endl;
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());

    } while (iResult > 0);
    // Hvis error opst�r output error code, og cleanup wsa
    iResult = closesocket(s);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    // hvis ingen fejl output recvbuf, send modtagelse og k�r cleanup
    iResult = send(s, sendbuf, sizeof(sendbuf), 0);
    WSACleanup();
    return 0;
}