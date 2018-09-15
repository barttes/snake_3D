/*
Niniejszy program jest wolnym oprogramowaniem; mo¿esz go
rozprowadzaæ dalej i / lub modyfikowaæ na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacjê Wolnego
Oprogramowania - wed³ug wersji 2 tej Licencji lub(wed³ug twojego
wyboru) którejœ z póŸniejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej¹, i¿ bêdzie on
u¿yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyœlnej
gwarancji PRZYDATNOŒCI HANDLOWEJ albo PRZYDATNOŒCI DO OKREŒLONYCH
ZASTOSOWAÑ.W celu uzyskania bli¿szych informacji siêgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnoœci¹ wraz z niniejszym programem otrzyma³eœ te¿ egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeœli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/


#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "mySquare.h"
#include "stage_gen.h"
#include "snake.h"
#include <iostream>
#include <cmath>

using namespace glm;
using namespace std;

//odleg³oœæ u¿ytkownika od œwiata
float user_distance = -5.0f;

//po³o¿enie kamery
float position_x = 0.0f;
float position_y = 0.0f;
bool is_mouse_left_buton_pressed = false;
float old_coordinate_x;
float old_coordinate_y;

//macierz postêpu
Snake* Sn;
mat4 move= mat4(1.0f);
int move_angle = 0;
float speed= 0.1;
bool keyPress = false;
vec3 direction = vec3(sin(move_angle*PI/180)*speed,0,-cos(move_angle*PI/180)*speed);
int cam_angle = 0;
int add_cam_angle= 0;
int moves_counter = 0;
int turn_counter = 0;
bool rigth_turn=false;
bool left_turn=false;

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//*****************************Obs³uga eventów*******************************

//Procedura obs³ugi przycisku scrollowania myszy
void scroll_callback(GLFWwindow* window, double x, double y){
    if ( user_distance+y > -40 && user_distance+y < -2) user_distance += y;
}

//Procedura obs³ugi przycisku  myszy
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_1) {
           is_mouse_left_buton_pressed = true;
        }
    }
    if (action == GLFW_RELEASE) {
        if (button == GLFW_MOUSE_BUTTON_1) {
           is_mouse_left_buton_pressed = false;
        }
    }
}

//Procedura obs³ugi przewijania obrazu za pomoc¹ kursora
void cursor_callback(GLFWwindow* window, double x, double y){
    if (is_mouse_left_buton_pressed == true) {
        if (x<old_coordinate_x) {
            position_x-=x/5000;
        } else {
            position_x+=x/5000;
        }
        if (y<old_coordinate_y) {
            position_y-=y/5000;
        } else {
            position_y+=y/5000;
        }

        old_coordinate_x = x;
        old_coordinate_y = y;
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod){
    if (action == GLFW_PRESS && !keyPress){
        keyPress=true;
        if (key==GLFW_KEY_LEFT) {
                rigth_turn=false;
                left_turn = true;
        }
        if (key==GLFW_KEY_RIGHT){
                rigth_turn=true;
                left_turn = false;
        }
    }
    if (action == GLFW_RELEASE && keyPress){
        if (key == GLFW_KEY_LEFT) keyPress = false;
        if (key==GLFW_KEY_RIGHT) keyPress = false;
    }
}


//Procedura inicjuj¹ca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glfwSetScrollCallback(window, scroll_callback); //Zarejestruj procedurê obs³ugi scrollowania myszy
    glfwSetMouseButtonCallback(window, mouse_button_callback); //Zarejestruj procedurê obs³ugi przycisków myszy
    glfwSetCursorPosCallback(window, cursor_callback); //Zarejestruj procedurê obs³ugi przycisków myszy(window, mouse_button_callback); //Zarejestruj procedurê obs³ugi przycisków myszy
    glfwSetKeyCallback(window, key_callback); //zarejestruj procedurê obs³ugi klawiatury

	glClearColor(0,0,0,1); //Ustaw kolor czyszczenia ekranu
    gen(n, mySquareVertices, mySquareColors);
    Sn = new Snake();
}

//*****************************Procedury rysowania modeli*******************************

void drawBoard(GLFWwindow* window, mat4 V) {

    int tile_number = 0;
    glVertexPointer(3,GL_FLOAT,0,mySquareVertices); //Ustaw tablicê mySquareVertices jako tablicê wierzcho³ków

    glColorPointer(3,GL_FLOAT,0,mySquareColors);

    //macierz modelu p³ytki
	mat4 tile_M=mat4(1.0f);
	//tile_M=rotate(tile_M,-80*PI/180, vec3(1.0f,0.0f,0.0f));
	//tile_M=translate(tile_M, vec3(0, 0, 0));
    mat4 tile_M_to_translate = tile_M;



            //rysowanie kafelków
            glLoadMatrixf(value_ptr(V*tile_M));  // wyliczenie macierzy
            glDrawArrays(GL_TRIANGLES,0,mySquareVertexCount); //rysowanie
            tile_M_to_translate = tile_M;  //przywrócenie domyœlnej macierzy modelu

            tile_number++;



}

void drawSnake(GLFWwindow* window, int ang, mat4 mov, mat4 V, int n){
    model* toDraw;
    toDraw = Sn->Gethead();
    float* Ver =toDraw->getVertices();
    float* Col =toDraw->getColors();
    int nofver = toDraw->getNumOfVer();
    mat4 seg_mov =mat4 (1.0f);
    seg_mov = rotate(seg_mov,-ang*PI/180,vec3(0,1,0));
    int sciana=n/3;
    int wierzcholek = n%3;
    //rotate(mov,ang*PI/180,vec3(0,1,0));
    glVertexPointer(3,GL_FLOAT,0 , Ver);
    glColorPointer(3,GL_FLOAT,0,Col);
    glLoadMatrixf(value_ptr(V*seg_mov));
    glDrawArrays(GL_TRIANGLES,0,nofver);
    /*int numer_of_segments= Sn->GetLength();
    int rotation;
    seg_mov = rotate(seg_mov,PI,vec3(0,1,0));
    for (int i =0;i<numer_of_segments;i++){
        toDraw = Sn->GetModel(i,&rotation);
        float* Ver =toDraw->getVertices();
        float* Col =toDraw->getColors();
        int nofver = toDraw->getNumOfVer();
        switch(rotation){
            case 0:
                glVertexPointer(3,GL_FLOAT,0 , Ver);
                glColorPointer(3,GL_FLOAT,0,Col);
                glLoadMatrixf(value_ptr(V*seg_mov));
                glDrawArrays(GL_TRIANGLES,0,nofver);
                seg_mov = translate(seg_mov,vec3(0,0,-0.1));
            case 1:
                seg_mov = translate(seg_mov, vec3(0,0,-PI/40+0.1));
                seg_mov = rotate(seg_mov,9*PI/180, vec3(0,1,0));
                glVertexPointer(3,GL_FLOAT,0 , Ver);
                glColorPointer(3,GL_FLOAT,0,Col);
                glLoadMatrixf(value_ptr(V*seg_mov));
                glDrawArrays(GL_TRIANGLES,0,nofver);
                seg_mov = translate(seg_mov,vec3(0,0,-0.1));
            case 2:
                seg_mov = translate(seg_mov, vec3(0,0,-PI/40+0.1));
                seg_mov = rotate(seg_mov,-9*PI/180, vec3(0,1,0));
                glVertexPointer(3,GL_FLOAT,0 , Ver);
                glColorPointer(3,GL_FLOAT,0,Col);
                glLoadMatrixf(value_ptr(V*seg_mov));
                glDrawArrays(GL_TRIANGLES,0,nofver);
                seg_mov = translate(seg_mov,vec3(0,0,-0.1));

        }

    }*/


}

