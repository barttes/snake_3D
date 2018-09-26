#include "split.h"
#include <vector>
#include <stdio.h>


using namespace std;

int split(char* input, char*** output, char delimiter, int** lengths){
vector<vector<char> > out;
vector<char> ne;
int i=0;
while(input[i]!=0){
    while(input[i]!=delimiter && input[i]!=0){

        ne.push_back(input[i]);
        i++;
    }
    ne.push_back(0);
    if(input[i]==delimiter){i++;}
    out.push_back(vector<char>(ne));
    ne.clear();

}
i=out.size();
if (lengths!=0){
    *lengths = new int [i];
    for(int j=0;j==i;j++){
        *lengths[j]=out[i].size();
    }
}
char** temp= new char* [out.size()];
for(int j=0;j<i;j++){
    temp[j]=new char [out[j].size()];
    copy(out[j].begin(),out[j].end(),temp[j]);
}
*output = temp;
return i;
}
int split(char* input, char*** output, int** lengths=0 ) {return split(input,output,' ',lengths);}
int split(char* input, char*** output, char delimiter=' ') {return split(input,output,delimiter,0);}
int split(char* input, char*** output) {return split(input,output,' ',0);}
