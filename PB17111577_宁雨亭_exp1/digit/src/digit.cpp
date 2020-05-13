#include <cstdlib>
#include <iostream>
#include <cstring>
// #include <cassert>
// #include <cmath>
#include <climits>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <ctime>
 
using namespace std;

// 目标状态
int goal[5][5] = {
    {1, 2, 3, 4, 5},
    {7, 7, 8, 9, 10},
    {6, 7, 11, 12, 13},
    {14, 15, 16, 17, 18},
    {19, 20, 21, 0, 0}
};

int step = 0;
// k=goal[gaol_pos_i[k]][goal_pos_j[k]]
int goal_pos_i[22] = {4, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4};
int goal_pos_j[22] = {4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2};

// 所有访问过的状态(open + close)
unordered_map<string, int> visited_states;
enum direction {UP, LEFT, DOWN, RIGHT};

class Node {
    public:
        int (*state)[5] = NULL;
        string* state_str = NULL;
        // 0所在的位置
        int pos_i_0s[2] = {-1, -1};
        int pos_j_0s[2] = {-1, -1};
        Node* parent = NULL;
        vector<Node*> children;
        vector<int> moves_i;
        vector<int> moves_j;
        vector<direction> moves_d;
        // 从parent到这一个节点所移动的数字和方向
        int num;
        direction d;
        int f = 0;
        int g = 0;
        int h = 0;

        Node(int cur_state[5][5], Node *cur) {
            state = (int(*)[5])malloc(sizeof(int)*5*5);
            state_str = new string();
            parent = cur;
            for(int i = 0; i < 5; i++){
                for (int j = 0; j < 5; j++){
                    state[i][j] = cur_state[i][j];
                }
            }
            update();
        }

        ~Node() {
            for (int i = 0; i < children.size(); i++){
                delete children[i];
            }
            free_state();
        }

        void free_state() {
            if (state) {
                free(state);
                state = NULL;
            }
            if(state_str) {
                delete state_str;
                state_str = NULL;
            }
        }

        void expand_node(){
            setup_moves();
            setup_children();
        }

        void move(int i, int j, direction move_direction){
            // 将[x][y]向move_direction方向移动
            if (state[i][j] == 7){
                // 7
                switch(move_direction){
                    case UP: {
                        state[i-1][j] = state[i][j];
                        state[i-1][j-1] = state[i][j-1];
                        // state[i][j] = state[i+1][j]; = 7
                        state[i][j-1] = 0;
                        state[i+1][j] = 0;
                        break;
                    }
                    case LEFT: {
                        state[i][j-2] = state[i][j-1];
                        state[i+1][j-1] = state[i+1][j];
                        state[i][j] = 0;
                        state[i+1][j] = 0;
                        break;
                    }
                    case DOWN: {
                        state[i+2][j] = state[i+1][j];
                        state[i+1][j-1] = state[i][j-1];
                        state[i][j] = 0;
                        state[i][j-1] = 0;
                        break;
                    }
                    case RIGHT: {
                        state[i][j+1] = state[i][j];
                        state[i+1][j+1] = state[i+1][j];
                        state[i+1][j] = 0;
                        state[i][j-1] = 0;
                        break;
                    }
                }
            }
            else {
                // not 7
                switch(move_direction) {
                    case UP: {
                        if(state[i-1][j] == 0) {
                            state[i-1][j] = state[i][j];
                            state[i][j] = 0;
                        }
                        break;
                    }
                    case LEFT: {
                        if(state[i][j-1] == 0) {
                            state[i][j-1] = state[i][j];
                            state[i][j] = 0;
                        }
                        break;
                    }
                    case DOWN: {
                        if(state[i+1][j] == 0) {
                            state[i+1][j] = state[i][j];
                            state[i][j] = 0;
                        }
                        break;
                    }
                    case RIGHT: {
                        if(state[i][j+1] == 0) {
                            state[i][j+1] = state[i][j];
                            state[i][j] = 0;
                        }
                        break;
                    }
                }
            }
            num = parent->state[i][j];
            d = move_direction;
            update();
        }

        bool is_goal() {
            return h == 0;
        }

    private:
        void get_center(int i, int j, int& center_i, int& center_j) {
            // 得到中心坐标（对于7）
            if(i < 0 || i >= 5 || j < 0 || j >= 5) {
                center_i = -1;
                center_j = -1;
            }
            else {
                if(state[i][j] == 7) {
                    if (j > 0 && state[i][j-1] == 7) {
                        center_i = i;
                        center_j = j;
                    }
                    else if (i > 0 && state[i-1][j] == 7) {
                        center_i = i - 1;
                        center_j = j;
                    }
                    else {
                        center_i = i;
                        center_j = j + 1;
                    }
                }
                else {
                    center_i = i;
                    center_j = j;
                }
            }
        }

