//
//  main.cpp
//  Projeto_CG
//

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION

    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>

    #include "RgbImage.h"
    

    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>

    #include "RgbImage.h"

    #include <GL/glut.h>
#endif


//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED         0.776, 0.482, 0.588, 1.0
#define YELLOW     1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define ORANGE   1.0, 0.5, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.050, 0.050, 0.050, 1.0
#define GRAY     0.639, 0.666, 0.741, 1.0
#define PURPLE   0.494, 0.125, 0.258, 1.0
#define PI         3.14159

GLfloat luzGlobalCorAmb[4] = { 1.0, 1.0, 1.0, 1.0 };
GLint ambOn = 0;
GLfloat intensidade = 1.0;
GLfloat   luzR = 0.0;
GLfloat   luzG = 0.0;
GLfloat   luzB = 0.0;
GLfloat localPos[4] = { 0.0, 5.0, 0.0, 1.0 };
GLfloat localCorAmb[4] = { 0, 0, 0, 0.0 };
GLfloat localCorDif[4] = { 0.0, 0.0, 1.0f*intensidade, 1.0 };
GLfloat localCorEsp[4] = { 0.0, 0.0, 1.0f*intensidade, 1.0 };


GLfloat Foco_direccao[] = { 0, -1, 0, 0 };
GLfloat Foco1_cor[] = { luzR, luzG,  luzB, 1 };
GLfloat Foco_ak = 1.0;
GLfloat Foco_al = 0.05f;
GLfloat Foco_aq = 0.0f;
GLfloat Foco_Expon = 2.0;
GLfloat aberturaFoco = 80.0;
GLfloat Pos1[] = { 0.0f, 2.0f,  0.0f, 1.0f };
GLint foco = 0;
GLint pontual = 0;


void updateLuz() {
    Foco1_cor[0] = luzR;
    Foco1_cor[1] = luzG;
    Foco1_cor[2] = luzB;

    glLightfv(GL_LIGHT0, GL_AMBIENT, Foco1_cor);
}


void updateLuz1() {
    localCorDif[2] = 1.0f*intensidade;
    localCorEsp[2] = 1.0f*intensidade;

    glLightfv(GL_LIGHT1, GL_AMBIENT, localCorAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, localCorDif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, localCorEsp);
}

void initLights(void) {
    // Ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

    //foco
    glLightfv(GL_LIGHT0, GL_POSITION, Pos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Foco1_cor);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Foco_ak);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Foco_al);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Foco_aq);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, aberturaFoco);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Foco_direccao);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, Foco_Expon);
    
    glLightfv(GL_LIGHT1, GL_POSITION, localPos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, localCorAmb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, localCorDif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, localCorEsp);
}

void initMaterials(int material);
char Materiais[18][30] = {
    "Esmerald",  "Jade",  "obsidian",    "Pearl",        "Ruby",
    "Turquoise", "Brass", "Bronze",      "Chrome",       "Copper",
    "Gold",      "Silver","blackPlastic","cyankPlastic", "greenPlastic",
    "redPlastic", "whitePlastic","yellowPlastic" };

void initTexturas();
void textures(GLfloat meshW, GLfloat meshH, GLfloat z);
void mesh(GLfloat meshW, GLfloat meshH, GLfloat z);
GLUquadricObj* disk = gluNewQuadric();
extern GLfloat coef;
int dim = 512;
GLint porMesh = 1;

//Texturas
RgbImage imag;
GLuint   texture[9];
int text = 0;

//===========================================================Variaveis e constantes

GLfloat centrox = 0, centroy = 0, centroz = 0;
GLfloat rotacao = 0;

