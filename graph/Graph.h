#pragma once 

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType; //边在遍历树中所属的类型
