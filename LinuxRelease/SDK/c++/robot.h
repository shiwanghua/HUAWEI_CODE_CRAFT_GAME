#ifndef ROBOT_H_
#define ROBOT_H_

namespace hwccg {
struct Robot {
  int8_t workbench_id;
  int8_t product_type;
  double time_value;     // 时间价值系数
  double collide_value;  // 碰撞价值系数
  double
};
}  // namespace hwccg
#endif  // ROBOT_H_