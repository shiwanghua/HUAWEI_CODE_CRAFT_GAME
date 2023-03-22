#include "scheduler.h"

namespace hwccg {
Scheduler::Scheduler()
    : GRID_HEIGHT(MAP_HEIGHT / ROW_NUM),
      GRID_LENGTH(MAP_LENGTH / COLUMN_NUM),
      HALF_GRID_HEIGHT(MAP_HEIGHT / ROW_NUM / 2.0),
      HALF_GRID_LENGTH(MAP_LENGTH / COLUMN_NUM / 2.0) {}

Scheduler::~Scheduler() {}

bool Scheduler::Init() {
  char line[256];
  int32_t robot_num = 0;
  for (int32_t r = ROW_NUM - 1; r >= 0; r--) {
    fgets(line, sizeof line, stdin);
    for (int32_t c = 0; c < COLUMN_NUM; c++) {
      if (line[c] == '.')
        continue;
      else if (line[c] == 'A') {
        m_robot[robot_num].curr_workbench_id = -1;
        m_robot[robot_num].product_type = 0;
        m_robot[robot_num].doing_task = 0;
        m_robot[robot_num].target_workbench_id = -1;
        m_robot[robot_num].time_value = 0;
        m_robot[robot_num].collide_value = 0;
        m_robot[robot_num].angular_speed = 0;
        m_robot[robot_num].x_line_speed = 0;
        m_robot[robot_num].y_line_speed = 0;
        m_robot[robot_num].direction = 100;  // 未知朝向
        m_robot[robot_num].x = c * GRID_LENGTH + HALF_GRID_LENGTH;
        m_robot[robot_num].y = r * GRID_HEIGHT + HALF_GRID_HEIGHT;
        robot_num++;
      } else if (isdigit(line[c])) {
        m_workbench[m_workbench_num].type = line[c] - '0';
        m_workbench[m_workbench_num].x = c * GRID_LENGTH + HALF_GRID_LENGTH;
        m_workbench[m_workbench_num].y = r * GRID_HEIGHT + HALF_GRID_HEIGHT;
        m_workbench[m_workbench_num].left_frame_num = -1;
        m_workbench[m_workbench_num].raw_material = 0;
        m_workbench[m_workbench_num].has_product = 0;
        m_workbench_num++;
      } else {
        std::cerr << "error map.\n";
        return false;
      }
    }
  }
  fgets(line, sizeof line, stdin);
  return line[0] == 'O';  // OK
}

void Scheduler::ProcessAFrame() {
  int32_t curr_frame_no = 0, workbench_num = 0;
  std::cin >> curr_frame_no >> m_curr_money >> workbench_num;
  if (workbench_num != m_workbench_num) {
    std::cerr << "workbench_num != m_workbench_num." << std::endl;
  }

  for (int32_t wi = 0; wi < m_workbench_num; wi++) {
    cin >> m_workbench[wi].type >> m_workbench[wi].x >> m_workbench[wi].y >>
        m_workbench[wi].left_frame_num >> m_workbench[wi].raw_material >>
        m_workbench[wi].has_product;
  }

  for (int32_t ri = 0; ri < m_workbench_num; ri++) {
    std::cin >> m_robot[ri].curr_workbench_id >> m_robot[ri].product_type >>
        m_robot[ri].time_value >> m_robot[ri].collide_value >>
        m_robot[ri].angular_speed >> m_robot[ri].x_line_speed >>
        m_robot[ri].y_line_speed >> m_robot[ri].x >> m_robot[ri].y;
    if (m_robot[ri].doing_task) {
      if (m_robot[ri].curr_workbench_id == m_robot[ri].target_workbench_id) {
        if (m_robot) }
    }
  }
  char line[256];
  fgets(line, sizeof line, stdin);
  if (line[0] != 'O') {
    std::cerr << "last line: " << line << std::endl;
  }
}
}  // namespace hwccg
