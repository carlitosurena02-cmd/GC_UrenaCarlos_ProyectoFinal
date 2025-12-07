#include<GL/glut.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>


// Estructuras Ocupadas
struct NodoLista{
	struct NodoLista *sig;
	struct NodoLista *ant;
	
};

struct NodoCola{

	struct NodoCola *sig;

};

struct Cola{
	struct NodoCola *inicio;
	struct NodoCola *final;
};

struct NodoPila{
	struct NodoPila *sig;
};

struct Pila{
	int tam;
	struct NodoPila *top;
};

struct PartePersonaje{
	int coordenadas[2];  
	struct PartePersonaje *sig;
};

struct NodoArbol{
	float color[3];
	char Nombre[20];
	struct PartePersonaje *parte;
	struct NodoArbol *izq;
	struct NodoArbol *der;
};

//Variables Globales
struct NodoArbol *Raiz = NULL;




//Prototipos de funciones
static void reshape01(int w, int h);
void display();
static void init01(void);


void ListinsertarParte(struct PartePersonaje **lista, int x, int y)
{
    struct PartePersonaje *nuevo=(struct PartePersonaje *) malloc(sizeof(struct PartePersonaje));

    if(nuevo == NULL)
        return;

    nuevo->coordenadas[0] = x;
    nuevo->coordenadas[1] = y;
    nuevo->sig = *lista;
    *lista = nuevo;
}

void insertarNodoArbol(struct NodoArbol **raiz, float color[3], char nombre[])
{
    struct NodoArbol *nuevo = (struct NodoArbol*) malloc(sizeof(struct NodoArbol));

    if(nuevo == NULL)
        return;

    strcpy(nuevo->Nombre,nombre);
    nuevo->color[0] = color[0];
    nuevo->color[1] = color[1];
    nuevo->color[2] = color[2];
    nuevo->parte = NULL;
    nuevo->izq = nuevo->der = NULL;
    *raiz = nuevo;
    return;
    
}

void dibujarpersonaje(struct NodoArbol *raiz)
{
	if(raiz == NULL)
		return;

	dibujarpersonaje(raiz->izq);
	
	glColor3f(raiz->color[0], raiz->color[1], raiz->color[2]);
	struct PartePersonaje *aux = raiz->parte;
        glBegin(GL_POLYGON);
        	for(;aux != NULL; aux = aux->sig)
        	{
	            glVertex3f(aux->coordenadas[0]*10, aux->coordenadas[1]*10,0.0);
        	}	
        glEnd();

    glColor3f(0.0,0.0,0.0);
    glLineWidth(8.0f);
    aux = raiz->parte;
    glBegin(GL_LINE_LOOP);
        	for(;aux != NULL; aux = aux->sig)
        	{
	            glVertex3f(aux->coordenadas[0]*10, aux->coordenadas[1]*10,0.0);
	            //glVertex3f(aux->sig->coordenadas[0]*20, aux->sig->coordenadas[1]*20,0.0);
        	}	
        glEnd();
	dibujarpersonaje(raiz->der);
}