GLfloat tam = 0.5;
static GLfloat vertices[] = {
    //…………………………………………………………………………………………………… x=tam (Esquerda)
        -3.0,   0.5,  0.7,    // 0
        -3.0,   1.0,  0.7,    // 1
        -3.0,   1.0, -0.7,    // 2
        -3.0,   0.5, -0.7,    // 3
//…………………………………………………… (Direita)
         3.0,   0.0,  0.7,    // 4
         3.0,   1.0,  0.7,    // 5
         3.0,   1.0, -0.7,    // 6
         3.0,   0.0, -0.7,    // 7
//……………………………………………………… (Cima)
        -3.0,  1.0,  0.7,    // 8
        -3.0,  1.0, -0.7,    // 9
         3.0,  1.0, -0.7,    // 10
         3.0,  1.0,  0.7,    // 11
//……………………………………………………… (Baixo)
        -1.2,  0.0,  0.7,    // 12
        -1.2,  0.0, -0.7,    // 13
         3.0,  0.0, -0.7,    // 14
         3.0,  0.0,  0.7,    // 15
//……………………………………………………… (Tras)
        3.0,   0.0, -0.7,    // 16
        3.0,   1.0, -0.7,    // 17
       -1.2,   1.0, -0.7,    // 18
       -1.2,   0.0, -0.7,    // 19
//……………………………………………………… (Frente)
        3.0,   0.0,  0.7,    // 20
        3.0,   1.0,  0.7,    // 21
       -1.2,   1.0,  0.7,    // 22
       -1.2,   0.0,  0.7,    // 23
};
static GLfloat normais[] = {
    //…………………………………………………………………………………………………… (Esquerda)
      -1.0,  0.0,  0.0,
      -1.0,  0.0,  0.0,
      -1.0,  0.0,  0.0,
      -1.0,  0.0,  0.0,
   //…………………………………………………………………………………………………… (Direita)
       1.0,  0.0,  0.0,
       1.0,  0.0,  0.0,
       1.0,  0.0,  0.0,
       1.0,  0.0,  0.0,
    //…………………………………………………………………………………………………… (Cima)
       0.0,  1.0,  0.0,
       0.0,  1.0,  0.0,
       0.0,  1.0,  0.0,
       0.0,  1.0,  0.0,
    //…………………………………………………………………………………………………… (Baixo)
       0.0, -1.0,  0.0,
       0.0, -1.0,  0.0,
       0.0, -1.0,  0.0,
       0.0, -1.0,  0.0,
    //…………………………………………………………………………………………………… (Tras)
       0.0,  0.0, -1.0,
       0.0,  0.0, -1.0,
       0.0,  0.0, -1.0,
       0.0,  0.0, -1.0,
    //…………………………………………………………………………………………………… (Frente)
       0.0,  0.0,  1.0,
       0.0,  0.0,  1.0,
       0.0,  0.0,  1.0,
       0.0,  0.0,  1.0,
};

static GLuint esquerda[] = { 0, 1,  2,  3 };
static GLuint  direita[] = { 4, 7,  6,  5 };
static GLuint     cima[] = { 8, 11, 10, 9 };
static GLuint    baixo[] = {12, 13, 14, 15};
static GLuint     tras[] = {16, 17, 18, 19};
static GLuint   frente[] = {20, 21, 22, 23};

//Sistema Coordenadas + objectos
GLint        wScreen = 800, hScreen = 600;        //.. janela
GLfloat        xC = 10.0, yC = 10.0, zC = 10.0;    //.. Mundo
GLboolean   frenteVisivel = 1, isdown = 0;

//Observador
GLfloat  rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  angZoom = 45;
GLfloat  incZoom = 3;
GLboolean flag = 0;
GLint proj = 0;


//Rotacao
GLint     msec = 10, butesq = 0, butdir = 0, butcim = 0, butbaix = 0, butback = -12, butaoon = 0, butaofundo = 0;
GLfloat   altbu = 0;
GLfloat   incALT = 0.03;
GLfloat altbut[9];



void init(void){
    for(GLint i = 0;i<9; i++)
        altbut[i]=0;
    glClearColor(BLACK);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normais);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
}



void Timer(int value){
    glutPostRedisplay();
    glutTimerFunc(msec, Timer, 1);
}
GLUquadricObj* esfera = gluNewQuadric();
void drawEsfera()
{
    //------------------------- Esfera
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glPushMatrix();
        glRotatef(-90, 1,0,0);
        gluQuadricDrawStyle(esfera, GLU_FILL);
        gluQuadricNormals(esfera, GLU_SMOOTH);
        gluQuadricTexture(esfera, GL_TRUE);
        gluSphere(esfera, 60.0, 300, 300);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}



void drawEixos(){
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo
    glColor4f(RED);
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(10, 0, 0);
    glEnd();
    glColor4f(GREEN);
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 10, 0);
    glEnd();
    glColor4f(BLUE);
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 0, 10);
    glEnd();

}


