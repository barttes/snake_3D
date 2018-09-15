#include "snake_segment.h"

void move (mat4 pos){
    position = pos
}







snake_segment::snake_segment(Snake_segment *prev()=NULL, snake_segment *next()=NULL, model *str(), model *cor)
{
    previous = prev;
    next_segment = next;
    straight = str;
    corner = cor;
}

snake_segment::~snake_segment()
{
    //dtor
}
