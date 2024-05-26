#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include <vector>

#define DINO_BOTTOM_Y 12
#define TREE_BOTTOM_Y 20
#define TREE_BOTTOM_X 45

class Dinosaur {
 private:
  int y;
  bool jumping;
  bool bottom;

 public:
  Dinosaur() {
    y = DINO_BOTTOM_Y;
    jumping = false;
    bottom = true;
  }

  void Jump() {
    if (!jumping && bottom) {
      jumping = true;
      bottom = false;
    }
  }

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

  int GetY() const { return y; }
  bool IsJumping() const { return jumping; }
};

void SetConsoleView() {
  system("mode con:cols=100 lines=25");
  system("title Google Dinosaurs. By BlockDMask.");
}

void GotoXY(int x, int y) {
  COORD Pos;
  Pos.X = 2 * x;
  Pos.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int GetKeyDown() {
  if (_kbhit() != 0) {
    return _getch();
  }
  return 0;
}

class Obstacle {
 private:
  int x;
  int type;

 public:
  Obstacle(int startX) {
    x = startX;
    type = rand() % 3;  // 장애물 타입을 랜덤으로 설정
  }

  void UpdatePosition(int speed) { x -= speed; }

  int GetX() const { return x; }
  int GetType() const { return type; }

  void Draw() const {
    GotoXY(x, TREE_BOTTOM_Y);
    switch (type) {
      case 0:
        printf("$$$$");
        GotoXY(x, TREE_BOTTOM_Y + 1);
        printf(" $$ ");
        GotoXY(x, TREE_BOTTOM_Y + 2);
        printf(" $$ ");
        GotoXY(x, TREE_BOTTOM_Y + 3);
        printf(" $$ ");
        GotoXY(x, TREE_BOTTOM_Y + 4);
        printf(" $$ ");
        break;
      case 1:
        printf("  $$  ");
        GotoXY(x, TREE_BOTTOM_Y + 1);
        printf(" $$$$ ");
        GotoXY(x, TREE_BOTTOM_Y + 2);
        printf(" $$$$ ");
        GotoXY(x, TREE_BOTTOM_Y + 3);
        printf(" $$$$ ");
        GotoXY(x, TREE_BOTTOM_Y + 4);
        printf(" $$$$ ");
        break;
      case 2:
        printf("$$$$$$$$");
        GotoXY(x, TREE_BOTTOM_Y + 1);
        printf("   $$   ");
        GotoXY(x, TREE_BOTTOM_Y + 2);
        printf("   $$   ");
        GotoXY(x, TREE_BOTTOM_Y + 3);
        printf("   $$   ");
        GotoXY(x, TREE_BOTTOM_Y + 4);
        printf("   $$   ");
        break;
    }
  }
};

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

bool isCollision(const Obstacle& obstacle, const int dinoY) {
  int treeX = obstacle.GetX();
  GotoXY(0, 0);
  printf("treeX : %d, dinoY : %d", treeX, dinoY);
  if (treeX <= 8 && treeX >= 4 && dinoY > 8) {
    return true;
  }
  return false;
}

int main() {
  SetConsoleView();

  while (true) {  // 전체 게임 루프
    Dinosaur dino;
    std::vector<Obstacle> obstacles;
    int score = 0;
    int level = 1;
    int treeSpeed = 2;
    clock_t start, curr;
    start = clock();

    while (true) {  // 게임 플레이 루프
      // 장애물과의 충돌을 체크하여 게임 오버 조건을 확인
      if (!obstacles.empty() && isCollision(obstacles.front(), dino.GetY())) {
        break;
      }

      // 'z' 키를 누르면 공룡이 점프
      if (GetKeyDown() == 'z') {
        dino.Jump();
      }

      dino.UpdatePosition();

      // 새로운 장애물을 추가
      if (obstacles.empty() || obstacles.back().GetX() < TREE_BOTTOM_X - 20) {
        obstacles.push_back(Obstacle(TREE_BOTTOM_X));
      }

      // 장애물 위치 업데이트
      for (auto& obstacle : obstacles) {
        obstacle.UpdatePosition(treeSpeed);
      }

      // 화면 밖으로 나간 장애물을 제거하고 점수를 증가
      if (!obstacles.empty() && obstacles.front().GetX() < 0) {
        obstacles.erase(obstacles.begin());
        score++;
        // 점수가 10의 배수일 때 속도 증가
        if (score % 10 == 0) {
          treeSpeed++;
					level++;
        }
      }

      // 공룡과 장애물을 그리기
      DrawDino(dino.GetY());
      for (const auto& obstacle : obstacles) {
        obstacle.Draw();
      }

      // 시간 경과에 따른 게임 업데이트
      curr = clock();
      if (((curr - start) / CLOCKS_PER_SEC) >= 1) {
        start = clock();
      }
      Sleep(60);
      system("cls");

      // 현재 점수를 출력
      GotoXY(22, 0);
      printf("Score : %d ", score);
			printf("  Level : %d ", level );
    }

    // 게임 오버 화면 표시
    DrawGameOver(score);

  }

  return 0;
}
