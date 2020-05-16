#include <stdio.h>
#include <string.h>
#include<time.h>

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

int MRVHeuristic(int row, int col) {
    // 返回(row, col)元素的可取值的个数
    int mrv = 0;
    for (int i = 1; i <= 9; i++){
        if (IsConsistent(row, col, i))
            mrv ++;
    }
    return mrv;
}

int DegreeHeuristic(int row, int col) {
    int degree = 0;
    for (int i = 0; i < 9; i++) {
        // 行
        if (i!= col && assign[row][i] == 0) degree++;
        // 列
        if (i != row && assign[i][col] == 0) degree++;
    }
    //小方块
    int box_row = row / 3 * 3;
    int box_col = col / 3 * 3;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if(box_row + i != row && box_col + j != col)
                if (assign[box_row + i][box_col + j] == 0)
                    degree ++;
        }
    }
    // X
    if(row == col){
        for (int i = 0; i < 9; i++){
            if (i != row && assign[i][i] == 0) degree ++;
        }
    }
    if (row == 8-col) {
        for (int i = 0; i < 9; i++){
            if (i != row && assign[i][8-i] == 0) degree ++;
        }
    }
    return degree;     
}

int SelectUnassigned() {
    // 找到未解决的位置中度最大的一个，若返回-1则全部解决，-2则无解
    int row = 0;
    int col = 0;
    int mrv = 10;

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (assign[i][j] == 0) {
                if (MRVHeuristic(i, j)){
                    if (mrv > MRVHeuristic(i, j)) {
                        row = i;
                        col = j;
                        mrv = MRVHeuristic(i, j);
                    }
                    else if (mrv == MRVHeuristic(i, j)){
                        // mrv相同时选择度启发式更大的
                        if (DegreeHeuristic(row, col) < DegreeHeuristic(i, j)) {
                            row = i;
                            col = j;
                            mrv = MRVHeuristic(i, j);
                        }
                    }
                }
                else {
                    // 如果mrv是0，表示出现无可取值的元素，无解，返回-2
                    return -2;
                }
            }
        }
    }

    // 如果mrv是10，表示没有未解决的元素，返回-1
    if (mrv != 10) return row * 9 + col;
    return -1;
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
    if (next == -2) {
        // 无解
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
    int begin_time,end_time;
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
    fclose(fin);
    
    InitSudoku();
    step = 0;
    flag = 0;
    begin_time = clock();
    BackTracking();
    end_time = clock();
    if (flag) FPrintSudoku(fout);
    if (CLOCKS_PER_SEC == 1000)
        printf("time: %d ms\n", end_time - begin_time);
    else {
        printf("time: %d us\n", end_time - begin_time);
    }
    fclose(fout);

    return 0;
}