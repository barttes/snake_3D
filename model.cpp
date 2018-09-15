#include "model.h"
#include <stdio.h>
#include <fstream>

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
