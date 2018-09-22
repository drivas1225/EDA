#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <cstdlib>

#include <time.h>
#include <iostream>
#include<GL/glut.h>
#include <sstream>
#include <fstream>
#include <string>
#include "quadTree.h"
#define KEY_ESC 27

using namespace std;

//Crear quad tree

    Point p(0,0);
    Rectan boundary = Rectan(p, 300, 300);
    QuadTree qtree = QuadTree(boundary,50 );
    Rectan range;
    vector<Point> points;
    double prom_x = 0;
    double prom_y = 0;
    Point center;



vector<Point> csvLector(string g)
{
    ifstream  archivodata(g);
    vector<Point> finalInput;
    string line;
    //cout<<getline(archivodata,line)<<"esto";
    int cont =0;
    while(getline(archivodata,line))
    {
            stringstream  lineStream(line);
            string cell;
            string latitud;
            string longitud;
            int i =0;

            while(getline(lineStream,cell,','))
            {
                    //cout<<"hola";
                    if (i==1){
                      latitud = cell;

                    }
                    if (i==2) {
                      longitud = cell;
                    }

                  i++;

            }
            cont++;
            double x= atof(latitud.c_str());
            double y= atof(longitud.c_str());
            prom_x=prom_x+x;
            prom_y=prom_y+y;
            Point a(x,y);
            //cout<<"punto "<<cont<< " : "<<x<<" "<<y<<endl;
            finalInput.push_back(a);
    }
    prom_x = prom_x / cont;
    prom_y = prom_y / cont;
    cout<<prom_x<<endl;
    cout<<prom_y<<endl;
    return finalInput;
}

void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex2d(0, 0);
	glVertex2d(300, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, 0);
	glVertex2d(0, 300);
	glEnd();
}

void displayRange()
{
    glColor3f(0,250,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(range.x - range.w , range.y-range.h);
    glVertex2f(range.x + range.w , range.y-range.h);
    glVertex2f(range.x + range.w , range.y+range.h);
    glVertex2f(range.x - range.w , range.y+range.h);
    glEnd();

    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for(int i = 0; i <points.size();i++){
        glVertex2i(points[i].x,points[i].y);
    }
    glEnd();


}


void OnMouseClick(int button, int state, int x, int y)
{


  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
      Point p(x-300,300-y);
        qtree.insert_p(p);
    //cout<<x-300 <<" "<<300-y<<endl;
    //convertir x,y
	//insertar un nuevo punto en el quadtree
  }
}

void OnMouseMotion(int x, int y)
{
    Point po(x-300,300-y);
    range = Rectan(po, 100, 100);

    points = qtree.consulta(range);
    cout<<"ciclo "<<points.size()<<endl;
    //int a;
    //cin>>a;


     //opcional
	 //hacer algo x,z cuando se mueve el mouse
}



void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-300.0f,  300.0f,-300.0f, 300.0f, -1.0f, 1.0f);



	//dibujar quadTree (qt->draw())
    qtree.draw();
    displayRange();

	//dibuja el gizmo
	//displayGizmo();

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.100f, 0.100f, 0.100f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {
    /*srand(time(0));
    int x ;
    int y ;
    for(int i = 0 ; i<500;i++){
        x = (rand()%600)-300;
        y = (rand()%600)-300;
        Point po(x,y);
        //cout<<x <<" " <<y<<endl;
        qtree.insert_p(po);
    }
   // cout<<qtree.points[0].x<< " "<<qtree.points[0].x<< " " << endl;
    */


     string archivo;
     archivo = "C:/Users/Lenovo/Desktop/Quadtree_gl/qtree_opengl/crime.csv";
     vector <Point> p_archivos = csvLector(archivo);
    //cout<<"archivo"<<p_archivos.size();
    for(int i = 0 ; i < p_archivos.size();i++){
        p_archivos[i].x = (p_archivos[i].x-prom_x)*1000;
        p_archivos[i].y = (p_archivos[i].y-prom_y)*1000;
        //cout<<p_archivos[i].x <<" "<<p_archivos[i].y<<endl;
        qtree.insert_p(p_archivos[i]);
    }

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP2 bis OpenGL : Bresenham"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutPassiveMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);



	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
