#include "Classes/Overall/WidelyUsed.h"
#include "cocos2d.h"

USING_NS_CC;

void problemLoading(const char* filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames.\n");
}

// ��״̬�б��е�n״̬�Ƚ�
bool isEqual(int a[N][N][maxState], int b[N][N], int n) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (a[i][j][n] != b[i][j])
                return false;
        }
    }
    return true;
}

// �뵥��ĳ��״̬�Ƚ�
bool isEqual(int a[N][N], int b[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (a[i][j] != b[i][j])
                return false;
        }
    }
    return true;
}

// ����������
int evalute1(int state[N][N], int target[N][N]) {
    int num = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            if (state[i][j] != target[i][j])
                num++;
    }
    return num;
}

// �����پ�����������
int evalute2(int state[N][N], int target[N][N]) {
    int num = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int fi = 0; fi < 3; fi++) {
                for (int fj = 0; fj < 3; fj++) {
                    if (state[i][j] == target[fi][fj]) {
                        num = num + abs(i - fi) + abs(j - fj);
                        break;
                    }
                }
            }
        }
    }
    return num;
}

// ��λ�ƶ�����
void findBrack(int a[N][N], int x, int y) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (a[i][j] == 0) {
                x = i; y = j;
                return;
            }
        }
    }
}

// �ƶ�����
void getBrack(int a[N][N], int& x, int& y) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (a[i][j] == 0) {
                x = i; y = j;
                return;
            }
        }
    }
}

// �ƶ�����
bool move(int a[N][N], int b[N][N], int dir) {
    //1 up 2 down 3 left 4 right
    int x = 0, y = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            b[i][j] = a[i][j];
            if (a[i][j] == 0) {
                x = i; y = j;
            }
        }
    }
    if (x == 0 && dir == 1)     return false;
    if (x == N - 1 && dir == 2) return false;
    if (y == 0 && dir == 3)     return false;
    if (y == N - 1 && dir == 4) return false;
    if (dir == 1) { b[x - 1][y] = 0; b[x][y] = a[x - 1][y]; }
    else if (dir == 2) { b[x + 1][y] = 0; b[x][y] = a[x + 1][y]; }
    else if (dir == 3) { b[x][y - 1] = 0; b[x][y] = a[x][y - 1]; }
    else if (dir == 4) { b[x][y + 1] = 0; b[x][y] = a[x][y + 1]; }
    else                return false;
    return true;
}

// ����״̬b��״̬��
void statecpy(int a[N][N][maxState], int b[N][N], int n) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j][n] = b[i][j];
        }
    }
}

// ��״̬����ȡ��״̬
void getState(int a[N][N][maxState], int b[N][N], int n) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            b[i][j] = a[i][j][n];
        }
    }
}

// ����״̬
void statecpy(int a[N][N], int b[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            a[i][j] = b[i][j];
    }
}

// ��״̬���в���b״̬λ��
int checkAdd(int a[N][N][maxState], int b[N][N], int n) {
    for (int i = 0; i < n; i++) {
        if (isEqual(a, b, i))
            return i;
    }
    return -1;  // û���ҵ�����״̬
}

// A*�㷨1
int Astar1(int a[N][N][maxState], int start[N][N], int target[N][N], int path[maxState]) {
    bool visited[maxState] = { false };
    int fitness[maxState] = { 0 };
    int passLen[maxState] = { 0 };
    int curpos[N][N];
    statecpy(curpos, start);
    int id = 0, Curid = 0;
    fitness[id] = evalute1(curpos, target);
    statecpy(a, start, id++);
    while (!isEqual(curpos, target)) {
        for (int i = 1; i < 5; i++) {//�������ҷ���
            int tmp[N][N] = { 0 };
            if (move(curpos, tmp, i)) {
                int state = checkAdd(a, tmp, id);
                if (state == -1) {  // ��״̬
                    path[id] = Curid;
                    passLen[id] = passLen[Curid] + 1;
                    fitness[id] = evalute1(tmp, target) + passLen[id];
                    statecpy(a, tmp, id++);
                }
                else {  // ����״̬
                    int len = passLen[Curid] + 1, fit = evalute1(tmp, target) + len;
                    if (fit < fitness[state]) {
                        path[state] = Curid;
                        passLen[state] = len;
                        fitness[state] = fit;
                        visited[state] = false;
                    }
                }
            }
        }
        visited[Curid] = true;
        //�ҵ���Ӧ����С����Ϊ��һ���������ڵ�
        int minCur = -1;
        for (int i = 0; i < id; i++)
            if (!visited[i] && (minCur == -1 || fitness[i] < fitness[minCur])) minCur = i;
        Curid = minCur;
        getState(a, curpos, Curid);
        if (id == maxState)
            return -1;
    }
    return Curid;
}

// A*�㷨2
int Astar2(int a[N][N][maxState], int start[N][N], int target[N][N], int path[maxState]) {
    bool visited[maxState] = { false };
    int fitness[maxState] = { 0 };
    int passLen[maxState] = { 0 };
    int curpos[N][N];
    statecpy(curpos, start);
    int id = 0, Curid = 0;
    fitness[id] = evalute2(curpos, target);
    statecpy(a, start, id++);
    while (!isEqual(curpos, target)) {
        for (int i = 1; i < 5; i++) {//�������ҷ���
            int tmp[N][N] = { 0 };
            if (move(curpos, tmp, i)) {
                int state = checkAdd(a, tmp, id);
                if (state == -1) {  // ��״̬
                    path[id] = Curid;
                    passLen[id] = passLen[Curid] + 1;
                    fitness[id] = evalute2(tmp, target) + passLen[id];
                    statecpy(a, tmp, id++);
                }
                else {  // ����״̬
                    int len = passLen[Curid] + 1, fit = evalute2(tmp, target) + len;
                    if (fit < fitness[state]) {
                        path[state] = Curid;
                        passLen[state] = len;
                        fitness[state] = fit;
                        visited[state] = false;
                    }
                }
            }
        }
        visited[Curid] = true;
        //�ҵ���Ӧ����С����Ϊ��һ���������ڵ�
        int minCur = -1;
        for (int i = 0; i < id; i++)
            if (!visited[i] && (minCur == -1 || fitness[i] < fitness[minCur])) minCur = i;
        Curid = minCur;
        getState(a, curpos, Curid);
        if (id == maxState)
            return -1;
    }
    return Curid;
}

// Ԥ�жϺ���
int calDe(int a[N][N]) {
    int sum = 0;
    for (int i = 0; i < N * N; i++) {
        for (int j = i + 1; j < N * N; j++) {
            int m, n, c, d;
            m = i / N; n = i % N;
            c = j / N; d = j % N;
            if (a[c][d] == 0) continue;
            if (a[m][n] > a[c][d]) sum++;
        }
    }
    return sum;
}

// �����ʼ������
void autoGenerate(int a[N][N]) {
    int maxMove = 100;
    srand((unsigned)time(NULL));
    int tmp[N][N];
    while (maxMove--) {
        int dir = rand() % 4 + 1;
        if (move(a, tmp, dir))
            statecpy(a, tmp);
    }
}