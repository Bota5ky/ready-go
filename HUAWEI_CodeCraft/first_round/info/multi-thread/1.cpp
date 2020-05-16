#include <iostream>
#include <thread>

using namespace std;

int main(){
    using namespace std::chrono;//关于时间的都在chrono内
    auto start=system_clock::now();
    //func
    duration<double> diff=system_clock::now()-start;
    cout << "elapsed : " << diff.count()<< "seconds" <<endl;
}