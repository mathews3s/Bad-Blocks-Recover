#pragma once
#pragma comment(lib,"winmm.lib")
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <time.h>
#include <winioctl.h>
#include <iostream>


#define MAX_COLUMNS 49
#define MAX_ROWS 23

enum COLORS
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

unsigned char B0[1] = { 0xB0 };
unsigned char B1[1] = { 0xB1 };
unsigned char B2[1] = { 4 };
unsigned char space[1] = { 0x20 };

unsigned char leg[] = "COLORS INFO";
unsigned char ms3[5] = { 0xB0, '<', '3', 'm', 's' };
unsigned char ms50[6] = { 0xB2, '<', '5', '0', 'm', 's' };
unsigned char ms10[6] = { 0xB1, '<', '1', '0', 'm', 's' };

unsigned char squ[1] = { 0xB2 };
unsigned char ms150[6] = { '<', '1', '5', '0', 'm', 's' };
unsigned char ms500[6] = { '<', '5', '0', '0', 'm', 's' };
unsigned char ms500_2[6] = { '>', '5', '0', '0', 'm', 's' };


using namespace std;