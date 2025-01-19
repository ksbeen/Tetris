#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdbool.h>

//게임 오버 기능 - 유찬
//색 추가 - 해웅
//점수 시스템 - 유찬
//UI (게임 인트로, pause기능 등등) - pause 령곤, 메인메뉴 - 성빈
//시간이 지날수록 하강속도 상승 + 난이도 - 해웅
//최고 점수 저장 -성빈
//다음 블럭 미리보기 - 유찬
//버그 수정 - 령곤

//ppt 제작

clock_t startDropT, endT, startGroundT;
int x = 8, y = 0;
RECT blockSize;
int blockForm; //블록 형태
int blockRotation = 0; //블록의 현재 회전 상태를 저장하는 변수
int key;
int score = 0; //사용자 입력 키를 저장하는 변수
int highScore = 0; //최고 점수 저장
int nextBlockForm; // 다음에 생성될 블록
int dropSpeed = 800; // 기본하강속도
int SpeedInterval = 30000; // 하강 속도 주기 (30초)
clock_t SpeedTime = 0; // 하강 속도 증가 시점

#define COLOR_WHITE 7
#define COLOR_YELLOW 6
#define COLOR_BLUE 1
#define COLOR_RED 4
#define COLOR_CYAN 3
#define COLOR_MAGENTA 5
#define COLOR_GREEN 2
#define COLOR_LIGHT_BLUE 9

int blockColors[7] = {
COLOR_YELLOW, // T형 블록
COLOR_BLUE, // L형 블록
COLOR_RED, // Z형 블록
COLOR_CYAN, // I형 블록
COLOR_MAGENTA, // O형 블록
COLOR_GREEN, // J형 블록
COLOR_LIGHT_BLUE // S형 블록
};

int block[7][4][4][4] = { //다차원 배열
{ // T모양 블럭
{
{0,0,0,0},
{0,1,0,0},
{1,1,1,0},
{0,0,0,0}
},
{
{0,0,0,0},
{0,1,0,0},
{0,1,1,0},
{0,1,0,0}
},
{
{0,0,0,0},
{0,0,0,0},
{1,1,1,0},
{0,1,0,0}
},
{
{0,0,0,0},
{0,1,0,0},
{1,1,0,0},
{0,1,0,0}
}
},
{ // 번개 블럭
{
{0,0,0,0},
{0,1,1,0},
{1,1,0,0},
{0,0,0,0}
},
{
{0,0,0,0},
{1,0,0,0},
{1,1,0,0},
{0,1,0,0}
},
{
{0,0,0,0},
{0,1,1,0},
{1,1,0,0},
{0,0,0,0}
},
{
{0,0,0,0},
{1,0,0,0},
{1,1,0,0},
{0,1,0,0}
}
},
{ // 번개 블럭 반대
{
{0,0,0,0},
{1,1,0,0},
{0,1,1,0},
{0,0,0,0}
},
{
{0,0,0,0},
{0,1,0,0},
{1,1,0,0},
{1,0,0,0}
},
{
{0,0,0,0},
{1,1,0,0},
{0,1,1,0},
{0,0,0,0}
},
{
{0,0,0,0},
{0,1,0,0},
{1,1,0,0},
{1,0,0,0}
}
},
{ // 1자형 블럭
{
{0,1,0,0},
{0,1,0,0},
{0,1,0,0},
{0,1,0,0}
},
{
{0,0,0,0},
{0,0,0,0},
{1,1,1,1},
{0,0,0,0}
},
{
{0,1,0,0},
{0,1,0,0},
{0,1,0,0},
{0,1,0,0}
},
{
{0,0,0,0},
{0,0,0,0},
{1,1,1,1},
{0,0,0,0}
}
},
{ // L자형 블럭
{
{0,0,0,0},
{1,0,0,0},
{1,1,1,0},
{0,0,0,0}
},
{
{0,0,0,0},
{1,1,0,0},
{1,0,0,0},
{1,0,0,0}
},
{
{0,0,0,0},
{1,1,1,0},
{0,0,1,0},
{0,0,0,0}
},
{
{0,1,0,0},
{0,1,0,0},
{1,1,0,0},
{0,0,0,0}
}
},
{ // L자형 블럭 반대
{
{0,0,0,0},
{0,0,1,0},
{1,1,1,0},
{0,0,0,0}
},
{
{1,0,0,0},
{1,0,0,0},
{1,1,0,0},
{0,0,0,0}
},
{
{0,0,0,0},
{1,1,1,0},
{1,0,0,0},
{0,0,0,0}
},
{
{0,0,0,0},
{1,1,0,0},
{0,1,0,0},
{0,1,0,0}
}
},
{ // 네모 블럭
{
{0,0,0,0},
{0,1,1,0},
{0,1,1,0},
{0,0,0,0}
},
{
{0,0,0,0},
{0,1,1,0},
{0,1,1,0},
{0,0,0,0}
},
{
{0,0,0,0},
{0,1,1,0},
{0,1,1,0},
{0,0,0,0}
},
{
{0,0,0,0},
{0,1,1,0},
{0,1,1,0},
{0,0,0,0}
}
}
};

