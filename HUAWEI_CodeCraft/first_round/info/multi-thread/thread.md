### pthread_attr_t  线程属性
```c++
typedef struct
{
            int                 detachstate;   线程的分离状态
            int                 schedpolicy;   线程调度策略
            struct sched_param   schedparam;   线程的调度参数
            int                inheritsched;   线程的继承性
            int                       scope;   线程的作用域
            size_t                guardsize;   线程栈末尾的警戒缓冲区大小
            int               stackaddr_set;
            void *                stackaddr;   线程栈的位置
            size_t                stacksize;   线程栈的大小
} pthread_attr_t;
```
[各属性具体含义点击链接](https://blog.csdn.net/hudashi/article/details/7709413)
主要函数：
**1、pthread_attr_init**

```c++
int pthread_attr_init (pthread_attr_t* attr);
```
功能：            对线程属性变量的初始化。
函数传入值：attr 线程属性的指针
函数返回值：成功： 0 ；失败： -1
**2、pthread_attr_setdetachstate**

```c++
int pthread_attr_setdetachstate (pthread_attr_t* attr, int detachstate);
```
功能：设置线程detachstate属性。该表示新线程是否与进程中其他线程脱离同步，如果设置为PTHREAD_CREATE_DETACHED则新线程不能用pthread_join()来同步，且在退出时自行释放所占用的资源。缺省为PTHREAD_CREATE_JOINABLE状态。这个属性也可以在线程创建并运行以后用pthread_detach()来设置，而一旦设置为PTHREAD_CREATE_DETACH状态（不论是创建时设置还是运行时设置）则不能再恢复到PTHREAD_CREATE_JOINABLE状态。
函数传入值：attr 线程属性的指针
detachstate：PTHREAD_CREATE_DETACHED，不能用pthread_join()来同步，且在退出时自行释放所占用的资源；PTHREAD_CREATE_JOINABLE，能用pthread_join()来同步
函数返回值：成功： 0 ；失败： -1

```cpp
 pthread_attr_t attr;
 pthread_attr_init(&attr);
 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);//0值 ddd使用的初始化
```
### pthread_create  创建线程
```c++
int pthread_create(pthread_t *restrict tidp,const pthread_attr_t *restrict_attr,void*（*start_rtn)(void*),void *restrict arg);
```
函数传入值：第一个参数为指向线程标识符的指针；第二个参数用来设置线程属性；第三个参数是线程运行函数的地址；最后一个参数是运行函数的参数。
函数返回值：成功： 0 ；否则返回出错编号。

```cpp
int rc = pthread_create(&tids[tc], NULL, magic_train, (void *)&(infos[tc]));
if (rc) {
      cerr << "Wrong!" << endl;
      exit(-1);
}//ddd的程序line_95
```

### pthread_t  声明线程ID

```cpp
typedef unsigned long int pthread_t;
```

### pthread_join  等待一个线程的结束

```cpp
int pthread_join(pthread_t thread, void **value_ptr);
//thread：等待退出线程的线程ID。
//value_ptr：用户定义的指针，用来存储被等待线程的返回值。
```

返回值 ： 0代表成功。 失败，返回的则是错误号。

### pthread_attr_destroy

```cpp
int pthread_attr_destroy(pthread_attr_t *attr);
```

销毁一个目标结构，并且使它在重新初始化之前不能重新使用

When a thread attributes object is no longer required, it should be destroyed using the pthread_attr_destroy() function.  Destroying a thread attributes object has no effect on threads that were created using that object.
注意：属性对象必须初始化，否则属性不能生效，创建线程时将返回错误。
属性对象被销毁，并不影响线程的属性。
pthread_attr_init是初始化一个线程对象的属性,需要用pthread_attr_destroy对其去除初始化
int pthread_attr_init(pthread_attr_t attr);
返回0，表示函数初始化对象成功。失败时返回一个错误代码。
参数：指向一个线程属性的指针。
attr对应单词attribute 即属性的意思
init对应单词initialize 即初始化的意思

函数返回值：成功： 0 ；否则返回出错编号

### 基础实例

```c++
#include <iostream>
// 必须的头文件
#include <pthread.h>
 
using namespace std;
 
#define NUM_THREADS 5
 
// 线程的运行函数
void* say_hello(void* args)
{
    cout << "Hello Runoob！" << endl;
    return 0;
}
 
int main()
{
    // 定义线程的 id 变量，多个变量使用数组
    pthread_t tids[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; ++i)
    {
        //参数依次是：创建的线程id，线程参数，调用的函数，传入的函数参数
        int ret = pthread_create(&tids[i], NULL, say_hello, NULL);
        if (ret != 0)
        {
           cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
    //等各个线程退出后，进程才结束，否则进程强制结束了，线程可能还没反应过来；
    pthread_exit(NULL);
}
```

```c++
使用 -lpthread 库编译下面的程序：

$ g++ test.cpp -lpthread -o test.o
现在，执行程序，将产生下列结果：

$ ./test.o
Hello Runoob！
Hello Runoob！
Hello Runoob！
Hello Runoob！
Hello Runoob！
```

### 线程传入参数必须强制转换为void* 类型，即无类型指针  

```c++
//文件名：test.cpp
 
#include <iostream>
#include <cstdlib>
#include <pthread.h>
 
using namespace std;
 
#define NUM_THREADS 5
 
void *PrintHello(void *threadid)
{  
   // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
   int tid = *((int*)threadid);
   cout << "Hello Runoob! 线程 ID, " << tid << endl;
   pthread_exit(NULL);
}
 
int main ()
{
   pthread_t threads[NUM_THREADS];
   int indexes[NUM_THREADS];// 用数组来保存i的值
   int rc;
   int i;
   for( i=0; i < NUM_THREADS; i++ ){      
      cout << "main() : 创建线程, " << i << endl;
      indexes[i] = i; //先保存i的值
      // 传入的时候必须强制转换为void* 类型，即无类型指针        
      rc = pthread_create(&threads[i], NULL, PrintHello, (void *)&(indexes[i]));
      if (rc){
         cout << "Error:无法创建线程," << rc << endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}
```

```c++
$ g++ test.cpp -lpthread -o test.o
$ ./test.o
main() : 创建线程, 0
main() : 创建线程, 1
Hello Runoob! 线程 ID, 0
main() : 创建线程, Hello Runoob! 线程 ID, 21

main() : 创建线程, 3
Hello Runoob! 线程 ID, 2
main() : 创建线程, 4
Hello Runoob! 线程 ID, 3
Hello Runoob! 线程 ID, 4
```

### 传入多个参数

```c++
#include <iostream>
#include <cstdlib>
#include <pthread.h>
 
using namespace std;
 
#define NUM_THREADS 5
 
struct thread_data{
   int  thread_id;
   char *message;
};
 
void *PrintHello(void *threadarg)
{
   struct thread_data *my_data;
 
   my_data = (struct thread_data *) threadarg;
 
   cout << "Thread ID : " << my_data->thread_id ;
   cout << " Message : " << my_data->message << endl;
 
   pthread_exit(NULL);
}
 
int main ()
{
   pthread_t threads[NUM_THREADS];
   struct thread_data td[NUM_THREADS];
   int rc;
   int i;
 
   for( i=0; i < NUM_THREADS; i++ ){
      cout <<"main() : creating thread, " << i << endl;
      td[i].thread_id = i;
      td[i].message = (char*)"This is message";
      rc = pthread_create(&threads[i], NULL,
                          PrintHello, (void *)&td[i]);
      if (rc){
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}
```

```c++
$ g++ -Wno-write-strings test.cpp -lpthread -o test.o
$ ./test.o
main() : creating thread, 0
main() : creating thread, 1
Thread ID : 0 Message : This is message
main() : creating thread, Thread ID : 21
 Message : This is message
main() : creating thread, 3
Thread ID : 2 Message : This is message
main() : creating thread, 4
Thread ID : 3 Message : This is message
Thread ID : 4 Message : This is message
```

### 连接和分离线程

```c++
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
 
using namespace std;
 
#define NUM_THREADS 5
 
void *wait(void *t)
{
   int i;
   long tid;
 
   tid = (long)t;
 
   sleep(1);
   cout << "Sleeping in thread " << endl;
   cout << "Thread with id : " << tid << "  ...exiting " << endl;
   pthread_exit(NULL);
}
 
int main ()
{
   int rc;
   int i;
   pthread_t threads[NUM_THREADS];
   pthread_attr_t attr;
   void *status;
 
   // 初始化并设置线程为可连接的（joinable）
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
 
   for( i=0; i < NUM_THREADS; i++ ){
      cout << "main() : creating thread, " << i << endl;
      rc = pthread_create(&threads[i], NULL, wait, (void *)&i );
      if (rc){
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
 
   // 删除属性，并等待其他线程
   pthread_attr_destroy(&attr);
   for( i=0; i < NUM_THREADS; i++ ){
      rc = pthread_join(threads[i], &status);
      if (rc){
         cout << "Error:unable to join," << rc << endl;
         exit(-1);
      }
      cout << "Main: completed thread id :" << i ;
      cout << "  exiting with status :" << status << endl;
   }
 
   cout << "Main: program exiting." << endl;
   pthread_exit(NULL);
}
```

```c++
main() : creating thread, 0
main() : creating thread, 1
main() : creating thread, 2
main() : creating thread, 3
main() : creating thread, 4
Sleeping in thread 
Thread with id : 4  ...exiting 
Sleeping in thread 
Thread with id : 3  ...exiting 
Sleeping in thread 
Thread with id : 2  ...exiting 
Sleeping in thread 
Thread with id : 1  ...exiting 
Sleeping in thread 
Thread with id : 0  ...exiting 
Main: completed thread id :0  exiting with status :0
Main: completed thread id :1  exiting with status :0
Main: completed thread id :2  exiting with status :0
Main: completed thread id :3  exiting with status :0
Main: completed thread id :4  exiting with status :0
Main: program exiting.
```

