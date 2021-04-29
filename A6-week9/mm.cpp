#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址
#define DEFAULT_ALG 3  // Define first-fit as the default allocate algorithm

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
My_algo algo[123];

struct free_block{	//空闲数据块
	int size;
	int start_addr;
	struct free_block *next;
};

struct allocated_block{ //已分配的数据块
	int pid;
	int size;
	int start_addr;
	int *data;
	struct allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
void rearrange(); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id); //使用对应的分配算法

int alg = DEFAULT_ALG;
//主函数
int main(){
	int op;
	pid = 0;
	free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	for(;;){
		sleep(1);
		display_menu();
		fflush(stdin);
		scanf("%d", &op);
		switch (op){
			case 1:{ set_mem_size(); break; }
			case 2:{
				printf("Choose an algorithm\n");
				printf("1: Best Fit\n 2: Worst Fit\n 3: First Fit\n 4: Buddy System\n");
				scanf("%d", &alg);
				break;
			}
			case 3:{ create_new_process(); break; }
			case 4:{ kill_process(); break; }
			case 5:{ display_mem_usage(); break; }
			case 233:{ puts("bye...."); sleep(1); return 0; }
			defaut: break;
		}
	}
}

allocated_block *find_process(int id){ //循环遍历分配块链表，寻找pid=id的进程所对应的块
    allocated_block *p = allocated_block_head;
    for(;p != NULL; p = p->next){
        if(p->pid == id) return p;
    }
    return NULL;
}

free_block *init_free_block(int mem_size){ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	if (p == NULL){
		puts("No memory left");
		return NULL;
	}
	p->size = mem_size;
	p->start_addr = DEFAULT_MEM_START;
	p->next = NULL;
	return p;
}

void display_menu(){
	puts("\n\n******************menu*******************");
	printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
	printf("2) Set memory allocation algorithm\n");
	printf("3) Create a new process\n");
	printf("4) Kill a process\n");
	printf("5) Display memory usage\n");
	printf("233) Exit\n");
}

void set_mem_size(){ //更改最大内存大小
    if(flag || allocated_block_head != NULL) return;
    int size = 0;
    scanf("%d", &size);
    if(size > 0){
        flag = true;
        mem_size = size;
        free(free_block_head);
        free_block_head = init_free_block(mem_size);
    }
}

free_block *allocated_best_fit(int size){
    free_block *p = free_block_head;
    free_block *ans = NULL;
    for(; p != NULL; p = p->next){
        if(p->size >= size){
            if(ans == NULL)
                ans = p;
            else if(p->size < ans->size)
                ans = p;
        }
    }
    return ans;
}

free_block *allocated_worst_fit(int size){
    free_block *p = free_block_head;
    free_block *ans = NULL;
    for(; p != NULL; p = p->next){
        if(p->size >= size) {
            if(ans == NULL)
                ans = p;
            else if(p->size > ans->size)
                ans = p;
        }
    }
    return ans;
}

free_block *allocated_first_fit(int size){
    free_block *p = free_block_head;
    for(; p != NULL; p = p->next){
        if(p->size >= size) return p;
    }
    return NULL;
}

int allocate_mem(allocated_block *ab){ //为块分配内存，真正的操作系统会在这里进行置换等操作
    free_block *freed_block = NULL;
    if(alg == 1)
        freed_block = allocated_best_fit(ab->size);
    if(alg == 2)
        freed_block = allocated_worst_fit(ab->size);
    if(alg == 3)
        freed_block = allocated_first_fit(ab->size);
    if (freed_block == NULL)
        return 0;
    ab->pid = ++pid;
    ab->start_addr = freed_block->start_addr;
    if(allocated_block_head == NULL)
        allocated_block_head = ab;
    else{
    	allocated_block *head = allocated_block_head;
    	for(; head != NULL && head->next != NULL; head = head->next);
        head->next = ab;
    }
    freed_block->start_addr += ab->size;
    freed_block->size -= ab->size;
    if(freed_block->size == 0){
        if(freed_block == free_block_head){
            free_block_head = free_block_head->next;
        } else{
            free_block *p = free_block_head;
            for(; p != NULL; p = p->next){
                if(p->next == freed_block){
                    p->next = freed_block->next;
                    break;
                }
            }
        }
        free(freed_block);
        freed_block = NULL;
    }
    return 1;
}

