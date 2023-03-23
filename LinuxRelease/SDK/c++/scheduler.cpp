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
        m_robot[robot_num].state = NoTask;
        m_robot[robot_num].transport_type = 0;
        m_robot[robot_num].buy_workbench_id = -1;
        m_robot[robot_num].sell_workbench_id = -1;
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

        m_workbench_type_id[m_workbench[m_workbench_num].type].push_back(
            m_workbench_num);
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
    std::cin >> m_workbench[wi].type >> m_workbench[wi].x >>
        m_workbench[wi].y >> m_workbench[wi].left_frame_num >>
        m_workbench[wi].raw_material >> m_workbench[wi].has_product;
  }

  for (int32_t ri = 0; ri < m_workbench_num; ri++) {
    Robot& robot = m_robot[ri];
    std::cin >> robot.curr_workbench_id >> robot.product_type >>
        robot.time_value >> robot.collide_value >> robot.angular_speed >>
        robot.x_line_speed >> robot.y_line_speed >> robot.x >> robot.y;
    switch (robot.state) {
      case NoTask:

        break;
      case OnTaskWait:

        break;

      case TurnBuy:

        break;
      case GoToBuy:
        // 还没到达工作台
        if (robot.curr_workbench_id != robot.buy_workbench_id) {
          // 离目的地的方向向量
          double dx = m_workbench[robot.buy_workbench_id].x - robot.x;
          double dy = m_workbench[robot.buy_workbench_id].y - robot.y;
          double distance = sqrt(dx * dx + dy * dy);  // 离目标多远
          // 求朝向弧度值
          double target_direction = asin(dy / distance);
          if (dx < 0) {  // 第2、3象限，asin无法表示
            if (dy > 0) {
              target_direction = M_PI - target_direction;
            } else {
              target_direction = -M_PI - target_direction;
            }
          }

          // 方向无偏差，直行
          if (abs(robot.direction - target_direction) <= MIN_DIRECTION_DIFF) {
            printf("rotate %d 0\n", ri);
            double line_speed = sqrt(robot.x_line_speed * robot.x_line_speed +
                                     robot.y_line_speed * robot.y_line_speed);
            distance -= STOP_DIS;  // 还要走多远就停下
            if (distance <= DECELERATION_DIS_IDLE) {  // 开始减速
              printf("forward %d 0.1\n", ri);
            } else {  // 保持最大速度
              printf("forward %d 6\n", ri);
            }
          } else {  // 方向有偏差，停下来旋转
          }
        } else {  // 到达目的工作台, 一直尝试买
          if (robot.product_type != robot.transport_type) {  // 买产品
            printf("rotate %d 0\nforward %d 0", ri, ri);
            printf("buy %d\n", ri);
          } else {  // 买到了，等待直到速度为 0
            robot.state = WaitAfterBuy;
            if (robot.angular_speed || robot.x_line_speed ||
                robot.y_line_speed) {
              printf("rotate %d 0\nforward %d 0", ri, ri);
            } else {  // 完全停止，开始去卖工作台的路上
              robot.state = TurnSell;
            }
          }
        }
        break;

      case WaitAfterBuy:

        break;

      case TurnSell:

        break;

      case GoToSell:

        break;

      default:
        std::cerr << "Error state." << std::endl;
    }
  }

  char line[256];
  fgets(line, sizeof line, stdin);
  if (line[0] != 'O') {
    std::cerr << "last line: " << line << std::endl;
  }
}
}  // namespace hwccg
