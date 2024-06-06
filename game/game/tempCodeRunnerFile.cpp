#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include <random>
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
  float x;
  int y;
  int type;
  bool isAirObstacle; // 공중 장애물 여부를 판단하는 변수

 public:
  Obstacle(int startX) {
    x = startX + (rand() % 20); // 장애물 간의 거리를 랜덤하게 설정
    isAirObstacle = rand() % 2 == 0; // 장애물이 공중에 있는지 여부를 랜덤하게 설정
    y = isAirObstacle ? TREE_BOTTOM_Y - 8 : TREE_BOTTOM_Y; // 공중이나 지상에 장애물 위치 설정
    type = rand() % 3;  // 장애물 타입을 랜덤으로 설정
  }

  void UpdatePosition(float speed) { x -= speed; }

  int GetX() const { return x; }
  int GetY() const { return y; }
  int GetType() const { return type; }
  bool IsAirObstacle() const { return isAirObstacle; }

  void Draw() const {
    GotoXY(x, y);
    switch (type) {
      case 0:
        printf("$$$$");
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
        printf(" $$$$ ");
        GotoXY(x, y + 2);
        printf(" $$$$ ");
        GotoXY(x, y + 3);
        printf(" $$$$ ");
        GotoXY(x, y + 4);
        printf(" $$$$ ");
        break;
      case 2:
        printf("$$$$$$$$");
        GotoXY(x, y + 1);
        printf("   $$   ");
        GotoXY(x, y + 2);
        printf("   $$   ");
        GotoXY(x, y + 3);
        printf("   $$   ");
        GotoXY(x, y + 4);
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
    int treeY = obstacle.GetY();
    GotoXY(0, 0);
    printf("treeX : %d, treeY : %d, dinoY : %d", treeX, treeY, dinoY);

    // 충돌 범위 수정: 공룡의 이미지 크기에 맞춰 조정
    if (treeX <= 8 && treeX >= 4) {
        if (obstacle.IsAirObstacle()) {
            // 공중 장애물과의 충돌 조건
            if (dinoY <= treeY + 4 && dinoY >= treeY - 4) {
                return true;
            }
        } else {
            // 지상 장애물과의 충돌 조건
            if (dinoY >= DINO_BOTTOM_Y - 6) {
                return true;
            }
        }
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
    float speed = 2;
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
      if (obstacles.empty() || obstacles.back().GetX() < TREE_BOTTOM_X - 20 - (rand() % 10)) {
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
      printf("  Level : %d ", level);
    }

    // 게임 오버 화면 표시
    DrawGameOver(score);
  }

  return 0;
}
