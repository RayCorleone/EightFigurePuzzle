#ifndef __WIDELYUSED_H__
#define __WIDELYUSED_H__

#include "cocos2d.h"
#include <iostream>
#include <ctime>
#define maxState 10000
#define N 3

using namespace std;

//加载错误提示
void problemLoading(const char* filename);

// 与状态列表中的n状态比较
bool isEqual(int a[N][N][maxState], int b[N][N], int n);

// 与单独某个状态比较
bool isEqual(int a[N][N], int b[N][N]);

// 简单估值函数
int evalute1(int state[N][N], int target[N][N]);

// 曼哈短距离估值函数
int evalute2(int state[N][N], int target[N][N]);

// 定位移动方块
void findBrack(int a[N][N], int x, int y);

// 移动方块
void getBrack(int a[N][N], int& x, int& y);

// 移动函数
bool move(int a[N][N], int b[N][N], int dir);

// 复制状态b到状态表
void statecpy(int a[N][N][maxState], int b[N][N], int n);

// 从状态表中取出状态
void getState(int a[N][N][maxState], int b[N][N], int n);

// 复制状态
void statecpy(int a[N][N], int b[N][N]);

// 在状态表中查找b状态位置
int checkAdd(int a[N][N][maxState], int b[N][N], int n);

// A*算法1
int Astar1(int a[N][N][maxState], int start[N][N], int target[N][N], int path[maxState]);

// A*算法2
int Astar2(int a[N][N][maxState], int start[N][N], int target[N][N], int path[maxState]);

// 预判断函数
int calDe(int a[N][N]);

// 随机初始化函数
void autoGenerate(int a[N][N]);

#endif // __WIDELYUSED_H__