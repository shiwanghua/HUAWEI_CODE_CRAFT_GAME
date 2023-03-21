#include "scheduler.h"

namespace hwccg {
Scheduler::Scheduler() {}

Scheduler::~Scheduler() {}

bool Scheduler::Init() {
  char line[256];
  int32_t robot_num = 0;
  for (int32_t r = 0; r < ROW_NUM; r++) {
    fgets(line, sizeof line, stdin);
    for (int32_t c = 0; c < COLUMN_NUM; c++) {
      if (line[r][c] == '.') continue;
      if (line[r][c] == 'A') {
        m_robot[robot_num].x =
            c * MAP_LENGTH / COLUMN_NUM + MAP_LENGTH / COLUMN_NUM / 2;
        m_robot[robot_num].y = ;
      }
    }
  }
  fgets(line, sizeof line, stdin);
  return line[0] == 'O';
}

}  // namespace hwccg