int space[20 + 1][10 + 2] = { // 세로 20+1(아래벽)칸, 가로 10+2(양쪽 벽)칸
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1}
};

void Init(); //게임 초기화 함수
void gotoxy(int x, int y); //커서 x y 값 함수
void CreateRandomForm(); //덤 블록 함수
bool CheckCrash(int x, int y); //블록이 충돌하는지 확인하는 함수
void DropBlock(); //블록 하강 함수
void BlockToGround(); //블록이 바닥에 닿았을 때, 블록을 고정시키는 함수
void RemoveLine(); //한 줄이 완성되었을 때 그 줄을 제거하고 위의 줄들을 내리는 함수
void DrawMap(); //게임 맵을 그리는 함수
void DrawBlock(); //현재 블록을 그리는 함수
void InputKey(); //사용자의 입력을 처리하는 함수
void MainMenu(); // 메인 메뉴 함수 선언
void LoadHighScore();
void SaveHighScore();
void DrawNextBlock();
bool CheckGameOver();

int main() {
	MainMenu(); // 메인 메뉴 호출
	Init();
	startDropT = clock();
	CreateRandomForm();

	while (true) {
		DrawMap();
		DrawNextBlock();
		DrawBlock();
		DropBlock();
		BlockToGround();
		RemoveLine();
		InputKey();
	}
	return 0;
}

// 메인 메뉴 함수 구현
void MainMenu() {
	int choice; // 사용자 입력 저장 변수
	int difficulty = 2; // 기본 난이도 '보통' (1: 쉬움, 2: 보통, 3: 어려움)

	while (true) {
		system("cls"); // 화면 초기화
		printf("=====================================\n");
		printf(" TETRIS GAME \n");
		printf("=====================================\n");
		printf("1. 게임시작\n");
		printf("2. 게임방법\n");
		printf("3. 나가기\n");
		printf("=====================================\n");
		printf("입력해주세요: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1: // 게임 시작
			system("cls");
			// 난이도 선택
			printf("=====================================\n");
			printf(" 난이도를 선택하세요! \n");
			printf("=====================================\n");
			printf("1. 쉬움\n");
			printf("2. 보통\n");
			printf("3. 어려움\n");
			printf("=====================================\n");
			printf("입력해주세요: ");
			scanf("%d", &difficulty);

			if (difficulty == 1) {
				SpeedInterval = 50000;
				dropSpeed = 1200; // 쉬움
			}
			else if (difficulty == 2) {
				SpeedInterval = 30000;
				dropSpeed = 800; // 보통
			}
			else if (difficulty == 3) {
				SpeedInterval = 15000;
				dropSpeed = 400; // 어려움
			}
			else {
				printf("잘못된 입력입니다! 기본값(보통)으로 설정됩니다.\n");
				dropSpeed = 800; // 기본값 설정
			}
			system("cls");
			return; // 메인 메뉴 종료 후 게임 시작
		case 2: // 게임 설명 출력
			system("cls");
			printf("=====================================\n");
			printf(" 게임방법 \n");
			printf("=====================================\n");
			printf("- 왼쪽 화살표를 사용하여 왼쪽으로 이동합니다.\n");
			printf("- 오른쪽 화살표를 사용하여 오른쪽으로 이동합니다.\n");
			printf("- 아래쪽 화살표를 사용하여 더 빨리 떨어뜨립니다.\n");
			printf("- 스페이스바를 사용하여 블록을 회전합니다.\n");
			printf("- ESC를 눌러 게임을 일시 중지합니다.\n");
			printf("\n아무 키나 눌러 메뉴로 돌아갑니다....");
			_getch(); // 키 입력 대기
			break;
		case 3: // 게임 종료
			system("cls");
			printf("종료되었습니다!\n");
			exit(0); // 프로그램 종료
		default: // 잘못된 입력 처리
			system("cls");
			printf("잘못된 선택입니다! 다시 입력해주세요.\n");
			Sleep(1000); // 1초 대기 후 다시 메뉴 표시
			break;
		}
	}
}


void Init() {
	CONSOLE_CURSOR_INFO cursorInfo; //CONSOLE_CURSOR_INFO 변수
	cursorInfo.bVisible = 0; //변수.변수
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	srand(time(NULL));
	nextBlockForm = rand() % 7; // 초기화 시 다음 블록 설정
}


void gotoxy(int x, int y) {
	COORD pos; //COORD는 구조체
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //함수(함수(상수))
}

void CreateRandomForm() {
	blockForm = nextBlockForm; // 현재 블록을 다음 블록으로
	nextBlockForm = rand() % 7; // 새로운 블록 생성
	x = 8;
	y = -1;
}

bool CheckCrash(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				int t = space[i + y][j + x / 2];
				if (t == 1 || t == 2) { // 벽일 때, 블럭일 때
					return true;
				}
			}
		}
	}
	return false;
}

