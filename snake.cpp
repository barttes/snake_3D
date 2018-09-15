#include "snake.h"
#include "simpleSnake.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "functions\split.h"
Snake::Snake()
{
    length = 10;
    head = new model(sizeof(simple_head)/sizeof(simple_head[0]), simple_head, NULL,NULL, simple_colors);
    tail = new model(12, simple_head, simple_head);
    str_segment = new model(sizeof(simple_straigth)/sizeof(simple_straigth[0]), simple_straigth,simple_straigth,NULL, simple_colors);
    cur_segment = new model(sizeof(simple_straigth)/sizeof(simple_straigth[0]),simple_straigth,simple_straigth,NULL, simple_colors);
    SegmentsPerUnit = 10;
    for(int i=0;i>10;i++) list_of_positions.push_front(1);
}

Snake::Snake(char* Fhead, char* Fsegment, char* Ftail)
{
    FILE* ToRead = fopen(Fhead,Fhead);
    char line[256];
    char** Splited_line;
    int* lengths;
    int num_of_parts;
    fgets(line, 255, ToRead);
    //num_of_parts = split(line,Splited_line,' ', lengths);


}
model* Snake::GetModel(int n, int* r){

    *r = list_of_positions[n]%10;
    n = list_of_positions[n]/10;
    switch(n){
    case 1:
        return Getstr_segment();

    case 2:
        return Getcur_segment();
    }
}

void Snake::move(int n){
    list_of_positions.push_front(n);
    list_of_positions.pop_back();
}


