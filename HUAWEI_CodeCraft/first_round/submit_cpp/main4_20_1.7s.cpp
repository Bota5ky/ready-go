#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <map>
#include <set>
#include <list>
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


forward_list<int> id_list;
int id_map[280000];
int map_id[280000];
int data[280000][2];
//正向路径 邻接表
int Graph_length[280000] = {0};
int Graph_Adjacency[280000][50];
//反向路径
int Feed_length[280000] = {0};
int Feed_Link_2[280000][255];

//记录每个顶点的入度，放弃一些点
int vec_indegree[280000];

int visitcut[280000] = {-1};
int visited[280000]={0};

int ans3_cnt = 0;
int ans4_cnt = 0;
int ans5_cnt = 0;
int ans6_cnt = 0;
int ans7_cnt = 0;
int ans3_offest = 0;
int ans4_offest = 0;
int ans5_offest = 0;
int ans6_offest = 0;
int ans7_offest = 0;
char ans3_buf[500000*3*10];
char ans4_buf[500000*4*10];
char ans5_buf[1000000*5*10];
char ans6_buf[2000000*6*10];
char ans7_buf[3000000*7*10];

class Solution
{
public:

    //临时变量
    unsigned int MAX_ID = 0;
    unsigned int REC_NUM = 0;

    Solution(const char *testFile)
    {
        RUN_TIME(load_data(testFile));

        id_list.sort();

        id_list.unique();

        //建立映射
        int n =0;
        forward_list<int>::iterator it=id_list.begin();
        for(;it!=id_list.end();++it)
        {
        	map_id[n] = *it;
        	id_map[*it] = n++;

        }
        MAX_ID = n;

        //创建邻接表
        for(int i=0; i<REC_NUM;i++)
        {
        	vec_indegree[id_map[data[i][1]]]++;
        	Graph_Adjacency[id_map[data[i][0]]][Graph_length[id_map[data[i][0]]]++] = id_map[data[i][1]];
        	Feed_Link_2[id_map[data[i][1]]][Feed_length[id_map[data[i][1]]]++] = id_map[data[i][0]];
        }

        //邻接表排序
        for(int i=0; i<MAX_ID; i++)
        {
        	sort(Graph_Adjacency[i],Graph_Adjacency[i]+Graph_length[i]);
        	sort(Feed_Link_2[i],Feed_Link_2[i]+Feed_length[i]);
        }


        All_Top_sort();
    }

    void All_Top_sort()
    {
        queue<int> Inqueue;
        int V;

        for(int id=0; id<MAX_ID; id++)
        {
            if(vec_indegree[id]==0)
            {
                Inqueue.push(id);
            }
        }

        while(!Inqueue.empty())
        {
            V = Inqueue.front();
            Inqueue.pop();

           	for(int i=0; i<Graph_length[V]; i++)
           	{
           		if(vec_indegree[Graph_Adjacency[V][i]] > 0)
           			vec_indegree[Graph_Adjacency[V][i]]--;

           		if(vec_indegree[Graph_Adjacency[V][i]]==0)
                {
                    Inqueue.push(Graph_Adjacency[V][i]);
                }
           	}
        }
    }

    void Top_sort(int &id)
    {
        queue<int> Inqueue;
        int V;

        vec_indegree[id]=0;
            
        Inqueue.push(id);
          
        while(!Inqueue.empty())
        {
            V = Inqueue.front();
            Inqueue.pop();
            for(int i=0; i<Graph_length[V]; i++)
           	{
				if(vec_indegree[Graph_Adjacency[V][i]] > 0)
				{
	           		if((--vec_indegree[Graph_Adjacency[V][i]])==0)
	                {
	                    Inqueue.push(Graph_Adjacency[V][i]);
	                }
	            }
           	}
        }
    }


    //3邻域剪枝
    void neighbour_cut(int &first)
    {
    	int second;
    	int third;
    	int end;
    	visited[first] = 1;

    	for(int i=0; i<Feed_length[first]; i++)
    	{
    		second = Feed_Link_2[first][i];
    		if(vec_indegree[second]==0) continue;
    		visited[second] = 1;
    		visitcut[second] = first; //# 将倒数第一个点（6+1）visit1标记为-2，其实可以新建一个visit2来标记的，要容易理解些。
    		for(int j=0; j<Feed_length[second]; j++)
    		{
    			if(visited[Feed_Link_2[second][j]]==1)continue;  
    			third =  Feed_Link_2[second][j];
    			if(vec_indegree[third]==0) continue;
    			visitcut[third] = first;
    			visited[third] = 1;

    			for(int k=0; k<Feed_length[third]; k++)
    			{
    				if(visited[Feed_Link_2[third][k]]==1)continue;  
    				end = Feed_Link_2[third][k];
    				if(vec_indegree[end]==0) continue;
    				visitcut[end] = first;
    			}
    			visited[third] = 0;
    		}
    		visited[second] = 0;
    	}

    	for(int i=0; i<Feed_length[first]; i++)
    		visitcut[Feed_Link_2[first][i]] = -2;

    }