void rectangle(GLfloat x, GLfloat y, GLfloat z){
    glPushMatrix();
    //cima
    if(altbut[6]<0 && butaoon == 1)
        glColor4f(BLUE);
    else
        glColor4f(BLACK);
    if(text==1){
        glPushMatrix();
            glRotatef(-90, 1.0, 0.0, 0.0);
            glTranslatef(0.0, 0.0, -z/1.5);
            textures(x, 1.5*y, z);
        glPopMatrix();
    }
    else{
        glBegin(GL_POLYGON);
        glVertex3f( x/2.0, y/2.0, -z/2.0);
        glVertex3f(-x/2.0, y/2.0, -z/2.0);
        glVertex3f(-x/2.0, y/2.0,  z/2.0);
        glVertex3f( x/2.0, y/2.0,  z/2.0);
        glEnd();
    }
    if(altbut[6]<0 && butaoon == 1)
        glColor4f(BLUE);
    else if(butaofundo == 1)
        glColor4f(BLACK);
    else
        glColor4f(PURPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 0.2);
    //baixo
    glBegin(GL_POLYGON);
    glVertex3f( x/2.0, -y/2.0, -z/2.0);
    glVertex3f(-x/2.0, -y/2.0, -z/2.0);
    glVertex3f(-x/2.0, -y/2.0,  z/2.0);
    glVertex3f( x/2.0, -y/2.0,  z/2.0);
    glEnd();
    //direita
    glBegin(GL_POLYGON);
    glVertex3f( x/2.0,  y/2.0, -z/2.0);
    glVertex3f( x/2.0,  y/2.0,  z/2.0);
    glVertex3f( x/2.0, -y/2.0,  z/2.0);
    glVertex3f( x/2.0, -y/2.0, -z/2.0);
    glEnd();
    //esquerda
    glBegin(GL_POLYGON);
    glVertex3f(-x/2.0,  y/2.0,  z/2.0);
    glVertex3f(-x/2.0,  y/2.0, -z/2.0);
    glVertex3f(-x/2.0, -y/2.0, -z/2.0);
    glVertex3f(-x/2.0, -y/2.0,  z/2.0);
    glEnd();
    //tras
    glBegin(GL_POLYGON);
    glVertex3f(-x/2.0,  y/2.0, -z/2.0);
    glVertex3f( x/2.0,  y/2.0, -z/2.0);
    glVertex3f( x/2.0, -y/2.0, -z/2.0);
    glVertex3f(-x/2.0, -y/2.0, -z/2.0);
    glEnd();
    //frente
    glBegin(GL_POLYGON);
    glVertex3f( x/2.0,  y/2.0,  z/2.0);
    glVertex3f(-x/2.0,  y/2.0,  z/2.0);
    glVertex3f(-x/2.0, -y/2.0,  z/2.0);
    glVertex3f( x/2.0, -y/2.0,  z/2.0);
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
}


/*void cilindro(float raio, float altura){
    glPushMatrix();
    GLint count = 0;
    GLfloat a = 0.0f, b = 0.0f;
    for(GLint i = 0; i<31; i++){
        GLfloat theta = 2.0f * PI * GLfloat(i) / GLfloat(30);
        GLfloat x = raio * cosf(theta);
        GLfloat y = raio * sinf(theta);
        if(count==1){
            glBegin(GL_POLYGON);
            glVertex3f(x, altura, y);
            glVertex3f(x, 0.0, y);
            glVertex3f(a, 0.0, b);
            glVertex3f(a, altura, b);
            glEnd();
            count = 0;
        }
        a = x;
        b = y;
        count++;
    }
    glBegin(GL_POLYGON);
    glColor4f(BLACK);
    for(GLint i = 0; i<31; i++){
        GLfloat theta = 2.0f * PI * GLfloat(i) / GLfloat(30);
        GLfloat x = raio * cosf(theta);
        GLfloat y = raio * sinf(theta);
        glVertex3f(x, altura, y);
    }
    glEnd();
    glColor4f(PURPLE);
    glBegin(GL_POLYGON);
    for(GLint i = 0; i<31; i++){
        GLfloat theta = 2.0f * PI * GLfloat(i) / GLfloat(30);
        GLfloat x = raio * cosf(theta);
        GLfloat y = raio * sinf(theta);
        glVertex3f(x, 0.0, y);
    }
    glEnd();
    glPopMatrix();
}*/


