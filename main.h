#pragma once
#include <windows.h>
#include "SimpleRenderer.h"

#define W_WIDTH 640
#define W_HEIGHT 480
LPCWSTR windowTitle = L"MSPaint brush customizer";

//��� �����
LPCWSTR option1Nop_1 = L"xx 47 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 64 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 3C xx";
//��� ���������������� �����
LPCWSTR option1Nop_2 = L"xx 47 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 63 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx B7";
//��� �������������� �����
LPCWSTR option1Nop_3 = L"xx 47 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 65 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx B8";
//��� ����� ��� �����
LPCWSTR option1Nop_4 = L"xx 47 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 63 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx B6";
//��� �������
LPCWSTR option1Nop_5 = L"xx 47 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 62 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx B6";
//��� ����������� ���������
LPCWSTR option1Nop_6 = L"xx 47 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 61 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx B5";
//��� ����� ��� ��������
LPCWSTR option1Nop_7 = L"xx 47 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 61 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx B4";
//��� �������
LPCWSTR option1Nop_8 = L"xx 47 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 60 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx B4";
// ��� ���������
LPCWSTR option1Nop_9 = L"0F 47 D8 xx xx xx xx xx xx xx 89";
//��� ���������� � �������
LPCWSTR option1Nop_10 = L"83 FA 32";

LPCWSTR option2Cave_1 = L"48 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 0D A7";
LPCWSTR option2Cave_2 = L"48 xx xx xx xx xx xx xx xx xx xx xx xx xx F9 3B";

BYTE  option2Bytes_1[] = { 
	0x49, 0x83, 0xFE, 0x00, 
	0x0F, 0x84, 0xF7, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x01, 
	0x0F, 0x84, 0x69, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x03, 
	0x0F, 0x84, 0x6B, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x05, 
	0x0F, 0x84, 0x6D, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x08, 
	0x0F, 0x84, 0x6F, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x0A, 
	0x0F, 0x84, 0x71, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x10, 
	0x0F, 0x84, 0x73, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x1E, 
	0x0F, 0x84, 0x75, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x28, 
	0x0F, 0x84, 0x77, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x04, 
	0x0F, 0x84, 0x79, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x06, 
	0x0F, 0x84, 0x7B, 0x00, 0x00, 0x00, 
	0x83, 0x7A, 0x10, 0x02, 
	0x0F, 0x84, 0x7D, 0x00, 0x00, 0x00, 
	0xE9, 0x84, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0x19, 0x00, 0x00, 0x00, 
	0xE9, 0x78, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0x32, 0x00, 0x00, 0x00, 
	0xE9, 0x6C, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0x64, 0x00, 0x00, 0x00, 
	0xE9, 0x60, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0xC8, 0x00, 0x00, 0x00, 
	0xE9, 0x54, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0xE1, 0x00, 0x00, 0x00, 
	0xE9, 0x48, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0xFA, 0x00, 0x00, 0x00, 
	0xE9, 0x3C, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0x2C, 0x01, 0x00, 0x00, 
	0xE9, 0x30, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0x5E, 0x01, 0x00, 0x00, 
	0xE9, 0x24, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0x4B, 0x00, 0x00, 0x00, 
	0xE9, 0x18, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0x7D, 0x00, 0x00, 0x00, 
	0xE9, 0x0C, 0x00, 0x00, 0x00, 
	0xC7, 0x42, 0x10, 0x26, 0x00, 0x00, 0x00, 
	0xE9, 0x00, 0x00, 0x00, 0x00 
};

BYTE option2Bytes_2[] = { 
	0x48, 0x83, 0xFF, 0x05,				// cmp, rdi,05
	0x0F, 0x8D, 0x04, 0x00, 0x00, 0x00, // jnl originalcode
	0x48, 0x6B, 0xD2, 0x0C				//imul rdx,rdx,0C
};
LPCWSTR option3Cave_1 = L"48 xx C7 xx xx xx xx 50";
BYTE option3Bytes_1[] = { 0x48, 0x8B, 0xFB };
