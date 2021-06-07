#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

int s, m, n;

int main(){
    scanf("%d%d%d", &s, &m, &n);
    int tracks[n + 5];
    for(int i = 0; i < n ; i++){
        scanf("%d", &tracks[i]);
    }
    int pre = s;
    long mov_d = 0;
    sort(tracks, tracks + n);  // ascending
    int start = upper_bound(tracks, tracks + n, s) - tracks - 1;
    if(start == -1){
    	printf("%d ", s);
	}
	else if(s != tracks[start]){
		printf("%d ", s);
	}
    for(int i = start; i >= 0; i--){
		printf("%d ", tracks[i]);
        mov_d += abs(tracks[i] - pre);
        pre = tracks[i];
    }
    for(int i = n-1; i > start; i--){
		printf("%d ", tracks[i]);
        mov_d += abs(tracks[i] - pre);
        pre = tracks[i];
    }
    printf("\n"); 
    printf("%ld", mov_d);
    return 0;
}
