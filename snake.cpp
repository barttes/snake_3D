#include "snake.h"
#include "simpleSnake.h"


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

Snake::Snake(char* Fhead, char* Ftexhead, char* Fsegment, char* Ftexsegment, char* Ftail, char* Ftextail)
{
    length = 10;
    head = new model(Fhead, Ftexhead);
    tail = new model(Ftail, Ftextail);
    str_segment = new model(Fsegment, Ftexsegment);
    cur_segment = new model(Fsegment, Ftexsegment);
    SegmentsPerUnit = 10;
    for(int i=0;i>10;i++) list_of_positions.push_front(1);




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

void Snake::mov(int n){
    list_of_positions.push_front(n);
    list_of_positions.pop_back();
}


