
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

// 地图属性
constexpr double MAP_WIDTH = 50;           // 米
constexpr double MAP_LENGTH = 50;          // 米
constexpr int32_t ROW_NUM = 100;           // 行数
constexpr int32_t COLUMN_NUM = 100;        // 列数
constexpr int32_t MAX_WORKBENCH_NUM = 50;  // 最大工作台数

// 机器人属性
constexpr int32_t ROBOT_NUM = 4;      // 机器人数
constexpr double DENSITY = 20;        // kg/m^2
constexpr double RADIUS_IDLE = 0.45;  // 空载半径
constexpr double MASS_IDLE =
    DENSITY * M_PI * RADIUS_IDLE * RADIUS_IDLE;  // 空载质量，kg
constexpr double RADIUS_LOAD = 0.53;             // 满载半径
constexpr double MASS_LOAD =
    DENSITY * M_PI * RADIUS_LOAD * RADIUS_LOAD;  // 满载质量，kg
constexpr double MAX_FORWARD_VELOCITY = 6.0;     // 最大前进速度 m/s
constexpr double MIN_BACKWARD_VELOCITY = 2.0;    // 最大后退速度 m/s
constexpr double MAX_ROTATE_VELOCITY = M_PI;     // 最大旋转速度 /s
constexpr double MAX_TRACTIVE_FORCE = 250;       // 最大牵引力 N
constexpr double MAX_MOMENT = 250;               // 最大力矩 N*m
constexpr double ARRIVE_DIS = 0.4;  // 小于该距离即到达工作台

// 物品属性
constexpr int32_t BUY_PRICE[8] = {0,     3000,  4400,  5800,
                                  15400, 17200, 19200, 76000};
constexpr int32_t SELL_PRICE[8] = {0, 6000, 7600, 9200, 22500, 27500, 105000};
constexpr int32_t PROFIT_AMOUNT[8] = {0,    3000, 3200, 3400,
                                      7100, 7800, 8300, 29000};

// 工作台属性
// 每种类型工作台的周期帧数
constexpr int32_t WORKBENCH_TYPE_CYCLE[10] = {0,   50,  50,   50, 500,
                                              500, 500, 1000, 1,  1}

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

  // 工作台信息
  int32_t m_workbench_num = 0;                  // 工作台实际数量
  int32_t m_workbench_type[MAX_WORKBENCH_NUM];  // 每个工作台的类型
  int32_t m_workbench_cycle[MAX_WORKBENCH_NUM];  // 每个工作台的周期帧数
  double m_workbench_pos[MAX_WORKBENCH_NUM]
                        [2];  // 记录每个工作台的中心坐标 (x,y)
  int32_t m_left_frame[MAX_WORKBENCH_NUM];  // 每个工作台的剩余生产时间
  unordered_set<int32_t>
      m_workbench_raw_material[MAX_WORKBENCH_NUM];  // 每个工作台有哪些原材料
  bool m_workbench_has_product[MAX_WORKBENCH_NUM];  // 记录每个工作台是否有产品

 private:
  // 根据行号列号算出具体坐标（x,y）
  std::pair<double, double> GetPosition(int32_t rno, int32_t cno);
  // 计算时间价值系数
  double CalTimeValue();
  // 计算碰撞价值系数
  double CalCollideValue(double);
};

}  // namespace hwccg

#endif  // SCHEDULER_H_