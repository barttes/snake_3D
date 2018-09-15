#ifndef MODEL_H
#define MODEL_H


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class model
{
    public:
        /** Default constructor */
        model(int , float*, float* =NULL , float* =NULL, float* =NULL);
        float* getVertices(){return vertices;}
        float* getNormals(){return vertices_normals;}
        float* getTexturs(){return vertices_textures;}
        float* getColors(){return vertices_colors;}
        int getNumOfVer(){return NumOfVer;}

    protected:

    private:
        float* vertices;
        float* vertices_normals;
        float* vertices_textures;
        float* vertices_colors;
        int NumOfVer;
};

#endif // MODEL_H