int create_new_process(){ //创建新进程
	int mem_sz = 0;
	printf("Please input memory size\n");
    scanf("%d", &mem_sz);
    // Write your code here
    if(mem_sz > 0 && mem_sz <= mem_size && free_block_head != NULL){
        allocated_block *new_allocated = (allocated_block *)malloc(sizeof(allocated_block));
        new_allocated->pid = 0;
        new_allocated->size = mem_sz;
        new_allocated->start_addr = 0;
        new_allocated->data = NULL;
        new_allocated->next = NULL;
        int f = allocate_mem(new_allocated);
        if(f == 0){
            free(new_allocated);
            new_allocated = NULL;
        }
        else
            rearrange();
    }
}

void swap(int *p, int *q){
	int tmp = *p;
	*p = *q;
	*q = tmp;
	return;
}

void rearrange(){ //将块按照地址大小进行排序
	free_block *tmp, *tmpx;
	puts("Rearrange begins...");
	puts("Rearrange by address...");
	tmp = free_block_head;
	while(tmp != NULL){
		tmpx = tmp->next;
		while (tmpx != NULL){
			if (tmpx->start_addr < tmp->start_addr){
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
    free_block *p = free_block_head, *p_next = NULL;
    for(; p != NULL;){
        p_next = p->next;
        if(p->next != NULL){
        	if(p->start_addr + p->size == p_next->start_addr){
            	p->size += p_next->size;
            	p->next = p_next->next;
            	free(p_next);
            	p_next = NULL;
            }	
            else
            	p = p->next;
		}
		else
			p = p->next;
    }
	usleep(500);
	puts("Rearrange Done.");
}

int free_mem(allocated_block *ab){ //释放某一块的内存
    if(ab == NULL) return 0;
    free_block *new_free = (free_block *)malloc(sizeof(free_block));
    new_free->size = ab->size;
    new_free->start_addr = ab->start_addr;
    new_free->next = NULL;
    if(free_block_head == NULL)
        free_block_head = new_free;
    else{
        new_free->next = free_block_head;
        free_block_head = new_free;
    }
    rearrange();
}

int dispose(allocated_block *fab){ //释放结构体所占的内存
	allocated_block *pre, *ab;
	if (fab == allocated_block_head){
		allocated_block_head = allocated_block_head->next;
		free(fab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while (ab != fab){ pre = ab; ab = ab->next;}
	pre->next = ab->next;
	free(ab);
	return 2;
}

void display_mem_usage(){
	free_block *fb = free_block_head;
	allocated_block *ab = allocated_block_head;
	puts("*********************Free Memory*********************");
	printf("%20s %20s\n", "start_addr", "size");
	int cnt = 0;
	while (fb != NULL){
		cnt++;
		printf("%20d %20d\n", fb->start_addr, fb->size);
		fb = fb->next;
	}
	if (!cnt) puts("No Free Memory");
	else printf("Totaly %d free blocks\n", cnt);
	puts("");
	puts("*******************Used Memory*********************");
	printf("%10s %10s %20s\n", "PID", "start_addr", "size");
	cnt = 0;
	while (ab != NULL){
		cnt++;
		printf("%10d %10d %20d\n", ab->pid, ab->start_addr, ab->size);
		ab = ab->next;
	}
	if (!cnt) puts("No allocated block");
	else printf("Totaly %d allocated blocks\n", cnt);
	return;
}

void kill_process(){ //杀死某个进程
	allocated_block *ab;
	int pid;
	puts("Please input the pid of Killed process");
	scanf("%d", &pid);
	ab = find_process(pid);
	if (ab != NULL){
		free_mem(ab);
		dispose(ab);
	}
}



