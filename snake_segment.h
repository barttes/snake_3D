#ifndef SNAKE_SEGMENT_H
#define SNAKE_SEGMENT_H


class snake_segment
{
    public:


        void move();


        /** Default constructor
         *  \brief constructor of a new snake_segment to be called only from snake's head.
         *  \param prev pointer to snake_segment that is closer to the head. head's pointer, when called.
         *  \param next pointer to snake_segment that in further form the head. previous next param of the head.
         *  \param str pointer to model of straight segment of a sneak.
         *  \param cor pointer to model of snake segment used when it turned in that spot.
         *
         *  Constructor used to generate new snake segment. Used mostly for elongating snake. being also the base for head's and tail's constructors.
         */
        snake_segment(snake_segment *prev(), snake_segment *next(), model *str(), model *cor);


        /** Default destructor */
        virtual ~snake_segment();


        /** Access *next_segment
         * \return The current value of *next_segment
         */
        snake_segment GetNext_segment() { return next_segment; }


        /** Sets value of previous pointer
        * \param prev new value of previous pointer
        */
        void SetPrevious(snake_segment* prev)
         {previous = prev}


        /** Access *previous
         * \return The current value of *previous
         */
        snake_segment GetPrevious() { return previous; }


        /** Access *straight
         * \return The current value of *straight
         */
        model GetStraight() { return straight; }


        /** Access *corner
         * \return The current value of *corner
         */
        model GetCorner() { return corner; }

    protected:

    private:
        snake_segment* next_segment; /** *next_segment" */
        snake_segment* previous; //!< Member variable "*previous"
        model* straight; //!< Member variable "*straight"
        model* corner; //!< Member variable "*corner"
        mat4 position;
};

#endif // SNAKE_SEGMENT_H
