#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <map>
#include <unordered_set>
#include <set>
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
//#define TEST 1
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

    //正向路径 邻接表
    vector<vector<int>> Graph_Adjacency;
    //反向路径
    vector<vector<int>> Feed_Link_2;

    //start end <pathway1 pathway2>(pathway可存在多条路径通往)
    vector<unordered_map<int,vector<PATHWAYS>>> Node_Link_4;

    //start end pathway1(pathway可存在多条路径通往)
    vector<unordered_map<int,vector<int>>> Node_Link_3;

    //存放4链的终点,起点
    vector<vector<int>> vec_link_start;

    //保存强连通路径
    vector<set<int>> SCC_PATH;

    vector<int> in_dregree;

    //记录链的条数
    int link3_count=0;
    int link4_count=0;

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


    void build_adjacency()
    {
        if(Graph_Adjacency.size()==0) Graph_Adjacency.resize(MAX_ID);
        if(Feed_Link_2.size()==0) Feed_Link_2.resize(MAX_ID);
        if(in_dregree.size()==0) in_dregree.resize(MAX_ID);

        for(int i=0; i < id1.size();i++)
        {
            Graph_Adjacency[id1[i]].push_back(id2[i]);
            Feed_Link_2[id2[i]].push_back(id1[i]);
            in_dregree[id2[i]]++;
        }     

        RUN_TIME(get_strong_connect_components(Graph_Adjacency));  

        //根据scc精简邻接表
        for(int i=0; i<SCC_PATH.size(); i++)
        {
            set<int>::iterator iter_scc = SCC_PATH[i].begin();
            for(; iter_scc != SCC_PATH[i].end(); iter_scc++)
            {
                 for(auto it = Graph_Adjacency[*iter_scc].begin(); it != Graph_Adjacency[*iter_scc].end(); it++)
                 {
                    if(SCC_PATH[i].count(*it) == 0)
                    {
                        it = Graph_Adjacency[*iter_scc].erase(it);
                        if(it == Graph_Adjacency[*iter_scc].end()) break;
                    }
                }
            }
        }

    }


    void build_edge_find_cycle3()
    {
        int first;
        if(Node_Link_4.size()==0) Node_Link_4.resize(MAX_ID);
        if(Node_Link_3.size()==0) Node_Link_3.resize(MAX_ID);
        if(vec_link_start.size()==0) vec_link_start.resize(MAX_ID);

        unordered_map<int,int>drop_link;

        //根据强连接分量创建链
        for(int i=0; i<SCC_PATH.size(); i++)
        {
            set<int>::iterator iter_scc = SCC_PATH[i].begin();
            for(; iter_scc != SCC_PATH[i].end(); iter_scc++)
            {
                first = *iter_scc;
                for(auto pathway1: Graph_Adjacency[first])
                {
                    for(auto pathway2: Graph_Adjacency[pathway1])
                    {
                        if(first == pathway2) continue;//舍弃2环A->B->A
                        //3链的限制条件
                        if(find(vec_link_start[first].begin(),vec_link_start[first].end(),pathway2) != vec_link_start[first].end())
                        {
                            Node_Link_3[first][pathway2].push_back(pathway1);
                            link3_count++;
#ifdef OUTPUT
                            cout << "3链" <<first << "->" << pathway1 << "->" << pathway2  << endl;
#endif
                        }
                            for(auto pathend: Graph_Adjacency[pathway2])
                            {   
                                    if(pathend==pathway1 || pathend==pathway2) continue;

                                    //满足起点最小
                                    if(!(find(vec_link_start[first].begin(), vec_link_start[first].end(), pathend)!= vec_link_start[first].end() ))
                                    {
                                        if(drop_link[first] == 1)continue;
                                        if(first > pathway1) continue;
                                        if(first > pathway2) continue;
                                        if(first > pathend) continue;      
                                    }

                                    //筛选出3环
                                    if(first == pathend) 
                                    {
                                        if(first > pathway1) continue; //筛除3-1-2-3
                                        if(first > pathway2) continue; //筛除2-3-1-2
                                        cycles3[{first,pathway1,pathway2}] = 1; 
#ifdef OUTPUT
                                        cout << first << "->" << pathway1 << "->" << pathway2  << endl;
#endif
                                        continue;
                                    } 

                                    link4_count++;
                                    Node_Link_4[first][pathend].push_back(PATHWAYS(pathway1,pathway2));
                                    vec_link_start[pathend].push_back(first);
                                    for(auto cycle7_end : Feed_Link_2[first]){vec_link_start[pathend].push_back(cycle7_end);}
                                    if(in_dregree[pathway1]==1){drop_link[pathway1] =1;}
#ifdef OUTPUT
                                    cout << "4链" << first << "->" << pathway1 << "->" << pathway2  << "->" << pathend << endl;
#endif
                            }
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

        //根据强连接分量创建链
        for(int i=0; i<SCC_PATH.size(); i++)
        {
            set<int>::iterator iter_scc = SCC_PATH[i].begin();
            for(; iter_scc != SCC_PATH[i].end(); iter_scc++)
            {
                first = *iter_scc;
                unordered_map<int,vector<PATHWAYS>>::iterator iter_link = Node_Link_4[first].begin();
                for(;iter_link!=Node_Link_4[first].end();++iter_link)
                {
                        path_end = iter_link->first; if(first >= path_end) continue;

                        //查找4环
                        search_cycle4(path_end,first,iter_link);

                        //查找5环
                        search_cycle5(path_end,first,iter_link);

                        //查找6环
                        search_cycle6(path_end,first,iter_link);

                        //查找7环
                        search_cycle7(path_end,first,iter_link);
                }
            }

 
        }
    }


    void show_cycles()
    {
        cout << "MAX_ID: " << MAX_ID <<endl;
        cout << "SCC: " << SCC_PATH.size() <<endl;
        cout << "link3_count: " << link3_count <<endl;
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
        int i = 0;
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



    // Tarjan 算法
    void Tarjan(vector<vector<int>> &G_NODE, unsigned int *DFN, unsigned int *LOW, \
        bool *visit, bool *instack, int node_u)
    {
    
        static unsigned int sccCounter = 0;
        static stack<int> s;          //DFS栈
    
        DFN[node_u] = LOW[node_u] = ++sccCounter;
        visit[node_u] = 1;
        s.push(node_u);
        instack[node_u] = 1;
    
        for(int i=0; i<G_NODE[node_u].size(); i++)
        {
            int v = G_NODE[node_u][i];
            if(visit[v]==0)
            {
                //cout << "入栈：" << v << endl;
                Tarjan(G_NODE, DFN, LOW, visit, instack, v);
                //cout << "出栈：" << v << endl;
    
                //在深度搜索返回时，如果v节点下存在子树，要将u节点的LOW[u]更新。
                LOW[node_u]=min(LOW[node_u],LOW[v]);
            }
            else if(DFN[node_u] > DFN[v]) 
            {
                if(instack[v] == 1)
                {
                    // v节点已经被访问，并且在栈中，说明在当前路径上存在环，此处只是赋值
                    // 但并不代表在u子树的底下的多个节点没有比当前环更大的环。无法作为深度终止条件。
                    LOW[node_u]=min(LOW[node_u],DFN[v]);
                }
            }
        }
        int m;
        // 在深度搜索完结后返回时，判断DFN[u]==LOW[u]，相等说明找到了一个环，将栈中节点弹出。注意tarjan算法认为单个节点也为环。
        if( DFN[node_u]==LOW[node_u] && (s.size() > 0) )
        {
            set<int> path;
            // 将栈中节点弹出，并计数
            do{
                m=s.top();
                s.pop();
                instack[m]=0;
                path.emplace(m);
            }while(DFN[m] > DFN[node_u]);
    
            if(path.size() > 2)
            {             
                SCC_PATH.push_back(path);
            }
        }
    }



    void get_strong_connect_components(vector<vector<int>> &node_adjacency)
    {
        unsigned int DFN[MAX_ID];
        unsigned int LOW[MAX_ID];
        bool visit[MAX_ID] ;    //记录是否访问，因为整个节点我们只访问一次就行。
        bool instack[MAX_ID] ;  //判断是否在栈里
        //标识位清 0 
        for(int i=0; i< MAX_ID; i++)
        {
            visit[i] = 0;
            instack[i] = 0;
        }
        for(int i = 0; i < MAX_ID; i++)
        {
            if(visit[i]==0)
            {
                visit[i] = 1;
                Tarjan(node_adjacency, DFN, LOW, visit, instack, i); 
            }   
        }
    }

};





int main()
{
#ifdef TEST
    //const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/初赛/test_data_3w.txt";
    const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/test_data_1w5.txt";
    string OUTPUT_FILE_NAME= "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/output.txt";
    clock_t time_start=clock();
#else
    const char* FILE_NAME = "/data/test_data.txt";
    string OUTPUT_FILE_NAME= "/projects/student/result.txt"; 
#endif
    

    Solution solution(FILE_NAME);
    RUN_TIME(solution.build_adjacency());
    RUN_TIME(solution.build_edge_find_cycle3());
    RUN_TIME(solution.find_cycle());
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

