#include <conio.h>    // 콘솔입력을 위한 라이브러리
#include <stdio.h>    // 표준 입출력 라이브러리
#include <stdlib.h>   // 유틸리티 함수
#include <time.h>     // 시간 함수 라이브러리
#include <windows.h>  // 윈도우 콘솔 관련 함수 라이브러리

#include <random>  // 랜덤 거리 및 장애물 타입 설정을 위한 라이브러리
#include <vector>  // 장애물 이용을 위한 라이브러리

#define DINO_BOTTOM_Y 12  // 공룡 바닥 y좌표
#define TREE_BOTTOM_Y 20  // 장애물 바닥 y좌표
#define TREE_BOTTOM_X 45  // 장애물 시작 x좌표

class Dinosaur {
 private:
  int y;         // 공룡의 y좌표
  bool jumping;  // 점프 여부
  bool bottom;   // 바닥에 있는지 여부

 public:
  // 생성자 : 공룡 초기화
  Dinosaur() {
    y = DINO_BOTTOM_Y;
    jumping = false;
    bottom = true;
  }
  // 점프 함수
  void Jump() {
    if (!jumping && bottom) {
      jumping = true;
      bottom = false;
    }
  }
  // 공룡 위치 업데이트
  void UpdatePosition() {
    const int gravity = 3;
    if (jumping) {
      y -= gravity;
    } else {
      y += gravity;
    }

		if (y >= DINO_BOTTOM_Y) {
			y = DINO_BOTTOM_Y;
			bottom = true;
		}

    if (y <= 3) {
      jumping = false;
    }
  }
  // 공룡 y좌표 반환
  int GetY() const { return y; }
  // 점프 여부 반환
  bool IsJumping() const { return jumping; }
};

// 콘솔 화면 설정
void SetConsoleView() {
  system("mode con:cols=100 lines=25");
  system("title CBNU Google Dinosaurs. By BlockDMask.");
  // 원 제작자 출처 남김
}

