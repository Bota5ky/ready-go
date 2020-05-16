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

class Solution
{
public:

    //临时变量
    unsigned int MAX_ID = 0;

    //保存结果
    map<vector<int>,int> cycles3; 
    map<vector<int>,int> cycles4; 
    map<vector<int>,int> cycles5;    
    map<vector<int>,int> cycles6;
    map<vector<int>,int> cycles7;

    Solution(const char *testFile)
    {
        RUN_TIME(load_data(testFile));

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
    			if(visitcut[p2]==-2){cycles3[{first,p1,p2}]=1;}
    			visited[p2] = 1; 


    			for(int k=0; k<Graph_length[p2]; k++)
    			{
    				p3 = Graph_Adjacency[p2][k];
    				if(vec_indegree[p3]==0)continue;
	    			if(visited[p3]==1)continue;
	    			if(visitcut[p3]==-2){cycles4[{first,p1,p2,p3}]=1;} 
	    			visited[p3] = 1; 
	    			
	    			for(int l=0; l<Graph_length[p3]; l++)
    				{
	    				p4 = Graph_Adjacency[p3][l];
		    			if(visited[p4]==1 || (visitcut[p4]!=first && visitcut[p4]!=-2))continue; 
		    			if(visitcut[p4]==-2){cycles5[{first,p1,p2,p3,p4}]=1;}
		    			visited[p4] = 1; 
		    			
		    			for(int m=0; m<Graph_length[p4]; m++)
		    			{
		    				p5 = Graph_Adjacency[p4][m];
			    			if(visited[p5]==1 || (visitcut[p5]!=first && visitcut[p5]!=-2))continue; 
			    			if(visitcut[p5]==-2){cycles6[{first,p1,p2,p3,p4,p5}]=1;}
			    			visited[p5] = 1; 

			    			for(int n=0; n<Graph_length[p5]; n++)
		    				{
			    				p6 = Graph_Adjacency[p5][n];
				    			if(visited[p6]==1 || (visitcut[p6]!=first && visitcut[p6]!=-2))continue; 
				    			if(visitcut[p6]==-2){cycles7[{first,p1,p2,p3,p4,p5,p6}]=1;}
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
    	cout << "Helloworld" << endl;

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
            vec_indegree[num2]++;
            Graph_Adjacency[num1][Graph_length[num1]++] = num2;
            Feed_Link_2[num2][Feed_length[num2]++] = num1;
   
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
