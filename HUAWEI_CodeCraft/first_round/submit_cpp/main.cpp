#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

class Solution
{
public:
    //临时变量
    unsigned int MAX_ID = 0;
    vector<int> id1;
    vector<int> id2;
    //临时记录正向连接表，用于delete_node
    vector<vector<int>> node_Link;
    //临时记录反向连接表
    vector<vector<int>> feed_Link;

    //正向路径 邻接表
    vector<vector<int>> Graph_Adjacency;
    //反向路径
    vector<vector<int>> Feed_Link_2;

    //start end <pathway1 pathway2>(pathway可存在多条路径通往)
    vector<unordered_map<int,vector<PATHWAYS>>> Node_Link_4;

    //start end pathway1(pathway可存在多条路径通往)
    vector<unordered_map<int,vector<int>>> Node_Link_3;

    //存放待删除的顶点
    unordered_map<int,int> map_vaild_node;

    //存放4链的终点,起点
    vector<vector<int>> vec_link_start;

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


    void delete_node()
    {
        if(node_Link.size()==0) node_Link.resize(MAX_ID);
        if(feed_Link.size()==0) feed_Link.resize(MAX_ID);

        //记录每个顶点的入度，放弃一些点
        vector<int> vec_indegree(MAX_ID,0);

        for(int i=0; i < id1.size();i++)
        {
            vec_indegree[id2[i]]++;
            node_Link[id1[i]].push_back(id2[i]);
        }

        for(int i=0; i < MAX_ID;i++)
        {   
              //找到入度为0的点
              if(vec_indegree[i]==0)
              {
                //考虑到该点被抛弃，故该点所指向的其他点的出度值减一
                  for(auto node : node_Link[i])
                  {
                      vec_indegree[node] -= 1;
                  }
              }
        }

        //将有效的点保存在map_vaild_node
        int map_node = 0;
        for(int i=0; i < MAX_ID;i++)
        {
            if(vec_indegree[i] > 0)
            {
                map_vaild_node[i]=1;
            }
        }
    }

    void build_adjacency()
    {
        if(Graph_Adjacency.size()==0) Graph_Adjacency.resize(MAX_ID);
        if(Feed_Link_2.size()==0) Feed_Link_2.resize(MAX_ID);

        for(int i=0; i < id1.size();i++)
        {
            if(map_vaild_node.count(id1[i]) == 0) continue;
            if(map_vaild_node.count(id2[i]) == 0) continue;
            Graph_Adjacency[id1[i]].push_back(id2[i]);
            Feed_Link_2[id2[i]].push_back(id1[i]);
        }        
    }

    void build_edge_find_cycle3()
    {
        int first;
        int pathway1;
        int pathway2;
        int end;
        if(Node_Link_4.size()==0) Node_Link_4.resize(MAX_ID);
        if(Node_Link_3.size()==0) Node_Link_3.resize(MAX_ID);
        if(vec_link_start.size()==0) vec_link_start.resize(MAX_ID);
        

        for(int i=0; i<MAX_ID; i++)
        {
            if(map_vaild_node.count(i) == 0) continue;
            first = i;
            for(auto pathway1 : Graph_Adjacency[first])
            {
                if(first == pathway1) continue;
                for(auto pathway2 : Graph_Adjacency[pathway1])
                {
                    if(first == pathway2) continue;
                    if(pathway1 == pathway2) continue;

                    //3链的限制条件
                    if(find(vec_link_start[first].begin(),vec_link_start[first].end(),pathway2) != vec_link_start[first].end())
                    {
                        Node_Link_3[first][pathway2].push_back(pathway1);
#ifdef OUTPUT
                        cout << "增3: " << first << "->" << pathway1 << "->" << pathway2  << endl;
#endif
                    } 
                    
                   for(auto path_end : Graph_Adjacency[pathway2])
                    {                          
                        //满足起点最小
                        if(vec_link_start[first].size() <= 0)
                        {
                            if(first > pathway1) continue;
                            if(first > pathway2) continue;
                            if(first > path_end) continue;
                        }

                        if(path_end==pathway1 || path_end==pathway2) continue;
                        //筛选出3环
                        if(first == path_end) 
                        {
                            if(first > pathway1) continue;
                            if(first > pathway2) continue;
                            cycles3[{first,pathway1,pathway2}] = 1; 
                            continue;
                        } 
                        vec_link_start[path_end].push_back(first);
                        Node_Link_4[first][path_end].push_back(PATHWAYS(pathway1,pathway2));
                        
#ifdef OUTPUT
                        cout << first << "->" << pathway1 << "->" << pathway2 << "->" << path_end << endl;
#endif
                    }
                }
            }
        }
    }