        bool move_check(int i, int j, direction move_direction) {
            // 这里i,j都是中心坐标
            if (i < 0 || i >= 5 || j < 0 || j >= 5){
                return false;
            }
            if (state[i][j] == 0){
                // 只移动1-21
                return false;
            }
            if(state[i][j] == 7){
                // 7
                switch(move_direction){
                    case UP: {
                        if (i > 0 && j > 0 && state[i-1][j] == 0 && state[i-1][j-1] == 0)
                            return true;
                        else return false;
                    }
                    case LEFT: {
                        if (j > 1 && i < 4 && state[i][j-2] == 0 && state[i+1][j-1] == 0)
                            return true;
                        else return false;
                    }
                    case DOWN: {
                        if (i < 3 && j > 0 && state[i+2][j] == 0 && state[i+1][j-1] == 0)
                            return true;
                        else return false;
                    }
                    case RIGHT: {
                        if (i < 4 && j < 4 && state[i][j+1] == 0 && state[i+1][j+1] == 0)
                            return true;
                        else return false;
                    }
                }
            }
            else {
                // not 7
                switch (move_direction){
                    case UP: {
                        if (i > 0 && state[i-1][j] == 0) return true;
                        else return false;
                    }
                    case LEFT: {
                        if (j > 0 && state[i][j-1] == 0) return true;
                        else return false;
                    }
                    case RIGHT: {
                        if (j < 4 && state[i][j+1] == 0) return true;
                        else return false;
                    }
                    case DOWN: {
                        if (i < 4 && state[i+1][j] == 0) return true;
                        else return false;
                    }
                }
            }
        }

        bool move_exist(int i, int j, direction move_direction){
            // 同样的数字、方向是否已经搜索过
            for (int k = 0; k < moves_i.size(); k++){
                if (moves_i[k] == i && moves_j[k] == j && moves_d[k] == move_direction){
                    return true;
                }
            }
            return false;
        }

        void add_move(int i, int j, direction move_direction){
            moves_i.push_back(i);
            moves_j.push_back(j);
            moves_d.push_back(move_direction);
        }

        void setup_moves() {
            for (int i = 0; i < 2; i++){
                // 2个0
                int pos_i = pos_i_0s[i];
                int pos_j = pos_j_0s[i];
                int center_i, center_j;
                // 上一个down
                get_center(pos_i-1, pos_j, center_i, center_j);
                if (move_check(center_i, center_j, DOWN) && !move_exist(center_i, center_j, DOWN))
                    add_move(center_i, center_j, DOWN);
                // 下一个UP
                get_center(pos_i+1, pos_j, center_i, center_j);
                if (move_check(center_i, center_j, UP) && !move_exist(center_i, center_j, UP))
                    add_move(center_i, center_j, UP);
                // 左一个RIGHT
                get_center(pos_i, pos_j-1, center_i, center_j);
                if (move_check(center_i, center_j, RIGHT) && !move_exist(center_i, center_j, RIGHT))
                    add_move(center_i, center_j, RIGHT);
                // 右一个LEFT
                get_center(pos_i, pos_j+1, center_i, center_j);
                if (move_check(center_i, center_j, LEFT) && !move_exist(center_i, center_j, LEFT))
                    add_move(center_i, center_j, LEFT);
            }
        }

        void setup_children() {
            for (int i = 0; i < moves_i.size(); i++) {
                int pos_i = moves_i[i];
                int pos_j = moves_j[i];
                direction move_direction = moves_d[i];
                Node * new_node = new Node(state, this);
                new_node->move(pos_i, pos_j, move_direction);
                if (visited_states.find(*(new_node->state_str)) == visited_states.end()){
                    // not visited
                    children.push_back(new_node);
                    visited_states[*(new_node->state_str)] = new_node->f;
                }
                else {
                    if (visited_states[*(new_node->state_str)] > new_node->f){
                        // f更小 更新评价函数 重新加入open
                        children.push_back(new_node);
                        visited_states[*(new_node->state_str)] = new_node->f;
                    }
                    else {
                        delete new_node;
                    }
                }
            }
        }

        void update() {
            // update pos_i_0s, pos_j_0s
            pos_i_0s[0] = -1;
            pos_i_0s[1] = -1;
            pos_j_0s[0] = -1;
            pos_j_0s[1] = -1;
            for (int i=0; i < 5; i++) {
                for (int j=0; j < 5; j++) {
                    if (state[i][j] == 0) {
                        if (pos_i_0s[0] == -1) {
                            pos_i_0s[0] = i;
                            pos_j_0s[0] = j;
                        } else {
                            pos_i_0s[1] = i;
                            pos_j_0s[1] = j;
                        }
                    }
                }
            }
            // update state_str
            *state_str = "";
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    *state_str += to_string(state[i][j]);
                    *state_str += " ";
                }
            }
            // update f, g, h
            if(parent) {
                g = parent->g + 1;
            }
            else g = 0;
            h = getHeuristic();
            // h = ManhattanHeuristic();
            f = g + h;
        }

        int ManhattanHeuristic() {
            int h = 0;
            for (int i = 0; i < 5; i++){
                for (int j = 0; j < 5; j++){
                    if (state[i][j] == 0) continue;
                    else {
                        int center_i, center_j;
                        get_center(i, j, center_i, center_j);
                        if (i == center_i && j == center_j)
                            h += abs(i - goal_pos_i[state[i][j]]) + abs(j - goal_pos_j[state[i][j]]);
                    }
                }
            }
            return h;
        }

        int getHeuristic() {
            int h = 0;
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if(state[i][j] == 0) {
                        continue;
                    }
                    else {
                        int center_i, center_j;
                        get_center(i, j, center_i, center_j);
                        if (i == center_i && j == center_j){
                            int dist = abs(i - goal_pos_i[state[i][j]]) + abs(j - goal_pos_j[state[i][j]]);
                            if (state[i][j] == 7){
                                if (dist <= 1) {
                                    h += dist;
                                }
                                else {
                                    h += 5 * (dist - 1) + 1;
                                }
                            }
                            else {
                                if (dist <= 2) {
                                    h += dist;
                                }
                                else {
                                    h += 3 * (dist - 2) + 2;
                                }
                            }
                        }
                    }
                }
            }
            return h;
        }
};

