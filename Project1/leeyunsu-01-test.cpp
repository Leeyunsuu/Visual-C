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

int uart_oneByte_display(HWND hWind, HDC hDC, int origin_x, int origin_y, char parity, char data);
int uart_multiByte_display(HWND hWind, HDC hDC, int origin_x, int origin_y, char parity, int data_cnt, char* data);

int windth_x = 80;
int windth_y = 80;
int windth_gap = 40;

int main(void)
{
	HWND hWind;
	HDC hDC;
	HANDLE hConsole;
	char chText[1024];

	int i;
	char chComm[16];
	char parity_In;
	int data_cnt_In;

	hWind = GetForegroundWindow();	//bulit in function "root경로 윈도우의 핸들을 받는 함수"
	hDC = GetDC(hWind);	//bulit in function "GDI 함수"
	//hDC = GetDC(GetForegroundWindow());
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	//bulit in function "핸들 반환 함수"

	do {
		printf("---command:");

		scanf(" %s", chComm);	//	char, char, int //de4
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
		case 'd': // uart : one byte tx
			parity_In = chComm[1];	//parity_In = e;
			chText[0] = chComm[2];	//chText[0] = 4;
			uart_oneByte_display(hWind, hDC, 100, 100, parity_In, chText[0]);	
			//uart_oneByte_display(핸들함수, GDI함수, 100, 100, e, 4);
			printf("uart data=%c[0x%02x]\n", chText[0], chText[0]);
			break;
		case 'e': // uart : multi byte tx
			parity_In = chComm[1];
			data_cnt_In = (int)chComm[2] - 48;
			for (i = 0; i < data_cnt_In; i++) {
				chText[i] = chComm[3 + i];
			}
			uart_multiByte_display(hWind, hDC, 100, 100, parity_In, data_cnt_In, chText);
			printf("uart data[%d]=", data_cnt_In);
			for (i = 0; i < data_cnt_In; i++) {
				printf("%c[0x%02x] ", chText[i], chText[i]);
			}
			printf("\n");
			break;
		default:
			break;
		}
	} while ('x' != chComm[0]);

	ReleaseDC(hWind, hDC);
	CloseHandle(hConsole);

	return(0);
}

int uart_oneByte_display(HWND hWind, HDC hDC, int origin_x, int origin_y, char parity, char data)
	//uart_oneByte_display(핸들함수, GDI함수, 100, 100, e, 4);
{
	int base_x, base_y;
	int i;
	int m, d;
	char bit_cnt;
	char chComp;

	/*SetPixel(

		HDC hdc, // hdc의 핸들.

		int X, // 출력할 픽셀의 x좌표.

		int Y, // 출력할 픽셀의 y좌표.

		COLORREF crColor); // 픽셀의 색깔.*/

	/////////////////////
	printf("uart_oneByte_display data : %c\n", data);
	printf("uart_oneByte_display parity : %02x\n", parity);
	///////////////////// test 후 지웁시다.


	base_x = origin_x; base_y = origin_y; //base_x = 100; base_y = 100;
	// idle(black)
	m = 0;
	for (i = 0; i < windth_x; i++) {
		SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(25, 0, 25)); 
		//SetPixel(핸들함수, (100 + (0 * 80) + 1~100), 100, 색상);
	}
	// start(red)
	m = 1;
	for (i = 0; i < windth_x; i++) {
		SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + windth_y), RGB(235, 0, 0));
		//SetPixel(핸들함수, (100 + (1 * 80) + 1~100), (100 + 80), 색상);
	}
	// data
	chComp = 0x01;
	bit_cnt = 0;
	d = 0;
	m = 2;
	
	while (1) {
		/////////////////////
		//char test = data & chComp;
		
		///////////////////// test 후 지웁시다.
		if ((data & chComp) == chComp) {	// bit = 1
			for (i = 0; i < windth_x; i++) {
				SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
				//SetPixel(핸들함수, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
			}
			bit_cnt += 1;
		}
		else {							// bit = 0
			for (i = 0; i < windth_x; i++) {
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + windth_y), RGB(0, 235, 0));
			}
		}
		chComp <<= 1;
		d += 1;
		if (d == 8) {
			break;
		}
		m += 1;
		printf("uart_oneByte_display data : %d\n", bit_cnt);
	}
	// parity

	//-----------------여기에 코드를 추가하세요.---------------------------------------------------
	
	if (bit_cnt%2 == 0) {
		m += 1;
		if (parity == 'o') {
			for (i = 0; i < windth_x; i++) {
				SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(0, 0, 235));
				//SetPixel(핸들함수, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
			}
		}
		else if (parity == 'e') {
			for (i = 0; i < windth_x; i++) {
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + windth_y), RGB(0, 0, 235));
				//SetPixel(핸들함수, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
			}
		}		
	}
	else if (bit_cnt % 2 == 1) {
		if (parity == 'e') {
			for (i = 0; i < windth_x; i++) {
				SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(0, 0, 235));
				//SetPixel(핸들함수, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
			}
		}
		else if (parity == 'o') {
			for (i = 0; i < windth_x; i++) {
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + windth_y), RGB(0, 0, 235));
				//SetPixel(핸들함수, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
			}
		}
	}
	/*m += 1;
	chComp <<= 1;
	if ((data & chComp) == chComp) { // bit = 1
		
		for (i = 0; i < windth_x; i++) {
			SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
			//SetPixel(핸들함수, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
		}
		//bit_cnt += 1;
	}
	else {							// bit = 0
		for (i = 0; i < windth_x; i++) {
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + windth_y), RGB(0, 235, 0));
		}
	}*/
		

	// stop
	m += 1;
	for (i = 0; i < windth_x; i++) {
		SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(235, 0, 0));
	}

	return(0);
}

int uart_multiByte_display(HWND hWind, HDC hDC, int origin_x, int origin_y, char parity, int data_cnt, char* data)
{
	int base_x, base_y;
	int i;
	int m, d;
	char bit_cnt;
	char chComp;
	int c;

	for (int c = 0; c < data_cnt; c++) {
		base_x = origin_x; base_y = origin_y + (c * windth_y) + (c * windth_gap);
		// idle(black)
		m = 0;
		for (i = 0; i < windth_x; i++) {
			SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(25, 25, 25));
		}
		// start(red)
		m = 1;
		for (i = 0; i < windth_x; i++) {
			SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + windth_y), RGB(235, 0, 0));
		}
		// data
		chComp = 0x01;
		bit_cnt = 0;
		d = 0;
		m = 2;
		while (1) {
			if ((data[c] & chComp) == chComp) {	// bit = 1
				for (i = 0; i < windth_x; i++) {
					SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
				}
				bit_cnt += 1;
			}
			else {							// bit = 0
				for (i = 0; i < windth_x; i++) {
					SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + windth_y), RGB(0, 235, 0));
				}
			}
			chComp <<= 1;
			d += 1;
			if (d == 8) {
				break;
			}
			m += 1;
		}
		// parity

		//-----------------여기에 코드를 추가하세요.---------------------------------------------------
		if (parity == 'o') {
			m += 1;
			for (i = 0; i < windth_x; i++) {
				SetPixel(hDC, (base_x + (m * windth_x) + i), (base_y + windth_y), RGB(0, 0, 235));
				//SetPixel(핸들함수, (base_x + (m * windth_x) + i), base_y, RGB(0, 235, 0));
			}
		}
		// stop
		m += 1;
		for (i = 0; i < windth_x; i++) {
			SetPixel(hDC, (base_x + (m * windth_x) + i), base_y, RGB(235, 0, 0));
		}
	}

	return(0);
}