//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    //Butoes
    case '1':
        if(altbut[1]==0){
            altbut[1] = -0.1;
        }
        else
            altbut[1] = 0;
        glutPostRedisplay();
        break;
    case '2':
        if(altbut[2]==0){
            altbut[2] = -0.1;
        }
        else
            altbut[2] = 0;
        glutPostRedisplay();
        break;
    case '3':
        if(altbut[0]==0){
            altbut[0] = -0.1;
        }
        else
            altbut[0] = 0;
        glutPostRedisplay();
        break;
    case '4':
    if(altbut[4]==0){
        altbut[4] = -0.05;
    }
    else
        altbut[4] = 0;
    glutPostRedisplay();
    break;
    case '5':
        if(altbut[5]==0)
            altbut[5] = -0.05;
        else
            altbut[5] = 0;
        glutPostRedisplay();
        break;
    case '6':
        if(altbut[6]==0){
            altbut[6] = -0.05;
        }
        else
            altbut[6] = 0;
        glutPostRedisplay();
        break;
    case '7':
        if(altbut[7]==0){
            altbut[7] = -0.04;
        }
        else
            altbut[7] = 0;
        glutPostRedisplay();
        break;
    case 'B':
    case 'b':
        if(butback==-9)
            butback = -12;
        else
            butback = -9;
        glutPostRedisplay();
        break;
    //Controlar objeto
    case 'A':
    case 'a':
        centrox = centrox - 0.25;
        glutPostRedisplay();
        break;
    case 'd':
    case 'D':
        centrox = centrox + 0.25;
        glutPostRedisplay();
        break;
    case 'w':
    case 'W':
        centroy = centroy + 0.25;
        glutPostRedisplay();
        break;
    case 's':
    case 'S':
        centroy = centroy - 0.25;
        glutPostRedisplay();
        break;
    case 'z':
    case 'Z':
        centroz = centroz + 0.25;
        glutPostRedisplay();
        break;
    case 'x':
    case 'X':
        centroz = centroz - 0.25;
        glutPostRedisplay();
        break;
    case 'e':
    case 'E':
        rotacao = rotacao + 2.5;
        glutPostRedisplay();
        break;
    case 'q':
    case 'Q':
        rotacao = rotacao - 2.5;
        glutPostRedisplay();
        break;
    //Butoes retangulos comando
    case 'F':
    case 'f':
        if(butesq==0)
            butesq = 8;
        else
            butesq = 0;
        glutPostRedisplay();
        break;
    case 'H':
    case 'h':
        if(butdir==0)
            butdir = -8;
        else
            butdir = 0;
        glutPostRedisplay();
        break;
    case 'T':
    case 't':
        if(butcim==0)
            butcim = 8;
        else
            butcim = 0;
        glutPostRedisplay();
        break;
    case 'G':
    case 'g':
        if(butbaix==0)
            butbaix = -8;
        else
            butbaix = 0;
        glutPostRedisplay();
        break;
    //Zoom
    case 'c':
    case 'C':
        rVisao++;
        obsP[0] = rVisao * cos(aVisao);
        obsP[2] = rVisao * sin(aVisao);
        glutPostRedisplay();
        break;
    case 'v':
    case 'V':
        rVisao--;
        obsP[0] = rVisao * cos(aVisao);
        obsP[2] = rVisao * sin(aVisao);
        glutPostRedisplay();
        break;
    //Mudanca projecao
    case '0':
        if(proj!=3)
            proj++;
        else
            proj = 0;
        glutPostRedisplay();
        break;
            
    //Coeficiente
    case 'M':
    case 'm':
            if(coef<=0.0)
                coef=1.0;
            else
                coef-=0.1;
            glutPostRedisplay();
            break;
            
    case 'N':
    case 'n':
            ambOn = !ambOn;
            if (ambOn) {
                luzGlobalCorAmb[0] = 1.0;
                luzGlobalCorAmb[1] = 1.0;
                luzGlobalCorAmb[2] = 1.0; }
            else {
                luzGlobalCorAmb[0] = 0.0;
                luzGlobalCorAmb[1] = 0.0;
                luzGlobalCorAmb[2] = 0.0; }
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
            glutPostRedisplay();
            break;
    case 'K':
    case 'k':
            
            if(foco==0)
                foco=1;
            else{
                foco=0;
                glDisable(GL_LIGHT0);
            }
            glutPostRedisplay();
            break;
            
    case 'i':
    case 'I':
            if(luzR>=1.0)
                luzR=0.0;
            else
                luzR+=0.1;
            updateLuz();
            glutPostRedisplay();
            break;
    case 'o':
    case 'O':
            if(luzG>=1.0)
                luzG=0.0;
            else
                luzG+=0.1;
            updateLuz();
            glutPostRedisplay();
            break;
    case 'p':
    case 'P':
            if(luzB>=1.0)
                luzB=0.0;
            else
                luzB+=0.1;
            updateLuz();
            glutPostRedisplay();
            break;
    case 'j':
    case 'J':
            if(pontual==0)
                pontual=1;
            else{
                pontual=0;
                glDisable(GL_LIGHT1);
            }
            glutPostRedisplay();
            break;
    case 'u':
    case 'U':
            if(intensidade>=1.0)
                intensidade=0.0;
            else
                intensidade+=0.1;
            updateLuz1();
            glutPostRedisplay();
            break;
    case '8':
            if(porMesh==1){
                porMesh=0;
                glShadeModel(GL_FLAT);
            }
            else{
                porMesh=1;
                glShadeModel(GL_SMOOTH);
            }
            glutPostRedisplay();
            break;
    
    //--------------------------- Escape
    case 27:
        exit(0);
        break;
    }
}



