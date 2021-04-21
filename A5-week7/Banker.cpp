#include <iostream>
#include<cstdio>
#include <map>
#include<vector>

using namespace std;

struct process{
    int id = -1;
    vector<int> allocated;
    vector<int> max;
};

process list_p[10005];
map<int, int> hashing;    // process_id, index(num)
int num = 0; // number of processes alive
int visit[10005];
int kill[10005];

int main(){
    for(int i = 0; i < 10005; i ++){
        visit[i] = 0;
        kill[i] = 0;
    }
    int kind;
    scanf("%d", &kind);
    int maximum[kind];
    int remain[kind];
    int need[kind]; // used in request
    for(int i = 0; i < kind ; i ++){
        scanf("%d", &remain[i]);
        maximum[i] = remain[i];
    }

    int process_id;
    char type[10];
    while(scanf("%d", &process_id) !=  EOF){
//        printf("%d\n", process_id);
// if the input is string, then the previous number is kept for process_id
        int flag = 1;
        scanf("%s", type);
        if(type[0] == 'n'){
            int allocated[kind];
            int max[kind];
            for(int i = 0; i < kind; i++){     // maximum need > maximum quantity of each resource, NOT OK
                scanf("%d", &max[i]);
                allocated[i] = 0;
                if(max[i] > maximum[i])
                    flag = 0;
            }   // finish reading
            if(hashing.count(process_id) == 1){    // not a valid process
                printf("%s\n", "NOT OK");
                continue;
            }
            if(flag == 0) printf("%s\n", "NOT OK");
            else {
                process *new_p = new process();
                new_p->id = process_id;
                for(int i = 0; i < kind; i++){
                    new_p->allocated.push_back(allocated[i]);
                    new_p->max.push_back(max[i]);
                }
                hashing.insert(pair<int, int>(process_id, num));
                list_p[num++] = *new_p;
                printf("%s\n", "OK");
            }
        }
        else if(type[0] == 'r'){
            for(int i = 0; i < kind; i ++){
                scanf("%d", &need[i]);
            }   // finish reading
            if(hashing.count(process_id) == 0){    // not a valid process
                printf("%s\n", "NOT OK");
                continue;
            }
            process *p = &list_p[hashing[process_id]];
            for(int i = 0; i < kind; i++){ // not exceed limit
                if(need[i] > remain[i] || need[i] > (p->max[i] - p->allocated[i])){
                    flag = 0;
                    break;
                }
            }
            if(flag == 1){
                for(int i = 0; i < kind; i ++){
                    remain[i] -= need[i];
                    p->allocated[i] += need[i];
                }
                //check deadlock
                int copy_res[kind];
                for(int i = 0 ; i < kind; i ++){
                    copy_res[i] = remain[i];
                }
                for(int i = 0; i < num; i ++){
                    visit[i] = 0;
                }
                while(true){
                    process *tmp;
                    int choice = -1;
                    for(int i = 0; i < num; i++){
                        if(visit[i] == 0 && kill[i] == 0){
                            flag = 1;
                            tmp = &list_p[i];
                            for(int j = 0; j < kind; j++){
                                if(tmp->max[j] - tmp->allocated[j] > copy_res[j]){
                                    flag = 0;
                                    break;
                                }
                            }
                            if(flag == 1){
                                choice = i;
                                for(int j = 0; j < kind; j++){
                                    copy_res[j] += tmp->allocated[j];
                                }
                                visit[i] = 1;
                            }
                        }
                    }
                    if(choice == -1)
                        break;
                }
                if(flag == 0){  // unsuccessfully
                    for(int i = 0; i < kind; i++){
                        p->allocated[i] -= need[i];
                        remain[i] += need[i];
                    }
                }
            }
            if(flag == 0) printf("%s\n", "NOT OK");
            else printf("%s\n", "OK");
        }
        else if(type[0] == 't'){
            if(hashing.count(process_id) == 0){    // not a valid process
                printf("%s\n", "NOT OK");
                continue;
            }
            process *p = &list_p[hashing[process_id]];
            for(int i = 0; i < kind; i++){
                remain[i] += p->allocated[i];
                p->allocated[i] = 0;
                p->max[i] = 0;
            }
            kill[hashing[process_id]] = 1;
            hashing.erase(process_id);
            printf("%s\n", "OK");
        }
    }
}