void iniciarpersonaje(struct NodoArbol **raiz)
{
	float aux[3]={0.8,1.0,0.29};
	insertarNodoArbol(raiz,aux,"Brazo");
	ListinsertarParte(&((*raiz)->parte),11,14);
	ListinsertarParte(&((*raiz)->parte),11,10);
	ListinsertarParte(&((*raiz)->parte),13,10);
	ListinsertarParte(&((*raiz)->parte),13,10);

	insertarNodoArbol(&((*raiz)->izq),aux,"Cuerpo");
	ListinsertarParte(&((*raiz)->izq->parte),8,21);
	ListinsertarParte(&((*raiz)->izq->parte),8,5);
	ListinsertarParte(&((*raiz)->izq->parte),9,5);
	ListinsertarParte(&((*raiz)->izq->parte),9,2);
	ListinsertarParte(&((*raiz)->izq->parte),10,1);
	ListinsertarParte(&((*raiz)->izq->parte),11,1);
	ListinsertarParte(&((*raiz)->izq->parte),12,2);
	ListinsertarParte(&((*raiz)->izq->parte),12,4);
	ListinsertarParte(&((*raiz)->izq->parte),13,5);
	ListinsertarParte(&((*raiz)->izq->parte),15,5);
	ListinsertarParte(&((*raiz)->izq->parte),16,6);
	ListinsertarParte(&((*raiz)->izq->parte),17,6);
	ListinsertarParte(&((*raiz)->izq->parte),18,7);
	ListinsertarParte(&((*raiz)->izq->parte),19,8);
	ListinsertarParte(&((*raiz)->izq->parte),19,9);
	ListinsertarParte(&((*raiz)->izq->parte),20,9);
	ListinsertarParte(&((*raiz)->izq->parte),20,13);
	ListinsertarParte(&((*raiz)->izq->parte),24,13);
	ListinsertarParte(&((*raiz)->izq->parte),25,14);
	ListinsertarParte(&((*raiz)->izq->parte),25,15);
	ListinsertarParte(&((*raiz)->izq->parte),24,16);
	ListinsertarParte(&((*raiz)->izq->parte),21,16);
	ListinsertarParte(&((*raiz)->izq->parte),20,17);
	ListinsertarParte(&((*raiz)->izq->parte),20,18);
	ListinsertarParte(&((*raiz)->izq->parte),23,18);
	ListinsertarParte(&((*raiz)->izq->parte),23,19);
	ListinsertarParte(&((*raiz)->izq->parte),25,19);
	ListinsertarParte(&((*raiz)->izq->parte),25,21);
	ListinsertarParte(&((*raiz)->izq->parte),24,22);
	ListinsertarParte(&((*raiz)->izq->parte),19,22);
	ListinsertarParte(&((*raiz)->izq->parte),19,23);

	insertarNodoArbol(&((*raiz)->izq->izq),aux,"Pieizq");
	ListinsertarParte(&((*raiz)->izq->izq->parte),16,6);
	ListinsertarParte(&((*raiz)->izq->izq->parte),16,2);
	ListinsertarParte(&((*raiz)->izq->izq->parte),17,1);
	ListinsertarParte(&((*raiz)->izq->izq->parte),18,1);
	ListinsertarParte(&((*raiz)->izq->izq->parte),19,2);
	ListinsertarParte(&((*raiz)->izq->izq->parte),19,8);

	aux[0]=1.0;
	aux[1]=1.0;
	aux[2]=1.0;
	insertarNodoArbol(&((*raiz)->izq->der),aux,"Ojo");
	ListinsertarParte(&((*raiz)->izq->der->parte),11,20);
	ListinsertarParte(&((*raiz)->izq->der->parte),11,17);
	ListinsertarParte(&((*raiz)->izq->der->parte),12,17);
	ListinsertarParte(&((*raiz)->izq->der->parte),12,16);
	ListinsertarParte(&((*raiz)->izq->der->parte),15,16);
	ListinsertarParte(&((*raiz)->izq->der->parte),15,17);
	ListinsertarParte(&((*raiz)->izq->der->parte),16,17);
	ListinsertarParte(&((*raiz)->izq->der->parte),16,20);
	ListinsertarParte(&((*raiz)->izq->der->parte),15,20);
	ListinsertarParte(&((*raiz)->izq->der->parte),15,21);
	ListinsertarParte(&((*raiz)->izq->der->parte),12,21);
	ListinsertarParte(&((*raiz)->izq->der->parte),12,20);

	aux[0]=0.0;
	aux[1]=0.0;
	aux[2]=0.0;
	insertarNodoArbol(&((*raiz)->der->izq),aux,"pupila");
	ListinsertarParte(&((*raiz)->der->izq->parte),13,19);
	ListinsertarParte(&((*raiz)->der->izq->parte),12,19);
	ListinsertarParte(&((*raiz)->der->izq->parte),12,20);
	ListinsertarParte(&((*raiz)->der->izq->parte),13,20);

}

static void reshape01(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); // establecer la vista a todo
   	glMatrixMode (GL_PROJECTION); // usar la matriz de proyección
   	glLoadIdentity (); // y cargarla
   	// establecer a toda la pantalla
   	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

void display() 
{
    glClear(GL_COLOR_BUFFER_BIT); // Limpia el búfer de color
    dibujarpersonaje(Raiz);




    // intercambio de buffers (porque usamos GLUT_DOUBLE)
    glutSwapBuffers();
}

static void init01(void) 
{
	glClearColor (1.0, 1.0, 1.0, 0.0);  // Limpiar a color RGB A
   	glShadeModel (GL_FLAT); // El valor default es GL_SMOOTH
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv); // inicializa las bibliotecas de glut

    // configuración del modo de visualización de la pantalla
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 300);
    glutInitWindowPosition(300, 100);
    glutCreateWindow("Gleep al rescate");
    init01();
    iniciarpersonaje(&Raiz);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape01);
    //glutKeyboardFunc(teclado);
    glutMainLoop(); // loop principal
    return 0;
}

