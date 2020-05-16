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


typedef unsigned long int  uint64_t;
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


struct Node { //到达顶点v,边权为w 
	unsigned int v = 0;
	uint64_t w = 0;
    Node(unsigned int v1,uint64_t v2) : v(v1),w(v2){}
};


//升序
bool Cmpare(const Node &a, const Node &b)            
{
     return a.v < b.v;
}

unsigned int ID_ARRAY[4000001];
unsigned int ID1[2000001];
unsigned int ID2[2000001];
uint64_t COST[2000001];


vector<vector<Node>> Graph_Ordered;
vector<vector<Node>> Graph_Reverse;

//正向路径 邻接表
unsigned char Graph_length[1000001] = {0};
unsigned char Feed_length[1000001] = {0};


//记录每个顶点的入度，放弃一些点
static unsigned int vec_indegree[1000001];

static uint64_t visit_end_w[1000001] ;
static int visitcut[1000001] = {-1};
static unsigned int visited[1000001]={0};
unordered_map<unsigned int,unsigned int> ID_MAPPING;

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

char ans3_buf[3000000*12*3];
char ans4_buf[4000000*12*4];
char ans5_buf[5000000*12*5];
char ans6_buf[6000000*12*6];
char ans7_buf[1000000000];


class Solution
{
public:

    //临时变量
    unsigned int True_ID_NUM = 0;
    unsigned int MAX_ID = 0;
    unsigned int REC_NUM = 0;

    Solution(const char *testFile)
    {
        RUN_TIME(load_data(testFile));

        RUN_TIME(id_mapping_buildG());

        RUN_TIME(All_Top_sort());
    }

    void id_mapping_buildG()
    {
        sort(ID_ARRAY,ID_ARRAY+REC_NUM*2);
        MAX_ID = unique(ID_ARRAY,ID_ARRAY+REC_NUM*2)-ID_ARRAY;
        //建立映射 
        for(int i=0; i<MAX_ID; i++)
        {
            ID_MAPPING[ID_ARRAY[i]] = i;
        }

        //构建图
        Graph_Ordered.reserve(MAX_ID);
        Graph_Reverse.reserve(MAX_ID);
        register unsigned int temp_num1 = 0;
        register unsigned int temp_num2 = 0;
        for(int i=0; i<REC_NUM; i++)
        {    
            temp_num1 = ID_MAPPING[ID1[i]];
            temp_num2 = ID_MAPPING[ID2[i]];
            Graph_Ordered[temp_num1].emplace_back(Node(temp_num2,COST[i]));
            Graph_Reverse[temp_num2].emplace_back(Node(temp_num1,COST[i]));
            Graph_length[temp_num1]++;
            Feed_length[temp_num2]++;
            vec_indegree[temp_num2]++;
           
        }
        //邻接表排序
        for(int i=0; i<MAX_ID; i++)
        {
            sort(Graph_Ordered[i].begin(),Graph_Ordered[i].end(),Cmpare);     
        }
    }

    
    void All_Top_sort()
    {
        unsigned int V;
        unsigned int my_stack[200000];
        unsigned int stack_top=0;

        for(int id=0; id<MAX_ID; id++)
        {
            if(vec_indegree[id]==0)
            {
                my_stack[stack_top++] = id;
            }
        }

        while(stack_top)
        {
            V = my_stack[--stack_top];

           	for(int i=0; i<Graph_length[V]; i++)
           	{
           		if(vec_indegree[Graph_Ordered[V][i].v] > 0)
                {
                    if((--vec_indegree[Graph_Ordered[V][i].v])==0)
                    {
                        my_stack[stack_top++] = Graph_Ordered[V][i].v;
                    }
                }
           	}
        }     
    }



inline bool cycle_check(uint64_t &end_cost, uint64_t &first_cost, uint64_t penult_low_cost, uint64_t penult_high_cost)
{
    uint64_t low_cost_X = (end_cost % 5 !=0) ? (end_cost/5 + 1) : (end_cost/5);
    uint64_t high_cost_X = end_cost * 3; 
    if(first_cost < low_cost_X || first_cost > high_cost_X) return false;
    if(end_cost < penult_low_cost ||end_cost > penult_high_cost) return false; 
    return true;

}


inline int int2str(char *str,unsigned int num, char c_end)
    {
        int count = 0;
        char temp[12];
        do{
            temp[count++] = num % 10 + '0';
        }while(num /= 10);

        for(int i=0; i<count; i++)
        {
            str[i] = temp[count-1-i];
        }
        str[count++] = c_end;
        return count;
    }




