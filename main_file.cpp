/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
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
#include <iostream>

using namespace glm;
using namespace std;

//odległość użytkownika od świata
float user_distance = -5.0f;

//położenie kamery
float position_x = 0.0f;
float position_y = 0.0f;
bool is_mouse_left_buton_pressed = false;
float old_coordinate_x;
float old_coordinate_y;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obsługi przycisku scrollowania myszy
void scroll_callback(GLFWwindow* window, double x, double y){
    if ( user_distance+y > -10 && user_distance+y < -2) user_distance += y;
}

//Procedura obsługi przycisku  myszy
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

//Procedura obsługi przewijania obrazu za pomocą kursora
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

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glfwSetScrollCallback(window, scroll_callback); //Zarejestruj procedurę obsługi scrollowania myszy
    glfwSetMouseButtonCallback(window, mouse_button_callback); //Zarejestruj procedurę obsługi przycisków myszy
    glfwSetCursorPosCallback(window, cursor_callback); //Zarejestruj procedurę obsługi przycisków myszy(window, mouse_button_callback); //Zarejestruj procedurę obsługi przycisków myszy

	glClearColor(0.5,0.5,0.5,1); //Ustaw kolor czyszczenia ekranu

}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float user_distance) {
	//************Tutaj umieszczaj kod rysujący obraz******************
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wyczyść bufor kolorów (czyli przygotuj "płótno" do rysowania)

	//***Przygotowanie do rysowania****
    mat4 P=perspective(50.0f*PI/180.0f,1.0f,1.0f,50.0f); //Wylicz macierz rzutowania P

    mat4 V=lookAt( //Wylicz macierz widoku
                  vec3(position_x,position_y,user_distance),
                  vec3(0.0f,0.0f,0.0f),
                  vec3(0.0f,1.0f,0.0f));

    glMatrixMode(GL_PROJECTION); //Włącz tryb modyfikacji macierzy rzutowania
    glLoadMatrixf(value_ptr(P)); //Załaduj macierz rzutowania
    glMatrixMode(GL_MODELVIEW);  //Włącz tryb modyfikacji macierzy model-widok

    glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania używaj tablicy wierzchołków
    glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania używaj tablicy kolorów

    glVertexPointer(3,GL_FLOAT,0,mySquareVertices); //Ustaw tablicę mySquareVertices jako tablicę wierzchołków
    glColorPointer(3,GL_FLOAT,0,mySquareColors); //Ustaw tablicę mySquareColors jako tablicę kolorów

    //macierz modelu
	mat4 M=mat4(1.0f);
    mat4 M_to_translate = M;

	//translacja i rysowanie
    M_to_translate = translate(M_to_translate,glm::vec3(1.2f,1.2f,0.0f));
    glLoadMatrixf(value_ptr(V*M_to_translate));
    M_to_translate = M;
    glDrawArrays(GL_TRIANGLES,0,mySquareVertexCount);

    M_to_translate = translate(M_to_translate,glm::vec3(-1.2f,1.2f,0.0f));
    glLoadMatrixf(value_ptr(V*M_to_translate));
    M_to_translate = M;
    glDrawArrays(GL_TRIANGLES,0,mySquareVertexCount);

    M_to_translate = translate(M_to_translate,glm::vec3(-1.2f,-1.2f,0.0f));
    glLoadMatrixf(value_ptr(V*M_to_translate));
    M_to_translate = M;
    glDrawArrays(GL_TRIANGLES,0,mySquareVertexCount);

    M_to_translate = translate(M_to_translate,glm::vec3(1.2f,-1.2f,0.0f));
    glLoadMatrixf(value_ptr(V*M_to_translate));
    M_to_translate = M;
    glDrawArrays(GL_TRIANGLES,0,mySquareVertexCount);

    //Posprzątaj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);


    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "Snake 3D", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		drawScene(window, user_distance); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
