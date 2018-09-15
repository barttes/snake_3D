#ifndef SNAKE_H
#define SNAKE_H
#include "model.h"
#include <deque>

class Snake
{
    public:
        /** Default constructor */
        Snake();

        /** Constructor loading models from file
        *\param Fhead - path to file containing head
        *\param Fsegment - path to file containing segment
        *\param Ftail - path to file containing tail
        */
        Snake(char* Fhead, char* Fsegment, char* Ftail);
        /** Access length
         * \return The current value of length
         */
        unsigned int GetLength() { return length; }
        /** Access head
         * \return The current value of head
         */
        model* Gethead() { return head; }
        /** Access str_segment
         * \return The current value of str_segment
         */
        model* Getstr_segment() { return str_segment; }
        /** Access cur_segment
         * \return The current value of cur_segment
         */
        model* Getcur_segment() { return cur_segment; }
        /** Access tail
         * \return The current value of tail
         */
        model* Gettail() { return tail; }
        /** Access given position on list of positions
         *\param n number of position that we want to extract
         *\param *r pointer to variable holding number of rotation matrix to be used
         *\return Model to be drawn
         */
         model* GetModel(int n, int* r);
         void move(int n);

    protected:

    private:
        unsigned int length; //!< Member variable "lenht"
        model* head; //!< Member variable "head"
        model* str_segment; //!< Member variable "str_segment"
        model* cur_segment; //!< Member variable "cur_segment"
        model* tail; //!< Member variable "tail"
        std::deque<int> list_of_positions;
        unsigned int SegmentsPerUnit;
};

#endif // SNAKE_H
