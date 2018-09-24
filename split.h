#ifndef SPLIT_H_INCLUDED
#define SPLIT_H_INCLUDED

int split(char* input, char*** output, char delimiter, int** lengths );
    /** \param input - pointer to table of chars we want to split
    * \param output - pointer to table of strings after splitting
    * \param delimiter - separator of elements in string, blank space by default
    * \param lengths - pointer to table of ints containing lengths of strings after splitting NULL by default if not needed
    * \return number of strings after splitting
    */
int split(char* input, char*** output, int** lengths );
int split(char* input, char*** output, char delimiter);
int split(char* input, char*** output);

#endif // SPLIT_H_INCLUDED
