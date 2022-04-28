#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#pragma warning(disable:4996)

//----- console color -----
#define BLACK	0x0
#define BLUE	0x1
#define GREEN	0x2
#define CYAN	0x3
#define RED		0x4
#define MAGENTA	0x5
#define BROWN	0x6
#define WHITE	0x7
//----- console color -----

int spi_address_data_CPOL_0_CPHA_0(HWND hWind, HDC hDC, int origin_x, int origin_y, int address, int* MOSI_data, int* MISO_data);
int spi_address_data_CPOL_0_CPHA_1(HWND hWind, HDC hDC, int origin_x, int origin_y, int address, int* MOSI_data, int* MISO_data);
int spi_address_data_CPOL_1_CPHA_0(HWND hWind, HDC hDC, int origin_x, int origin_y, int address, int* MOSI_data, int* MISO_data);
int spi_address_data_CPOL_1_CPHA_1(HWND hWind, HDC hDC, int origin_x, int origin_y, int address, int* MOSI_data, int* MISO_data);

int windth_x = 80;
int windth_y = 80;
int windth_gap = 20;

int main(void)
{
	HWND hWind;
	HDC hDC;
	HANDLE hConsole;

	char chComm[16];
	int spi_address = 0xaa;
	int spi_MOSI_data = 0x55;
	int spi_MISO_data = 0x55;

	hWind = GetForegroundWindow();
	hDC = GetDC(hWind);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	do {
		printf("---command:");

		scanf(" %s", chComm);
		switch (chComm[0]) {
		case 'a': // 콘솔 색상 변경(배경:밝은힌색, 전경:어두운검은색)
			system("color f0");
			break;
		case 'b': // 콘솔창 크기 변경
			system("mode con:cols=160 lines=50");
			break;
		case 'c': // 콘솔 지우기
			system("cls");
			break;
		case '0': // spi : CPOL=0 CPHA=0
			spi_address_data_CPOL_0_CPHA_0(hWind, hDC, 100, 100, spi_address, &spi_MOSI_data, &spi_MISO_data);
			printf("spi_address_data_CPOL_0_CPHA_0 addr=0x%02x MOSI=0x%02x MISO=0x%02x\n", spi_address, spi_MOSI_data, spi_MISO_data);
			break;
		case '1': // spi : CPOL=0 CPHA=1
			spi_address_data_CPOL_0_CPHA_1(hWind, hDC, 100, 100, spi_address, &spi_MOSI_data, &spi_MISO_data);
			printf("spi_address_data_CPOL_0_CPHA_1 addr=0x%02x MOSI=0x%02x MISO=0x%02x\n", spi_address, spi_MOSI_data, spi_MISO_data);
			break;
		case '2': // spi : CPOL=1 CPHA=0
			spi_address_data_CPOL_1_CPHA_0(hWind, hDC, 100, 100, spi_address, &spi_MOSI_data, &spi_MISO_data);
			printf("spi_address_data_CPOL_1_CPHA_0 addr=0x%02x MOSI=0x%02x MISO=0x%02x\n", spi_address, spi_MOSI_data, spi_MISO_data);
			break;
		case '3': // spi : CPOL=1 CPHA=1
			spi_address_data_CPOL_1_CPHA_1(hWind, hDC, 100, 100, spi_address, &spi_MOSI_data, &spi_MISO_data);
			printf("spi_address_data_CPOL_1_CPHA_1 addr=0x%02x MOSI=0x%02x MISO=0x%02x\n", spi_address, spi_MOSI_data, spi_MISO_data);
			break;
		default:
			break;
		}
	} while ('x' != chComm[0]);

	ReleaseDC(hWind, hDC);
	CloseHandle(hConsole);

	return(0);
}

int spi_address_data_CPOL_0_CPHA_0(HWND hWind, HDC hDC, int origin_x, int origin_y, int address, int* MOSI_data, int* MISO_data)
{
	int base_x, base_y;
	int i;
	int m, d;
	int chComp;

	base_x = origin_x; base_y = origin_y;
	// idle(black)
	m = 0;
	for (i = 0; i < windth_x; i++) {	// SS=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SCLK=0
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MOSI=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MISO=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3) + windth_gap), RGB(25, 25, 25));
	}

	// address
	chComp = 0x80;
	m = 1;
	for (d = 0; d < 8; d++) {
		for (i = 0; i < windth_x; i++) {	// SS=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(25, 25, 25));
		}
		for (i = 0; i < ((windth_x * 1) / 2); i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 2); i < windth_x; i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}
		if ((address & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// MOSI=address
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// MOSI=address
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3)), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < windth_x; i++) {	// MISO=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 4)), RGB(0, 0, 235));
		}
		m += 1;
		chComp >>= 1;
	}

	// MOSI data
	chComp = 0x80;
	for (d = 0; d < 8; d++) {
		for (i = 0; i < windth_x; i++) {	// SS=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(25, 25, 25));
		}
		for (i = 0; i < ((windth_x * 1) / 2); i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 2); i < windth_x; i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}
		if ((*MOSI_data & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// MOSI=data
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// MOSI=data
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3)), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < windth_x; i++) {	// MISO=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 4)), RGB(0, 0, 235));
		}
		m += 1;
		chComp >>= 1;
	}

	// idle(black)
	for (i = 0; i < windth_x; i++) {	// SS=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SCLK=0
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MOSI=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MISO=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3) + windth_gap), RGB(25, 25, 25));
	}

	return(0);
}