void teclasNotAscii(int key, int x, int y) {

    if (key == GLUT_KEY_UP)
        obsP[1] = (obsP[1] + 0.2);
    if (key == GLUT_KEY_DOWN)
        obsP[1] = (obsP[1] - 0.2);

    if (obsP[1] > yC)
        obsP[1] = yC;
    if (obsP[1] < -yC)
        obsP[1] = -yC;

    if (key == GLUT_KEY_LEFT)
        aVisao = (aVisao + 0.2);
    if (key == GLUT_KEY_RIGHT)
        aVisao = (aVisao - 0.2);

    obsP[0] = rVisao * cos(aVisao);
    obsP[2] = rVisao * sin(aVisao);


    glutPostRedisplay();

}



void rectbut(){
    glPushMatrix();
        glTranslatef(-2.15, 1.0, 0.0);
        glRotatef(butesq, 1.0, 0.0, 0.0);
        glRotatef(butdir, 1.0, 0.0, 0.0);
        glRotatef(butcim, 0.0, 0.0, 1.0);
        glRotatef(butbaix, 0.0, 0.0, 1.0);
        glTranslatef(2.15, -1.0, 0.0);
        glPushMatrix();
            glTranslatef(-2.15, 1.0, 0.0);
            text=1;
            rectangle(0.7, 0.2, 0.3);
            text=0;
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-2.15, 1.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            text=1;
            rectangle(0.7, 0.2, 0.3);
            text=0;
        glPopMatrix();
    glPopMatrix();
}