    void find_cycle()
    {
        int first;
        int path_end;
        int path_way1;
        int path_way2;
        
        unordered_map<int,int>::iterator drop_iter = map_vaild_node.begin();
        for(;drop_iter!=map_vaild_node.end();++drop_iter)
        {
            first = drop_iter->first;
            unordered_map<int,vector<PATHWAYS>>::iterator iter = Node_Link_4[first].begin();
            for(;iter!=Node_Link_4[first].end();++iter)
            {
                path_end = iter->first; if(first >= path_end) continue;

                    //查找4环
                    search_cycle4(path_end,first,iter);

                    //查找5环
                    search_cycle5(path_end,first,iter);

                    //查找6环
                    search_cycle6(path_end,first,iter);

                    //查找7环
                    search_cycle7(path_end,first,iter);
            }

 
        }
    }


    void show_cycles()
    {
        cout << "MAX_ID: " << MAX_ID <<endl;
        cout << "delete_node: " << map_vaild_node.size() <<endl;
        cout << "3 cycles: " << cycles3.size() << endl;
        cout << "4 cycles: " << cycles4.size() << endl;
        cout << "5 cycles: " << cycles5.size() << endl;
        cout << "6 cycles: " << cycles6.size() << endl;
        cout << "7 cycles: " << cycles7.size() << endl;
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
        //unsigned int end =  0;
        int num1 = 0;
        int num2 = 0;
        //int a[6] = {1,10,100,1000,10000,100000};
        int i = 0;
        //遍历保存数据
        while(*(pointer) != 0 )
        {
           // end = 1;
            num1 = 0;
            num2 = 0;
            //读取num1
            while(*(pointer++)!= ',') {
                num1=num1*10+*(pointer)-48
            }
            MAX_ID = MAX_ID > num1 ? MAX_ID : num1;
            //读取num2
            while(*(pointer++)!= ',') {
                num2=num2*10+*(pointer)-48
            }
            MAX_ID = MAX_ID > num2 ? MAX_ID : num2;
            //跳转到下一行读取
            while(*(pointer++)!= 10);
            id1.push_back(num1);
            id2.push_back(num2);
        }
        MAX_ID ++;
        munmap(mmappedData, filesize);
        close(fd);
    }


    void search_cycle4(int path_end, int first, unordered_map<int,vector<PATHWAYS>>::iterator &iter)
    {
        if(find(Graph_Adjacency[path_end].begin(),Graph_Adjacency[path_end].end(),first) != Graph_Adjacency[path_end].end())
        {
            //输出多条中间路径
            int path_way1;
            int path_way2;
            vector<PATHWAYS> &vec_pathway = iter->second;
            for(int i=0; i<vec_pathway.size(); i++)
            {
                path_way1 = vec_pathway[i].pathway1; if(first >= path_way1) continue;
                path_way2 = vec_pathway[i].pathway2; if(first >= path_way2) continue;

                cycles4[{first,path_way1,path_way2,path_end}] = 1;
#ifdef OUTPUT
                cout << first << "->" << path_way1 << "->" << path_way2 << "->" << path_end << endl;
#endif  
            }
        }   
    }

    void search_cycle5(int path_end, int first, unordered_map<int,vector<PATHWAYS>>::iterator &iter)
    {
       if(Node_Link_3[path_end].find(first) != Node_Link_3[path_end].end())
       {
           int path_way1;
           int path_way2;
           vector<PATHWAYS> &vec_pathway = iter->second;
           vector<int> &temp_vertor = Node_Link_3[path_end].find(first)->second;

           for(int i=0; i<vec_pathway.size(); i++)
           {
               path_way1 = vec_pathway[i].pathway1; if(first >= path_way1) continue;
               path_way2 = vec_pathway[i].pathway2; if(first >= path_way2) continue;

               for(auto cycle5_end : temp_vertor)
               {
                   if(first >= cycle5_end) continue;
                   //筛除内环
                   if(cycle5_end == path_way1 || cycle5_end == path_way2) continue;
                   cycles5[{first,path_way1,path_way2,path_end,cycle5_end}] = 1;
#ifdef OUTPUT
                   cout << first << "->" << path_way1 << "->" << path_way2 << "->" << path_end << "->" << cycle5_end << endl;
#endif 
               }
           }

       }
    }