struct CompareNode : public std::binary_function<Node*, Node*, bool>                                                                                     
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return lhs->f> rhs->f;
    }
};

Node *astar_search(int start[5][5]) {
    visited_states.clear();
    priority_queue<Node *, vector<Node *>, CompareNode > open;
    Node *init_node = new Node(start, NULL);
    Node *node;
    step = 0;
    open.push(init_node);
    visited_states[*(init_node->state_str)] = init_node->f;
    step += 1;
    while (!open.empty()){
        node = open.top();
        open.pop();
        if(node->is_goal()){
            // finish
            break;
        }
        else if (visited_states[*(node->state_str)] == node->f){
            // 同样的state中f最小的
            node->expand_node();
            for (int i = 0; i < node->children.size(); i++){
                open.push(node->children[i]);
                step += 1;
            }
        }
        node->free_state();
    }
    return node;
}

Node *idastar_search(int start[5][5]) {
    int d_limit = 0;
    Node *node;
    Node *init_node = new Node(start, NULL);
    step = 0;
    d_limit = init_node->f;
    while (true) {
        int next_d_limit = INT_MAX;
        visited_states.clear();
        priority_queue<Node *, vector<Node *>, CompareNode > open;
        open.push(init_node);
        visited_states[*(init_node->state_str)] = init_node->f;
        step ++;
        cout << "d_limit: " << d_limit << endl;
        while (!open.empty()) {
            node = open.top();
            open.pop();
            if(node->f > d_limit) {
                next_d_limit = next_d_limit > node->f ? node->f : next_d_limit;
            }
            else {
                if (node->is_goal()){
                    return node;
                }
                else if (visited_states[*(node->state_str)] == node->f){
                    node->expand_node();
                    for (int i = 0; i < node->children.size(); i++){
                        open.push(node->children[i]);
                        step ++;
                    }
                }
            }
            node ->free_state();
        }
        d_limit = next_d_limit;
        // delete init_node;
        // init_node = new Node(start, NULL);
    }
    return NULL;
}

void input_state(int start[5][5], FILE *fin){
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if(j == 4)
                fscanf(fin, "%d", &start[i][j]);
            else
                fscanf(fin, "%d,", &start[i][j]);
        }
    }
}

Node *print_path(FILE *fout, Node *node){
    stack<int> moves_num;
    stack<direction> moves_d;
    while (node->parent){
        moves_num.push(node->num);
        moves_d.push(node->d);
        node = node->parent;
    }
    int num;
    direction move_direction;
    char direction_map[4] = {'u', 'l', 'd', 'r'};
    while (!moves_num.empty()){
        num = moves_num.top();
        moves_num.pop();
        move_direction = moves_d.top();
        moves_d.pop();
        if (moves_num.empty()){
            fprintf(fout, "(%d,%c)\n", num, direction_map[move_direction]);
        }
        else {
            fprintf(fout, "(%d,%c); ", num, direction_map[move_direction]);
        }
    }
    return node;
}

int main() {
    int start[5][5];
    int algorithm;
    int begin_time, end_time;

    char input_dir[30] = "../input/";
    char output_dir[30] = "../output/";
    char filename[20];
    printf("Input filename：\n");
    scanf("%s", filename);
    printf("Choose algorithm: 0 for astar, 1 for idastar\n");
    scanf("%d", &algorithm);
    strcat(input_dir, filename);
    strcat(output_dir, filename);

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
    input_state(start, fin);
    Node *node;
    begin_time = clock();
    if (algorithm == 0)
        node = astar_search(start);
    else node = idastar_search(start);
    end_time = clock();
    cout << "visited step: " << step << endl;
    cout << "move step: " << node->f << endl;
    if (CLOCKS_PER_SEC == 1000)
        printf("time: %d ms\n", end_time - begin_time);
    else {
        printf("time: %d us\n", end_time - begin_time);
    }
    node = print_path(fout, node);
    delete node;
    fclose(fin);
    fclose(fout);
}