void buttons(){
    //Butao A
    glColor4f(PURPLE);
    glPushMatrix();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_BLEND );
        glColor4f(1.0, 1.0, 1.0, 0.2);
        glTranslatef(-1.2, 0.9 + altbut[0], 0.0);
        glRotatef(-90, 1.0, 0.0 , 0.0);
        gluCylinder(gluNewQuadric(), 0.3, 0.3, 0.22, 100, 100);
        glTranslatef(0.0, 0.0, 0.22);
        glEnable(GL_LIGHTING);
        if(foco==1)
            glEnable(GL_LIGHT0);
        if(pontual==1)
            glEnable(GL_LIGHT1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[6]);
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
        gluQuadricTexture(disk, GL_TRUE);
        glRotatef(90, 0.0, 0.0 , 1.0);
        gluDisk(disk, 0.0, 0.3, 100, 100);
        glDisable(GL_BLEND);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //Butao 1
    glPushMatrix();
        glTranslatef(1.2, 0.9 + altbut[1], 0.0);
        initMaterials(16);
        glRotatef(-90, 1.0, 0.0 , 0.0);
        gluCylinder(gluNewQuadric(), 0.15, 0.15, 0.21, 100, 100);
        glTranslatef(0.0, 0.0, 0.21);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glPushMatrix();
            gluQuadricDrawStyle(disk, GLU_FILL);
            gluQuadricNormals(disk, GLU_SMOOTH);
            gluQuadricTexture(disk, GL_TRUE);
            glRotatef(90, 0.0, 0.0 , 1.0);
            gluDisk(disk, 0.0, 0.15, 100, 100);
        glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    //Butao 2
    glPushMatrix();
        glTranslatef(1.8, 0.9 + altbut[2], 0.0);
        glRotatef(-90, 1.0, 0.0 , 0.0);
        gluCylinder(gluNewQuadric(), 0.15, 0.15, 0.21, 100, 100);
        glTranslatef(0.0, 0.0, 0.21);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
        gluQuadricTexture(disk, GL_TRUE);
        glRotatef(90, 0.0, 0.0 , 1.0);
        gluDisk(disk, 0.0, 0.15, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //Butao -
    glPushMatrix();
        glTranslatef(0.0, 0.83 + altbut[4], 0.5);
        glRotatef(-90, 1.0, 0.0 , 0.0);
        gluCylinder(gluNewQuadric(), 0.1, 0.1, 0.25, 100, 100);
        glTranslatef(0.0, 0.0, 0.25);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
        gluQuadricTexture(disk, GL_TRUE);
        glRotatef(90, 0.0, 0.0 , 1.0);
        gluDisk(disk, 0.0, 0.1, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //Butao +
    glPushMatrix();
        glTranslatef(0.0, 0.83 + altbut[5], -0.5);
        glRotatef(-90, 1.0, 0.0 , 0.0);
        gluCylinder(gluNewQuadric(), 0.1, 0.1, 0.25, 100, 100);
        glTranslatef(0.0, 0.0, 0.25);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
        gluQuadricTexture(disk, GL_TRUE);
        glRotatef(90, 0.0, 0.0 , 1.0);
        gluDisk(disk, 0.0, 0.1, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //Butao On
    glPushMatrix();
    glTranslatef(-2.7, 0.83 + altbut[6], 0.5);
        glRotatef(-90, 1.0, 0.0 , 0.0);
        gluCylinder(gluNewQuadric(), 0.05, 0.05, 0.25, 100, 100);
        glTranslatef(0.0, 0.0, 0.25);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
        gluQuadricTexture(disk, GL_TRUE);
        glRotatef(90, 0.0, 0.0 , 1.0);
        gluDisk(disk, 0.0, 0.05, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //Butao Home
    glPushMatrix();
        glTranslatef(0.0, 0.80 + altbut[7], 0.0);
        glRotatef(-90, 1.0, 0.0 , 0.0);
        gluCylinder(gluNewQuadric(), 0.1, 0.1, 0.25, 100, 100);
        glTranslatef(0.0, 0.0, 0.25);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
        gluQuadricTexture(disk, GL_TRUE);
        glRotatef(90, 0.0, 0.0 , 1.0);
        gluDisk(disk, 0.0, 0.1, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    //Butoes do fundo
    glColor4f(BLACK);
    butaofundo = 1;
    glPushMatrix();
    glTranslatef(2.5, 0.98, 0.15);
    rectangle(0.1, 0.1, 0.1);
    glPopMatrix();
    glPushMatrix();
    if(altbut[6]<0)
        glColor4f(BLUE);
    glTranslatef(2.5, 0.98, 0.4);
    butaoon = 1;
    rectangle(0.1, 0.1, 0.1);
    butaoon = 0;
    glPopMatrix();
    glColor4f(BLACK);
    glPushMatrix();
    glTranslatef(2.5, 0.98, -0.15);
    rectangle(0.1, 0.1, 0.1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.5, 0.98, -0.4);
    rectangle(0.1, 0.1, 0.1);
    butaofundo = 0;
    glPopMatrix();
}



void drawBackbutton(){
    glEnable(GL_LIGHTING);
    if(foco==1)
        glEnable(GL_LIGHT0);
    if(pontual==1)
        glEnable(GL_LIGHT1);
    
    initMaterials(15);
    glColor4f(PURPLE);
    glPushMatrix();
    glTranslatef(-3.0, 0.25, 0.0);
    glRotatef(butback, 0.0, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
    GLfloat dist = 1.2, a = 0.0f, b = 0.0f;
    GLint count = 0;
    for(GLint i = 0; i< 51; i++){
        GLfloat theta = 2.0f * PI / 4.0f * GLfloat(i) / GLfloat(50);
        GLfloat x = 0.3f * cosf(theta);
        GLfloat y = 0.3f * sinf(theta);
        glVertex3f(x + dist, y, 0.35f);
        if(count==1){
            glBegin(GL_POLYGON);
            glVertex3f(x + dist, y, 0.35f);
            glVertex3f(x + dist, y, -0.35f);
            glVertex3f(a, b, -0.35f);
            glVertex3f(a, b, 0.35f);
            glEnd();
            //face frente
            glBegin(GL_POLYGON);
            glVertex3f(x + dist, y, 0.35f);
            glVertex3f(a, b, 0.35f);
            glVertex3f(a, 0.5, 0.35f);
            glVertex3f(+ x + dist, 0.5, 0.35f);
            glEnd();
            //face tras
            glBegin(GL_POLYGON);
            glVertex3f(x + dist, y, -0.35f);
            glVertex3f(a, b, -0.35f);
            glVertex3f(a, 0.5, -0.35f);
            glVertex3f(x + dist, 0.5, -0.35f);
            glEnd();
            count = 0;
        }
        a = x + dist;
        b = y;
        dist = dist - 1.2f/50;
        count++;
    }
    glEnd();
    //face direita
    glBegin(GL_POLYGON);
    glVertex3f(1.5, 0.0, -0.35f);
    glVertex3f(1.5, 0.5, -0.35f);
    glVertex3f(1.5, 0.5,  0.35f);
    glVertex3f(1.5, 0.0,  0.35f);
    glEnd();
    glBegin(GL_LINE_STRIP);
    dist = 1.2;
    for(GLint i = 0; i< 50; i++){
        GLfloat theta = 2.0f * PI / 4.0f * GLfloat(i) / GLfloat(50);
        GLfloat x = 0.3f * cosf(theta);
        GLfloat y = 0.3f * sinf(theta);
        glVertex3f(x + dist, y, -0.35f);
        dist = dist - 1.2f/50;
    }
    glEnd();
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}



void drawObs(){
    glPushMatrix();
    glTranslatef(obsP[0], obsP[1], obsP[2]);
    glutWireSphere(0.3, 10, 5);
    glPopMatrix();
}



void drawComando() {
    //==================================== Comando
    glColor4f(GRAY);
    glTranslatef(centrox, centroy, centroz);
    glRotatef(rotacao, 0.0, 1.0, 0.0);
    glEnable(GL_LIGHTING);
    if(foco==1)
        glEnable(GL_LIGHT0);
    if(pontual==1)
        glEnable(GL_LIGHT1);
    
    initMaterials(11);
    glPushMatrix();
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
        if(porMesh==1){
            glPushMatrix();
                glTranslatef(0.0, -0.4, 0.0);
                glRotatef(-90, 1.0, 0.0, 0.0);
                mesh(6, 1.4, 1.4);
            glPopMatrix();
        }
        else{
            glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
        }
        initMaterials(11);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
        glColor4f(BLACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor4f(GRAY);
        glBegin(GL_LINE_STRIP);
        GLfloat dist = 1.3, a = 0.0f, b = 0.0f;
        GLint count = 0;
        for(GLint i = 0; i< 51; i++){
            GLfloat theta = 2.0f * PI / 4.0f * GLfloat(i) / GLfloat(50);
            GLfloat x = 0.5f * cosf(theta);
            GLfloat y = 0.5f * sinf(theta);
            if(count==1){
                glBegin(GL_POLYGON);
                glVertex3f(-3.0 + x + dist, y, 0.7f);
                glVertex3f(-3.0 + x + dist, y, -0.7f);
                glVertex3f(a, b, -0.7f);
                glVertex3f(a, b, 0.7f);
                glEnd();
                //face frente
                glBegin(GL_POLYGON);
                glVertex3f(-3.0 + x + dist, y, 0.7f);
                glVertex3f(a, b, 0.7f);
                glVertex3f(a, 1.0, 0.7f);
                glVertex3f(-3.0 + x + dist, 1.0, 0.7f);
                glEnd();
                //face tras
                glBegin(GL_POLYGON);
                glVertex3f(-3.0 + x + dist, y, -0.7f);
                glVertex3f(-3.0 + x + dist, 1.0, -0.7f);
                glVertex3f(a, 1.0, -0.7f);
                glVertex3f(a, b, -0.7f);
                glEnd();
                count = 0;
            }
            a = -3.0f + x + dist;
            b = y;
            dist = dist - 1.3f/50;
            count++;
        }
        glEnd();
        //circunferencia
        glBegin(GL_LINE_STRIP);
        glColor4f(BLACK);
        dist = 1.3;
        for(GLint i = 0; i< 50; i++){
            GLfloat theta = 2.0f * PI / 4.0f * GLfloat(i) / GLfloat(50);
            GLfloat x = 0.5f * cosf(theta);
            GLfloat y = 0.5f * sinf(theta);
            glVertex3f(-3.0 + x + dist, y, -0.7f);
            dist = dist - 1.3f/50;
        }
        glEnd();
        //circunferencia
        glBegin(GL_LINE_STRIP);
        dist = 1.3; a = 0.0f; b = 0.0f;
        for(GLint i = 0; i< 51; i++){
            GLfloat theta = 2.0f * PI / 4.0f * GLfloat(i) / GLfloat(50);
            GLfloat x = 0.5f * cosf(theta);
            GLfloat y = 0.5f * sinf(theta);
            glVertex3f(-3.0 + x + dist, y, 0.7f);
            dist = dist - 1.3f/50;
        }
        glEnd();
        glColor4f(GRAY);
    glDisable(GL_LIGHTING);
    glPopMatrix();
}



void display(void) {
    //================================================================= Apaga
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if(proj==0){
        gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 30.0 * zC);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);
    }
    if(proj==1){
        glOrtho(-20, 20, -20, 20, -100, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
        drawObs();
    }
    if(proj==2){
        glOrtho(-20, 20, -20, 20, -100, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(10, 0, 0, 0, 0, 0, 0, 1, 0);
        drawObs();
    }
    if(proj==3){
        glOrtho(-20, 20, -20, 20, -100, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 10, 0, 0, 0, 0, 0, 0, 1);
        drawObs();
    }
    drawEsfera();
    drawEixos();
    initLights();
    drawComando();
    buttons();
    rectbut();
    drawBackbutton();
    

    glutSwapBuffers();
}



int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(wScreen, hScreen);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("Mário Lemos|  |Observador-'SETAS'|  |Andar-'w/a/s/d/z/x'|  |Rodar-'q/e'|  |Butoes-'1/2/3/4/5/6/7/b'&&'t/f/g/h'|  |Projecao-'0'|");

    init();

    initTexturas();

    glutSpecialFunc(teclasNotAscii);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(msec, Timer, 1);

    glutMainLoop();

    return 0;
}


void initTexturas() {
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/1.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/2.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    
    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/-.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    
    glGenTextures(1, &texture[3]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/+.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    
    glGenTextures(1, &texture[4]);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/home.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    
    glGenTextures(1, &texture[5]);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/on.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    
    glGenTextures(1, &texture[6]);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/a.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    
    glGenTextures(1, &texture[7]);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/rect.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
    
    glGenTextures(1, &texture[8]);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    imag.LoadBmpFile("/Users/mariolemos/Documents/Universidade/CG/Projeto_CG/Projeto_CG/skybox2.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        (int)imag.GetNumCols(),
        (int)imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());
}



void textures(GLfloat meshW, GLfloat meshH, GLfloat z){
    int i, j;

    glEnable(GL_LIGHTING);
    if(foco==1)
        glEnable(GL_LIGHT0);
    if(pontual==1)
        glEnable(GL_LIGHT1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[7]);

    //Desenha mesh com normais perpendiculares
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();

    glTranslatef(-meshW * 0.5, -meshH * 0.5, 0);  // meio do poligono

    glNormal3f(0, 0, 1);

    glBegin(GL_QUADS);
    for (i = 0; i < dim - 1; i++) {
        for (j = 0; j < dim - 1; j++) {
            GLfloat x0 = meshW * i / GLfloat(dim - 1);
            GLfloat y0 = meshH * j / GLfloat(dim - 1);
            GLfloat u0 = i / GLfloat(dim - 1);
            GLfloat v0 = j / GLfloat(dim - 1);

            GLfloat x1 = meshW * (i + 1) / GLfloat(dim - 1);
            GLfloat y1 = meshH * j / GLfloat(dim - 1);
            GLfloat u1 = (i + 1) / GLfloat(dim - 1);
            GLfloat v1 = j / GLfloat(dim - 1);

            GLfloat x2 = meshW * (i + 1) / GLfloat(dim - 1);
            GLfloat y2 = meshH * (j + 1) / GLfloat(dim - 1);
            GLfloat u2 = (i + 1) / GLfloat(dim - 1);
            GLfloat v2 = (j + 1) / GLfloat(dim - 1);

            GLfloat x3 = meshW * i / GLfloat(dim - 1);
            GLfloat y3 = meshH * (j + 1) / GLfloat(dim - 1);
            GLfloat u3 = i / GLfloat(dim - 1);
            GLfloat v3 = (j + 1) / GLfloat(dim - 1);

            glTexCoord2f(u0, v0);
            glVertex3f(x0, y0, z);
            glTexCoord2f(u1, v1);
            glVertex3f(x1, y1, z);
            glTexCoord2f(u2, v2);
            glVertex3f(x2, y2, z);
            glTexCoord2f(u3, v3);
            glVertex3f(x3, y3, z);

        }
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

}


void mesh(GLfloat meshW, GLfloat meshH, GLfloat z){
    int i, j;

    //Desenha mesh com normais perpendiculares
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();

    glTranslatef(-meshW * 0.5, -meshH * 0.5, 0);  // meio do poligono

    glNormal3f(0, 0, 1);          //normal

    glBegin(GL_QUADS);
    for (i = 0; i < dim - 1; i++) {
        for (j = 0; j < dim - 1; j++) {
            GLfloat x0 = meshW * i / GLfloat(dim - 1);
            GLfloat y0 = meshH * j / GLfloat(dim - 1);
            GLfloat u0 = i / GLfloat(dim - 1);
            GLfloat v0 = j / GLfloat(dim - 1);

            GLfloat x1 = meshW * (i + 1) / GLfloat(dim - 1);
            GLfloat y1 = meshH * j / GLfloat(dim - 1);
            GLfloat u1 = (i + 1) / GLfloat(dim - 1);
            GLfloat v1 = j / GLfloat(dim - 1);

            GLfloat x2 = meshW * (i + 1) / GLfloat(dim - 1);
            GLfloat y2 = meshH * (j + 1) / GLfloat(dim - 1);
            GLfloat u2 = (i + 1) / GLfloat(dim - 1);
            GLfloat v2 = (j + 1) / GLfloat(dim - 1);

            GLfloat x3 = meshW * i / GLfloat(dim - 1);
            GLfloat y3 = meshH * (j + 1) / GLfloat(dim - 1);
            GLfloat u3 = i / GLfloat(dim - 1);
            GLfloat v3 = (j + 1) / GLfloat(dim - 1);

            glTexCoord2f(u0, v0);
            glVertex3f(x0, y0, z);
            glTexCoord2f(u1, v1);
            glVertex3f(x1, y1, z);
            glTexCoord2f(u2, v2);
            glVertex3f(x2, y2, z);
            glTexCoord2f(u3, v3);
            glVertex3f(x3, y3, z);

        }
    }
    glEnd();
    glPopMatrix();

}