void DropBlock() {
	endT = clock();
	if ((endT - SpeedTime) >= SpeedInterval) {
		dropSpeed = (dropSpeed > 100) ? dropSpeed - 50 : 100; // 최소 속도 설정
		SpeedTime = endT; // 시간 갱신
	}

	if ((float)(endT - startDropT) >= dropSpeed) {
		if (CheckCrash(x, y + 1)) {
			return;
		}
		y++;
		startDropT = clock(); // 하강 시작 시간 갱신
		startGroundT = clock(); // 바닥에 닿은 시간 갱신
		system("cls");
	}
}

void LoadHighScore() {
	FILE* file = fopen("highscore.txt", "r");
	if (file != NULL) {
		fscanf(file, "%d", &highScore); // 파일에서 최고 점수를 읽어옴
		fclose(file);
	}
}

void SaveHighScore() {
	FILE* file = fopen("highscore.txt", "w");
	if (file != NULL) {
		fprintf(file, "%d점", highScore); // 파일에 최고 점수를 기록
		fclose(file);
	}
}

// 게임 오버 함수 (최고 점수 비교 및 저장 추가)
bool CheckGameOver() {
	for (int x = 1; x <= 10; x++) {
		if (space[1][x] != 0) {
			if (score > highScore) {
				highScore = score; // 새로운 최고 점수로 업데이트
				SaveHighScore(); // 파일에 최고 점수 저장
			}
			system("cls");
			gotoxy(10, 8);
			printf("Game Over!\n");
			printf("Your Score: %d\n", score);
			printf("High Score: %d\n", highScore);
			exit(0); // 프로그램 종료
		}
	}
	return false;
}

void BlockToGround() {
	if (CheckCrash(x, y + 1)) {
		if ((float)(endT - startGroundT) > 1500) {
			// 현재 블록 저장
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[blockForm][blockRotation][i][j] == 1) {
						space[i + y][j + x / 2] = 2;
					}
				}
			}

			if (CheckGameOver()) {
				system("cls");
				gotoxy(10, 8); // 게임 화면 중앙에 메시지 출력
				printf("Game Over!\n");
				exit(0); // 프로그램 종료
			}
			// 새로운 블록 생성
			x = 8;
			y = 0;
			CreateRandomForm();
		}
	}
}

void RemoveLine() {
	for (int i = 20; i >= 0; i--) { // 벽라인 제외한 값
		int cnt = 0;
		for (int j = 1; j < 11; j++) {
			if (space[i][j] == 2) {
				cnt++;
			}
		}
		if (cnt >= 10) { // 벽돌이 다 차있다면
			score += 100; // 줄이 하나 지워질 때마다 100점 증가
			for (int j = 0; i - j >= 0; j++) {
				for (int x = 1; x < 11; x++) {
					if (i - j - 1 >= 0)
						space[i - j][x] = space[i - j - 1][x];
					else // 천장이면 0저장
						space[i - j][x] = 0;
				}
			}
		}
	}
}

void DrawMap() {
	gotoxy(0, 0);
	for (int i = 0; i < 21; i++) {
		for (int j = 0; j < 12; j++) {
			if (space[i][j] == 1) {
				gotoxy(j * 2, i);
				printf("□");
			}
			else if (space[i][j] == 2) {
				gotoxy(j * 2, i);
				printf("■");
			}
		}
	}

	// 점수를 화면에 표시
	gotoxy(24, 19); // 화면의 첫 번째 줄에 점수 출력
	printf("Score: %d", score); // 현재 점수 출력
}

void DrawNextBlock() {
	int color = blockColors[nextBlockForm];
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

	gotoxy(25, 2); // 미리보기 블록 위치
	printf("Next Block:");

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(25 + j * 2, 4 + i);
			if (block[nextBlockForm][0][i][j] == 1) {
				printf("■");
			}
			else {
				printf(" ");
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
}


void DrawBlock() {
	int color = blockColors[blockForm];
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				gotoxy(x + j * 2, y + i);
				printf("■");
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
}

void InputKey() {
	if (_kbhit()) {
		key = _getch();
		switch (key) {
		case 27: // esc
			system("PAUSE");
			break;
		case 32: // space
		{
			int oldRotation = blockRotation;
			blockRotation = (blockRotation + 1) % 4;
			if (CheckCrash(x, y)) {
				if (CheckCrash(x - 2, y) == false) {
					x -= 2;
				}
				else if (CheckCrash(x + 2, y) == false) {
					x += 2;
				}
				else {
					blockRotation = oldRotation;
				}
			}
			startGroundT = clock();
		}
		break;
		case 75: // left
			if (CheckCrash(x - 2, y) == false) {
				x -= 2;
				startGroundT = clock();
			}
			break;
		case 77: // right
			if (CheckCrash(x + 2, y) == false) {
				x += 2;
				startGroundT = clock();
			}
			break;
		case 80: // down
			if (CheckCrash(x, y + 1) == false)
				y++;
			break;
		}
		system("cls");
	}
}
