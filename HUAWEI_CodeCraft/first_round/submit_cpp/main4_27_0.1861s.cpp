#include <iostream>
#include <ctime>
#include <queue>
#include <map>
#include <pthread.h>
#include <algorithm>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

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

//正向路径 邻接表
int Graph_length[50001] ;
int Graph_Adjacency[50001][30];
//反向路径
int Feed_length[50001] ;
int Feed_Link_2[50001][30];

int task_stack[50001];
int task_stack_top=0;

int ans3_cnt_total = 0;
int ans4_cnt_total = 0;
int ans5_cnt_total = 0;
int ans6_cnt_total = 0;
int ans7_cnt_total = 0;


int ans3_cnt[4] = {0};
int ans4_cnt[4] = {0};
int ans5_cnt[4] = {0};
int ans6_cnt[4] = {0};
int ans7_cnt[4] = {0};
int ans3_offest_base[4] = {0,7500000,15000000,22500000};
int ans4_offest_base[4] = {0,10000000,20000000,30000000};
int ans5_offest_base[4] = {0,25000000,50000000,75000000};
int ans6_offest_base[4] = {0,60000000,120000000,180000000};
int ans7_offest_base[4] = {0,105000000,210000000,315000000};
int ans3_offest[4] = {0};
int ans4_offest[4] = {0};
int ans5_offest[4] = {0};
int ans6_offest[4] = {0};
int ans7_offest[4] = {0};


char ans3_buf[30000000];
char ans4_buf[40000000];
char ans5_buf[100000000];
char ans6_buf[240000000];
char ans7_buf[420000000];

//临时变量
unsigned int MAX_ID = 0;
unsigned int NUM_ID = 0;

