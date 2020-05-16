#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

using namespace std;
#define TEST 1
//#define OUTPUT 1

#ifdef TEST
    #define RUN_TIME(fun) do{               \
        string fun_name = #fun;             \
        clock_t time_start=clock();         \
        fun;                                \
        clock_t time_end=clock();           \
        cout<< fun_name <<" #Time use:"<<(time_end-time_start)/(double)CLOCKS_PER_SEC<<"s"<<endl; \
    }while(0)
#else
    #define RUN_TIME(fun) do{               \
        fun;                                \
    }while(0)
#endif

struct PATHWAYS
{
    int pathway1;
    int pathway2;

    PATHWAYS(int p1, int p2) : pathway1(p1),pathway2(p2){}
};


typedef pair<int, int> PAIR;

struct CmpByValue {
  bool operator()(const PAIR& lhs, const PAIR& rhs) {
    return lhs.second > rhs.second;
  }
};



class Solution
{
public:

    //临时变量
    unsigned int MAX_ID = 0;

    //正向路径 邻接表
    forward_list<int> Graph_Adjacency[100000];

    //反向路径
    forward_list<int> Feed_Link_2[100000];

    //记录每个顶点的入度，放弃一些点
    int vec_indegree[10*10000] = {0};
    int vec_outdegree[10*10000] = {0};


    //start end <pathway1 pathway2>(pathway可存在多条路径通往)
    vector<unordered_map<int,forward_list<PATHWAYS>>> Node_Link_4;

    //start end pathway1(pathway可存在多条路径通往)
    vector<unordered_map<int,forward_list<int>>> Node_Link_3;


    //记录链的条数
    int flink4_count=0;
    int link4_count=0;

    map<int, int> degree_sort;

    //保存结果
    map<vector<int>,int> cycles3; 
    map<vector<int>,int> cycles4; 
    map<vector<int>,int> cycles5;    
    map<vector<int>,int> cycles6;
    map<vector<int>,int> cycles7;

    Solution(const char *testFile)
    {
        RUN_TIME(load_data(testFile));
    }

    void Top_sort(int &id)
    {
        deque<int> Ndeque;
        int V;

        if(vec_indegree[id]==0)
            Ndeque.push_back(id);

        while(!Ndeque.empty())
        {
            V = Ndeque.front();
            Ndeque.pop_front();
            for(auto node : Graph_Adjacency[V])
            {
                vec_indegree[node] -= 1;
                if(vec_indegree[node]==0)
                    Ndeque.push_back(node);
            }
        }
    }


    //前向建链+寻找3 4环
    void build_forward_edge()
    {
        int first;

        if(Node_Link_4.size() == 0) Node_Link_4.resize(MAX_ID);

        for(int first=0; first<MAX_ID; first++)
        {
            
            if(vec_indegree[first] == 0) break;

            forward_link(first);
            
            feed_link(first);

            vec_indegree[first] = 0;

            Top_sort(first);
        }
    }


    bool forward_link(int &first)
    {
        for(auto pathway1 : Graph_Adjacency[first])
            {
                //建链基础条件：满足起点最小
                if(first > pathway1)continue;
                for(auto pathway2 : Graph_Adjacency[pathway1])
                {
                    //筛除A->B->A->B
                    if(first >= pathway2)continue;
                    for(auto path_end : Graph_Adjacency[pathway2])
                    {  
                        if(first > path_end) continue;

                        if(first == path_end)
                        {
                            cycles3[{first,pathway1,pathway2}] = 1;
                        } 
                        link4_count++;
                        Node_Link_4[first][path_end].push_front(PATHWAYS(pathway1,pathway2));
                    }
                }
            }
    }


    void feed_link(int &first)
    {
        
        //NO.2
        for(auto pathway1 : Feed_Link_2[first])
            {
                if(first > pathway1) continue;
                //寻找四环
                if(Node_Link_4[first].count(pathway1) != 0)
                {    
                    for(auto it : Node_Link_4[first][pathway1])
                    {
                        cycles4[{first,it.pathway1,it.pathway2,pathway1}]=1;
                    }
                }
                //NO.3
                for(auto pathway2 : Feed_Link_2[pathway1])
                {
                    //筛除A->B->A->B
                    if(first >= pathway2)continue;
                    //寻找五环 保证pathway2是最小
                    if(Node_Link_4[first].count(pathway2) != 0)
                    {
                        for(auto it : Node_Link_4[first][pathway2])
                        {
                            cycles5[{first,it.pathway1,it.pathway2,pathway2,pathway1}]=1;
                        }
                    }
                    //NO.4
                    for(auto pathway3 : Feed_Link_2[pathway2])
                    {    
                        if(first > pathway3)continue; 

                        //寻找六环
                        if(Node_Link_4[first].count(pathway3) != 0)
                        {
                            for(auto it : Node_Link_4[first][pathway3])
                            {
                                cycles6[{first,it.pathway1,it.pathway2,pathway3,pathway2,pathway1}]=1;
                            }
                        }

                        for(auto cyc7_end : Feed_Link_2[pathway3])
                        {
                                if(Node_Link_4[first].count(cyc7_end) != 0)
                                {
                                    for(auto it : Node_Link_4[first][cyc7_end])
                                    {
                                        cycles7[{first,it.pathway1,it.pathway2,cyc7_end,pathway3,pathway2,pathway1}]=1;
                                    }
                                }
                        }
                        flink4_count++;
                    }
                }
            }
    }






