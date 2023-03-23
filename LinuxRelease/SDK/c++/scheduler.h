
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

namespace hwccg {

// #define M_PI 3.14159265358979323846

#pragma pack(push, 4)
struct WorkBench {
  int8_t has_product;      // 工作台是否有产品
  int8_t type;             // 工作台的类型
  int16_t left_frame_num;  // 剩余生产时间
  float x;                 // 坐标
  float y;
  int16_t raw_material;      // 已经有哪些原材料
  int16_t pending_material;  // 有哪些原材料正在等待机器人送货
};

enum RobotState : int8_t {
  NoTask,  // 目前没有任务（不需要等待停下来，立刻开始分配任务）
  OnTaskWait,  // 分配了一个新任务，还在等停下来（上一轮任务还没停下来）
  TurnBuy,  // 前面速度已完全停下，开始旋转到买工作台的方向
  GoToBuy,       // 前面已旋转到正确方向，前往买工作台的路上
  WaitAfterBuy,  // 到达买工作台，等待速度为 0
  TurnSell,  // 前面速度已完全停下，开始旋转到卖工作台的方向
  GoToSell  // 前面已旋转到正确方向，前往卖工作台的路上
            // 到达后就卖，卖成功就转为 NoTask
};

struct Robot {
  int8_t curr_workbench_id;  // 当前所处工作台id -1 ~ n-1
  int8_t product_type;       // 已携带的物品类型，0 ~ 7

  // 是否正在执行任务, 0 表示没有执行任务
  // 1 表示在买东西的路上，2 表示刚买到东西，等待速度为 0，3 表示在卖东西的路上
  int8_t state;
  int8_t transport_type;     // 要运送的产品的类型，0 ~ 7
  int8_t buy_workbench_id;   // 买的目的地，0 ~ n-1
  int8_t sell_workbench_id;  // 卖的目的地，0 ~ n-1

  double time_value;     // 时间价值系数
  double collide_value;  // 碰撞价值系数
  double angular_speed;  // 角速度
  double x_line_speed;   // 横向线速度, m/s
  double y_line_speed;   // 纵向线速度, m/s
  double direction;      // 朝向，[-pi,pi]
  float x;               // 坐标
  float y;
};

// 地图属性
constexpr float MAP_HEIGHT = 50;           // 高度，米
constexpr float MAP_LENGTH = 50;           // x方向长度，米
constexpr int32_t ROW_NUM = 100;           // 行数
constexpr int32_t COLUMN_NUM = 100;        // 列数
constexpr int32_t MAX_WORKBENCH_NUM = 50;  // 最大工作台数

// 机器人属性
constexpr int32_t ROBOT_NUM = 4;               // 机器人数
constexpr double DENSITY = 20;                 // kg/m^2
constexpr double RADIUS_IDLE = 0.45;           // 空载半径
constexpr double RADIUS_LOAD = 0.53;           // 满载半径
constexpr double MAX_FORWARD_VELOCITY = 6.0;   // 最大前进速度 m/s
constexpr double MIN_BACKWARD_VELOCITY = 2.0;  // 最大后退速度 m/s
constexpr double MAX_ROTATE_VELOCITY = M_PI;   // 最大旋转速度 /s
constexpr double MAX_TRACTIVE_FORCE = 250;     // 最大牵引力 N
constexpr double MAX_MOMENT = 250;             // 最大力矩 N*m
constexpr double ARRIVE_DIS = 0.4;  // 小于该距离即到达工作台

constexpr double STOP_DIS = ARRIVE_DIS / 2;  // 离工作台中心多远停下 0.2m
constexpr double MASS_IDLE =
    DENSITY * M_PI * RADIUS_IDLE * RADIUS_IDLE;  // 空载质量，12.72 kg
constexpr double MASS_LOAD =
    DENSITY * M_PI * RADIUS_LOAD * RADIUS_LOAD;  // 满载质量，17.65 kg
constexpr double ACCELERATED_SPEED_IDLE =
    MAX_TRACTIVE_FORCE / MASS_IDLE;  // 空载加速度 19.65 m/s^2
constexpr double ACCELERATED_SPEED_LOAD =
    MAX_TRACTIVE_FORCE / MASS_LOAD;  // 满载加速度 14.16 m/s^2
// 空载从最大速度减为 0 的滑行距离 0.916m
constexpr double DECELERATION_DIS_IDLE =
    MAX_FORWARD_VELOCITY * MAX_FORWARD_VELOCITY / 2.0 / ACCELERATED_SPEED_IDLE;
// 满载从最大速度减为 0 的滑行距离 1.271m
constexpr double DECELERATION_DIS_LOAD =
    MAX_FORWARD_VELOCITY * MAX_FORWARD_VELOCITY / 2.0 / ACCELERATED_SPEED_LOAD;

// 物品属性
constexpr int32_t BUY_PRICE[8] = {0,     3000,  4400,  5800,
                                  15400, 17200, 19200, 76000};
constexpr int32_t SELL_PRICE[8] = {0, 6000, 7600, 9200, 22500, 27500, 105000};
constexpr int32_t PROFIT_AMOUNT[8] = {0,    3000, 3200, 3400,
                                      7100, 7800, 8300, 29000};

// 工作台属性
// 每种类型工作台的周期帧数
constexpr int32_t WORKBENCH_TYPE_CYCLE[10] = {0,   50,  50,   50, 500,
                                              500, 500, 1000, 1,  1};

// 一些自定义的阈值
// 方向偏差超过多少度就停下来旋转
constexpr double MIN_DIRECTION_DIFF = 3.0 / M_PI;

class Scheduler {
 public:
  Scheduler();
  ~Scheduler();

  bool Init();  // 读取初始数据

  void ProcessAFrame();  // 处理一帧数据

 private:
  // 游戏实时状态信息
  int32_t m_curr_money = 200000;  // 当前金钱数
  int32_t m_frame_no = 1;         // 帧序号（从 1 开始递增）
  const float GRID_HEIGHT;        // 一个网格的高度
  const float GRID_LENGTH;        // 一个网格的长度
  const float HALF_GRID_HEIGHT;   // 一个网格的一半高度
  const float HALF_GRID_LENGTH;   // 一个网格的一半长度
  int8_t m_workbench_num = 0;     // 工作台实际数量
  WorkBench m_workbench[MAX_WORKBENCH_NUM];
  std::vector<int8_t> m_workbench_type_id[10];  // 记录9种类型的工作台的id
  Robot m_robot[ROBOT_NUM];

 private:
  // 根据行号列号算出具体坐标（x,y）
  std::pair<double, double> GetPosition(int32_t rno, int32_t cno);
  // 计算时间价值系数
  double CalTimeValue();
  // 计算碰撞价值系数
  double CalCollideValue(double);

  void GoToBuyF();
  void WaitAfterBuyF();
  void TurnSellF();
};

}  // namespace hwccg

#endif  // SCHEDULER_H_