    void search_cycle6(int path_end, int first, unordered_map<int,vector<PATHWAYS>>::iterator &iter)
    {
        if(Node_Link_4[path_end].find(first) != Node_Link_4[path_end].end())
       {
            int path_way1;
            int path_way2;
            int path_way3;
            int cycle6_end;
            vector<PATHWAYS> &vec_link1 = iter->second;
            vector<PATHWAYS> &vec_link2 = Node_Link_4[path_end].find(first)->second;
            for(int i=0; i<vec_link1.size();i++)
            {
                path_way1 = vec_link1[i].pathway1; if(first >= path_way1) continue;
                path_way2 = vec_link1[i].pathway2; if(first >= path_way2) continue;

                for(int j=0; j<vec_link2.size(); j++)
                {
                    path_way3 = vec_link2[j].pathway1; if(first >= path_way3) continue;
                    cycle6_end = vec_link2[j].pathway2; if(first >= cycle6_end) continue;
                    if(first > path_end) continue;
                    //筛除内环
                    if(path_way3 == path_way1 || path_way3 == path_way2) continue;
                    if(cycle6_end == path_way1 || cycle6_end == path_way2) continue;
                    cycles6[{first,path_way1,path_way2,path_end,path_way3,cycle6_end}] = 1;
#ifdef OUTPUT
                   cout << first << "->" << path_way1 << "->" << path_way2 << "->" << path_end << "->" << path_way3 << "->" << cycle6_end <<endl;
#endif 
                }
            }
       }
    }



    void search_cycle7(int path_end, int first, unordered_map<int,vector<PATHWAYS>>::iterator &iter)
    {
        for(auto Link2_end : Feed_Link_2[first])
        {
            if(Node_Link_4[path_end].find(Link2_end) != Node_Link_4[path_end].end())
            {
                int path_way1;
                int path_way2;
                int path_way3;
                int path_way4;
                vector<PATHWAYS> &vec_link1 = iter->second;
                vector<PATHWAYS> &vec_link2 = Node_Link_4[path_end].find(Link2_end)->second;
                for(int i=0;i<vec_link1.size();i++)
                {
                    path_way1 = vec_link1[i].pathway1; if(first >= path_way1) continue;
                    path_way2 = vec_link1[i].pathway2; if(first >= path_way2) continue;
                    for(int j=0; j<vec_link2.size(); j++)
                    {
                        path_way3 = vec_link2[j].pathway1; if(first >= path_way3) continue;
                        path_way4 = vec_link2[j].pathway2; if(first >= path_way4) continue;
                        if(first >= Link2_end) continue;
                        //筛除内环
                        if(path_way3 == path_way1 || path_way3 == path_way2) continue;
                        if(path_way4 == path_way1 || path_way4 == path_way2) continue;
                        if(Link2_end == path_way1 || Link2_end == path_way2) continue;

                        cycles7[{first,path_way1,path_way2,path_end,path_way3,path_way4,Link2_end}] = 1;
#ifdef OUTPUT
                        cout << first << "->" << path_way1 << "->" << path_way2 << "->" << path_end << "->" << path_way3 << "->" << path_way4 << "->" << Link2_end <<endl;
#endif                         
                    }
                }
            }
        }        
    }

};

int main()
{
#ifdef TEST
    const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/初赛/test_data.txt";
    //const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/test_data_demo.txt";
    string OUTPUT_FILE_NAME= "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/output.txt";
    clock_t time_start=clock();
#else
    const char* FILE_NAME = "/data/test_data.txt";
    string OUTPUT_FILE_NAME= "/projects/student/result.txt"; 
#endif
    

    Solution solution(FILE_NAME);
    RUN_TIME(solution.delete_node());
    RUN_TIME(solution.build_adjacency());
    RUN_TIME(solution.build_edge_find_cycle3());
    RUN_TIME(solution.find_cycle());
    RUN_TIME(solution.show_cycles());
    RUN_TIME(solution.save(OUTPUT_FILE_NAME));
    
#ifdef TEST
    clock_t time_end=clock();
    cout <<"#Time use:"<<(time_end-time_start)/(double)CLOCKS_PER_SEC<<"s"<<endl; 
    cout <<"#1000次 Time use:"<<(time_end-time_start)/(double)CLOCKS_PER_SEC*1000<<"s"<<endl; 
    exit(0);
#else
    exit(0);
#endif

    return 0;
}

