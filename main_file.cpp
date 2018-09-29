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

#include "lodepng.h"
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
#include "allmodels.h"
#include <ctime>
#include <vector>
#include "modelPaths.h"


using namespace glm;
using namespace std;

GLuint tex; //Globalnie

//œwiat³o
vec4 lightPos = vec4(0, 20, 0, 1);
vec4 lightDir = vec4(0, -1, 0, 0);
mat4 lightMov = mat4(1.0f);

//informacja nt. wielkoœci generowanej planszy:
int boardSize;
int minAmountOfFood;
int maxAmountOfFood;
int actAmountOfFood;

float foodAddingTime = 10.0f;

int maxPossibleTranslation;

struct foodTranslation {
    int translationX;
    int translationY;
    int translationZ;
};

vector < foodTranslation > foodTranslationVec;

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
mat4 mov= mat4(1.0f);
int move_angle = 0;
bool keyPress = false;
vec3 direction;
int cam_angle = 0;
int add_cam_angle= 0;
int turn_counter = 0;
bool rigth_turn=false;
bool left_turn=false;


// związanie ruchu sneak'a z czasem
const double mainTimePeriod = 2.0;
short speed =20 ;
double unitTimePeriod = mainTimePeriod/speed;
//double toGoTimeperiod = unitTimePeriod;
double leftFromUnit = 1;
bool toLeft;
double last_drawing;



//Procedura#include <lodepng.h> obs³ugi b³êdów
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

void initFirstFood(){
    srand(time(NULL));

    minAmountOfFood = 1;
    maxAmountOfFood = boardSize - 5;
    maxPossibleTranslation = boardSize - 1;
    foodTranslation tempFoodTranslation;

    actAmountOfFood = rand()%maxAmountOfFood + 1;

    for (int i=0; i < actAmountOfFood; i++) {
        tempFoodTranslation.translationX = (rand() % (maxPossibleTranslation*2)) - maxPossibleTranslation;
        tempFoodTranslation.translationY = (rand() % (maxPossibleTranslation*2)) - maxPossibleTranslation;
        tempFoodTranslation.translationZ = 0;

        foodTranslationVec.push_back(tempFoodTranslation);
    }


    cout << "Min amount of food = " << minAmountOfFood << "\n";
    cout << "Max amount of food = " << maxAmountOfFood << "\n";
    cout << "Act amount of food = " << actAmountOfFood << "\n";
    cout << "foodTranslationVec = " << foodTranslationVec.size() << "\n";
    cout << "Max possible translation = " << maxPossibleTranslation << "\n";

    for (int i=0; i < foodTranslationVec.size(); i++) {
        cout << i << "X: " << foodTranslationVec[i].translationX << "\n";
        cout << i << "Y: " << foodTranslationVec[i].translationY << "\n";
        cout << i << "Z: " << foodTranslationVec[i].translationZ << "\n";
        cout << "\n";
        //foodTranslationVec.push_back(tempFoodTranslation);
    }
}

void newFoodAdding(){
    if (foodAddingTime < 0) {
        cout << "Podmieniam jedzenie" << "\n";

        srand(time(NULL));
        foodTranslation tempFoodTranslation;
        tempFoodTranslation.translationX = (rand() % (maxPossibleTranslation*2)) - maxPossibleTranslation;
        tempFoodTranslation.translationY = (rand() % (maxPossibleTranslation*2)) - maxPossibleTranslation;
        tempFoodTranslation.translationZ = 0;

        if ( foodTranslationVec.size() == maxAmountOfFood) {
            foodTranslationVec.erase(foodTranslationVec.begin());
        }

        foodTranslationVec.push_back(tempFoodTranslation);

        for (int i=0; i < foodTranslationVec.size(); i++) {
        cout << i << "X: " << foodTranslationVec[i].translationX << "\n";
        cout << i << "Y: " << foodTranslationVec[i].translationY << "\n";
        cout << i << "Z: " << foodTranslationVec[i].translationZ << "\n";
        cout << "\n";
        //foodTranslationVec.push_back(tempFoodTranslation);
    }

        foodAddingTime = 10.0f;
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

	glEnable(GL_LIGHTING); //W³¹cz tryb cieniowania
	glEnable(GL_LIGHT0); //W³¹cz zerowe Ÿród³o œwiat³a
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie bufora g³êbokoœci
	glEnable(GL_COLOR_MATERIAL); //W³¹cz œledzenie kolorów przez materia³
	glEnable(GL_NORMALIZE); // dodanie znormalizowanego wektora normalnego

	//Wczytanie do pamiêci komputera
    std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
    unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
    //Wczytaj obrazek
    unsigned error = lodepng::decode(image, width, height, "bricks.png");

    //Import do pamiêci karty graficznej
    glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
    glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
    //Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
     GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

     //wybór algorytmu teksturowania
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_TEXTURE_2D);

    boardSize = gen(n, mySquareVertices, mySquareColors, mySquareNormals);
    initFirstFood();

    Sn = new Snake(headPath,"",tailPath,"",segmentPath,"");
}

