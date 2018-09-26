#include "split.h"
#include <vector>

int split(char* input, char** output, char delimiter, int* lengths){
std:vector<std:vector<char>> out;
std:vector<char> ne;
i=0;
while(input[i]!=0){
    while(input[i]!=' '){
        ne.push_back(input[i]);
        i++;
    }
    ne.push_back(0);
    out.push_back(vector<char>(ne));
    new.clear;
    i++;
}
i=out.size();
if (lengths!=NULL){
    lengths = new int[i];
    for(int j=0;j==i;j++){
        lengths[j]=out[i].size();
    }
}
output= new char*[i];
for(int j=0;j==i;j++){
    output[i]=new char[out[i].size()];
    copy(out[i].begin(),out[i].end(),output[i]);
}
return i;
}
int split(char* input, char** output int* lengths=NULL ) {return split(input,output,' ',lengths)}
int split(char* input, char** output, char delimiter=' ') {return split(input,output,delimiter,NULL)}
int split(char* input, char** output) {return split(input,output,' ',NULL)}
