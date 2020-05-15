# README

## 文件结构

```
PB17111577_宁雨亭_exp1
├── README.md
├── report.pdf
├── digit
│   ├── input	测试样例输入
│   │   ├── 1.txt
│   │   ├── 2.txt
│   │   └── 3.txt
│   ├── output	测试样例输出
│   │   ├── 1.txt
│   │   ├── 2.txt
│   │   └── 3.txt
│   └── src
│       ├── digit	可执行文件（mac编译）
│       └── digit.cpp	A*算法和迭代A*算法源代码
└── sudoku
    ├── input	测试样例输入
    │   ├── sudoku01.txt
    │   ├── sudoku02.txt
    │   └── sudoku03.txt
    ├── output	测试样例输出
    │   ├── sudoku01.txt
    │   ├── sudoku02.txt
    │   └── sudoku03.txt
    └── src
        ├── basic	优化后可执行文件（mac编译）
        ├── basic.c	优化前代码
        ├── Xsudoku	优化后可执行文件(mac编译)
        └── Xsudoku.c	优化后代码
```

## usage

### digit

编译：

```bash
cd digit/src
g++ digit.cpp -o digit -O2 -std=c++11
```

运行：

```bash
./digit
```

运行时按照提示输入测试文件名，测试文件需要放在`digit/input/`目录下，再根据提示选择搜索算法（0代表A\*算法，1代表迭代A\*算法）。程序运行完成后会在`digit/output/`目录下得到结果，与输入文件同名。并打印算法总搜索步数、得到的解的步数和搜索所花时间。

### sudoku

编译：

```bash
cd sudoku/src
gcc Xsudoku.c -o Xsudoku
```

运行：

```bash
./Xsudoku
```

运行时按照提示输入测试文件名，测试文件需要放在`sudoku/input/`目录下。程序运行完成后会在`sudoku/output/`目录下得到结果，与输入文件同名。并打印出结果、搜索步数、搜索所花时间。

> 若需要编译运行优化前的代码，则将上述过程中的Xsudoku.c换成basic.c即可