// 콘솔 커서 위치 설정 함수
void GotoXY(int x, int y) {
  COORD Pos;
  Pos.X = 2 * x;
  Pos.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 키 입력 확인
int GetKeyDown() {
  if (_kbhit() != 0) {
    return _getch();
  }
  return 0;
}

// 장애물 클래스 정의
class Obstacle {
 private:
  float x;
  int y;
  int type;
  bool isAirObstacle;  // 공중 장애물 여부를 판단하는 변수

 public:
  // 생성자 : 장애물 초기화
  Obstacle(int startX) {
    x = startX + (rand() % 20);  // 장애물 간의 거리를 랜덤하게 설정
    isAirObstacle =
        rand() % 2 == 0;  // 장애물이 공중에 있는지 여부를 랜덤하게 설정
    y = isAirObstacle ? TREE_BOTTOM_Y - 14
                      : TREE_BOTTOM_Y;  // 공중이나 지상에 장애물 위치 설정
    type = rand() % 4;  // 장애물 타입을 랜덤으로 설정
  }


  // 장애물 위치 업데이트 함수
  void UpdatePosition(float speed) { x -= speed; }

  // 장애물 좌표 반환
  int GetX() const { return x; }
  int GetY() const { return y; }
  // 장애물 타입 반환 >> 장애물 종류 결정
  int GetType() const { return type; }
  // 공중 장애물인지 반환
  bool IsAirObstacle() const { return isAirObstacle; }


  //장애물 그리기 함수
  void Draw() const {
    GotoXY(x, y);
    switch (type) {
      case 0:
        printf("CBNU");
        GotoXY(x, y + 1);
        printf(" $$ ");
        GotoXY(x, y + 2);
        printf(" $$ ");
        GotoXY(x, y + 3);
        printf(" $$ ");
        GotoXY(x, y + 4);
        printf(" $$ ");
        break;
      case 1:
        printf("  $$  ");
        GotoXY(x, y + 1);
        printf(" C$$$ ");
        GotoXY(x, y + 2);
        printf(" $B$$ ");
        GotoXY(x, y + 3);
        printf(" $$N$ ");
        GotoXY(x, y + 4);
        printf(" $$$U ");
        break;
      case 2:
        printf("SOFT");
        GotoXY(x, y + 1);
        printf(" $$ ");
        GotoXY(x, y + 2);
        printf(" $$ ");
        GotoXY(x, y + 3);
        printf(" $$ ");
        GotoXY(x, y + 4);
        printf(" $$ ");
        break;
      case 3:
        printf("$$$ ");
        GotoXY(x, y + 1);
        printf(" $$ ");
        GotoXY(x, y + 2);
        printf(" $$$");
        GotoXY(x, y + 3);
        printf(" $$ ");
        GotoXY(x, y + 4);
        printf("$$$ ");
    }
  }
};

// 공룡 그리기 함수
void DrawDino(int dinoY) {
  GotoXY(0, dinoY);
  static bool legFlag = true;
  printf("        $$$$$$$ \n");
  printf("       $$ $$$$$$\n");
  printf("       $$$$$$$$$\n");
  printf("$      $$$      \n");
  printf("$$     $$$$$$$  \n");
  printf("$$$   $$$$$     \n");
  printf(" $$  $$$$$$$$$$ \n");
  printf(" $$$$$$$$$$$    \n");
  printf("  $$$$$$$$$$    \n");
  printf("    $$$$$$$$    \n");
  printf("     $$$$$$     \n");
  if (legFlag) {
    printf("     $    $$$    \n");
    printf("     $$          ");
    legFlag = false;
  } else {
    printf("     $$$  $     \n");
    printf("          $$    ");
    legFlag = true;
  }
}

// 게임 오버 화면 출력 함수
void DrawGameOver(const int score) {
  system("cls");
  int x = 18;
  int y = 8;
  GotoXY(x, y);
  printf("===========================");
  GotoXY(x, y + 1);
  printf("======G A M E O V E R======");
  GotoXY(x, y + 2);
  printf("===========================");
  GotoXY(x, y + 5);
  printf("SCORE : %d", score);

	printf("\n\n\n\n\n\n\n\n\n");
	system("pause");
}

// 충돌 감지 함수
bool isCollision(const Obstacle& obstacle, const int dinoY) {
  int treeX = obstacle.GetX();
  int treeY = obstacle.GetY();
  GotoXY(0, 0);
  printf("treeX : %d, treeY : %d, dinoY : %d", treeX, treeY, dinoY); // 디버깅용

  // 충돌 범위
  if (treeX <= 8 && treeX >= 4) {
    if (obstacle.IsAirObstacle()) {
      // 공중 장애물과의 충돌 조건
      if (dinoY <= treeY + 4 && dinoY >= treeY - 4) {
        return true;
      }
    } else {
      // 지상 장애물과의 충돌 조건
      if (treeX <= 8 && treeX >= 4 && dinoY > 8) {
        return true;
      }
    }
  }
  return false;
}

int main()
{
	SetConsoleView();

  while (true) {  // 전체 게임 루프
    Dinosaur dino;
    std::vector<Obstacle> obstacles;
    int score = 0;
    int level = 1;
    float speed = 2;
    clock_t start, curr;
    start = clock();

		while (true)
		{
			if (isCollision(treeX, dino.GetY()))
				break;

			if (GetKeyDown() == 'z' && isBottom)
			{
				dino.Jump();
				isJumping = true;
				isBottom = false;
			}

			dino.UpdatePosition();

      // 새로운 장애물을 추가
      if (obstacles.empty() ||
          obstacles.back().GetX() < TREE_BOTTOM_X - 20 - (rand() % 10)) {
        obstacles.push_back(Obstacle(TREE_BOTTOM_X));
      }

      // 장애물 위치 업데이트
      for (auto& obstacle : obstacles) {
        obstacle.UpdatePosition(speed);
      }

      // 화면 밖으로 나간 장애물을 제거하고 점수를 증가
      if (!obstacles.empty() && obstacles.front().GetX() < 0) {
        obstacles.erase(obstacles.begin());
        score++;
        // 점수가 10의 배수일 때 속도 증가
        if (score % 10 == 0) {
          speed += 0.5;
          level++;
        }
      }

      // 공룡과 장애물을 그리기
      DrawDino(dino.GetY());
      for (const auto& obstacle : obstacles) {
        obstacle.Draw();
      }

			curr = clock();
			if (((curr - start) / CLOCKS_PER_SEC) >= 1)
			{
				score++;
				start = clock();
			}
			Sleep(60);
			system("cls");

      // 현재 점수를 출력
      GotoXY(22, 0);
      printf("Score : %d ", score);
      printf("  Level : %d ", level);
    }

    // 게임 오버 화면 표시
    DrawGameOver(score);
  }

	return 0;
}
