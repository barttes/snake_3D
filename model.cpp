#include "model.h"
#include <stdio.h>
#include <fstream>
#include "split.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>

model::model(int n,float* ver, float* norm, float* tex, float* color)
/**constructor for models from source code
*
*\param n number of vertices * number of coords per vertice
*\param ver pointer to a vertices table
*\param norm pinter to normals table
*\param tex pointer to textures table
*\param color poiter to colors table
*
*creates model instance from data generated or embedded in game code*/

{
    int i =0;
    int Size =n;
    vertices = new float[Size];
    vertices_colors = new float[Size];
    vertices_normals = new float[Size];
    vertices_textures = new float[Size];
    for(i=0 ;i<Size;i++){vertices[i]=ver[i];}
    if (norm!=NULL){
        for (i = 0; i< Size;i++) vertices_normals[i] = norm[i];
    }
    if (tex!=NULL){
        for (i = 0; i< Size;i++) vertices_textures[i] = tex[i];
    }
    if (color!=NULL){
        for (i = 0; i< Size;i++) vertices_colors[i] = color[i];
    }
    else{
        for (i = 0; i< Size;i++){
            int j =i%3;
            switch(j){
                case 0:
                    vertices_colors[i]=0.2;
                    break;
                case 1:
                    vertices_colors[i]= 0.8;
                    break;
                case 2:
                    vertices_colors[i]=0;
                    break;
            }

        }
    }
    NumOfVer =n/3;
}


model::model(char* path, char* texPath){
 FILE* ToRead = fopen(path,"r");
 if (!(ToRead)){
    perror("model not read");
    exit(1);
 }
    char line[256];
    char** Splited_line;
    int* lengths;
    int num_of_parts;
    std::vector<float> vX;
    std::vector<float> vY;
    std::vector<float> vZ;
    std::vector<float> vtX;
    std::vector<float> vtY;
    std::vector<float>Verout;
    std::vector<float>TexVerout;
    char* to_do;
    memset(line,0,sizeof(line));
    fgets(line, 255, ToRead);
    do{
    num_of_parts = split(line,&Splited_line,' ', &lengths);
    to_do = Splited_line[0];
    if(strcmp(to_do,"v") == 0){
        vX.push_back(atof(Splited_line[1]));
        vY.push_back(atof(Splited_line[2]));
        vZ.push_back(atof(Splited_line[3]));
    }
    if(strcmp(to_do,"vt") == 0){
        vtX.push_back(atof(Splited_line[1]));
        vtY.push_back(atof(Splited_line[2]));
    }
    if(strcmp(to_do,"f") == 0){
        char** CurVer;
        for (int j=1;j<4;j++){
            int num =split(Splited_line[j],&CurVer,47, 0);
            Verout.push_back(vX[atoi(CurVer[0])]);
            Verout.push_back(vY[atoi(CurVer[0])]);
            Verout.push_back(vZ[atoi(CurVer[0])]);
            if(num>1){
                    TexVerout.push_back(vtX[atoi(CurVer[1])]);
                    TexVerout.push_back(vtY[atoi(CurVer[1])]);
                    delete[] CurVer[1];
                }
            }
        delete[] CurVer[0];
        delete[] CurVer;
    }
    for(int j=0; j<num_of_parts;j++){delete[] Splited_line[j];}
    memset(line,0,sizeof(line));
    fgets(line, 255, ToRead);
    }while(!(feof(ToRead)));
    vertices= new float[Verout.size()];
    copy(Verout.begin(),Verout.end(),vertices);
    vertices_normals= new float[Verout.size()];
    copy(Verout.begin(),Verout.end(),vertices_normals);
    vertices_textures= new float[TexVerout.size()];
    copy(TexVerout.begin(),TexVerout.end(),vertices_textures);
    vertices_colors = new float[Verout.size()];
    for(int j=0; j<Verout.size();j++){
        switch(j%3){
            case 0:
                vertices_colors[j]= 1;
                break;
            case 1:
                vertices_colors[j]= 1;
                break;
            case 2:
                vertices_colors[j]= 0;
                break;
            }
    }
    NumOfVer = Verout.size()/3;
    fclose(ToRead);
}

/*model::model(char* file,float* ver, float* norm, float* tex, float* color)

{
   std::fstream model_file;
   model_file.open(file, sdt::fstream::in);
   if( model_file==NULL )
   {
        perror('file '+file+' could not be loaded');
        exit(1);
   }
   char* line = new char[256];
   while (line[0]!='g')
   {
       fgets(line, 255, model_file);
   }
   line = line[2:];
   line

}*/
void Resize(float* tab, int counter){
float* temp = tab;
tab = new float[counter*10];
for (int i = 0; i < (counter-1)*10;i++){tab[i] = temp[i];}
delete temp;
}

void Resize(float* tab, int counter2, int counter){
float* temp = tab;
tab = new float[counter2*10+counter];
for (int i = 0; i < (counter-1)*10;i++){tab[i] = temp[i];}
delete temp;
}