int spi_address_data_CPOL_0_CPHA_1(HWND hWind, HDC hDC, int origin_x, int origin_y, int address, int* MOSI_data, int* MISO_data)
{

	//----- 코드 작성 하기 -----

	int base_x, base_y;
	int i;
	int m, d;
	int chComp;

	base_x = origin_x; base_y = origin_y;
	// idle(black)
	m = 0;
	for (i = 0; i < windth_x; i++) {	// SS=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SCLK=0
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MOSI=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MISO=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3) + windth_gap), RGB(25, 25, 25));
	}

	// address
	chComp = 0x80;
	m = 1;
	for (d = 0; d < 8; d++) {
		for (i = 0; i < windth_x; i++) {	// SS=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(25, 25, 25));
		}
		for (i = 0; i < ((windth_x * 1) / 2); i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 2); i < windth_x; i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(235, 0, 0));
		}
		if ((address & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// MOSI=address
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// MOSI=address
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3)), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < windth_x; i++) {	// MISO=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 4)), RGB(0, 0, 235));
		}
		m += 1;
		chComp >>= 1;
	}

	// MOSI data
	chComp = 0x80;
	for (d = 0; d < 8; d++) {
		for (i = 0; i < windth_x; i++) {	// SS=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(25, 25, 25));
		}
		for (i = 0; i < ((windth_x * 1) / 2); i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 2); i < windth_x; i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(235, 0, 0));
		}
		if ((*MOSI_data & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// MOSI=data
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// MOSI=data
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3)), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < windth_x; i++) {	// MISO=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 4)), RGB(0, 0, 235));
		}
		m += 1;
		chComp >>= 1;
	}

	// idle(black)
	for (i = 0; i < windth_x; i++) {	// SS=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SCLK=0
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MOSI=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MISO=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3) + windth_gap), RGB(25, 25, 25));
	}

	return(0);
}

int spi_address_data_CPOL_1_CPHA_0(HWND hWind, HDC hDC, int origin_x, int origin_y, int address, int* MOSI_data, int* MISO_data)
{

	//----- 코드 작성 하기 -----
	int base_x, base_y;
	int i;
	int m, d;
	int chComp;

	base_x = origin_x; base_y = origin_y;
	// idle(black)
	m = 0;
	for (i = 0; i < windth_x; i++) {	// SS=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SCLK=0
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MOSI=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MISO=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3) + windth_gap), RGB(25, 25, 25));
	}

	// address
	chComp = 0x80;
	m = 1;
	for (d = 0; d < 8; d++) {
		for (i = 0; i < windth_x; i++) {	// SS=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(25, 25, 25));
		}
		for (i = 0; i < ((windth_x * 1) / 2); i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 2); i < windth_x; i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(235, 0, 0));
		}
		if ((address & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// MOSI=address
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// MOSI=address
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3)), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < windth_x; i++) {	// MISO=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 4)), RGB(0, 235, 0));
		}
		m += 1;
		chComp >>= 1;
	}

	// MOSI data
	chComp = 0x80;
	for (d = 0; d < 8; d++) {
		for (i = 0; i < windth_x; i++) {	// SS=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(25, 25, 25));
		}
		for (i = 0; i < ((windth_x * 1) / 2); i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 2); i < windth_x; i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(235, 0, 0));
		}
		if ((*MOSI_data & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// MOSI=data
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// MOSI=data
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3)), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < windth_x; i++) {	// MISO=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 4)), RGB(0, 235, 0));
		}
		m += 1;
		chComp >>= 1;
	}

	// idle(black)
	for (i = 0; i < windth_x; i++) {	// SS=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SCLK=0
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MOSI=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MISO=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3) + windth_gap), RGB(25, 25, 25));
	}

	return(0);
	
}

int spi_address_data_CPOL_1_CPHA_1(HWND hWind, HDC hDC, int origin_x, int origin_y, int address, int* MOSI_data, int* MISO_data)
{

	//----- 코드 작성 하기 -----

	int base_x, base_y;
	int i;
	int m, d;
	int chComp;

	base_x = origin_x; base_y = origin_y;
	// idle(black)
	m = 0;
	for (i = 0; i < windth_x; i++) {	// SS=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SCLK=0
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MOSI=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MISO=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3) + windth_gap), RGB(25, 25, 25));
	}

	// address
	chComp = 0x80;
	m = 1;
	for (d = 0; d < 8; d++) {
		for (i = 0; i < windth_x; i++) {	// SS=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(25, 25, 25));
		}
		for (i = 0; i < ((windth_x * 1) / 2); i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 2); i < windth_x; i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}
		if ((address & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// MOSI=address
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// MOSI=address
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3)), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < windth_x; i++) {	// MISO=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 4)), RGB(0, 235, 0));
		}
		m += 1;
		chComp >>= 1;
	}

	// MOSI data
	chComp = 0x80;
	for (d = 0; d < 8; d++) {
		for (i = 0; i < windth_x; i++) {	// SS=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(25, 25, 25));
		}
		for (i = 0; i < ((windth_x * 1) / 2); i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2)), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 2); i < windth_x; i++) {	// SCLK
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}
		if ((*MOSI_data & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// MOSI=data
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// MOSI=data
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3)), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < windth_x; i++) {	// MISO=0
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 4)), RGB(0, 235, 0));
		}
		m += 1;
		chComp >>= 1;
	}

	// idle(black)
	for (i = 0; i < windth_x; i++) {	// SS=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SCLK=0
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MOSI=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// MISO=1
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 3) + windth_gap), RGB(25, 25, 25));
	}

	return(0);
}
