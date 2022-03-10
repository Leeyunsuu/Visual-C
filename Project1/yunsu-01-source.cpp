#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

//�����ڵ� ��Ȱ��ȭ
#pragma warning(disable : 4996)

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

int base_display(HWND hWind, HDC hDC);
int ConsoleColorTest(HANDLE hConsole);
int gotoxy(int x, int y);
int gotoxySprint(int x, int y, char* chText);
int gotoxySprintColor(int x, int y, char* chText, WORD wColor);

int main(void)
{
	//��������
	HWND hWind;
	HDC hDC;
	HANDLE hConsole;
	char chText[1024];

	int x, y;
	char chComm;

	hWind = GetForegroundWindow(); //bulit in function "root��� �������� �ڵ��� �޴� �Լ�"
	hDC = GetDC(hWind); //bulit in function "GDI �Լ�"
	//hDC = GetDC(GetForegroundWindow());
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //bulit in function "�ڵ� ��ȯ �Լ�"



	do {
		printf("---command:");

		scanf(" %c", &chComm);
		switch (chComm) {
		case 'a':
			base_display(hWind, hDC); //�ش� �Լ� ����
			break;
		case 'b': // �ܼ� ���� �׽�Ʈ(���,����)
			ConsoleColorTest(hConsole); 
			break;
		case 'c': // �ܼ� �����
			system("cls"); 
			break;
		case 'd': // �ܼ� ���� ����(���:��������, ����:��ο������)
			system("color f0"); //�ش� �Լ� ����
			break;
		case 'e': // �ܼ�â ũ�� ����
			system("mode con:cols=120 lines=40");
			break;
		case 'f': // x,y cursor move and print
			x = 15; y = 12;
			gotoxy(x, y);
			printf("---cursor move test---");
			break;
		case 'g': // x,y cursor move and print
			x = 15; y = 12;
			sprintf(chText, "---cursor move and string---");
			gotoxySprint(x, y, chText);
			break;
		case 'h': // x,y cursor move and print, color
			x = 15; y = 12;
			sprintf(chText, "---cursor move and string, color---");
			//gotoxySprintColor(x, y, chText, ((BROWN << 4) + RED));
			//gotoxySprintColor(x, y, chText, ((CYAN << 4) + RED));
			gotoxySprintColor(x, y, chText, ((WHITE << 4) + RED));
			break;
		default:
			break;
		}
	} while ('x' != chComm);

	ReleaseDC(hWind, hDC);
	CloseHandle(hConsole);

	return(0);
}

int base_display(HWND hWind, HDC hDC)
{
	int x, y;
	int i;

	x = 150; y = 170;
	//��ǥ �̵��Լ�(x.y)
	MoveToEx(hDC,	//��� dc
		x,			//���� x
		y,			//���� y
		NULL);		//������ǥ
	x = 150; y = 250;
	//�� �߱� �Լ�(x.y)����
	LineTo(hDC, x, y);

	Rectangle(hDC, 100, 100, 200, 150); //�簢�� �׸��� �Լ�

	Ellipse(hDC, 200, 200, 300, 300);	//�� �׸��� �Լ�

	x = 300; y = 70;
	for (i = 30; i < 250; i++) {
		SetPixel(hDC, (x + i), (y + i), RGB(255, 0, 0)); //�ȼ� ��� �Լ��� �̿��� ������ �߱�
	}

	return(0);
}

int ConsoleColorTest(HANDLE hConsole)
{
	int i, m;

	/*SetConsoleTextAttribute�Լ��� �ι�° argu�� ������ �ؽ�Ʈ ���� 16bit�� �޴µ�
	�� �� 3��° 4bit�� ����, 4��° 4bit�� �ؽ�Ʈ ���� �����Ѵ�.*/
	//1111 1111 1111 1111
	//			���	 �ؽ�Ʈ

	for (i = 0; i < 16; i++) { // ����
		for (m = 0; m < 8; m++) { // ��ο� �����
			SetConsoleTextAttribute(hConsole, ((i << 4) + m)); //SetConsoleTextAttribute(�ܼ� ��� �ڵ�,���� ��)
			printf("(test:%02x)", ((i << 4) + m));
		}
		printf("\n");
		for (m = 8; m < 16; m++) { // ���� �����
			SetConsoleTextAttribute(hConsole, ((i << 4) + m));
			printf("(test:%02x)", ((i << 4) + m));
		}
		printf("\n");
	}

	return(0);
}

int gotoxy(int x, int y)
{

	//SetConsoleCursorPosition(�ڵ鰪, ��ġ��) : �ش���ġ�� Ŀ�� �̵�

	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);

	return(0);
}

int gotoxySprint(int x, int y, char* chText)
{

	//GetConsoleScreenBufferInfo(�ڵ鰪, Ŀ����ġ���� ������ ����): ���� Ŀ����ġ �����Լ�

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo; //console ��� ����ü ����
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo); //��ġ����

	COORD curPos = { x, y }; // === {15,12} �迭����
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curPos);  //x=15,y=12�� Ŀ���̵�
	printf("%s", chText);

	COORD prePos = { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), prePos);

	return(0);
}

int gotoxySprintColor(int x, int y, char* chText, WORD wColor)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo); //��ġ����

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);	//SetConsoleTextAttribute(�ܼ� ��� �ڵ�,���� ��)
	//((WHITE << 4) + RED)�������� ����

	COORD curPos = { x, y }; // === {15,12} �迭����
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curPos); //x=15,y=12�� Ŀ���̵�
	printf("%s", chText);

	COORD prePos = { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y }; // ==={����� x,����� y}
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), prePos); //����Ǿ��� ��ġ�� Ŀ���̵�

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bufferInfo.wAttributes); //����� �������� ����

	return(0);
}

