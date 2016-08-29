#include <iostream>
#include <thread>

using namespace std;

void hello(){
    while(true){
        cout << "hello thread." << endl;
    }
}
void hello2(){
    while(true){
        cout << "hello2 thread." << endl;
    }
}
int main1(){
    thread t(hello);
    thread t2(hello2);
    t.join();
    t2.join();
}