    void Top_sort(unsigned int &id)
    {
        queue<unsigned int> Inqueue;
        unsigned int V;

        vec_indegree[id]=0;
            
        Inqueue.push(id);
          
        while(!Inqueue.empty())
        {
            V = Inqueue.front();
            Inqueue.pop();
            for(int i=0; i<Graph_length[V]; i++)
           	{
				if(vec_indegree[Graph_Ordered[V][i].v] > 0)
				{
	           		if((--vec_indegree[Graph_Ordered[V][i].v])==0)
	                {
	                    Inqueue.push(Graph_Ordered[V][i].v);
	                }
	            }
           	}
        }  
    }


    //3邻域剪枝
    void neighbour_cut(unsigned int &first)
    {
    	register unsigned int second;
    	register unsigned int third;
    	register unsigned int end;

        register uint64_t cost;
        register uint64_t low_cost_Y[2];
        register uint64_t high_cost_Y[2];
    	visited[first] = 1;
        
        //A<-B<-C<-D

    	for(int i=0; i<Feed_length[first]; i++)
    	{
            second = Graph_Reverse[first][i].v;
    		if(vec_indegree[second]==0) continue;
            //转账约束
            cost = Graph_Reverse[first][i].w;
            low_cost_Y[0] = cost % 3 != 0 ? (cost / 3 + 1) : (cost / 3);
            high_cost_Y[0] = cost*5;
    		visited[second] = 1;
    		visitcut[second] = first; //# 将倒数第一个点（6+1）visit1标记为-2，其实可以新建一个visit2来标记的，要容易理解些。
    		for(int j=0; j<Feed_length[second]; j++)
    		{
                third =  Graph_Reverse[second][j].v;
    			if(vec_indegree[third]==0) continue;
    			if(visited[third]==1)continue;
                //转账约束
                cost = Graph_Reverse[second][j].w;
                if(cost < low_cost_Y[0] || cost > high_cost_Y[0]) continue;
                low_cost_Y[1] = cost % 3 != 0 ? (cost / 3 + 1) : (cost / 3);
                high_cost_Y[1] = cost*5;

    			visitcut[third] = first;
    			visited[third] = 1;

    			for(int k=0; k<Feed_length[third]; k++)
    			{
                    end = Graph_Reverse[third][k].v;
    				if(vec_indegree[end]==0) continue;
    				if(visited[end]==1)continue; 
                    //转账约束
                    cost = Graph_Reverse[third][k].w;
                    if(cost < low_cost_Y[1] || cost > high_cost_Y[1]) continue; 
    		
    				visitcut[end] = first;
    			}
    			visited[third] = 0;
    		}
    		visited[second] = 0;
    	}

    	for(int i=0; i<Feed_length[first]; i++)
        {
    		visitcut[Graph_Reverse[first][i].v] = -2;
            visit_end_w[Graph_Reverse[first][i].v] = Graph_Reverse[first][i].w; //记录反向的转账金额
        }

    }


