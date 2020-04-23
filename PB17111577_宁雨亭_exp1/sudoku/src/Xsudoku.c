#include <stdio.h>
#include <string.h>

int map[9][9];
int assign[9][9]; // 1 - solved, 0 - unsolved
int step;
int flag;

void PrintSudoku() {
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (j == 8) printf("%d", map[i][j]);
            else printf("%d ", map[i][j]);
        }
        printf("\n");
    }
    printf("step: %d\n", step);
    return;
}

void FPrintSudoku(FILE * fout) {
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (j == 8) fprintf(fout, "%d", map[i][j]);
            else fprintf(fout, "%d ", map[i][j]);
        }
        fprintf(fout, "\n");
    }
    return;
}

void GetInput(FILE *fin) {
    // 读取input文件
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++)
        {
            fscanf(fin, "%d", &map[i][j]);
        }
    }
    // PrintSudoku();
    return;
}

void InitSudoku(){
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++){
            if (map[i][j] != 0){
                assign[i][j] = 1;
            }
            else assign[i][j] = 0;
        }
    }
    return;
}

int SelectUnassigned() {
    // 找到下一个未解决的位置，若返回-1则全部解决
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (assign[i][j] == 0)
                return i * 9 + j;
        }
    }
    return -1;
}

int IsConsistent(int row, int col, int value) {
    for (int i = 0; i < 9; i++) {
        // 行
        if (i!= col && map[row][i] == value) return 0;
        // 列
        if (i != row && map[i][col] == value) return 0;
    }
    //小方块
    int box_row = row / 3 * 3;
    int box_col = col / 3 * 3;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if(box_row + i != row && box_col + j != col)
                if (map[box_row + i][box_col + j] == value)
                    return 0;
        }
    } 
    // X
    if(row == col){
        for (int i = 0; i < 9; i++){
            if (i != row && map[i][i] == value) return 0;
        }
    }
    if (row == 8-col) {
        for (int i = 0; i < 9; i++){
            if (i != row && map[i][8-i] == value) return 0;
        }
    }
    return 1;
}

int GetNextPossibleAns (int row, int col){
    for(int i = map[row][col]+1; i < 10; i++){
        if (IsConsistent(row, col, i)) {
            return i;
        }
    }
    return 0;
}

void BackTracking() {
    int next = SelectUnassigned();
    if (next == -1) {
        PrintSudoku();
        flag = 1;
        return;
    }
    int row = next / 9;
    int col = next % 9;
    while ((map[row][col] = GetNextPossibleAns(row, col)) != 0) {
        assign[row][col] = 1;
        step ++;
        BackTracking();
        if(flag) break;
        assign[row][col] = 0;
    }
    return;
}

int main() {
     
    // 读取输入
    char input_dir[30] = "../input/";
    char output_dir[30] = "../output/";
    char filename[20];
    printf("please input the test file name：\n");
    scanf("%s",filename);
    strcat(input_dir,filename);
    strcat(output_dir,filename);
    // printf("%s",input_dir);
    FILE *fin, *fout;
    fin = fopen(input_dir, "r");
    if (fin == NULL) {
        printf("Error: cannot open %s!\n", input_dir);
        return -1;
    }
    fout = fopen(output_dir, "w");
    if (fout == NULL)
    {
        printf("Error: cannot open %s!\n", output_dir);
        return -1;
    }

    GetInput(fin);
    InitSudoku();
    step = 0;
    flag = 0;
    BackTracking();
    if (flag) FPrintSudoku(fout);
    return 0;
}