#include "bits/stdc++.h"
#include <string>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include<string.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <vector>
#include <cstring>
#include <chrono>
#include <stdio.h>

#define MAX_RECORD_NUM 280000     //转账记录最大数量
#define  MAX_ID_NUM 200000        //ID最大数量  改为20w
#define MAX_RING_NUM 3000000      //图的可能最大环数
#define MAX_FILE_SIZE_7 1024 * 1024 * 100//70M的内存
#define MAX_FILE_SIZE_6 1024 * 1024 * 50//35M的内存
#define MAX_FILE_SIZE_5 1024 * 1024 * 30//20M的内存
#define MAX_FILE_SIZE_4 1024 * 1024 * 20//10M的内存
#define MAX_FILE_SIZE_3 1024 * 1024 * 20//10M的内存
#define MAX_DEGREE 50

using namespace std;
// 待完善    边搜索边剪枝  
// 已完善 intToChar 剪枝策略 判八环  for循环改为递归 4+3方案  切片并行

// 把某些全局变量变成局部变量  去除fill修改映射范围 修改最大ID数目为20W 修改池大小

// const string outputTxtPath = "./data/my_result.txt"; //输出路径
// const string dasasetPath = "./data/1004812/test_data.txt";
// const string dasasetPath = "./data/77409/test_data.txt";
// const string dasasetPath = "./data/38252/test_data.txt";
// const string dasasetPath = "77409_test_data.txt";
// const string outputTxtPath = "myresult.txt"; //输出路径
// const string dasasetPath = "28w_lines_test_data.txt";
// const string outputTxtPath = "myresult.txt"; //输出路径
const string dasasetPath = "/data/test_data.txt";
const string outputTxtPath = "/projects/student/result.txt"; //输出路径
int connectionRight[MAX_ID_NUM][MAX_DEGREE];  // 最大出度为50
// int connectionRightTemp[MAX_ID_NUM][MAX_DEGREE];
int connectionRight2[MAX_ID_NUM][MAX_DEGREE];
int connectionLeft[MAX_ID_NUM][MAX_DEGREE];
int connectionLeft2[MAX_ID_NUM][MAX_DEGREE];
int tempRecoderNode[MAX_ID_NUM];
int connectionRightSize[MAX_ID_NUM] = {0};
int connectionLeftSize[MAX_ID_NUM] = {0};
char numTempChar1[11];
char numTempChar2[11];
int *outDegree;
int *inDegree;
int *g_numToStrSize;
char g_numToStr[MAX_ID_NUM][11];//数字到字符串，将来变成字符串的时候直接索引   
char g_numToStrEnd[MAX_ID_NUM][11];//数字到字符串+\n，将来变成字符串的时候直接索引
int g_id2Index[MAX_ID_NUM];// id->index
int g_numOfRecord          = 0;    // 记录的数量
int g_payer[MAX_RECORD_NUM];
int g_payee[MAX_RECORD_NUM];
int g_numOfIdSet           = 0;    // 节点的数量


/*** 线程变量 ***/
int canReachable[MAX_ID_NUM][2]; // 必须是-1
int canReachable3[MAX_ID_NUM][2];
int reversalLayer3[50000];   // 50000在复赛增加数据量的情况下 可能不好用
int P2[5000][50];
int P2Size[5000];
int P3[20000][50];
int P3Size[20000];
int g_ringNum = 0;                 //已找到的环的总数
char g_content_3[MAX_FILE_SIZE_3];
char g_content_4[MAX_FILE_SIZE_4];
char g_content_5[MAX_FILE_SIZE_5];
char g_content_6[MAX_FILE_SIZE_6];
char g_content_7[MAX_FILE_SIZE_7];
int visit[MAX_ID_NUM];

char *contentPointer3;
char *contentPointer4;
char *contentPointer5;
char *contentPointer6;
char *contentPointer7;
int nodeArray[6] = {0};
int headNode;

//线程1的变量
int canReachable_1[MAX_ID_NUM][2]; // 必须是-1 //放在栈里
int canReachable3_1[MAX_ID_NUM][2];//放在栈里
int reversalLayer3_1[50000];   // 50000在复赛增加数据量的情况下 可能不好用
int P2_1[5000][50];                 
int P2Size_1[5000];                 
int P3_1[20000][50];                 
int P3Size_1[20000];                 
int g_ringNum_1 = 0;                 //线程1已找到的环的总数
// 待修改 自定义函数转char
char g_content_3_1[MAX_FILE_SIZE_3];
char g_content_4_1[MAX_FILE_SIZE_4];
char g_content_5_1[MAX_FILE_SIZE_5];
char g_content_6_1[MAX_FILE_SIZE_6];
char g_content_7_1[MAX_FILE_SIZE_7];
int visit_1[MAX_ID_NUM];            //放在栈里
char *contentPointer3_1;
char *contentPointer4_1;
char *contentPointer5_1;
char *contentPointer6_1;
char *contentPointer7_1;
int nodeArray_1[6] = {0};       //放在栈里最好
int headNode_1;

//线程2的变量
int canReachable_2[MAX_ID_NUM][2]; // 必须是-1 //放在栈里
int canReachable3_2[MAX_ID_NUM][2];//放在栈里
int reversalLayer3_2[50000];   // 50000在复赛增加数据量的情况下 可能不好用
int P2_2[5000][50];                 
int P2Size_2[5000];                 
int P3_2[20000][50];                 
int P3Size_2[20000];                 
int g_ringNum_2 = 0;                 //线程1已找到的环的总数
// 待修改 自定义函数转char
char g_content_3_2[MAX_FILE_SIZE_3];
char g_content_4_2[MAX_FILE_SIZE_4];
char g_content_5_2[MAX_FILE_SIZE_5];
char g_content_6_2[MAX_FILE_SIZE_6];
char g_content_7_2[MAX_FILE_SIZE_7];
int visit_2[MAX_ID_NUM];            //放在栈里
char *contentPointer3_2;
char *contentPointer4_2;
char *contentPointer5_2;
char *contentPointer6_2;
char *contentPointer7_2;
int nodeArray_2[6] = {0};       //放在栈里最好
int headNode_2;

//线程3的变量
int canReachable_3[MAX_ID_NUM][2]; // 必须是-1 //放在栈里
int canReachable3_3[MAX_ID_NUM][2];//放在栈里
int reversalLayer3_3[50000];   // 50000在复赛增加数据量的情况下 可能不好用
int P2_3[5000][50];                 
int P2Size_3[5000];                 
int P3_3[20000][50];                 
int P3Size_3[20000];                 
int g_ringNum_3 = 0;                 //线程1已找到的环的总数
// 待修改 自定义函数转char
char g_content_3_3[MAX_FILE_SIZE_3];
char g_content_4_3[MAX_FILE_SIZE_4];
char g_content_5_3[MAX_FILE_SIZE_5];
char g_content_6_3[MAX_FILE_SIZE_6];
char g_content_7_3[MAX_FILE_SIZE_7];
int visit_3[MAX_ID_NUM];            //放在栈里
char *contentPointer3_3;
char *contentPointer4_3;
char *contentPointer5_3;
char *contentPointer6_3;
char *contentPointer7_3;
int nodeArray_3[6] = {0};       //放在栈里最好
int headNode_3;

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_mutex_2 = PTHREAD_MUTEX_INITIALIZER;
void dfs3(int Node);
void dfs4(int Node);
void dfs5(int Node);
void dfs6(int Node);

