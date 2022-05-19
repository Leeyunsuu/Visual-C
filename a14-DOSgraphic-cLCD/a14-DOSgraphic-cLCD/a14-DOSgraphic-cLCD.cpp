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

int i2c_address_data_masterWrite(HWND hWind, HDC hDC, int origin_x, int origin_y, int addr, int data, char read_write_mode);
int i2c_address_data_masterRead(HWND hWind, HDC hDC, int origin_x, int origin_y, int addr, int data, char read_write_mode);

int windth_x = 80;
int windth_y = 80;
int windth_gap = 20;

int main(void)
{
	HWND hWind;
	HDC hDC;
	HANDLE hConsole;

	char chComm[16];
	int i2c_address = 0xaa;
	int i2c_write_data = 0x55;
	int i2c_read_data = 0xb6;

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
			system("mode con:cols=240 lines=50");
			break;
		case 'c': // 콘솔 지우기
			system("cls");
			break;
		case 'd': // i2c : address[write mode](green color)-ack[low] : data(green color)-ack[low]
			i2c_address_data_masterWrite(hWind, hDC, 50, 100, i2c_address, i2c_write_data, 'w');
			printf("i2c addres[write mode](green color)-ack[low]=0x%02x, data(green color)-ack[low]=0x%02x\n", i2c_address, i2c_write_data);
			break;
		case 'e': // i2c : address[read mode](green color)-ack[low] : data(yellow color)-Nack[high]
			i2c_address_data_masterRead(hWind, hDC, 50, 100, i2c_address, i2c_read_data, 'r');
			printf("i2c addres[read mode](green color)-ack[low]=0x%02x, data(yellow color)-Nack[high]=0x%02x\n", i2c_address, i2c_read_data);
			break;
		default:
			break;
		}
	} while ('x' != chComm[0]);

	ReleaseDC(hWind, hDC);
	CloseHandle(hConsole);

	return(0);
}

int i2c_address_data_masterWrite(HWND hWind, HDC hDC, int origin_x, int origin_y, int addr, int data, char read_write_mode)
{
	int base_x, base_y;
	int i;
	int m, d;
	int address;
	int writedata;
	int chComp;

	base_x = origin_x; base_y = origin_y;
	// idle(black)
	m = 0;
	for (i = 0; i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(25, 25, 25));
	}
	// start(red)
	m = 1;
	for (i = 0; i < ((windth_x * 3) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(235, 0, 0));
	}
	for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(235, 0, 0));
	}
	for (i = 0; i < ((windth_x * 2) / 4); i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
	}
	for (i = ((windth_x * 2) / 4); i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(235, 0, 0));
	}

	// address(green)
	if ('w' == read_write_mode) {
		address = (addr & 0xfe) + 0x00;
	}
	else {
		address = (addr & 0xfe) + 0x01;
	}
	chComp = 0x80;
	m = 2;
	for (d = 0; d < 8; d++) {
		if ((address & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// SDA
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// SDA
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 235, 0));
		}
		for (i = ((windth_x * 1) / 4); i < ((windth_x * 3) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(0, 235, 0));
		}
		for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 235, 0));
		}
		m += 1;
		chComp >>= 1;
	}

	// ack(blue)
	for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 0, 235));
	}
	for (i = ((windth_x * 1) / 4); i < ((windth_x * 3) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(0, 0, 235));
	}
	for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 0, 235));
	}
	// ack = OK
	for (i = 0; i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 0, 235));
	}

	// data[write](green)
	writedata = data;
	chComp = 0x80;
	m += 1;
	for (d = 0; d < 8; d++) {
		// SDA
		if ((writedata & chComp) == chComp) {

			//---- - 코드추가---- -
			for (i = 0; i < windth_x; i++) {	// SDA
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// SDA
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		// SCL

		//---- - 코드추가---- -
		for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 235, 0));
		}
		for (i = ((windth_x * 1) / 4); i < ((windth_x * 3) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(0, 235, 0));
		}
		for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 235, 0));
		}
			m += 1;
		chComp >>= 1;
	}

	// ack(blue)
	for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 0, 235));
	}
	for (i = ((windth_x * 1) / 4); i < ((windth_x * 3) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(0, 0, 235));
	}
	for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 0, 235));
	}
	// ack = OK
	for (i = 0; i < windth_x; i++) {	// SDA

		//---- - 코드추가---- -
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 0, 235));
	}

	// stop(red)
	m += 1;
	for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(235, 0, 0));
	}
	for (i = ((windth_x * 1) / 4); i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(235, 0, 0));
	}
	for (i = 0; i < ((windth_x * 2) / 4); i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(235, 0, 0));
	}
	for (i = ((windth_x * 2) / 4); i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
	}

	// idle(black)
	m += 1;
	for (i = 0; i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(25, 25, 25));
	}

	return(0);
}