//*****************************Procedury rysowania modeli*******************************

void drawBoard(GLFWwindow* window, mat4 V) {

    int tile_number = 0;
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,mySquareVertices); //Ustaw tablicê mySquareVertices jako tablicê wierzcho³ków
    glNormalPointer(GL_FLOAT,0,mySquareNormals);
    glColorPointer(3,GL_FLOAT,0,mySquareColors);

    //macierz modelu p³ytki
	mat4 tile_M=mat4(1.0f);

    //rysowanie kafelków
    glLoadMatrixf(value_ptr(V*tile_M));  // wyliczenie macierzy

    glDrawArrays(GL_TRIANGLES,0,mySquareVertexCount); //rysowanie


    glDisableClientState(GL_NORMAL_ARRAY);

}

void drawSnake(GLFWwindow* window, int ang, mat4 mov, mat4 V){
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

void drawTorus(GLFWwindow* window, mat4 V) {
    float angle = 90*PI/180;
    glColor3d(0.9f,0.8f,0.4f);
    mat4 torusV = V;

    for (int i = 0; i<foodTranslationVec.size(); i++) {
        //macierz modelu torusa
        mat4 torus_M=mat4(1.0f);
        //obróæ torusa o 90 stopni - po³o¿enie w poziomie
        torus_M = rotate(torus_M,angle,vec3(1.0f,0.0f,0.0f));

        //dokonaj translacji torusa
        torus_M = translate(torus_M, vec3(
                                  foodTranslationVec[i].translationX,
                                  foodTranslationVec[i].translationY,
                                  foodTranslationVec[i].translationZ));

        //zmniejsz torusa
        torus_M = scale(torus_M, vec3(0.3f,0.3f,0.3f));

        //rysowanie torusa
        glLoadMatrixf(value_ptr(torusV*torus_M));  // wyliczenie macierzy
        Models::torus.drawSolid();
        torusV = V;
    }

}

void drawWall(GLFWwindow* window, mat4 V) {
    float translation = (float)boardSize;
    glColor3d(1.0f,1.0f,1.0f);
    mat4 cubeV = V;
    //macierz modelu torusa
    mat4 cubeM1, cubeM2, cubeM3, cubeM4 = mat4(1.0f);

    //dokonaj translacji œciany #1
    cubeM1 = translate(cubeM1, vec3(translation,0.0f,-1.0f));
    cubeM1 = scale(cubeM1, vec3(1.0f,1.0f,translation));
    //rysowanie torusa
    glLoadMatrixf(value_ptr(cubeV*cubeM1));  // wyliczenie macierzy
    glBindTexture(GL_TEXTURE_2D,tex);
    Models::detailedCube.drawSolid();

    //dokonaj translacji œciany #2
    cubeM2 = translate(cubeM2, vec3(-translation-2,0.0f,-1.0f));
    cubeM2 = scale(cubeM2, vec3(1.0f,1.0f,translation));
    //rysowanie torusa
    glLoadMatrixf(value_ptr(cubeV*cubeM2));  // wyliczenie macierzy
    Models::detailedCube.drawSolid();

    //dokonaj translacji œciany #3
    cubeM3 = translate(cubeM3, vec3(-1.0f,0.0f,translation));
    cubeM3 = scale(cubeM3, vec3(translation,1.0f,1.0f));
    //rysowanie torusa
    glLoadMatrixf(value_ptr(cubeV*cubeM3));  // wyliczenie macierzy
    Models::detailedCube.drawSolid();

    //dokonaj translacji œciany #4
    cubeM4 = translate(cubeM4, vec3(-1.0f,0.0f,-translation-2));
    cubeM4 = scale(cubeM4, vec3(translation,1.0f,1.0f));
    //rysowanie torusa
    glLoadMatrixf(value_ptr(cubeV*cubeM4));  // wyliczenie macierzy
    Models::detailedCube.drawSolid();


}

//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window, double jump) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wyczyœæ bufor kolorów (czyli przygotuj "p³ótno" do rysowania)

	//***Przygotowanie do rysowania****
    mat4 P=perspective(50.0f*PI/180.0f,1.0f,1.0f,50.0f); //Wylicz macierz rzutowania P
    mat4 V=lookAt( //Wylicz macierz widoku
                  vec3(position_x,4.0f,user_distance),
                  vec3(0.0f,0.0f,0.0f),
                  vec3(0.0f,1.0f,0.0f));
    if (rigth_turn && leftFromUnit==1.0){
        rigth_turn = false;
        toLeft = false;
        move_angle+=90;
        turn_counter = 90;
        if(unitTimePeriod<0.1){add_cam_angle=90;}
        else{
                if(unitTimePeriod<0.3){add_cam_angle=30;}
                else{
                        if(unitTimePeriod<0.8){add_cam_angle=9;}
                        else{add_cam_angle=6;}
                }
        }
    }
    if (left_turn && leftFromUnit==1.0){
        left_turn = false;
        toLeft = true;
        move_angle-=90;
        turn_counter = 90;
        if(unitTimePeriod<0.1){add_cam_angle=-90;}
        else{
                if(unitTimePeriod<0.3){add_cam_angle=-45;}
                else{
                        if(unitTimePeriod<0.8){add_cam_angle=-10;}
                        else{add_cam_angle=-9;}
                }
        }
    }
    double actJump;
    if(jump>leftFromUnit){
        actJump=leftFromUnit;
        //toGoTimeperiod = unitTimePeriod;
        leftFromUnit=1.0;
    }
    else{
        actJump=jump;
        leftFromUnit-=jump;
        //toGoTimeperiod -= last_drawing;
    }
    if (turn_counter){
        turn_counter -=  abs(add_cam_angle);
        /*if(add_cam_angle == 0){
            add_cam_angle = 1;
            turn_counter -= 1;
        }*/
        cam_angle +=  add_cam_angle;
        cam_angle = cam_angle%360;
    }

    /*if (turn_counter){
        if(turn_counter> 90*jump){
            add_cam_angle =  90*jump;
            turn_counter -=  90*jump;
            if(add_cam_angle == 0){
                add_cam_angle = 1;
                turn_counter -= 1;
            }
        }
        else{
            add_cam_angle = turn_counter;
            turn_counter = 0;
        }
        if(toLeft){add_cam_angle = (-1)*add_cam_angle;}
        cam_angle +=  add_cam_angle;
        cam_angle = cam_angle%360;
    }*/
    direction = vec3(sin(move_angle*PI/180)*actJump,0,-cos(move_angle*PI/180)*actJump);
    mov = translate(mov, direction);
    printf("x: %f\ty: %f\tz: %f\n",mov[3][0],mov[3][1],mov[3][2]);
    lightMov = translate(lightMov,vec3((-1*direction.x)/2,0,(-1*direction.y)/2));
    V=rotate(V,(cam_angle)*PI/180,vec3(0,1,0));

    /*if (leftFromUnit>jump){
        direction = vec3(sin(move_angle*PI/180)*jump,0,-cos(move_angle*PI/180)*jump);
        mov = translate(mov, direction);
        lightMov = translate(lightMov,vec3((-1*direction.x)/2,0,(-1*direction.y)/2));
        leftFromUnit -= jump;
        toGoTimeperiod -= last_drawing;
        V=rotate(V,(cam_angle)*PI/180,vec3(0,1,0));
        printf("%d\n",cam_angle);
    }
    else{
        direction = vec3(sin(move_angle*PI/180)*leftFromUnit,0,-cos(move_angle*PI/180)*leftFromUnit);
        mov = translate(mov, direction);
        lightMov = translate(lightMov,vec3((-1*direction.x)/2,0,(-1*direction.y)/2));
        leftFromUnit = 1;
        toGoTimeperiod = unitTimePeriod;
        V=rotate(V,(cam_angle)*PI/180,vec3(0,1,0));
    }*/

    //*****************oœwietlenie*************************
    float lightColor[]={3,3,3,1};

    vec4 lightPosAct = V*lightMov*lightPos;
    vec4 lightDirAct = V*lightMov*lightDir;
    float lightPosTab[] = {lightPosAct.x, lightPosAct.y, lightPosAct.z, lightPosAct.w};
    float lightDirTab[] = {lightDirAct.x, lightDirAct.y, lightDirAct.z, lightDirAct.w};

    //float lightPos[]={0,10,0,1};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColor);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosTab);

    float lightDir[]={0,-1,0,0};

    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,lightDirTab);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF,60);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,10);

    //***************************************************

    glMatrixMode(GL_PROJECTION); //W³¹cz tryb modyfikacji macierzy rzutowania
    glLoadMatrixf(value_ptr(P)); //Za³aduj macierz rzutowania
    glMatrixMode(GL_MODELVIEW);  //W³¹cz tryb modyfikacji macierzy model-widok

    drawTorus(window, V*mov); //dodawanie torusów
    drawWall(window, V*mov); //dodawanie œciany

    glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u¿ywaj tablicy kolorów

    drawBoard(window, V*mov);
    drawSnake(window,cam_angle, mov, V);

    if (cam_angle==move_angle){
        Sn->mov(10);
    }
    if(cam_angle > move_angle){
        Sn->mov(22);
    }
    if(cam_angle<move_angle){
        Sn->mov(21);
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

	window = glfwCreateWindow(500, 500, "Snake 3D", NULL, NULL);  //Utwórz okno 500x500 o tytule "Snake 3D" i kontekst OpenGL.

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

	glfwSetTime(0); //Wyzeruj licznik czasu
	//G³ówna pêtla
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
	    //dla procedury synchronizacji czasowej
	    last_drawing = glfwGetTime();// zmienna przechowująca na czas rysowania kolejnej klatki, długość rysowania poprzedniej

	    foodAddingTime-= last_drawing;//Pomniejsz czas na generowanie nowego jedzonka o czas który up³yn¹³ od wykonania poprzedniej klatki
        newFoodAdding();
	    glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, last_drawing/unitTimePeriod); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.

	}

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}
