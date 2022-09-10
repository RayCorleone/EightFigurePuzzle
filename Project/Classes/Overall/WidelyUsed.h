#ifndef __WIDELYUSED_H__
#define __WIDELYUSED_H__

#include "cocos2d.h"
#include <iostream>
#include <ctime>
#define maxState 10000
#define N 3

using namespace std;

//���ش�����ʾ
void problemLoading(const char* filename);

// ��״̬�б��е�n״̬�Ƚ�
bool isEqual(int a[N][N][maxState], int b[N][N], int n);

// �뵥��ĳ��״̬�Ƚ�
bool isEqual(int a[N][N], int b[N][N]);

// �򵥹�ֵ����
int evalute1(int state[N][N], int target[N][N]);

// �����̾����ֵ����
int evalute2(int state[N][N], int target[N][N]);

// ��λ�ƶ�����
void findBrack(int a[N][N], int x, int y);

// �ƶ�����
void getBrack(int a[N][N], int& x, int& y);

// �ƶ�����
bool move(int a[N][N], int b[N][N], int dir);

// ����״̬b��״̬��
void statecpy(int a[N][N][maxState], int b[N][N], int n);

// ��״̬����ȡ��״̬
void getState(int a[N][N][maxState], int b[N][N], int n);

// ����״̬
void statecpy(int a[N][N], int b[N][N]);

// ��״̬���в���b״̬λ��
int checkAdd(int a[N][N][maxState], int b[N][N], int n);

// A*�㷨1
int Astar1(int a[N][N][maxState], int start[N][N], int target[N][N], int path[maxState]);

// A*�㷨2
int Astar2(int a[N][N][maxState], int start[N][N], int target[N][N], int path[maxState]);

// Ԥ�жϺ���
int calDe(int a[N][N]);

// �����ʼ������
void autoGenerate(int a[N][N]);

#endif // __WIDELYUSED_H__