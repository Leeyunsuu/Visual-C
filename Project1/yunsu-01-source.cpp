#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

//오류코드 비활성화
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
	//지역변수
	HWND hWind;
	HDC hDC;
	HANDLE hConsole;
	char chText[1024];

	int x, y;
	char chComm;

	hWind = GetForegroundWindow(); //bulit in function "root경로 윈도우의 핸들을 받는 함수"
	hDC = GetDC(hWind); //bulit in function "GDI 함수"
	//hDC = GetDC(GetForegroundWindow());
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //bulit in function "핸들 반환 함수"



	do {
		printf("---command:");

		scanf(" %c", &chComm);
		switch (chComm) {
		case 'a':
			base_display(hWind, hDC); //해당 함수 실행
			break;
		case 'b': // 콘솔 색상 테스트(배경,전경)
			ConsoleColorTest(hConsole); 
			break;
		case 'c': // 콘솔 지우기
			system("cls"); 
			break;
		case 'd': // 콘솔 색상 변경(배경:밝은힌색, 전경:어두운검은색)
			system("color f0"); //해당 함수 실행
			break;
		case 'e': // 콘솔창 크기 변경
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
	//좌표 이동함수(x.y)
	MoveToEx(hDC,	//대상 dc
		x,			//시작 x
		y,			//시작 y
		NULL);		//이전좌표
	x = 150; y = 250;
	//선 긋기 함수(x.y)까지
	LineTo(hDC, x, y);

	Rectangle(hDC, 100, 100, 200, 150); //사각형 그리기 함수

	Ellipse(hDC, 200, 200, 300, 300);	//원 그리기 함수

	x = 300; y = 70;
	for (i = 30; i < 250; i++) {
		SetPixel(hDC, (x + i), (y + i), RGB(255, 0, 0)); //픽셀 찍는 함수를 이용해 빨간선 긋기
	}

	return(0);
}

int ConsoleColorTest(HANDLE hConsole)
{
	int i, m;

	/*SetConsoleTextAttribute함수는 두번째 argu로 배경색과 텍스트 색을 16bit로 받는데
	그 중 3번째 4bit는 배경색, 4번째 4bit는 텍스트 색을 지정한다.*/
	//1111 1111 1111 1111
	//			배경	 텍스트

	for (i = 0; i < 16; i++) { // 배경색
		for (m = 0; m < 8; m++) { // 어두운 전경색
			SetConsoleTextAttribute(hConsole, ((i << 4) + m)); //SetConsoleTextAttribute(콘솔 출력 핸들,색상 값)
			printf("(test:%02x)", ((i << 4) + m));
		}
		printf("\n");
		for (m = 8; m < 16; m++) { // 밝은 전경색
			SetConsoleTextAttribute(hConsole, ((i << 4) + m));
			printf("(test:%02x)", ((i << 4) + m));
		}
		printf("\n");
	}

	return(0);
}

int gotoxy(int x, int y)
{

	//SetConsoleCursorPosition(핸들값, 위치값) : 해당위치로 커서 이동

	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);

	return(0);
}

int gotoxySprint(int x, int y, char* chText)
{

	//GetConsoleScreenBufferInfo(핸들값, 커서위치정보 저장할 변수): 현재 커서위치 저장함수

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo; //console 출력 구조체 변수
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo); //위치저장

	COORD curPos = { x, y }; // === {15,12} 배열구성
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curPos);  //x=15,y=12로 커서이동
	printf("%s", chText);

	COORD prePos = { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), prePos);

	return(0);
}

int gotoxySprintColor(int x, int y, char* chText, WORD wColor)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo); //위치저장

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);	//SetConsoleTextAttribute(콘솔 출력 핸들,색상 값)
	//((WHITE << 4) + RED)색상으로 변경

	COORD curPos = { x, y }; // === {15,12} 배열구성
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curPos); //x=15,y=12로 커서이동
	printf("%s", chText);

	COORD prePos = { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y }; // ==={저장된 x,저장된 y}
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), prePos); //저장되었던 위치로 커서이동

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bufferInfo.wAttributes); //저장된 색상으로 변경

	return(0);
}

