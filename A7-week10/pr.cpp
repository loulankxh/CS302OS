#include <iostream>
#include <map>
#include <queue>

using namespace std;

void FIFO();
void Min();
void LRU();
void Clock();

int k_size = 0, a_index = 0, n_num = 0, page = 0, hit = 0;

void fastRead(int& in){
    in=0;
    char ch=getchar();
    while(ch<'0'||ch>'9')ch=getchar();
    while(ch>='0'&&ch<='9')
    {
        in*=10;
        in+=ch-'0';
        ch=getchar();
    }
}

int main(){
    scanf("%d%d%d", &k_size, &a_index, &n_num);
    if(a_index == 0)
        FIFO();
    else if(a_index == 1)
        LRU();
    else if(a_index == 2)
        Min();
    else if(a_index == 3)
        Clock();
    printf("Hit ratio = %05.2lf%%", 100.0*hit/n_num);
    return 0;
}

void FIFO(){
    int queue[n_num+5];
    int front = 0, rear = 0, size = 0;
    map<int, int> contain;
    for(int q = 0; q < n_num; q++){
        fastRead(page);
        if(contain.count(page) == 1){
            hit ++;
            contain[page] = q;
            continue;
        }
        if(size == k_size)
            contain.erase(queue[front++]);
        else size++;
        contain.insert(pair<int ,int>(page, rear));
        queue[rear++] = page;
    }
}

struct cmp{
    template <typename T, typename U>
    bool operator()(T const &left, U const &right){ // descending
        if (right.second > left.second)
            return true;
        return false;
    }
};

void Min(){
    int query[n_num + 5];
    int next_q[n_num + 5];
    map<int, int> position; // page, index
    map<int, int> contain;
    int size = 0;
    priority_queue<pair<int, int>, vector<pair<int, int> >, cmp> max_q;
    for(int q = 0; q < n_num; q ++){
        fastRead(page);
        query[q] = page;
        next_q[q] = n_num;
        if(position.count(page) == 1){
            next_q[position[page]] = q;
            position[page] = q;
        }
        else
            position.insert(pair<int, int>(page, q));
    }
    for(int i = 0; i < n_num; i ++){
        page = query[i];
        if(contain.count(page) == 1){
            hit ++;
            max_q.push(pair<int, int>(page, next_q[i]));
            contain[page] = i;
            continue;
        }
        if(size == k_size){
            if(!max_q.empty()){
                pair<int, int> tmp = max_q.top();
                max_q.pop();
                while(tmp.second <= i){
                    // actually this will not happen
                    tmp = max_q.top();
                    max_q.pop();
                }
                contain.erase(tmp.first);
            }
        }
        else size++;
        contain.insert(pair<int, int>(page, i));
        max_q.push(pair<int, int>(page, next_q[i]));
    }
}

void LRU(){
    int query[n_num + 5];
    for(int q = 0; q < n_num; q ++) {
        fastRead(page);
        query[q] = page;
    }
    int valid[n_num+5];
    for(int i = 0; i < n_num +5; i++){
        valid[i] = 1;
    }
    int pointer = 0, size = 0;
    map<int, int> contain;
    for(int q = 0; q < n_num; q++){
        page = query[q];
        if(contain.count(page) == 1){
            hit ++;
            valid[contain[page]] = 0;
            contain[page] = q;
            continue;
        }
        if(size == k_size){
            while(valid[pointer++] == 0);
            contain.erase(query[pointer - 1]);
        }
        else size++;
        contain.insert(pair<int, int>(page, q));
    }

}

void Clock(){
    int value[k_size + 5];
    int valid[k_size + 5];
    for(int i = 0 ; i < k_size + 5; i ++){
        value[i] = 0;
        valid[i] = 0;
    }
    map<int, int> contain;
    int pointer = 0;
    for(int q = 0; q < n_num; q ++){
        fastRead(page);
        if(contain.count(page) == 1){
            hit ++;
            valid[contain[page]] = 1;
            continue;
        }
        while(valid[pointer] == 1){
            valid[pointer++] = 0;
            if(pointer >= k_size)
                pointer -= k_size;
        }
        if(contain.count(value[pointer]) == 1)
            contain.erase(value[pointer]);
        value[pointer] = page;
        valid[pointer] = 1;
        contain.insert(pair<int, int>(page, pointer++));
        if(pointer >= k_size)
            pointer -= k_size;
    }
}