//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wyczyœæ bufor kolorów (czyli przygotuj "p³ótno" do rysowania)

	//***Przygotowanie do rysowania****
    mat4 P=perspective(50.0f*PI/180.0f,1.0f,1.0f,50.0f); //Wylicz macierz rzutowania P
    mat4 V=lookAt( //Wylicz macierz widoku
                  vec3(position_x,3.0f,user_distance),
                  vec3(0.0f,0.0f,0.0f),
                  vec3(0.0f,1.0f,0.0f));
    if (rigth_turn && moves_counter%10 == 0){
        move_angle+=90;
        turn_counter = 10;
        add_cam_angle = 9;
        rigth_turn = false;
    }
    if (left_turn && moves_counter%10 == 0){
        move_angle-=90;
        turn_counter = 10;
        add_cam_angle = -9;
        left_turn = false;
        printf("l");
    }
    if (turn_counter){
        cam_angle += add_cam_angle;
        cam_angle = cam_angle%360;
        turn_counter -=1;
    }

    direction = vec3(sin(move_angle*PI/180)*speed,0,-cos(move_angle*PI/180)*speed);
    move = translate(move, direction);
    moves_counter +=1;
    V=rotate(V,(cam_angle)*PI/180,vec3(0,1,0));

    glMatrixMode(GL_PROJECTION); //W³¹cz tryb modyfikacji macierzy rzutowania
    glLoadMatrixf(value_ptr(P)); //Za³aduj macierz rzutowania
    glMatrixMode(GL_MODELVIEW);  //W³¹cz tryb modyfikacji macierzy model-widok

    glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u¿ywaj tablicy kolorów


    drawBoard(window, V*move);
    drawSnake(window,cam_angle, move, V, moves_counter);
    if (cam_angle==move_angle){
        Sn->move(10);
    }
    if(cam_angle > move_angle){
        Sn->move(22);
    }
    if(cam_angle<move_angle){
        Sn->move(21);
    }


    //Posprz¹taj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);


    glfwSwapBuffers(window); //Przerzuæ tylny bufor na przedni
}

int main(void)
{
	GLFWwindow* window; //WskaŸnik na obiekt reprezentuj¹cy okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów

	if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "Snake 3D", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Zainicjuj bibliotekê GLEW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjuj¹ce

	//G³ówna pêtla
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		drawScene(window); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}