inline void read_mmap(const string &path)
{
    int fd = open(path.c_str(), O_RDONLY);
    int buf_len = 10 * 1024 * 1024;  // 28w lines 数据 10mb肯定够用了
    char * buf = (char *) mmap(NULL, buf_len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    char *currentChar = buf;
    int currentNum = 0;
    int phase = 0;
    while(*currentChar != '\0')
    {
        //先读第一个数据
        do
        {
            currentNum = 10 * currentNum + *currentChar - '0';
            currentChar++;            
        } while (*currentChar != ',');
        if(currentNum > 50000)
        {
            currentNum = 0;
            while(*currentChar != '\n')currentChar++;
            currentChar++;
            continue;
        }
        tempRecoderNode[currentNum] = 1;
        g_payer[g_numOfRecord] = currentNum;
        currentNum = 0,currentChar++;
        //读第二个数据
        do
        {
            currentNum = 10 * currentNum + *currentChar - '0';
            currentChar++;
        } while (*currentChar != ',');
        if(currentNum > 50000)
        {
            currentNum = 0;
            while(*currentChar != '\n')currentChar++;
            currentChar++;
            continue;
        }
        g_payee[g_numOfRecord] = currentNum;
        currentNum = 0,currentChar++;
        g_numOfRecord++;
        while(*currentChar != '\n')currentChar++;
        currentChar++;
    }
    numTempChar1[10] = ',';
    numTempChar2[10] = '\n';
}



void TriadSetMapToAllVector(void)
{
    int currentId = 0;
    int currentIndexPointer = 0;
    int length = 9;
    int cnt = 0;
    for(int i=0; i<=50000; i++)
    {
        if(tempRecoderNode[i])
        {
            cnt++;
            int tempNode = i; // i是节点id
            if(tempNode ==0 )
            {
                numTempChar1[length] = '0';
                numTempChar2[length] = '0';
                length--;
            }
            else
            {
                while(tempNode)
                {
                    numTempChar1[length] = numTempChar2[length] = tempNode%10 + '0';
                    tempNode /= 10;
                    length--;
                }
            }
            int index = cnt;   // 从1开始映射
            memcpy(g_numToStr[index], numTempChar1+length+1, 10-length);
            memcpy(g_numToStrEnd[index], numTempChar2+length+1, 10-length);
            g_numToStrSize[index] = 10-length;
            g_id2Index[i] = index;  // 不是实际的节点数 只记录了有出度的节点的数目 对这些节点做映射
            length = 9;
        }  
    }
    g_numOfIdSet = cnt;
    // cout<<"total node size is "<<g_numOfIdSet<<endl;

   
    //将所有的id换成映射后的索引值
    for(int i =0;i < g_numOfRecord;i++)
    {
        int &p_temp = g_payee[i];
        if(g_id2Index[p_temp] != 0)  //  既有入度又有出度的点 才记录到构图中 
        {                                      //  但需要明白 以上剪枝只做最外围的剪枝 目前的图中仍然出度入度为0的节点
            int &payer = g_id2Index[g_payer[i]];
            int &payee = g_id2Index[p_temp];
            connectionRight[payer][outDegree[payer]++] = payee;
            inDegree[payee]++;
        }
    }
}

int q1[MAX_ID_NUM];
int qq[MAX_ID_NUM];
void prunningGraph()
{
    int front=0;
    int tail = 0;
    for(int i = 1; i < g_numOfIdSet+1; i++) {
        if(!inDegree[i]) q1[tail++] = i;  // 入度为0 
    }
    while(front < tail) {
        int &u = q1[front++];
        int &outSize =  outDegree[u];
        for(int j = 0; j < outSize; j++) {
            int &v = connectionRight[u][j];
            inDegree[v]--;
            if(!inDegree[v]) q1[tail++] = v;
        }
    }
    // 构建反向图
    for(int i = 1 ; i < g_numOfIdSet+1; i++) {
        if(inDegree[i]) {  // 如果i有入度 算到图中
            int &tempSize = outDegree[i];
            for(int j =0; j < tempSize; j++) {
                int &x = connectionRight[i][j];
                connectionLeft[x][connectionLeftSize[x]++] = i;   
                // connectionRight[i][connectionRightSize[i]++] = x;  // 注意 即便是可以直接覆盖
                connectionRightSize[i]++; // 用副本重建正向图
            }
        }
        inDegree[i] = 0;
        outDegree[i] = 0;
    }
    // 把出度为0的点清掉 为了在构造前向F4少走不必要的节点
    front=0;
    tail=0;
    for(int i=1; i<g_numOfIdSet+1; i++)
    {
        if(!connectionRightSize[i]) qq[tail++]=i;  // 记录出度为0的点
    }
    while(front < tail)
    {
        int &u = qq[front++];
        int &inSize =  connectionLeftSize[u];
        for(int j=0; j<inSize; j++)
        {
            int &v = connectionLeft[u][j];
            connectionRightSize[v]--;  // 出度减1
            if(!connectionRightSize[v]) qq[tail++]=v;
        }
    }
    // 构建新的正向图
    for(int i=1; i<g_numOfIdSet+1; i++)
    {
        if(connectionRightSize[i])  // 节点有出度
        {
            for(int j=0; j<connectionLeftSize[i]; j++)
            {
                int &x = connectionLeft[i][j];
                connectionRight2[x][outDegree[x]++] = i;
                connectionLeft2[i][inDegree[i]++] = x;
            }
        }
        // connectionRightSize 和connectionLeftSize 从此就废弃了
    }
    // 目前的图中，既没有出度为0的节点 也没有入度为0的节点  完美！！！
    // memset(inDegree,0,sizeof(int)*MAX_ID_NUM);
    for(int i = 1;i < g_numOfIdSet+1;i++)   
    {
        sort(connectionRight2[i], connectionRight2[i]+outDegree[i]);
        sort(connectionLeft2[i], connectionLeft2[i]+inDegree[i]);  // 反向图需要排序吗？
        // for(int j = 0;j<outDegree[i];j++)
        // {
        //     int &nextPoint = connectionRight2[i][j];
        //     connectionLeft2[nextPoint][inDegree[nextPoint]++] = i;
        // }
    }
}

void loadTriadIntoGraph(const string &path)
{
    read_mmap(path);
    TriadSetMapToAllVector();
    prunningGraph();   
}


void startFindRing(void)//启动寻环算法
{
    bool flag = false;
    contentPointer3 = g_content_3;
    contentPointer4 = g_content_4;
    contentPointer5 = g_content_5;
    contentPointer6 = g_content_6;
    contentPointer7 = g_content_7;
    // fill(canReachable[0], canReachable[0]+g_numOfIdSet*2, -1);
    // fill(reversalLayer3, reversalLayer3+50000, -1);
    // fill(canReachable3[0], canReachable3[0]+g_numOfIdSet*2, -1);
    while(1)
    {
        pthread_mutex_lock(&g_mutex);
        if(headNode_1 - headNode <= 1)flag=true;
        headNode++;
        pthread_mutex_unlock(&g_mutex);
        if(flag)break;
        if(outDegree[headNode] < 1 ||
        inDegree[headNode] < 1)
        {   
            continue;//跳过入度和出度为0的点
        }
        int headNodePos=0;
        int layer3Size = 0;
        for(int k=0; k<inDegree[headNode]; k++)
        {
            int &n1 = connectionLeft2[headNode][k];
            if(n1 <= headNode) continue;
            int &inSizeTemp = inDegree[n1];
            for(int j=0; j< inSizeTemp; j++)
            {
                int &n2 = connectionLeft2[n1][j];
                if(n2 <= headNode) continue;
                if(canReachable[n2][0] != headNode)
                {
                    canReachable[n2][0] = headNode;
                    canReachable[n2][1] = headNodePos;
                    P2Size[headNodePos++] = 0;
                    reversalLayer3[layer3Size++] = n2;
                }
                int add = canReachable[n2][1];
                P2[add][P2Size[add]++] = n1;
            }
        }
        int layer3Pos = 0;
        sort(reversalLayer3, reversalLayer3+layer3Size);
        for(int k=0; k<layer3Size; k++)
        {
            int &n2 = reversalLayer3[k];
            int &inSizeTemp = inDegree[n2];
            for(int i3=0; i3<inSizeTemp; i3++)
            {
                int &n3 = connectionLeft2[n2][i3];
                if(n3<= headNode) continue;
                if(canReachable3[n3][0] != headNode)
                {
                    canReachable3[n3][0] = headNode;
                    canReachable3[n3][1] = layer3Pos;
                    P3Size[layer3Pos++] = 0;
                }
                int add = canReachable3[n3][1];
                P3[add][P3Size[add]++] = n2;
            }
        }

        nodeArray[0] = headNode;
        dfs3(headNode);
    }
    // printf("线程0：找到的环的总数为：%d\n",g_ringNum);
}

void addpath3();
void addpath4();
void addpath5();
void addpath6();
void addpath7();

void dfs3(int Node)
{
    visit[Node] = 1;
    int &nodeOutSize = outDegree[Node];
    for(int i1=0; i1<nodeOutSize; i1++)
    {
        int &nodeTempID = connectionRight2[Node][i1];
        if(nodeTempID <= nodeArray[0] || visit[nodeTempID] == 1) continue;
        nodeArray[1] = nodeTempID;
        if(canReachable[nodeTempID][0] == nodeArray[0]) // addpath3
            addpath3();
        dfs4(nodeTempID);
    }
    visit[Node] = 0;
}
void dfs4(int Node)
{
    visit[Node] = 1;
    int &nodeOutSize = outDegree[Node];
    for(int i1=0; i1< nodeOutSize; i1++)
    {
        int &nodeTempID = connectionRight2[Node][i1];
        if(nodeTempID <= nodeArray[0] || visit[nodeTempID] == 1) continue;
        nodeArray[2] = nodeTempID;
        if(canReachable[nodeTempID][0] == nodeArray[0]) // addpath3
            addpath4();
        if(canReachable3[nodeTempID][0] == nodeArray[0])
        {
            visit[nodeTempID] = 1;
            addpath5();
            visit[nodeTempID] = 0;
        }       
        dfs5(nodeTempID);
    }
    visit[Node] = 0;
}
void dfs5(int Node)
{
    visit[Node] = 1;
    int &nodeOutSize = outDegree[Node];
    for(int i1=0; i1<nodeOutSize; i1++)
    {
        int &nodeTempID = connectionRight2[Node][i1];
        if(nodeTempID <= nodeArray[0] || visit[nodeTempID] == 1) continue;
        nodeArray[3] = nodeTempID;
        if(canReachable3[nodeTempID][0] == nodeArray[0])
        {
            visit[nodeTempID] = 1;
            addpath6();
            visit[nodeTempID] = 0;
        }
        dfs6(nodeTempID);
    }
    visit[Node] = 0;
}
void dfs6(int Node)
{
    visit[Node] = 1;
    int &nodeOutSize = outDegree[Node];
    for(int i1=0; i1<nodeOutSize; i1++)
    {
        int &nodeTempID = connectionRight2[Node][i1];
        if(nodeTempID <= nodeArray[0] || visit[nodeTempID] == 1) continue;
        nodeArray[4] =nodeTempID;
        if(canReachable3[nodeTempID][0] == nodeArray[0])
        {
            visit[nodeTempID] = 1;
            addpath7();
            visit[nodeTempID] = 0;
        }
    }
    visit[Node] = 0;
}


void addpath3()
{
    int addTemp = canReachable[nodeArray[1]][1];
    for(int i=0; i<P2Size[addTemp]; i++)
    {
        int &Node3 = P2[addTemp][i];
        memcpy(contentPointer3, g_numToStr[headNode], g_numToStrSize[headNode]);
        contentPointer3 += g_numToStrSize[headNode];
        memcpy(contentPointer3, g_numToStr[nodeArray[1]], g_numToStrSize[nodeArray[1]]);
        contentPointer3 += g_numToStrSize[nodeArray[1]];
        memcpy(contentPointer3, g_numToStrEnd[Node3], g_numToStrSize[Node3]);
        contentPointer3 +=  g_numToStrSize[Node3];
        g_ringNum++;
    }
}

void addpath4()
{
    int addTemp = canReachable[nodeArray[2]][1];
    for(int i=0; i<P2Size[addTemp]; i++)
    {
        int &Node4 = P2[addTemp][i];
        if(nodeArray[1] == Node4) continue;  // 8环

        memcpy(contentPointer4, g_numToStr[headNode], g_numToStrSize[headNode]);
        contentPointer4 += g_numToStrSize[headNode];
        memcpy(contentPointer4, g_numToStr[nodeArray[1]], g_numToStrSize[nodeArray[1]]);
        contentPointer4 += g_numToStrSize[nodeArray[1]];
        memcpy(contentPointer4, g_numToStr[nodeArray[2]], g_numToStrSize[nodeArray[2]]);
        contentPointer4 += g_numToStrSize[nodeArray[2]];
        memcpy(contentPointer4, g_numToStrEnd[Node4], g_numToStrSize[Node4]);
        contentPointer4 += g_numToStrSize[Node4];
        g_ringNum++;
    }
}

void addpath5()
{
    int addTemp = canReachable3[nodeArray[2]][1];
    for(int i=0; i<P3Size[addTemp]; i++)
    {
        int &Node4 = P3[addTemp][i];
        // nodeArray[3] = P3[addTemp][i];
        if(visit[Node4]) continue;
        int addTemp3 = canReachable[Node4][1];
        int &tempSize = P2Size[addTemp3];
        for(int ii=0; ii<tempSize; ii++)
        {
            int Node5 = P2[addTemp3][ii];
            if(visit[Node5]) continue;
            memcpy(contentPointer5, g_numToStr[headNode], g_numToStrSize[headNode]);
            contentPointer5 += g_numToStrSize[headNode];
            memcpy(contentPointer5, g_numToStr[nodeArray[1]], g_numToStrSize[nodeArray[1]]);
            contentPointer5 += g_numToStrSize[nodeArray[1]];
            memcpy(contentPointer5, g_numToStr[nodeArray[2]], g_numToStrSize[nodeArray[2]]);
            contentPointer5 += g_numToStrSize[nodeArray[2]];
            memcpy(contentPointer5, g_numToStr[Node4], g_numToStrSize[Node4]);
            contentPointer5 += g_numToStrSize[Node4];
            memcpy(contentPointer5, g_numToStrEnd[Node5], g_numToStrSize[Node5]);
            contentPointer5 += g_numToStrSize[Node5];
            g_ringNum++;
        }
    }
}
void addpath6()
{
    int addTemp = canReachable3[nodeArray[3]][1];
    for(int i=0; i<P3Size[addTemp]; i++)
    {
        // nodeArray[4] = P3[addTemp][i];
        int &Node5 = P3[addTemp][i];
        if(visit[Node5]) continue;
        int addTemp3 = canReachable[Node5][1];
        int &tempSize = P2Size[addTemp3];
        for(int ii=0; ii<tempSize; ii++)
        {
            int Node6 = P2[addTemp3][ii];
            if(visit[Node6]) continue;
            memcpy(contentPointer6, g_numToStr[headNode], g_numToStrSize[headNode]);
            contentPointer6 += g_numToStrSize[headNode];
        
            memcpy(contentPointer6, g_numToStr[nodeArray[1]], g_numToStrSize[nodeArray[1]]);
            contentPointer6 += g_numToStrSize[nodeArray[1]];
        
            memcpy(contentPointer6, g_numToStr[nodeArray[2]], g_numToStrSize[nodeArray[2]]);
            contentPointer6 += g_numToStrSize[nodeArray[2]];
        
            memcpy(contentPointer6, g_numToStr[nodeArray[3]], g_numToStrSize[nodeArray[3]]);
            contentPointer6 += g_numToStrSize[nodeArray[3]];
        
            memcpy(contentPointer6, g_numToStr[Node5], g_numToStrSize[Node5]);
            contentPointer6 += g_numToStrSize[Node5];
        
            memcpy(contentPointer6, g_numToStrEnd[Node6], g_numToStrSize[Node6]);
            contentPointer6 += g_numToStrSize[Node6];
            g_ringNum++;
        }
    }
}

void addpath7()
{
    int addTemp = canReachable3[nodeArray[4]][1];
    for(int i=0; i<P3Size[addTemp]; i++)
    {
        int &Node6 = P3[addTemp][i];
        if(visit[Node6]) continue;
        int addTemp3 = canReachable[Node6][1];
        int &tempSize = P2Size[addTemp3];
        for(int ii=0; ii<tempSize; ii++)
        {
            int &Node7 = P2[addTemp3][ii];
            if(visit[Node7]) continue;

            memcpy(contentPointer7, g_numToStr[headNode], g_numToStrSize[headNode]);
            contentPointer7 += g_numToStrSize[headNode];
        
            memcpy(contentPointer7, g_numToStr[nodeArray[1]], g_numToStrSize[nodeArray[1]]);
            contentPointer7 += g_numToStrSize[nodeArray[1]];
        
            memcpy(contentPointer7, g_numToStr[nodeArray[2]], g_numToStrSize[nodeArray[2]]);
            contentPointer7 += g_numToStrSize[nodeArray[2]];
        
            memcpy(contentPointer7, g_numToStr[nodeArray[3]], g_numToStrSize[nodeArray[3]]);
            contentPointer7 += g_numToStrSize[nodeArray[3]];

            memcpy(contentPointer7, g_numToStr[nodeArray[4]], g_numToStrSize[nodeArray[4]]);
            contentPointer7 += g_numToStrSize[nodeArray[4]];

            memcpy(contentPointer7, g_numToStr[Node6], g_numToStrSize[Node6]);
            contentPointer7 += g_numToStrSize[Node6];

            memcpy(contentPointer7, g_numToStrEnd[Node7], g_numToStrSize[Node7]);
            contentPointer7 += g_numToStrSize[Node7];
            g_ringNum++;
        }
    }
}

//线程1的写
void dfs3_1(int Node);
void dfs4_1(int Node);
void dfs5_1(int Node);
void dfs6_1(int Node);
void startFindRing_1(void)//启动寻环算法
{
    bool flag = false;
    //初始化第二个线程所需函数
    g_content_3_1[MAX_FILE_SIZE_3-1] = '\0';
    g_content_4_1[MAX_FILE_SIZE_4-1] = '\0';
    g_content_5_1[MAX_FILE_SIZE_5-1] = '\0';
    g_content_6_1[MAX_FILE_SIZE_6-1] = '\0';
    g_content_7_1[MAX_FILE_SIZE_7-1] = '\0';
    contentPointer3_1 = g_content_3_1 + MAX_FILE_SIZE_3 - 1;
    contentPointer4_1 = g_content_4_1 + MAX_FILE_SIZE_4 - 1;
    contentPointer5_1 = g_content_5_1 + MAX_FILE_SIZE_5 - 1;
    contentPointer6_1 = g_content_6_1 + MAX_FILE_SIZE_6 - 1;
    contentPointer7_1 = g_content_7_1 + MAX_FILE_SIZE_7 - 1;
    // fill(canReachable_1[0], canReachable_1[0]+g_numOfIdSet*2, -1);
    // fill(reversalLayer3_1, reversalLayer3_1+50000, -1);
    // fill(canReachable3_1[0], canReachable3_1[0]+g_numOfIdSet*2, -1);
    while(1)
    {
        pthread_mutex_lock(&g_mutex);
        if(headNode_1 - headNode <= 1)flag = true;
        headNode_1--;
        pthread_mutex_unlock(&g_mutex);
        if(flag)break;
        if(outDegree[headNode_1] < 1 ||
        inDegree[headNode_1] < 1)
        {   
            continue;//跳过入度和出度为0的点
        }
        int headNodePos=0;
        int layer3Size_1 = 0;
        for(int k=0; k<inDegree[headNode_1]; k++)
        {
            int &n1 = connectionLeft2[headNode_1][k];
            if(n1 <= headNode_1) continue;
            int &tempSize = inDegree[n1];
            for(int j=0; j< tempSize; j++)
            {
                int &n2 = connectionLeft2[n1][j];
                if(n2 <= headNode_1) continue;
                if(canReachable_1[n2][0] != headNode_1)
                {
                    canReachable_1[n2][0] = headNode_1;
                    canReachable_1[n2][1] = headNodePos;
                    P2Size_1[headNodePos++] = 0;
                    reversalLayer3_1[layer3Size_1++] = n2;
                }
                int add_1 = canReachable_1[n2][1];
                P2_1[add_1][P2Size_1[add_1]++] = n1;
            }
        }
        int layer3Pos = 0;
        sort(reversalLayer3_1, reversalLayer3_1+layer3Size_1);
        for(int k=0; k<layer3Size_1; k++)
        {
            int &n2 = reversalLayer3_1[k];
            int &tempSize = inDegree[n2];
            for(int i3=0; i3 < tempSize; i3++)
            {
                int &n3 = connectionLeft2[n2][i3];
                if(n3<= headNode_1) continue;
                if(canReachable3_1[n3][0] != headNode_1)
                {
                    canReachable3_1[n3][0] = headNode_1;
                    canReachable3_1[n3][1] = layer3Pos;
                    P3Size_1[layer3Pos++] = 0;
                }
                int add_1 = canReachable3_1[n3][1];
                P3_1[add_1][P3Size_1[add_1]++] = n2;
            }
        }

        nodeArray_1[0] = headNode_1;
        dfs3_1(headNode_1);
    }
    // printf("线程1：找到的环的总数为：%d\n",g_ringNum_1);
}

void addpath3_1();
void addpath4_1();
void addpath5_1();
void addpath6_1();
void addpath7_1();

void dfs3_1(int Node)
{
    visit_1[Node] = 1;
    for(int i1=outDegree[Node]-1; i1>=0; i1--)
    {
        int &nodeTemp = connectionRight2[Node][i1];
        if(nodeTemp <= nodeArray_1[0] || visit_1[nodeTemp] == 1) break;  // break?
        nodeArray_1[1] =nodeTemp;
        if(canReachable_1[nodeTemp][0] == nodeArray_1[0]) // addpath3_1
            addpath3_1();
        dfs4_1(nodeTemp);
    }
    visit_1[Node] = 0;
}
void dfs4_1(int Node)
{
    visit_1[Node] = 1;
    for(int i1=outDegree[Node]-1; i1>=0; i1--)
    {
        // nodeArray_1[2] = connectionRight2[Node][i1];
        int &nodeTemp = connectionRight2[Node][i1];
        if(nodeTemp <= nodeArray_1[0] || visit_1[nodeTemp] == 1) continue;
        nodeArray_1[2] = nodeTemp;
        if(canReachable_1[nodeTemp][0] == nodeArray_1[0]) // addpath3_1
            addpath4_1();
        if(canReachable3_1[nodeTemp][0] == nodeArray_1[0])
        {
            visit_1[nodeTemp] = 1;
            addpath5_1();
            visit_1[nodeTemp] = 0;
        }       
        dfs5_1(nodeTemp);
    }
    visit_1[Node] = 0;
}
void dfs5_1(int Node)
{
    visit_1[Node] = 1;
    for(int i1=outDegree[Node]-1; i1>=0; i1--)
    {
        // nodeArray_1[3] = connectionRight2[Node][i1];
        int &nodeTemp =  connectionRight2[Node][i1];
        if(nodeTemp <= nodeArray_1[0] || visit_1[nodeTemp] == 1) continue;
        nodeArray_1[3] = nodeTemp;
        if(canReachable3_1[nodeTemp][0] == nodeArray_1[0])
        {
            visit_1[nodeTemp] = 1;
            addpath6_1();
            visit_1[nodeTemp] = 0;
        }
        dfs6_1(nodeTemp);
    }
    visit_1[Node] = 0;
}
void dfs6_1(int Node)
{
    visit_1[Node] = 1;
    for(int i1=outDegree[Node]-1; i1>=0; i1--)
    {
        // nodeArray_1[4] = connectionRight2[Node][i1];
        int &nodeTemp =  connectionRight2[Node][i1];
        if(nodeTemp <= nodeArray_1[0] || visit_1[nodeTemp] == 1) continue;
        nodeArray_1[4] = nodeTemp;
        if(canReachable3_1[nodeTemp][0] == nodeArray_1[0])
        {
            visit_1[nodeTemp] = 1;
            addpath7_1();
            visit_1[nodeTemp] = 0;
        }
    }
    visit_1[Node] = 0;
}


void addpath3_1()
{
    int addTemp_1 = canReachable_1[nodeArray_1[1]][1];
    for(int i=P2Size_1[addTemp_1]-1; i>=0; i--)
    {
        int &Node3 = P2_1[addTemp_1][i];
        contentPointer3_1 -=  g_numToStrSize[Node3];
        memcpy(contentPointer3_1, g_numToStrEnd[Node3], g_numToStrSize[Node3]);
        contentPointer3_1 -= g_numToStrSize[nodeArray_1[1]];
        memcpy(contentPointer3_1, g_numToStr[nodeArray_1[1]], g_numToStrSize[nodeArray_1[1]]);
        contentPointer3_1 -= g_numToStrSize[headNode_1];
        memcpy(contentPointer3_1, g_numToStr[headNode_1], g_numToStrSize[headNode_1]);
        g_ringNum_1++;
    }
}

void addpath4_1()
{
    int addTemp_1 = canReachable_1[nodeArray_1[2]][1];
    for(int i=P2Size_1[addTemp_1]-1; i>=0; i--)
    {
        int &Node4 = P2_1[addTemp_1][i];
        if(nodeArray_1[1] == Node4) continue;  // 8环

        contentPointer4_1 -= g_numToStrSize[Node4];
        memcpy(contentPointer4_1, g_numToStrEnd[Node4], g_numToStrSize[Node4]);
        contentPointer4_1 -= g_numToStrSize[nodeArray_1[2]];
        memcpy(contentPointer4_1, g_numToStr[nodeArray_1[2]], g_numToStrSize[nodeArray_1[2]]);
        contentPointer4_1 -= g_numToStrSize[nodeArray_1[1]];
        memcpy(contentPointer4_1, g_numToStr[nodeArray_1[1]], g_numToStrSize[nodeArray_1[1]]);
        contentPointer4_1 -= g_numToStrSize[headNode_1];
        memcpy(contentPointer4_1, g_numToStr[headNode_1], g_numToStrSize[headNode_1]);
        g_ringNum_1++;
    }
}

void addpath5_1()
{
    int addTemp_1 = canReachable3_1[nodeArray_1[2]][1];
    for(int i=P3Size_1[addTemp_1]-1; i>=0; i--)
    {
        // nodeArray_1[3] = P3_1[addTemp_1][i];
        int &Node4 =  P3_1[addTemp_1][i];
        if(visit_1[Node4]) continue;
        int addTemp3_1 = canReachable_1[Node4][1];
        for(int ii=P2Size_1[addTemp3_1]-1; ii>=0; ii--)
        {
            int &Node5 = P2_1[addTemp3_1][ii];
            if(visit_1[Node5]) continue;
            contentPointer5_1 -= g_numToStrSize[Node5];
            memcpy(contentPointer5_1, g_numToStrEnd[Node5], g_numToStrSize[Node5]);
            contentPointer5_1 -= g_numToStrSize[Node4];
            memcpy(contentPointer5_1, g_numToStr[Node4], g_numToStrSize[Node4]);
            contentPointer5_1 -= g_numToStrSize[nodeArray_1[2]];
            memcpy(contentPointer5_1, g_numToStr[nodeArray_1[2]], g_numToStrSize[nodeArray_1[2]]);
            contentPointer5_1 -= g_numToStrSize[nodeArray_1[1]];
            memcpy(contentPointer5_1, g_numToStr[nodeArray_1[1]], g_numToStrSize[nodeArray_1[1]]);
            contentPointer5_1 -= g_numToStrSize[headNode_1];
            memcpy(contentPointer5_1, g_numToStr[headNode_1], g_numToStrSize[headNode_1]);
            g_ringNum_1++;
        }

        // visit_1[nodeArray_1[3]] = 0;
       
    }
}

void addpath6_1()
{
    int addTemp_1 = canReachable3_1[nodeArray_1[3]][1];
    for(int i=P3Size_1[addTemp_1]-1; i>=0; i--)
    {
        int &Node5 = P3_1[addTemp_1][i];
        if(visit_1[Node5]) continue;
        int addTemp3_1 = canReachable_1[Node5][1];
        for(int ii=P2Size_1[addTemp3_1]-1; ii>=0; ii--)
        {
            int &Node6 = P2_1[addTemp3_1][ii];
            if(visit_1[Node6]) continue;
            contentPointer6_1 -= g_numToStrSize[Node6];
            memcpy(contentPointer6_1, g_numToStrEnd[Node6], g_numToStrSize[Node6]);
            contentPointer6_1 -= g_numToStrSize[Node5];
            memcpy(contentPointer6_1, g_numToStr[Node5], g_numToStrSize[Node5]);
            contentPointer6_1 -= g_numToStrSize[nodeArray_1[3]];
            memcpy(contentPointer6_1, g_numToStr[nodeArray_1[3]], g_numToStrSize[nodeArray_1[3]]);
            contentPointer6_1 -= g_numToStrSize[nodeArray_1[2]];
            memcpy(contentPointer6_1, g_numToStr[nodeArray_1[2]], g_numToStrSize[nodeArray_1[2]]);
            contentPointer6_1 -= g_numToStrSize[nodeArray_1[1]];
            memcpy(contentPointer6_1, g_numToStr[nodeArray_1[1]], g_numToStrSize[nodeArray_1[1]]);
            contentPointer6_1 -= g_numToStrSize[headNode_1];
            memcpy(contentPointer6_1, g_numToStr[headNode_1], g_numToStrSize[headNode_1]);
            g_ringNum_1++;
        }
    }
}

void addpath7_1()
{
    int addTemp_1 = canReachable3_1[nodeArray_1[4]][1];
    for(int i=P3Size_1[addTemp_1]-1; i>=0; i--)
    {
        int &Node6 = P3_1[addTemp_1][i];
        if(visit_1[Node6]) continue;
        int addTemp3_1 = canReachable_1[Node6][1];
        for(int ii=P2Size_1[addTemp3_1]-1; ii>=0; ii--)
        {
            int Node7 = P2_1[addTemp3_1][ii];
            if(visit_1[Node7]) continue;

            contentPointer7_1 -= g_numToStrSize[Node7];
            memcpy(contentPointer7_1, g_numToStrEnd[Node7], g_numToStrSize[Node7]);
            contentPointer7_1 -= g_numToStrSize[Node6];
            memcpy(contentPointer7_1, g_numToStr[Node6], g_numToStrSize[Node6]);
            contentPointer7_1 -= g_numToStrSize[nodeArray_1[4]];
            memcpy(contentPointer7_1, g_numToStr[nodeArray_1[4]], g_numToStrSize[nodeArray_1[4]]);
            contentPointer7_1 -= g_numToStrSize[nodeArray_1[3]];
            memcpy(contentPointer7_1, g_numToStr[nodeArray_1[3]], g_numToStrSize[nodeArray_1[3]]);
            contentPointer7_1 -= g_numToStrSize[nodeArray_1[2]];
            memcpy(contentPointer7_1, g_numToStr[nodeArray_1[2]], g_numToStrSize[nodeArray_1[2]]);
            contentPointer7_1 -= g_numToStrSize[nodeArray_1[1]];
            memcpy(contentPointer7_1, g_numToStr[nodeArray_1[1]], g_numToStrSize[nodeArray_1[1]]);
            contentPointer7_1 -= g_numToStrSize[headNode_1];
            memcpy(contentPointer7_1, g_numToStr[headNode_1], g_numToStrSize[headNode_1]);
            
            g_ringNum_1++;
        }
    }

 
}
//线程2
void dfs3_2(int Node);
void dfs4_2(int Node);
void dfs5_2(int Node);
void dfs6_2(int Node);
void startFindRing_2(void)//启动寻环算法
{
    bool flag = false;
    //初始化第三个线程
    contentPointer3_2 = g_content_3_2;
    contentPointer4_2 = g_content_4_2;
    contentPointer5_2 = g_content_5_2;
    contentPointer6_2 = g_content_6_2;
    contentPointer7_2 = g_content_7_2;
    // fill(canReachable_2[0], canReachable_2[0]+g_numOfIdSet*2, -1);
    // fill(reversalLayer3_2, reversalLayer3_2+50000, -1);
    // fill(canReachable3_2[0], canReachable3_2[0]+g_numOfIdSet*2, -1);
    while(1)
    {
        pthread_mutex_lock(&g_mutex_2);
        if(headNode_3 - headNode_2 <= 1)flag=true;
        headNode_2++;
        pthread_mutex_unlock(&g_mutex_2);
        if(flag)break;
        if(outDegree[headNode_2] < 1 ||
        inDegree[headNode_2] < 1)
        {   
            continue;//跳过入度和出度为0的点
        }
        int headNodePos=0;
        int layer3Size_2 = 0;
        for(int k=0; k<inDegree[headNode_2]; k++)
        {
            int &n1 = connectionLeft2[headNode_2][k];
            if(n1 <= headNode_2) continue;
            int &tempSize = inDegree[n1];
            for(int j=0; j< tempSize; j++)
            {
                int &n2 = connectionLeft2[n1][j];
                if(n2 <= headNode_2) continue;
                if(canReachable_2[n2][0] != headNode_2)
                {
                    canReachable_2[n2][0] = headNode_2;
                    canReachable_2[n2][1] = headNodePos;
                    P2Size_2[headNodePos++] = 0;
                    reversalLayer3_2[layer3Size_2++] = n2;
                }
                int add_2 = canReachable_2[n2][1];
                P2_2[add_2][P2Size_2[add_2]++] = n1;
            }
        }
        int layer3Pos = 0;
        sort(reversalLayer3_2, reversalLayer3_2+layer3Size_2);
        for(int k=0; k<layer3Size_2; k++)
        {
            int &n2 = reversalLayer3_2[k];
            int &tempSize = inDegree[n2];
            for(int i3=0; i3<tempSize; i3++)
            {
                int &n3 = connectionLeft2[n2][i3];
                if(n3<= headNode_2) continue;
                if(canReachable3_2[n3][0] != headNode_2)
                {
                    canReachable3_2[n3][0] = headNode_2;
                    canReachable3_2[n3][1] = layer3Pos;
                    P3Size_2[layer3Pos++] = 0;
                }
                int add_2 = canReachable3_2[n3][1];
                P3_2[add_2][P3Size_2[add_2]++] = n2;
            }
        }

        nodeArray_2[0] = headNode_2;
        dfs3_2(headNode_2);
    }
    // printf("线程0：找到的环的总数为：%d\n",g_ringNum_2);
}

void addpath3_2();
void addpath4_2();
void addpath5_2();
void addpath6_2();
void addpath7_2();

void dfs3_2(int Node)
{
    visit_2[Node] = 1;
    int &sizeTemp = outDegree[Node];
    for(int i1=0; i1<sizeTemp; i1++)
    {
        int &nodeTemp = connectionRight2[Node][i1];
        // nodeArray_2[1] = connectionRight2[Node][i1];
        if(nodeTemp <= nodeArray_2[0] || visit_2[nodeTemp] == 1) continue;
        nodeArray_2[1] = nodeTemp;
        if(canReachable_2[nodeTemp][0] == nodeArray_2[0]) // addpath3_2
            addpath3_2();
        dfs4_2(nodeTemp);
    }
    visit_2[Node] = 0;
}
void dfs4_2(int Node)
{
    visit_2[Node] = 1;
    int &sizeTemp = outDegree[Node];
    for(int i1=0; i1<sizeTemp; i1++)
    {
        // nodeArray_2[2] = connectionRight2[Node][i1];
        int &nodeTemp = connectionRight2[Node][i1];
        if(nodeTemp <= nodeArray_2[0] || visit_2[nodeTemp] == 1) continue;
        nodeArray_2[2] = nodeTemp;
        if(canReachable_2[nodeTemp][0] == nodeArray_2[0]) // addpath3_2
            addpath4_2();
        if(canReachable3_2[nodeTemp][0] == nodeArray_2[0])
        {
            visit_2[nodeTemp] = 1;
            addpath5_2();
            visit_2[nodeTemp] = 0;
        }       
        dfs5_2(nodeTemp);
    }
    visit_2[Node] = 0;
}
void dfs5_2(int Node)
{
    visit_2[Node] = 1;
    int &tempSize = outDegree[Node];
    for(int i1=0; i1<tempSize; i1++)
    {
        // nodeArray_2[3] = connectionRight2[Node][i1];
        int &nodeTemp = connectionRight2[Node][i1];
        if(nodeTemp <= nodeArray_2[0] || visit_2[nodeTemp] == 1) continue;
        nodeArray_2[3] = nodeTemp;
        if(canReachable3_2[nodeTemp][0] == nodeArray_2[0])
        {
            visit_2[nodeTemp] = 1;
            addpath6_2();
            visit_2[nodeTemp] = 0;
        }
        dfs6_2(nodeTemp);
    }
    visit_2[Node] = 0;
}
void dfs6_2(int Node)
{
    visit_2[Node] = 1;
    int &tempSize = outDegree[Node];
    for(int i1=0; i1<tempSize; i1++)
    {
        // nodeArray_2[4] = connectionRight2[Node][i1];
        int &nodeTemp = connectionRight2[Node][i1];
        if(nodeTemp <= nodeArray_2[0] || visit_2[nodeTemp] == 1) continue;
        nodeArray_2[4] = nodeTemp;
        if(canReachable3_2[nodeTemp][0] == nodeArray_2[0])
        {
            visit_2[nodeTemp] = 1;
            addpath7_2();
            visit_2[nodeTemp] = 0;
        }
    }
    visit_2[Node] = 0;
}


void addpath3_2()
{
    int addTemp_2 = canReachable_2[nodeArray_2[1]][1];
    int &tempSize = P2Size_2[addTemp_2];
    for(int i=0; i<tempSize; i++)
    {
        int &Node3 = P2_2[addTemp_2][i];
        memcpy(contentPointer3_2, g_numToStr[headNode_2], g_numToStrSize[headNode_2]);
        contentPointer3_2 += g_numToStrSize[headNode_2];
        memcpy(contentPointer3_2, g_numToStr[nodeArray_2[1]], g_numToStrSize[nodeArray_2[1]]);
        contentPointer3_2 += g_numToStrSize[nodeArray_2[1]];
        memcpy(contentPointer3_2, g_numToStrEnd[Node3], g_numToStrSize[Node3]);
        contentPointer3_2 +=  g_numToStrSize[Node3];
        g_ringNum_2++;
    }
}

void addpath4_2()
{
    int addTemp_2 = canReachable_2[nodeArray_2[2]][1];
    int &tempSize = P2Size_2[addTemp_2];
    for(int i=0; i<tempSize; i++)
    {
        int &Node4 = P2_2[addTemp_2][i];
        if(nodeArray_2[1] == Node4) continue;  // 8环

        memcpy(contentPointer4_2, g_numToStr[headNode_2], g_numToStrSize[headNode_2]);
        contentPointer4_2 += g_numToStrSize[headNode_2];
        memcpy(contentPointer4_2, g_numToStr[nodeArray_2[1]], g_numToStrSize[nodeArray_2[1]]);
        contentPointer4_2 += g_numToStrSize[nodeArray_2[1]];
        memcpy(contentPointer4_2, g_numToStr[nodeArray_2[2]], g_numToStrSize[nodeArray_2[2]]);
        contentPointer4_2 += g_numToStrSize[nodeArray_2[2]];
        memcpy(contentPointer4_2, g_numToStrEnd[Node4], g_numToStrSize[Node4]);
        contentPointer4_2 += g_numToStrSize[Node4];
        g_ringNum_2++;
    }
}

void addpath5_2()
{
    int addTemp_2 = canReachable3_2[nodeArray_2[2]][1];
    int &tempSize = P3Size_2[addTemp_2];
    for(int i=0; i<tempSize; i++)
    {
        // nodeArray_2[3] = P3_2[addTemp_2][i];
        int &Node4 =  P3_2[addTemp_2][i];
        if(visit_2[Node4]) continue;
        int addTemp3_2 = canReachable_2[Node4][1];
        int &size3 = P2Size_2[addTemp3_2];
        for(int ii=0; ii<size3; ii++)
        {
            int &Node5 = P2_2[addTemp3_2][ii];
            if(visit_2[Node5]) continue;
            memcpy(contentPointer5_2, g_numToStr[headNode_2], g_numToStrSize[headNode_2]);
            contentPointer5_2 += g_numToStrSize[headNode_2];
            memcpy(contentPointer5_2, g_numToStr[nodeArray_2[1]], g_numToStrSize[nodeArray_2[1]]);
            contentPointer5_2 += g_numToStrSize[nodeArray_2[1]];
            memcpy(contentPointer5_2, g_numToStr[nodeArray_2[2]], g_numToStrSize[nodeArray_2[2]]);
            contentPointer5_2 += g_numToStrSize[nodeArray_2[2]];
            memcpy(contentPointer5_2, g_numToStr[Node4], g_numToStrSize[Node4]);
            contentPointer5_2 += g_numToStrSize[Node4];
            memcpy(contentPointer5_2, g_numToStrEnd[Node5], g_numToStrSize[Node5]);
            contentPointer5_2 += g_numToStrSize[Node5];
            g_ringNum_2++;
        }

        // visit_2[nodeArray_2[3]] = 0;
       
    }
}

void addpath6_2()
{
    int addTemp_2 = canReachable3_2[nodeArray_2[3]][1];
    int &size1 = P3Size_2[addTemp_2];
    for(int i=0; i<size1; i++)
    {
        // nodeArray_2[4] = P3_2[addTemp_2][i];
        int &Node5 = P3_2[addTemp_2][i];
        if(visit_2[Node5]) continue;
        int addTemp3_2 = canReachable_2[Node5][1];
        int &size2 = P2Size_2[addTemp3_2];
        for(int ii=0; ii<size2; ii++)
        {
            int &Node6 = P2_2[addTemp3_2][ii];
            if(visit_2[Node6]) continue;
            memcpy(contentPointer6_2, g_numToStr[headNode_2], g_numToStrSize[headNode_2]);
            contentPointer6_2 += g_numToStrSize[headNode_2];
        
            memcpy(contentPointer6_2, g_numToStr[nodeArray_2[1]], g_numToStrSize[nodeArray_2[1]]);
            contentPointer6_2 += g_numToStrSize[nodeArray_2[1]];
        
            memcpy(contentPointer6_2, g_numToStr[nodeArray_2[2]], g_numToStrSize[nodeArray_2[2]]);
            contentPointer6_2 += g_numToStrSize[nodeArray_2[2]];
        
            memcpy(contentPointer6_2, g_numToStr[nodeArray_2[3]], g_numToStrSize[nodeArray_2[3]]);
            contentPointer6_2 += g_numToStrSize[nodeArray_2[3]];
        
            memcpy(contentPointer6_2, g_numToStr[Node5], g_numToStrSize[Node5]);
            contentPointer6_2 += g_numToStrSize[Node5];
        
            memcpy(contentPointer6_2, g_numToStrEnd[Node6], g_numToStrSize[Node6]);
            contentPointer6_2 += g_numToStrSize[Node6];
            g_ringNum_2++;
        }
    }
}

void addpath7_2()
{
    int addTemp_2 = canReachable3_2[nodeArray_2[4]][1];
    int &size1 = P3Size_2[addTemp_2];
    for(int i=0; i<size1; i++)
    {
        // nodeArray_2[5] = P3_2[addTemp_2][i];
        int &Node6 = P3_2[addTemp_2][i];
        if(visit_2[Node6]) continue;
        int addTemp3_2 = canReachable_2[Node6][1];
        int &size2 = P2Size_2[addTemp3_2];
        for(int ii=0; ii<size2; ii++)
        {
            int &Node7 = P2_2[addTemp3_2][ii];
            if(visit_2[Node7]) continue;

            memcpy(contentPointer7_2, g_numToStr[headNode_2], g_numToStrSize[headNode_2]);
            contentPointer7_2 += g_numToStrSize[headNode_2];
        
            memcpy(contentPointer7_2, g_numToStr[nodeArray_2[1]], g_numToStrSize[nodeArray_2[1]]);
            contentPointer7_2 += g_numToStrSize[nodeArray_2[1]];
        
            memcpy(contentPointer7_2, g_numToStr[nodeArray_2[2]], g_numToStrSize[nodeArray_2[2]]);
            contentPointer7_2 += g_numToStrSize[nodeArray_2[2]];
        
            memcpy(contentPointer7_2, g_numToStr[nodeArray_2[3]], g_numToStrSize[nodeArray_2[3]]);
            contentPointer7_2 += g_numToStrSize[nodeArray_2[3]];

            memcpy(contentPointer7_2, g_numToStr[nodeArray_2[4]], g_numToStrSize[nodeArray_2[4]]);
            contentPointer7_2 += g_numToStrSize[nodeArray_2[4]];

            memcpy(contentPointer7_2, g_numToStr[Node6], g_numToStrSize[Node6]);
            contentPointer7_2 += g_numToStrSize[Node6];

            memcpy(contentPointer7_2, g_numToStrEnd[Node7], g_numToStrSize[Node7]);
            contentPointer7_2 += g_numToStrSize[Node7];
            g_ringNum_2++;
        }
    }

 
}

//线程3的写
void dfs3_3(int Node);
void dfs4_3(int Node);
void dfs5_3(int Node);
void dfs6_3(int Node);
void startFindRing_3(void)//启动寻环算法
{
    bool flag = false;
    g_content_3_3[MAX_FILE_SIZE_3-1] = '\0';
    g_content_4_3[MAX_FILE_SIZE_4-1] = '\0';
    g_content_5_3[MAX_FILE_SIZE_5-1] = '\0';
    g_content_6_3[MAX_FILE_SIZE_6-1] = '\0';
    g_content_7_3[MAX_FILE_SIZE_7-1] = '\0';
    contentPointer3_3 = g_content_3_3 + MAX_FILE_SIZE_3 - 1;
    contentPointer4_3 = g_content_4_3 + MAX_FILE_SIZE_4 - 1;
    contentPointer5_3 = g_content_5_3 + MAX_FILE_SIZE_5 - 1;
    contentPointer6_3 = g_content_6_3 + MAX_FILE_SIZE_6 - 1;
    contentPointer7_3 = g_content_7_3 + MAX_FILE_SIZE_7 - 1;
    // fill(canReachable_3[0], canReachable_3[0]+g_numOfIdSet*2, -1);
    // fill(reversalLayer3_3, reversalLayer3_3+50000, -1);
    // fill(canReachable3_3[0], canReachable3_3[0]+g_numOfIdSet*2, -1); 
    while(1)
    {
        pthread_mutex_lock(&g_mutex_2);
        if(headNode_3 - headNode_2 <= 1)flag = true;
        headNode_3--;
        pthread_mutex_unlock(&g_mutex_2);
        if(flag)break;
        if(outDegree[headNode_3] < 1 ||
        inDegree[headNode_3] < 1)
        {   
            continue;//跳过入度和出度为0的点
        }
        int headNodePos=0;
        int layer3Size_3 = 0;
        for(int k=0; k<inDegree[headNode_3]; k++)
        {
            int n1 = connectionLeft2[headNode_3][k];
            if(n1 <= headNode_3) continue;
            int &sizeTemp = inDegree[n1];
            for(int j=0; j< sizeTemp; j++)
            {
                int n2 = connectionLeft2[n1][j];
                if(n2 <= headNode_3) continue;
                if(canReachable_3[n2][0] != headNode_3)
                {
                    canReachable_3[n2][0] = headNode_3;
                    canReachable_3[n2][1] = headNodePos;
                    P2Size_3[headNodePos++] = 0;
                    reversalLayer3_3[layer3Size_3++] = n2;
                }
                int add_3 = canReachable_3[n2][1];
                P2_3[add_3][P2Size_3[add_3]++] = n1;
            }
        }
        int layer3Pos = 0;
        sort(reversalLayer3_3, reversalLayer3_3+layer3Size_3);
        for(int k=0; k<layer3Size_3; k++)
        {
            int n2 = reversalLayer3_3[k];
            int &sizeTemp = inDegree[n2];
            for(int i3=0; i3<sizeTemp; i3++)
            {
                int n3 = connectionLeft2[n2][i3];
                if(n3<= headNode_3) continue;
                if(canReachable3_3[n3][0] != headNode_3)
                {
                    canReachable3_3[n3][0] = headNode_3;
                    canReachable3_3[n3][1] = layer3Pos;
                    P3Size_3[layer3Pos++] = 0;
                }
                int add_3 = canReachable3_3[n3][1];
                P3_3[add_3][P3Size_3[add_3]++] = n2;
            }
        }

        nodeArray_3[0] = headNode_3;
        dfs3_3(headNode_3);
    }
//     printf("线程1：找到的环的总数为：%d\n",g_ringNum_3);
}

void addpath3_3();
void addpath4_3();
void addpath5_3();
void addpath6_3();
void addpath7_3();

void dfs3_3(int Node)
{
    visit_3[Node] = 1;
    for(int i1=outDegree[Node]-1; i1>=0; i1--)
    {
        // nodeArray_3[1] = connectionRight2[Node][i1];
        int &tempNode = connectionRight2[Node][i1];
        if(tempNode <= nodeArray_3[0] || visit_3[tempNode] == 1) break;
        nodeArray_3[1] = tempNode;
        if(canReachable_3[tempNode][0] == nodeArray_3[0]) // addpath3_3
            addpath3_3();
        dfs4_3(tempNode);
    }
    visit_3[Node] = 0;
}
void dfs4_3(int Node)
{
    visit_3[Node] = 1;
    for(int i1=outDegree[Node]-1; i1>=0; i1--)
    {
        int &tempNode = connectionRight2[Node][i1];
        // nodeArray_3[2] = connectionRight2[Node][i1];
        if(tempNode <= nodeArray_3[0] || visit_3[tempNode] == 1) continue;
        nodeArray_3[2] = tempNode;
        if(canReachable_3[tempNode][0] == nodeArray_3[0]) // addpath3_3
            addpath4_3();
        if(canReachable3_3[tempNode][0] == nodeArray_3[0])
        {
            visit_3[tempNode] = 1;
            addpath5_3();
            visit_3[tempNode] = 0;
        }       
        dfs5_3(tempNode);
    }
    visit_3[Node] = 0;
}
void dfs5_3(int Node)
{
    visit_3[Node] = 1;
    for(int i1=outDegree[Node]-1; i1>=0; i1--)
    {
        // nodeArray_3[3] = connectionRight2[Node][i1];
        int &tempNode = connectionRight2[Node][i1];
        if(tempNode <= nodeArray_3[0] || visit_3[tempNode] == 1) continue;
        nodeArray_3[3] = tempNode;
        if(canReachable3_3[tempNode][0] == nodeArray_3[0])
        {
            visit_3[tempNode] = 1;
            addpath6_3();
            visit_3[tempNode] = 0;
        }
        dfs6_3(tempNode);
    }
    visit_3[Node] = 0;
}
void dfs6_3(int Node)
{
    visit_3[Node] = 1;
    for(int i1=outDegree[Node]-1; i1>=0; i1--)
    {
        // nodeArray_3[4] = connectionRight2[Node][i1];
        int &tempNode = connectionRight2[Node][i1];
        if(tempNode <= nodeArray_3[0] || visit_3[tempNode] == 1) continue;
        nodeArray_3[4] = tempNode;
        if(canReachable3_3[tempNode][0] == nodeArray_3[0])
        {
            visit_3[tempNode] = 1;
            addpath7_3();
            visit_3[tempNode] = 0;
        }
    }
    visit_3[Node] = 0;
}


void addpath3_3()
{
    int addTemp_3 = canReachable_3[nodeArray_3[1]][1];
    for(int i=P2Size_3[addTemp_3]-1; i>=0; i--)
    {
        int &Node3 = P2_3[addTemp_3][i];
        contentPointer3_3 -=  g_numToStrSize[Node3];
        memcpy(contentPointer3_3, g_numToStrEnd[Node3], g_numToStrSize[Node3]);
        contentPointer3_3 -= g_numToStrSize[nodeArray_3[1]];
        memcpy(contentPointer3_3, g_numToStr[nodeArray_3[1]], g_numToStrSize[nodeArray_3[1]]);
        contentPointer3_3 -= g_numToStrSize[headNode_3];
        memcpy(contentPointer3_3, g_numToStr[headNode_3], g_numToStrSize[headNode_3]);
        g_ringNum_3++;
    }
}

void addpath4_3()
{
    int addTemp_3 = canReachable_3[nodeArray_3[2]][1];
    for(int i=P2Size_3[addTemp_3]-1; i>=0; i--)
    {
        int &Node4 = P2_3[addTemp_3][i];
        if(nodeArray_3[1] == Node4) continue;  // 8环

        contentPointer4_3 -= g_numToStrSize[Node4];
        memcpy(contentPointer4_3, g_numToStrEnd[Node4], g_numToStrSize[Node4]);
        contentPointer4_3 -= g_numToStrSize[nodeArray_3[2]];
        memcpy(contentPointer4_3, g_numToStr[nodeArray_3[2]], g_numToStrSize[nodeArray_3[2]]);
        contentPointer4_3 -= g_numToStrSize[nodeArray_3[1]];
        memcpy(contentPointer4_3, g_numToStr[nodeArray_3[1]], g_numToStrSize[nodeArray_3[1]]);
        contentPointer4_3 -= g_numToStrSize[headNode_3];
        memcpy(contentPointer4_3, g_numToStr[headNode_3], g_numToStrSize[headNode_3]);
        g_ringNum_3++;
    }
}

void addpath5_3()
{
    int addTemp_3 = canReachable3_3[nodeArray_3[2]][1];
    for(int i=P3Size_3[addTemp_3]-1; i>=0; i--)
    {
        // nodeArray_3[3] = P3_3[addTemp_3][i];
        int &Node4 = P3_3[addTemp_3][i];
        if(visit_3[Node4]) continue;
        int addTemp3_3 = canReachable_3[Node4][1];
        for(int ii=P2Size_3[addTemp3_3]-1; ii>=0; ii--)
        {
            int &Node5 = P2_3[addTemp3_3][ii];
            if(visit_3[Node5]) continue;
            contentPointer5_3 -= g_numToStrSize[Node5];
            memcpy(contentPointer5_3, g_numToStrEnd[Node5], g_numToStrSize[Node5]);
            contentPointer5_3 -= g_numToStrSize[Node4];
            memcpy(contentPointer5_3, g_numToStr[Node4], g_numToStrSize[Node4]);
            contentPointer5_3 -= g_numToStrSize[nodeArray_3[2]];
            memcpy(contentPointer5_3, g_numToStr[nodeArray_3[2]], g_numToStrSize[nodeArray_3[2]]);
            contentPointer5_3 -= g_numToStrSize[nodeArray_3[1]];
            memcpy(contentPointer5_3, g_numToStr[nodeArray_3[1]], g_numToStrSize[nodeArray_3[1]]);
            contentPointer5_3 -= g_numToStrSize[headNode_3];
            memcpy(contentPointer5_3, g_numToStr[headNode_3], g_numToStrSize[headNode_3]);
            g_ringNum_3++;
        }

        // visit_3[nodeArray_3[3]] = 0;
       
    }
}

void addpath6_3()
{
    int addTemp_3 = canReachable3_3[nodeArray_3[3]][1];
    for(int i=P3Size_3[addTemp_3]-1; i>=0; i--)
    {
        // nodeArray_3[4] = P3_3[addTemp_3][i];
        int &Node5 = P3_3[addTemp_3][i];
        if(visit_3[Node5]) continue;
        int addTemp3_3 = canReachable_3[Node5][1];
        for(int ii=P2Size_3[addTemp3_3]-1; ii>=0; ii--)
        {
            int &Node6 = P2_3[addTemp3_3][ii];
            if(visit_3[Node6]) continue;
            contentPointer6_3 -= g_numToStrSize[Node6];
            memcpy(contentPointer6_3, g_numToStrEnd[Node6], g_numToStrSize[Node6]);
            contentPointer6_3 -= g_numToStrSize[Node5];
            memcpy(contentPointer6_3, g_numToStr[Node5], g_numToStrSize[Node5]);
            contentPointer6_3 -= g_numToStrSize[nodeArray_3[3]];
            memcpy(contentPointer6_3, g_numToStr[nodeArray_3[3]], g_numToStrSize[nodeArray_3[3]]);
            contentPointer6_3 -= g_numToStrSize[nodeArray_3[2]];
            memcpy(contentPointer6_3, g_numToStr[nodeArray_3[2]], g_numToStrSize[nodeArray_3[2]]);
            contentPointer6_3 -= g_numToStrSize[nodeArray_3[1]];
            memcpy(contentPointer6_3, g_numToStr[nodeArray_3[1]], g_numToStrSize[nodeArray_3[1]]);
            contentPointer6_3 -= g_numToStrSize[headNode_3];
            memcpy(contentPointer6_3, g_numToStr[headNode_3], g_numToStrSize[headNode_3]);
            g_ringNum_3++;
        }
    }
}

void addpath7_3()
{
    int addTemp_3 = canReachable3_3[nodeArray_3[4]][1];
    for(int i=P3Size_3[addTemp_3]-1; i>=0; i--)
    {
        // nodeArray_3[5] = P3_3[addTemp_3][i];
        int &Node6 = P3_3[addTemp_3][i];
        if(visit_3[Node6]) continue;
        int addTemp3_3 = canReachable_3[Node6][1];
        for(int ii=P2Size_3[addTemp3_3]-1; ii>=0; ii--)
        {
            int &Node7 = P2_3[addTemp3_3][ii];
            if(visit_3[Node7]) continue;

            contentPointer7_3 -= g_numToStrSize[Node7];
            memcpy(contentPointer7_3, g_numToStrEnd[Node7], g_numToStrSize[Node7]);
            contentPointer7_3 -= g_numToStrSize[Node6];
            memcpy(contentPointer7_3, g_numToStr[Node6], g_numToStrSize[Node6]);
            contentPointer7_3 -= g_numToStrSize[nodeArray_3[4]];
            memcpy(contentPointer7_3, g_numToStr[nodeArray_3[4]], g_numToStrSize[nodeArray_3[4]]);
            contentPointer7_3 -= g_numToStrSize[nodeArray_3[3]];
            memcpy(contentPointer7_3, g_numToStr[nodeArray_3[3]], g_numToStrSize[nodeArray_3[3]]);
            contentPointer7_3 -= g_numToStrSize[nodeArray_3[2]];
            memcpy(contentPointer7_3, g_numToStr[nodeArray_3[2]], g_numToStrSize[nodeArray_3[2]]);
            contentPointer7_3 -= g_numToStrSize[nodeArray_3[1]];
            memcpy(contentPointer7_3, g_numToStr[nodeArray_3[1]], g_numToStrSize[nodeArray_3[1]]);
            contentPointer7_3 -= g_numToStrSize[headNode_3];
            memcpy(contentPointer7_3, g_numToStr[headNode_3], g_numToStrSize[headNode_3]);
            
            g_ringNum_3++;
        }
    }

 
}
string temp;
void write_mmap(const string& path)
{
    int g_sizeOfFile3, g_sizeOfFile4, g_sizeOfFile5, g_sizeOfFile6, g_sizeOfFile7;
    int g_sizeOfFile31, g_sizeOfFile41, g_sizeOfFile51, g_sizeOfFile61, g_sizeOfFile71;
    int g_sizeOfFile32, g_sizeOfFile42, g_sizeOfFile52, g_sizeOfFile62, g_sizeOfFile72;
    int g_sizeOfFile33, g_sizeOfFile43, g_sizeOfFile53, g_sizeOfFile63, g_sizeOfFile73;
    temp = to_string(g_ringNum+g_ringNum_1+g_ringNum_2+g_ringNum_3)+'\n';
    g_sizeOfFile3 = contentPointer3 - g_content_3;
    g_sizeOfFile4 = contentPointer4 - g_content_4;
    g_sizeOfFile5 = contentPointer5 - g_content_5;
    g_sizeOfFile6 = contentPointer6 - g_content_6;
    g_sizeOfFile7 = contentPointer7 - g_content_7;
    g_content_3[g_sizeOfFile3] = '\0';
    g_content_4[g_sizeOfFile4] = '\0';
    g_content_5[g_sizeOfFile5] = '\0';
    g_content_6[g_sizeOfFile6] = '\0';
    g_content_7[g_sizeOfFile7] = '\0';
    //线程1
    g_sizeOfFile31 = g_content_3_1 + MAX_FILE_SIZE_3 - contentPointer3_1-1;
    g_sizeOfFile41 = g_content_4_1 + MAX_FILE_SIZE_4 - contentPointer4_1-1;
    g_sizeOfFile51 = g_content_5_1 + MAX_FILE_SIZE_5 - contentPointer5_1-1;
    g_sizeOfFile61 = g_content_6_1 + MAX_FILE_SIZE_6 - contentPointer6_1-1;
    g_sizeOfFile71 = g_content_7_1 + MAX_FILE_SIZE_7 - contentPointer7_1-1;
    //线程2
    g_sizeOfFile32 = contentPointer3_2 - g_content_3_2;
    g_sizeOfFile42 = contentPointer4_2 - g_content_4_2;
    g_sizeOfFile52 = contentPointer5_2 - g_content_5_2;
    g_sizeOfFile62 = contentPointer6_2 - g_content_6_2;
    g_sizeOfFile72 = contentPointer7_2 - g_content_7_2;
    g_content_3_2[g_sizeOfFile32] = '\0';
    g_content_4_2[g_sizeOfFile42] = '\0';
    g_content_5_2[g_sizeOfFile52] = '\0';
    g_content_6_2[g_sizeOfFile62] = '\0';
    g_content_7_2[g_sizeOfFile72] = '\0';
    //线程3
    g_sizeOfFile33 = g_content_3_3 + MAX_FILE_SIZE_3 - contentPointer3_3-1;
    g_sizeOfFile43 = g_content_4_3 + MAX_FILE_SIZE_4 - contentPointer4_3-1;
    g_sizeOfFile53 = g_content_5_3 + MAX_FILE_SIZE_5 - contentPointer5_3-1;
    g_sizeOfFile63 = g_content_6_3 + MAX_FILE_SIZE_6 - contentPointer6_3-1;
    g_sizeOfFile73 = g_content_7_3 + MAX_FILE_SIZE_7 - contentPointer7_3-1;
    FILE *fd = fopen(path.c_str(), "w");
    fwrite(temp.c_str(), sizeof(char), temp.size(), fd);
    fwrite(g_content_3, sizeof(char), g_sizeOfFile3, fd);
    fwrite(contentPointer3_1, sizeof(char), g_sizeOfFile31, fd);
    fwrite(g_content_3_2, sizeof(char), g_sizeOfFile32, fd);
    fwrite(contentPointer3_3, sizeof(char), g_sizeOfFile33, fd);

    fwrite(g_content_4, sizeof(char), g_sizeOfFile4, fd);
    fwrite(contentPointer4_1, sizeof(char), g_sizeOfFile41, fd);
    fwrite(g_content_4_2, sizeof(char), g_sizeOfFile42, fd);
    fwrite(contentPointer4_3, sizeof(char), g_sizeOfFile43, fd);

    fwrite(g_content_5, sizeof(char), g_sizeOfFile5, fd);
    fwrite(contentPointer5_1, sizeof(char), g_sizeOfFile51, fd);
    fwrite(g_content_5_2, sizeof(char), g_sizeOfFile52, fd);
    fwrite(contentPointer5_3, sizeof(char), g_sizeOfFile53, fd);

    fwrite(g_content_6, sizeof(char), g_sizeOfFile6, fd);
    fwrite(contentPointer6_1, sizeof(char), g_sizeOfFile61, fd);
    fwrite(g_content_6_2, sizeof(char), g_sizeOfFile62, fd);
    fwrite(contentPointer6_3, sizeof(char), g_sizeOfFile63, fd);

    fwrite(g_content_7, sizeof(char), g_sizeOfFile7, fd);
    fwrite(contentPointer7_1, sizeof(char), g_sizeOfFile71, fd);
    fwrite(g_content_7_2, sizeof(char), g_sizeOfFile72, fd);
    fwrite(contentPointer7_3, sizeof(char), g_sizeOfFile73, fd);
    fclose(fd);
}

int m_outDegree[MAX_ID_NUM] = {0};
int m_inDegree[MAX_ID_NUM] = {0};
int m_g_numToStrSize[MAX_ID_NUM];
char m_g_numToStr[MAX_ID_NUM][11];   
char m_g_numToStrEnd[MAX_ID_NUM][11];
int m_g_id2Index[MAX_ID_NUM];

int main(void)
{
    // clock_t start, finish; 
 
    outDegree = m_outDegree;
    inDegree = m_inDegree;
    g_numToStrSize = m_g_numToStrSize;

    //线程1
    // double duration;
    // start = clock();
    loadTriadIntoGraph(dasasetPath);
    // finish = clock();
    // duration = (double)(finish - start) / CLOCKS_PER_SEC;
    // printf("读取txt,用时为：%f秒\n",duration);
   
    /*** 进行环寻找算法 ***/
    // start = clock();
    int threadNum = 4;
    headNode = 0;    // 因为改了映射范围 从 -1 变成0
    // 0.2 : 0.2488
    // 0.13 : 0.2403
    // 0.1 : 0.2258
    // 0.09: 0.2199
    //0.15 0.19
    //0.20 0.18
    //0.17 0.1800
    //0.18 0.1798
    //0.183 0.1809
    headNode_1 = (int)(g_numOfIdSet * 0.18);
    headNode_2 = headNode_1 - 1;
    headNode_3 = g_numOfIdSet-1;    // 因为改了映射范围 从 -2 变成 -1
    vector<thread> threadpool(threadNum);
    threadpool[0] = thread(&startFindRing);
    threadpool[1] = thread(&startFindRing_1);
    threadpool[2] = thread(&startFindRing_2);
    threadpool[3] = thread(&startFindRing_3);
    threadpool[0].join();
    threadpool[1].join();
    threadpool[2].join();
    threadpool[3].join();
    //startFindRing();
    // finish = clock();
    // duration = (double)(finish - start) / CLOCKS_PER_SEC;
    // printf("完成环寻找,用时为：%f秒\n",duration);
    // printf("headNode=%d\n",headNode);
    /*** 线程初始化 ***/
    // start = clock();
    sleep(2);
    write_mmap(outputTxtPath);
    // finish = clock();
    // duration = (double)(finish - start) / CLOCKS_PER_SEC;
    // printf("写入环,用时为：%f秒\n",duration);
    return 0;
}