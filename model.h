#ifndef MODEL_H
#define MODEL_H


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class model
{
    public:
        /** Default constructor */
        model(int , float*, float* =NULL , float* =NULL, float* =NULL);
        model(char* path, char* texPath);
        float* getVertices(){return vertices;}
        float* getNormals(){return vertices_normals;}
        float* getTexturs(){return vertices_textures;}
        float* getColors(){return vertices_colors;}
        int getNumOfVer(){return NumOfVer;}
        unsigned int getTex_width(){return tex_width;}
        unsigned int getTex_hight(){return tex_hight;}
        std::vector<unsigned char> getTex_image(){return tex_image;}

    protected:

    private:
        float* vertices;
        float* vertices_normals;
        float* vertices_textures;
        float* vertices_colors;
        unsigned int tex_width;
        unsigned int tex_hight;
        std::vector<unsigned char> tex_image;

        int NumOfVer;

};

#endif // MODEL_H