    void dfs6(unsigned int &first)
    {
    	int p1,p2,p3,p4,p5,p6;
        register uint64_t cost;
        register uint64_t low_cost_X[7]; //[3]是临时变量
        register uint64_t high_cost_X[7];

        //a->b->c->d
    	for(int i=0; i<Graph_length[first]; i++)
    	{
    		p1 = Graph_Ordered[first][i].v;
    		if(vec_indegree[p1]==0)continue; 
    		visited[p1] = 1;
            //转账约束
            cost = Graph_Ordered[first][i].w;
            low_cost_X[0] = (cost % 5 !=0) ? (cost/5 + 1) : (cost/5);
            high_cost_X[0] = cost * 3;


    		for(int j=0; j<Graph_length[p1]; j++)
    		{
    			p2 = Graph_Ordered[p1][j].v;
    			if(visited[p2]==1)continue; 
                //转账约束
                cost = Graph_Ordered[p1][j].w;
                if(cost < low_cost_X[0] || cost > high_cost_X[0]) continue;
                low_cost_X[1] = (cost % 5 !=0) ? (cost/5 + 1) : (cost/5);
                high_cost_X[1] = cost * 3;
    			if(visitcut[p2]==-2 && cycle_check(visit_end_w[p2], Graph_Ordered[first][i].w, low_cost_X[1], high_cost_X[1]))
    			{
                    //A->B->C  1~
                    //c->a->b  1?
                    //B->C->A  1？
                    //用来检查c->a->b  [c,a]到[a,b]是否满足约束
                    //检查环的终点到起点之间是否满足约束
                    ans3_offest += int2str(ans3_buf+ans3_offest,ID_ARRAY[first],',');
                    ans3_offest += int2str(ans3_buf+ans3_offest,ID_ARRAY[p1],',');
                    ans3_offest += int2str(ans3_buf+ans3_offest,ID_ARRAY[p2],'\n');
    				ans3_cnt++;
    			}
    			visited[p2] = 1; 

    			for(int k=0; k<Graph_length[p2]; k++)
    			{
    				p3 = Graph_Ordered[p2][k].v;
    				if(vec_indegree[p3]==0)continue;
	    			if(visited[p3]==1)continue;
                    //转账约束
                    cost = Graph_Ordered[p2][k].w;
                    if(cost < low_cost_X[1] || cost > high_cost_X[1]) continue;
                    low_cost_X[2] = (cost % 5 !=0) ? (cost/5 + 1) : (cost/5);
                    high_cost_X[2] = cost * 3;                    

	    			if(visitcut[p3]==-2 && cycle_check(visit_end_w[p3], Graph_Ordered[first][i].w, low_cost_X[2], high_cost_X[2]))
	    			{
                        //检查环的终点到起点之间是否满足约束
                        //A->B->C->D 2~b->c->d
                        //D->A->B    1?
                        //C->D->A    1?                   
                        ans4_offest += int2str(ans4_buf+ans4_offest,ID_ARRAY[first],',');
                        ans4_offest += int2str(ans4_buf+ans4_offest,ID_ARRAY[p1],',');
                        ans4_offest += int2str(ans4_buf+ans4_offest,ID_ARRAY[p2],',');
                        ans4_offest += int2str(ans4_buf+ans4_offest,ID_ARRAY[p3],'\n');
    					ans4_cnt++;
	    			} 
	    			visited[p3] = 1; 

	    			for(int l=0; l<Graph_length[p3]; l++)
    				{
	    				p4 = Graph_Ordered[p3][l].v;
		    			if(visited[p4]==1 || (visitcut[p4]!=first && visitcut[p4]!=-2))continue; 
                        //转账约束
                        cost = Graph_Ordered[p3][l].w;
                        if(cost < low_cost_X[2] || cost > high_cost_X[2]) continue;
                        low_cost_X[3] = (cost % 5 !=0) ? (cost/5 + 1) : (cost/5);
                        high_cost_X[3] = cost * 3;  
		    			if(visitcut[p4]==-2 && cycle_check(visit_end_w[p4], Graph_Ordered[first][i].w, low_cost_X[3], high_cost_X[3]))
		    			{
                            //检查环的终点到起点之间是否满足约束
                            //A->B->C->D->E      3~b->c->d->e
                            //E->A->B            1?
                            //D->E->A            1?
                            ans5_offest += int2str(ans5_buf+ans5_offest,ID_ARRAY[first],',');
                            ans5_offest += int2str(ans5_buf+ans5_offest,ID_ARRAY[p1],',');
                            ans5_offest += int2str(ans5_buf+ans5_offest,ID_ARRAY[p2],',');
                            ans5_offest += int2str(ans5_buf+ans5_offest,ID_ARRAY[p3],',');
                            ans5_offest += int2str(ans5_buf+ans5_offest,ID_ARRAY[p4],'\n');
		    				ans5_cnt++;
	    			
		    			}
		    			visited[p4] = 1; 
		    			
		    			for(int m=0; m<Graph_length[p4]; m++)
		    			{
		    				p5 = Graph_Ordered[p4][m].v;
			    			if(visited[p5]==1 || (visitcut[p5]!=first && visitcut[p5]!=-2))continue;
                            //转账约束
                            cost = Graph_Ordered[p4][m].w;
                            if(cost < low_cost_X[3] || cost > high_cost_X[3]) continue;
                            low_cost_X[4] = (cost % 5 !=0) ? (cost/5 + 1) : (cost/5);
                            high_cost_X[4] = cost * 3;   

			    			if(visitcut[p5]==-2 && cycle_check(visit_end_w[p5], Graph_Ordered[first][i].w, low_cost_X[4], high_cost_X[4]))
			    			{
                                ans6_offest += int2str(ans6_buf+ans6_offest,ID_ARRAY[first],',');
                                ans6_offest += int2str(ans6_buf+ans6_offest,ID_ARRAY[p1],',');
                                ans6_offest += int2str(ans6_buf+ans6_offest,ID_ARRAY[p2],',');
                                ans6_offest += int2str(ans6_buf+ans6_offest,ID_ARRAY[p3],',');
                                ans6_offest += int2str(ans6_buf+ans6_offest,ID_ARRAY[p4],',');
                                ans6_offest += int2str(ans6_buf+ans6_offest,ID_ARRAY[p5],'\n');
		    					ans6_cnt++;
			    			}
			    			visited[p5] = 1; 

			    			for(int n=0; n<Graph_length[p5]; n++)
		    				{
			    				p6 = Graph_Ordered[p5][n].v;
				    			if(visited[p6]==1 || (visitcut[p6]!=first && visitcut[p6]!=-2))continue; 
                                 //转账约束
                                cost = Graph_Ordered[p5][n].w;
                                if(cost < low_cost_X[4] || cost > high_cost_X[4]) continue;
                                low_cost_X[5] = (cost % 5 !=0) ? (cost/5 + 1) : (cost/5);
                                high_cost_X[5] = cost * 3; 
				    			if(visitcut[p6]==-2 && cycle_check(visit_end_w[p6], Graph_Ordered[first][i].w, low_cost_X[5], high_cost_X[5]))
				    			{
                                    ans7_offest += int2str(ans7_buf+ans7_offest,ID_ARRAY[first],',');
                                    ans7_offest += int2str(ans7_buf+ans7_offest,ID_ARRAY[p1],',');
                                    ans7_offest += int2str(ans7_buf+ans7_offest,ID_ARRAY[p2],',');
                                    ans7_offest += int2str(ans7_buf+ans7_offest,ID_ARRAY[p3],',');
                                    ans7_offest += int2str(ans7_buf+ans7_offest,ID_ARRAY[p4],',');
                                    ans7_offest += int2str(ans7_buf+ans7_offest,ID_ARRAY[p5],',');
                                    ans7_offest += int2str(ans7_buf+ans7_offest,ID_ARRAY[p6],'\n');
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
    	for(unsigned int first=0; first<MAX_ID; first++)
    	{
    		if(vec_indegree[first]==0) continue;

    		neighbour_cut(first);

    		dfs6(first);

    		Top_sort(first);
    		//vec_indegree[first]=0;

    		for(int k=0; k<Feed_length[first]; k++)
    		{
    			visitcut[Graph_Reverse[first][k].v] = first;
    		}
    	}
    }

    void show_cycles()
    { 
        cout << "MAX_ID: " << MAX_ID <<endl; 
        cout << "REC_NUM: " << REC_NUM <<endl;    
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
        char buf[16];

        unsigned int idx=sprintf(buf,"%d\n",(ans3_cnt + ans4_cnt + ans5_cnt + ans6_cnt + ans7_cnt));
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
        register unsigned int num1 = 0;
        register unsigned int num2 = 0;
        register uint64_t num3 = 0;
        //遍历保存数据
        while(*(pointer) != 0 )
        {            
            num1 = 0;
            num2 = 0;
            num3 = 0;
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
            pointer++;

            //读取num3
            do{
                num3 = num3*10+(*pointer-48);
            }while(*(++pointer)!= '\r');  //\r pointer += 2;
            pointer += 2;

            ID_ARRAY[REC_NUM*2] = num1;
            ID_ARRAY[REC_NUM*2+1] = num2;
            ID1[REC_NUM] = num1;
            ID2[REC_NUM] = num2;
            COST[REC_NUM++] = num3;
        }
        MAX_ID ++;
        munmap(mmappedData, filesize);
        close(fd);
    }
};

int main()
{
#ifdef TEST
    const char* FILE_NAME = "/root/code/data_4/test_data.txt";
    string OUTPUT_FILE_NAME= "/root/code/data_4/output.txt";
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
