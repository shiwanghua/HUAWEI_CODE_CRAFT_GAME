
#include "scheduler.h"

bool readUntilOK() {
  char line[256];
  while (fgets(line, sizeof line, stdin)) {
    if (line[0] == 'O' && line[1] == 'K') {
      return true;
    }
    // do something
  }
  return false;
}

#pragma pack(4)
union WorkBench {
  struct {
    int8_t t : 4;
    int8_t a;
    int16_t b;
  };
  int32_t eq;
} w1;

int main() {
  w1.t = 15;
  std::cout << "t= " << (int32_t)w1.t << std::endl;

  // readUntilOK();
  // puts("OK");
  // fflush(stdout);
  // int frameID;
  // while (scanf("%d", &frameID) != EOF) {
  //   readUntilOK();
  //   printf("%d\n", frameID);
  //   int lineSpeed = 3;
  //   double angleSpeed = 1.5;
  //   for (int robotId = 0; robotId < 4; robotId++) {
  //     printf("forward %d %d\n", robotId, lineSpeed);
  //     printf("rotate %d %f\n", robotId, angleSpeed);
  //   }
  //   printf("OK\n", frameID);
  //   fflush(stdout);
  // }
  return 0;
}
