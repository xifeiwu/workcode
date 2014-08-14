/* 
 * 回溯法解N皇后问题 
 * 使用一个一维数组表示皇后的位置 
 * 其中数组的下标表示皇后所在的行 
 * 数组元素的值表示皇后所在的列 
 * 这样设计的棋盘，所有皇后必定不在同一行 
 * 
 * 假设前n-1行的皇后已经按照规则排列好 
 * 那么可以使用回溯法逐个试出第n行皇后的合法位置 
 * 所有皇后的初始位置都是第0列 
 * 那么逐个尝试就是从0试到N-1 
 * 如果达到N，仍未找到合法位置 
 * 那么就置当前行的皇后的位置为初始位置0 
 * 然后回退一行，且该行的皇后的位置加1，继续尝试 
 * 如果目前处于第0行，还要再回退，说明此问题已再无解 
 * 
 * 如果当前行的皇后的位置还是在0到N-1的合法范围内 
 * 那么首先要判断该行的皇后是否与前几行的皇后互相冲突 
 * 如果冲突，该行的皇后的位置加1，继续尝试 
 * 如果不冲突，判断下一行的皇后 
 * 如果已经是最后一行，说明已经找到一个解，输出这个解 
 * 然后最后一行的皇后的位置加1，继续尝试下一个解 
 */  
#include <stdio.h>
#define MAX_LENGTH 1024  
/* 
 * 检查第n行的皇后与前n-1行的皇后是否有冲突 
 * 发生冲突的充分必要条件是： 
 * a) 两皇后处于同一列，即a[i] == a[n] 
 * b) 两皇后处于同一斜线，即|a[i] - a[n]| == |i - n| == n - i 
 */  
int is_conflict(int *a, int n) {  
    int flag = 0;  
    int i;  
    for ( i = 0; i < n; i++ ) {  
        if ( a[i] == a[n] || a[i] - a[n] == n - i || a[n] - a[i] == n - i ) {  
            flag = 1;  
            break;  
        }  
    }  
    return flag;  
}  
/* 
 * 输出皇后的排列 
 */  
void print_board(int *a, int n) {  
    int i, j;  
    for ( i = 0; i < n; i++ ) {  
        for ( j = 0; j < a[i]; j++ ) {  
            printf("0");  
        }  
        printf("1");  
        for ( j = a[i] + 1; j < n; j++ ) {  
            printf("0");  
        }  
        printf("\n");  
    }  
    printf("--------\n");  
}  
/* 
 * 初始化棋盘，所有皇后都在第0列 
 */  
void init_board(int *a, int n) {  
    int i;  
    for ( i = 0; i < n; i++ ) {  
        a[i] = 0;  
    }  
}  
/* 
 * 解决N皇后问题 
 */  
int queen(int n) {  
    int count = 0;  
    int a[MAX_LENGTH];  
    init_board(a, n);  
    int i = 0;  
    while ( 1 ) {  
        if ( a[i] < n ) {  
            // 如果皇后的位置尚未超出棋盘范围  
            // 需要检查第i行的皇后是否与前i-1行的皇后冲突  
            if ( is_conflict(a, i) ) {  
                // 如果冲突，尝试下一个位置  
                a[i]++;  
                continue;  
            }  
            if ( i >= n - 1 ) {  
                // 如果已经到最后一行，也即找到一个解，首先输出它  
                count++;  
                //print_board(a, n);  
                // 然后尝试当前行的皇后的下一个位置  
                a[n-1]++;  
                continue;  
            }  
            // 没有冲突，尝试下一行  
            i++;  
            continue;  
        }  
        else {  
            // 皇后的位置已经超出棋盘范围  
            // 那么该行的皇后复位  
            a[i] = 0;  
            // 回退到上一行  
            i--;  
            if ( i < 0 ) {  
                // 已经不能再退了，函数结束  
                return count;  
            }  
            // 尝试上一行的皇后的下个位置  
            a[i]++;  
            continue;  
        }  
    }  
}  
int main(void) {  
    int n = 16;  
    int count = queen(n);  
    printf("%d solutions in %d queens problem\n", count, n);  
    return 0;  
}