inline int int2str(char *str,int num, char c_end)
    {
        int count = 0;
        char temp[5];
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

    //3邻域剪枝
void neighbour_cut(int &first,int visited[], int visitcut[])
    {
    	register int second,third,end;
    	visited[first] = 1;

    	for(int i=0; i<Feed_length[first]; i++)
    	{
            second = Feed_Link_2[first][i];
            if(first > second) continue;	
    		visited[second] = 1;
    		for(int j=0; j<Feed_length[second]; j++)
    		{
                third =  Feed_Link_2[second][j];
                if(first > third) continue;    
    			if(visited[third]==1)continue;  
    			visitcut[third] = first;
    			visited[third] = 1;

    			for(int k=0; k<Feed_length[third]; k++)
    			{
                    end = Feed_Link_2[third][k];
                    if(first > end) continue;    
    				if(visited[end]==1)continue;  
    				visitcut[end] = first;
    			}
    			visited[third] = 0;
    		}
    		visited[second] = 0;
    	}

        for(int i=0; i<Feed_length[first];i++)
        {
            if(first < Feed_Link_2[first][i])
                visitcut[Feed_Link_2[first][i]] = -2;
        }
    }


void dfs6(int &first,int visited[], int visitcut[],int tid)
    {
    	register int p1,p2,p3,p4,p5,p6;
        register int temp=0;
        register char *pointer;

    	for(int i=0; i<Graph_length[first]; i++)
    	{
            p1 = Graph_Adjacency[first][i]; 
            if(first > p1) continue;
    		visited[p1] = 1;

    		for(int j=0; j<Graph_length[p1]; j++)
    		{
                p2 = Graph_Adjacency[p1][j];
                if(first > p2) continue;
                if(visited[p2]==1)continue; 
    			if(visitcut[p2]== -2)
    			{
                    pointer = ans3_buf + ans3_offest_base[tid];
                    ans3_offest[tid] += int2str(pointer+ans3_offest[tid],first,',');
                    ans3_offest[tid] += int2str(pointer+ans3_offest[tid],p1,',');
                    ans3_offest[tid] += int2str(pointer+ans3_offest[tid],p2,'\n');
    				ans3_cnt[tid]++;
    			}
    			visited[p2] = 1; 

    			for(int k=0; k<Graph_length[p2]; k++)
    			{
                    p3 = Graph_Adjacency[p2][k];
                    if(first > p3) continue;
                    if(visited[p3]==1)continue;				
	    			if(visitcut[p3]== -2)
	    			{
                        pointer = ans4_buf + ans4_offest_base[tid];
                        ans4_offest[tid] += int2str(pointer+ans4_offest[tid],first,',');
                        ans4_offest[tid] += int2str(pointer+ans4_offest[tid],p1,',');
                        ans4_offest[tid] += int2str(pointer+ans4_offest[tid],p2,',');
                        ans4_offest[tid] += int2str(pointer+ans4_offest[tid],p3,'\n');
    					ans4_cnt[tid]++;
	    			} 
	    			visited[p3] = 1; 
	    			
	    			for(int l=0; l<Graph_length[p3]; l++)
    				{
	    				p4 = Graph_Adjacency[p3][l];
                        temp = visitcut[p4];
		    			if(visited[p4]==1 || (temp!=first && temp!=-2))continue; 
		    			if(temp==-2)
		    			{
                            pointer = ans5_buf + ans5_offest_base[tid];
                            ans5_offest[tid] += int2str(pointer+ans5_offest[tid],first,',');
                            ans5_offest[tid] += int2str(pointer+ans5_offest[tid],p1,',');
                            ans5_offest[tid] += int2str(pointer+ans5_offest[tid],p2,',');
                            ans5_offest[tid] += int2str(pointer+ans5_offest[tid],p3,',');
                            ans5_offest[tid] += int2str(pointer+ans5_offest[tid],p4,'\n');
		    				ans5_cnt[tid]++;
	    			
		    			}
		    			visited[p4] = 1; 
		    			
		    			for(int m=0; m<Graph_length[p4]; m++)
		    			{
		    				p5 = Graph_Adjacency[p4][m];
                            temp = visitcut[p5];
			    			if(visited[p5]==1 || (temp!=first && temp!=-2))continue; 
                            if(temp==-2)
			    			{
                                pointer = ans6_buf + ans6_offest_base[tid];
                                ans6_offest[tid] += int2str(pointer+ans6_offest[tid],first,',');
                                ans6_offest[tid] += int2str(pointer+ans6_offest[tid],p1,',');
                                ans6_offest[tid] += int2str(pointer+ans6_offest[tid],p2,',');
                                ans6_offest[tid] += int2str(pointer+ans6_offest[tid],p3,',');
                                ans6_offest[tid] += int2str(pointer+ans6_offest[tid],p4,',');
                                ans6_offest[tid] += int2str(pointer+ans6_offest[tid],p5,'\n');
		    					ans6_cnt[tid]++;
			    			}
			    			visited[p5] = 1; 

			    			for(int n=0; n<Graph_length[p5]; n++)
		    				{
			    				p6 = Graph_Adjacency[p5][n];
				    			temp = visitcut[p6];
                                if(visited[p6]==1 || (temp!=first && temp!=-2))continue; 
                                if(temp==-2)
				    			{
                                    pointer = ans7_buf + ans7_offest_base[tid];
                                    ans7_offest[tid] += int2str(pointer+ans7_offest[tid],first,',');
                                    ans7_offest[tid] += int2str(pointer+ans7_offest[tid],p1,',');
                                    ans7_offest[tid] += int2str(pointer+ans7_offest[tid],p2,',');
                                    ans7_offest[tid] += int2str(pointer+ans7_offest[tid],p3,',');
                                    ans7_offest[tid] += int2str(pointer+ans7_offest[tid],p4,',');
                                    ans7_offest[tid] += int2str(pointer+ans7_offest[tid],p5,',');
                                    ans7_offest[tid] += int2str(pointer+ans7_offest[tid],p6,'\n');
		    						ans7_cnt[tid]++;
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

#define NUM_THREADS 4 
int table[5]={0,1,3,5,23};    

    void *findcycles(void *threadid)
    {
        int tid = *((int*)threadid);
        int visitcut[MAX_ID];
        int visited[MAX_ID+1];
        int thread_step = task_stack_top / 32;
        int i = table[tid]*thread_step;
        int end = tid==3?task_stack_top:(table[tid+1])*thread_step;
        int first;

    	for(; i<end; i++)
    	{
            first = task_stack[i];

    		neighbour_cut(first,visited,visitcut);

    		dfs6(first,visited,visitcut,tid);

            for(int i=0; i<Feed_length[first];i++)
            {
                visitcut[Feed_Link_2[first][i]] = first;
            }
    	}
        pthread_exit(NULL);
    }

    void test()
    {
       pthread_t threads[NUM_THREADS];
       int indexes[NUM_THREADS];// 用数组来保存i的值
       int rc;
       int i;

       pthread_attr_t attr;
       // 初始化并设置线程为可连接的（joinable）
       pthread_attr_init(&attr);
       pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

       for( i=0; i < NUM_THREADS; i++ ){      
          indexes[i] = i; //先保存i的值
          // 传入的时候必须强制转换为void* 类型，即无类型指针        
          rc = pthread_create(&threads[i], NULL, 
                              findcycles, (void *)&(indexes[i]));
       }

       void *status;
       pthread_attr_destroy(&attr);
       for( i=0; i < NUM_THREADS; i++ )
       {
          rc = pthread_join(threads[i], &status);
       }
    }

    void total_cycles()
    {
        for(int i=0;i<4;i++)
        {
            ans3_cnt_total += ans3_cnt[i];
            ans4_cnt_total += ans4_cnt[i];
            ans5_cnt_total += ans5_cnt[i];
            ans6_cnt_total += ans6_cnt[i];
            ans7_cnt_total += ans7_cnt[i];
        }

    }

    void show_cycles()
    {
        cout << "MAX_ID: " << MAX_ID <<endl;   
        cout << "3 cycles: " << ans3_cnt_total << endl;
        cout << "4 cycles: " << ans4_cnt_total << endl;
        cout << "5 cycles: " << ans5_cnt_total << endl;
        cout << "6 cycles: " << ans6_cnt_total << endl;
        cout << "7 cycles: " << ans7_cnt_total << endl;
        cout << "Total cycles: " << ans3_cnt_total + ans4_cnt_total + ans5_cnt_total + ans6_cnt_total + ans7_cnt_total << endl;
    }

    void save(const string &outputFile)
    {

        FILE *fp = fopen(outputFile.c_str(), "wb");
        char buf[10];
        int i=0;
        int idx = int2str(buf,(ans3_cnt_total + ans4_cnt_total + ans5_cnt_total + ans6_cnt_total + ans7_cnt_total),'\n');
        fwrite(buf, idx , 1, fp);
        for(i=0;i<4;i++)
            fwrite(ans3_buf+ans3_offest_base[i], ans3_offest[i] , 1, fp );
        for(i=0;i<4;i++)
            fwrite(ans4_buf+ans4_offest_base[i], ans4_offest[i] , 1, fp );
        for(i=0;i<4;i++)
            fwrite(ans5_buf+ans5_offest_base[i], ans5_offest[i] , 1, fp );
        for(i=0;i<4;i++)
            fwrite(ans6_buf+ans6_offest_base[i], ans6_offest[i] , 1, fp );
        for(i=0;i<4;i++)
            fwrite(ans7_buf+ans7_offest_base[i], ans7_offest[i] , 1, fp );
        fclose(fp);
    }

    void load_data(const char *testFile)
    {
        struct stat st;
        stat(testFile,&st);
        size_t filesize = st.st_size;
        int fd = open(testFile, O_RDONLY, 0);   
        unsigned char* mmappedData;
        mmappedData = (unsigned char*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    
        unsigned char* pointer =  mmappedData;
        int num1,num2;
        //遍历保存数据
        while(*(pointer) != 0 )
        {            
            num1 = 0;
            num2 = 0;
            //读取num1
            do{
                num1=num1*10+ (*pointer-48);
            }while(*(++pointer) != ',');
            
            pointer++;

            //读取num2
            do{
                num2=num2*10+ (*pointer-48);
            }while(*(++pointer) != ',');
         
            //跳转到下一行读取
            while(*(pointer++)!= 10);
            if(num1>50000 || num2>50000) continue;
            MAX_ID = MAX_ID > num1 ? MAX_ID : num1;
            MAX_ID = MAX_ID > num2 ? MAX_ID : num2;
            Graph_Adjacency[num1][Graph_length[num1]++] = num2;
            Feed_Link_2[num2][Feed_length[num2]++] = num1;
        }
        MAX_ID ++;
        munmap(mmappedData, filesize);
        close(fd);
    }


int main()
{
#ifdef TEST
    const char* FILE_NAME = "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/初赛/test_data_3w.txt";
    string OUTPUT_FILE_NAME= "/Users/CYJ/Desktop/GitHub/huawei2020/初赛/new algorithm/output.txt";
    clock_t time_start=clock();
#else
    const char* FILE_NAME = "/data/test_data.txt";
    string OUTPUT_FILE_NAME= "/projects/student/result.txt"; 
#endif

    RUN_TIME(load_data(FILE_NAME));
    //RUN_TIME(All_Top_sort());

    //邻接表排序
    for(int i=0; i<MAX_ID; i++)
    {
        if(Graph_length[i] > 0 && Feed_length[i] > 0)
        {  
            sort(Graph_Adjacency[i],Graph_Adjacency[i]+Graph_length[i]);
            sort(Feed_Link_2[i],Feed_Link_2[i]+Feed_length[i]);
            task_stack[task_stack_top++] = i;
        }
    }

    RUN_TIME(test());
    total_cycles();
    RUN_TIME(save(OUTPUT_FILE_NAME));
      
    
#ifdef TEST
    RUN_TIME(show_cycles());
    clock_t time_end=clock();
    cout <<"#Time use:"<<(time_end-time_start)/(double)CLOCKS_PER_SEC<<"s"<<endl;
    exit(0);
#else
    exit(0);
#endif

    return 0;
}