    void show_cycles()
    {
        cout << "MAX_ID: " << MAX_ID <<endl;
        cout << "flink4_count: " << flink4_count <<endl;
        cout << "link4_count: " << link4_count <<endl;    
        cout << "3 cycles: " << cycles3.size() << endl;
        cout << "4 cycles: " << cycles4.size() << endl;
        cout << "5 cycles: " << cycles5.size() << endl;
        cout << "6 cycles: " << cycles6.size() << endl;
        cout << "7 cycles: " << cycles7.size() << endl;
        cout << "Total cycles: " <<cycles3.size()+cycles4.size()+cycles5.size()+cycles6.size()+cycles7.size() << endl;
    }


    void save(const string &outputFile)
    {
        FILE *fp = fopen(outputFile.c_str(), "wb");
        map<vector<int>, int>::iterator it;
        char buf[1024];

        int idx=sprintf(buf,"%d\n",(cycles3.size()+cycles4.size()+cycles5.size()+cycles6.size()+cycles7.size()));
        buf[idx]='\0';
        fwrite(buf, idx , sizeof(char), fp);

        it = cycles3.begin();
        for(;it!=cycles3.end();++it)
        {         
            idx=sprintf(buf,"%d,%d,%d\n",it->first[0],it->first[1],it->first[2]);
            fwrite(buf, idx , sizeof(char), fp );
        }

        it = cycles4.begin();
        for(;it!=cycles4.end();++it)
        {         
            idx=sprintf(buf,"%d,%d,%d,%d\n",it->first[0],it->first[1],it->first[2],it->first[3]);
            fwrite(buf, idx , sizeof(char), fp );
        }

        it = cycles5.begin();
        for(;it!=cycles5.end();++it)
        {         
            idx=sprintf(buf,"%d,%d,%d,%d,%d\n",it->first[0],it->first[1],it->first[2],it->first[3],it->first[4]);
            fwrite(buf, idx , sizeof(char), fp );
        }

        it = cycles6.begin();
        for(;it!=cycles6.end();++it)
        {         
            idx=sprintf(buf,"%d,%d,%d,%d,%d,%d\n",it->first[0],it->first[1],it->first[2],it->first[3],it->first[4],it->first[5]);
            fwrite(buf, idx , sizeof(char), fp );
        }

        it = cycles7.begin();
        for(;it!=cycles7.end();++it)
        {         
            idx=sprintf(buf,"%d,%d,%d,%d,%d,%d,%d\n",it->first[0],it->first[1],it->first[2],it->first[3],it->first[4],it->first[5],it->first[6]);
            fwrite(buf, idx , sizeof(char), fp );
        }
        fclose(fp);
    }


private:
    void load_data(const char *testFile)
    {
        //获取文件size
        struct stat st;
        stat(testFile,&st);
        size_t filesize = st.st_size;
        int fd = open(testFile, O_RDONLY, 0);   
        unsigned char* mmappedData;
        mmappedData = (unsigned char*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    
        unsigned char* pointer =  mmappedData;
        int num1 = 0;
        int num2 = 0;
        //遍历保存数据
        while(*(pointer) != 0 )
        {            
            num1 = 0;
            num2 = 0;
            //读取num1
            do{
                num1=num1*10+ (*pointer-48);
            }while(*(++pointer) != ',');

            MAX_ID = MAX_ID > num1 ? MAX_ID : num1;
            pointer++;

            //读取num2
            do{
                num2=num2*10+ (*pointer-48);
            }while(*(++pointer) != ',');

            MAX_ID = MAX_ID > num2 ? MAX_ID : num2;
            //跳转到下一行读取
            while(*(pointer++)!= 10);
            vec_indegree[num2] +=1;
            vec_outdegree[num1] +=1;
            Graph_Adjacency[num1].push_front(num2);
            Feed_Link_2[num2].push_front(num1);
        }
        MAX_ID ++;
        munmap(mmappedData, filesize);
        close(fd);
    }



};

int main()
{
#ifdef TEST
    const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/初赛/test_data.txt";
    //const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/test_data_online2.txt";
    string OUTPUT_FILE_NAME= "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/output.txt";
    clock_t time_start=clock();
#else
    const char* FILE_NAME = "/data/test_data.txt";
    string OUTPUT_FILE_NAME= "/projects/student/result.txt"; 
#endif
    

    Solution solution(FILE_NAME);
    RUN_TIME(solution.build_forward_edge());
    RUN_TIME(solution.save(OUTPUT_FILE_NAME));
    
#ifdef TEST
    RUN_TIME(solution.show_cycles());
    clock_t time_end=clock();
    cout <<"#Time use:"<<(time_end-time_start)/(double)CLOCKS_PER_SEC<<"s"<<endl;
    exit(0);
#else
    exit(0);
#endif

    return 0;
}