int i2c_address_data_masterRead(HWND hWind, HDC hDC, int origin_x, int origin_y, int addr, int data, char read_write_mode)
{
	int base_x, base_y;
	int i;
	int m, d;
	int address;
	int readdata;
	int chComp;

	base_x = origin_x; base_y = origin_y;
	// idle(black)
	m = 0;
	for (i = 0; i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(25, 25, 25));
	}
	// start(red)

	//---- - 코드추가---- -
	m = 1;
	for (i = 0; i < ((windth_x * 3) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(235, 0, 0));
	}
	for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(235, 0, 0));
	}
	for (i = 0; i < ((windth_x * 2) / 4); i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
	}
	for (i = ((windth_x * 2) / 4); i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(235, 0, 0));
	}

		// address(green)
		if ('w' == read_write_mode) {
			address = (addr & 0xfe) + 0x00;
		}
		else {
			address = (addr & 0xfe) + 0x01;
		}
	chComp = 0x80;
	m = 2;

	//---- - 코드추가---- -
	for (d = 0; d < 8; d++) {
		if ((address & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// SDA
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(0, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// SDA
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 235, 0));
			}
		}
		for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 235, 0));
		}
		for (i = ((windth_x * 1) / 4); i < ((windth_x * 3) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(0, 235, 0));
		}
		for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 235, 0));
		}
		m += 1;
		chComp >>= 1;
	}
	

		// ack(blue)

		//---- - 코드추가---- -
	for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 0, 235));
	}
	for (i = ((windth_x * 1) / 4); i < ((windth_x * 3) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(0, 0, 235));
	}
	for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 0, 235));
	}
	// ack = OK
	for (i = 0; i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(0, 0, 235));
	}

		// data[read](yellow)
		readdata = data;
	chComp = 0x80;
	m += 1;

	//---- - 코드추가---- -
	for (d = 0; d < 8; d++) {
		if ((readdata & chComp) == chComp) {
			for (i = 0; i < windth_x; i++) {	// SDA
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 235, 0));
			}
		}
		else {
			for (i = 0; i < windth_x; i++) {	// SDA
				SetPixel(hDC, (base_x + (m * windth_x+1) + i), (base_y + (windth_y * 2) + windth_gap), RGB(235, 235, 0));
			}
		}
		for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 235, 0));
		}
		for (i = ((windth_x * 1) / 4); i < ((windth_x * 3) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(0, 235, 0));
		}
		for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 235, 0));
		}
		m += 1;
		chComp >>= 1;
	}

		// Nack(blue)

		//---- - 코드추가---- -
	for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 0, 235));
	}
	for (i = ((windth_x * 1) / 4); i < ((windth_x * 3) / 4); i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(0, 0, 235));
	}
	for (i = ((windth_x * 3) / 4); i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(0, 0, 235));
	}
	// ack = OK
	for (i = 0; i < windth_x; i++) {	// SDA

		//---- - 코드추가---- -
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(0, 0, 235));
	}

	

		// stop(red)
		m += 1;

	//---- - 코드추가---- -
		for (i = 0; i < ((windth_x * 1) / 4); i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1)), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 1) / 4); i < windth_x; i++) {	// SCL
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(235, 0, 0));
		}
		for (i = 0; i < ((windth_x * 2) / 4); i++) {	// SDA
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 2) + windth_gap), RGB(235, 0, 0));
		}
		for (i = ((windth_x * 2) / 4); i < windth_x; i++) {	// SDA
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(235, 0, 0));
		}

		// idle(black)
		m += 1;
	for (i = 0; i < windth_x; i++) {	// SCL
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 0)), RGB(25, 25, 25));
	}
	for (i = 0; i < windth_x; i++) {	// SDA
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + (windth_y * 1) + windth_gap), RGB(25, 25, 25));
	}

	return(0);
}