    void dfs6(int &first)
    {
    	int p1;
    	int p2;
    	int p3;
    	int p4;
    	int p5;
    	int p6;

    	for(int i=0; i<Graph_length[first]; i++)
    	{
    		p1 = Graph_Adjacency[first][i];
    		if(vec_indegree[p1]==0)continue; 
    		visited[p1] = 1;

    		for(int j=0; j<Graph_length[p1]; j++)
    		{
    			p2 = Graph_Adjacency[p1][j];
    			//if(vec_indegree[p2]==0)continue;
    			if(visited[p2]==1)continue; 
    			if(visitcut[p2]==-2)
    			{
    				ans3_offest += sprintf(ans3_buf+ans3_offest,"%d,%d,%d\n",map_id[first],map_id[p1],map_id[p2]);
    				ans3_cnt++;
    			}

    			visited[p2] = 1; 


    			for(int k=0; k<Graph_length[p2]; k++)
    			{
    				p3 = Graph_Adjacency[p2][k];
    				if(vec_indegree[p3]==0)continue;
	    			if(visited[p3]==1)continue;
	    			if(visitcut[p3]==-2)
	    			{
	    				ans4_offest += sprintf(ans4_buf+ans4_offest,"%d,%d,%d,%d\n",map_id[first],map_id[p1],map_id[p2],map_id[p3]);
    					ans4_cnt++;
	    			} 
	    			visited[p3] = 1; 
	    			
	    			for(int l=0; l<Graph_length[p3]; l++)
    				{
	    				p4 = Graph_Adjacency[p3][l];
		    			if(visited[p4]==1 || (visitcut[p4]!=first && visitcut[p4]!=-2))continue; 
		    			if(visitcut[p4]==-2)
		    			{
			    			ans5_offest += sprintf(ans5_buf+ans5_offest,"%d,%d,%d,%d,%d\n",map_id[first],map_id[p1],map_id[p2],map_id[p3],map_id[p4]);
		    				ans5_cnt++;
	    			
		    			}
		    			visited[p4] = 1; 
		    			
		    			for(int m=0; m<Graph_length[p4]; m++)
		    			{
		    				p5 = Graph_Adjacency[p4][m];
			    			if(visited[p5]==1 || (visitcut[p5]!=first && visitcut[p5]!=-2))continue; 
			    			if(visitcut[p5]==-2)
			    			{
			    				ans6_offest += sprintf(ans6_buf+ans6_offest,"%d,%d,%d,%d,%d,%d\n",map_id[first],map_id[p1],map_id[p2],map_id[p3],map_id[p4],map_id[p5]);
		    					ans6_cnt++;
			    			}
			    			visited[p5] = 1; 

			    			for(int n=0; n<Graph_length[p5]; n++)
		    				{
			    				p6 = Graph_Adjacency[p5][n];
				    			if(visited[p6]==1 || (visitcut[p6]!=first && visitcut[p6]!=-2))continue; 
				    			if(visitcut[p6]==-2)
				    			{
				    				ans7_offest += sprintf(ans7_buf+ans7_offest,"%d,%d,%d,%d,%d,%d,%d\n",map_id[first],map_id[p1],map_id[p2],map_id[p3],map_id[p4],map_id[p5],map_id[p6]);
		    						ans7_cnt++;
				    			}
		    				}	
		    				visited[p5] = 0;
		    			}
		    			visited[p4] = 0;	
    				}
    				visited[p3] = 0;	
    			}
    			visited[p2] = 0;	
    		}
    		visited[p1] = 0;	
    	}

    }


    void findcycles()
    {
    	for(int i=0; i<MAX_ID; i++)
    	{
    		if(vec_indegree[i]==0) continue;

    		neighbour_cut(i);

    		dfs6(i);

    		Top_sort(i);

    		for(int k=0; k<Feed_length[i]; k++)
    		{
    			visitcut[Feed_Link_2[i][k]] = i;
    		}
    	}
    }




    void show_cycles()
    {
        cout << "MAX_ID: " << MAX_ID <<endl;   
        cout << "3 cycles: " << ans3_cnt << endl;
        cout << "4 cycles: " << ans4_cnt << endl;
        cout << "5 cycles: " << ans5_cnt << endl;
        cout << "6 cycles: " << ans6_cnt << endl;
        cout << "7 cycles: " << ans7_cnt << endl;
        cout << "Total cycles: " << ans3_cnt + ans4_cnt + ans5_cnt + ans6_cnt + ans7_cnt << endl;
    }


    void save(const string &outputFile)
    {
        FILE *fp = fopen(outputFile.c_str(), "wb");
        map<vector<int>, int>::iterator it;
        char buf[1024];

        int idx=sprintf(buf,"%d\n",(ans3_cnt + ans4_cnt + ans5_cnt + ans6_cnt + ans7_cnt));
        buf[idx]='\0';
        fwrite(buf, idx , sizeof(char), fp);
        fwrite(ans3_buf, ans3_offest , sizeof(char), fp );
        fwrite(ans4_buf, ans4_offest , sizeof(char), fp );
        fwrite(ans5_buf, ans5_offest , sizeof(char), fp );
        fwrite(ans6_buf, ans6_offest , sizeof(char), fp );
        fwrite(ans7_buf, ans7_offest , sizeof(char), fp );
        fclose(fp);
    }


private:
    void load_data(const char *testFile)
    {
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

            id_list.push_front(num1);
            id_list.push_front(num2);

            data[REC_NUM][0]=num1;
            data[REC_NUM][1]=num2;

            REC_NUM++;
        }
        MAX_ID ++;
        munmap(mmappedData, filesize);
        close(fd);
    }
};

int main()
{
#ifdef TEST
    const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/初赛/test_data_3w.txt";
    //const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/test_data_38252.txt";
    string OUTPUT_FILE_NAME= "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/output.txt";
    clock_t time_start=clock();
#else
    const char* FILE_NAME = "/data/test_data.txt";
    string OUTPUT_FILE_NAME= "/projects/student/result.txt"; 
#endif

    Solution solution(FILE_NAME);
    RUN_TIME(solution.findcycles());
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
