#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Estructuras Ocupadas
struct NodoLista{
	struct NodoLista *sig;
	struct NodoLista *ant;
	
};

struct coordenada{
	int x;
	int y;
};

struct camino{
	struct coordenada coord;
	struct camino *sig;
};

struct Camino{
	struct camino *inicio;
	struct camino *final;
};

struct NodoCola{
	int scene;
	int textura;
	struct coordenada coords[4];
	struct NodoCola *sig;
};

struct Cola{
	struct NodoCola *inicio;
	struct NodoCola *final;
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


//Prototipos de funciones
void ListinsertarParte(struct PartePersonaje **lista, int x, int y);
void insertarNodoArbol(struct NodoArbol **raiz, float color[3], char nombre[]);
void dibujarpersonaje(struct NodoArbol *raiz);
void iniciarpersonaje(struct NodoArbol **raiz);
static void reshape01(int w, int h);
void display();
static void init01(void);
GLuint CargarTextura(const char *ruta);


//Variables Globales
struct NodoArbol *Raiz = NULL;
struct NodoArbol *Aux = NULL;
struct NodoArbol *Aux2 = NULL;
struct camino *caminoaux;
struct Cola Cola;
struct Camino Camino;
int texturapiso;
int texturafondo;
int caja;
int gujero;
int tesoro;
int pasoAnim = 0;
int escena = 0;
int ventana = 0;


void enqueue(struct Cola *cola, int tam, int x, int y, int escena,int textura)
{
    struct NodoCola *nuevo = (struct NodoCola *)malloc(sizeof(struct NodoCola));
    
    if(nuevo == NULL) 
        return;

    float dist = tam /2; 
    
    nuevo->coords[0].x = x - dist;
    nuevo->coords[0].y = y - dist;
    nuevo->coords[1].x = x + dist;
    nuevo->coords[1].y = y - dist;
    nuevo->coords[2].x = x + dist;
    nuevo->coords[2].y = y + dist;
    nuevo->coords[3].x = x - dist;
    nuevo->coords[3].y = y + dist;
    nuevo->scene = escena;
    nuevo->textura = textura;
    nuevo->sig = NULL;
    
    // primer elemento de la cola
    if(cola->inicio == NULL) {
        cola->inicio = nuevo;
        cola->final = nuevo;
    }
    else {
        cola->final->sig = nuevo;
        cola->final = nuevo;
    }
}

void insertarcamino(struct Camino *cola, int x, int y)
{
    struct camino *nuevo = (struct camino *)malloc(sizeof(struct camino));
    
    if(nuevo == NULL)
        return;
    
    nuevo->coord.x = x;
    nuevo->coord.y = y;
    nuevo->sig = NULL;
    
    // primer elemento de la cola
    if(cola->inicio == NULL) {
        cola->inicio = nuevo;
        cola->final = nuevo;
    }
    else {
        cola->final->sig = nuevo;
        cola->final = nuevo;
    }
}

struct NodoCola* dequeue(struct Cola *cola)
{
    if(cola->inicio == NULL) {
        printf("Error: La cola está vacía\n");
        return NULL;
    }
    
    struct NodoCola *aux = cola->inicio;
    cola->inicio = cola->inicio->sig;
    
    if(cola->inicio == NULL) {
        cola->final = NULL;
    }
    
    aux->sig = NULL;
    
    return aux;  
}

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

void swapPartes(struct NodoArbol *nodo1, struct NodoArbol *nodo2)
{
    if(nodo1 == NULL || nodo2 == NULL) 
        return;

    struct PartePersonaje *temp = nodo1->parte;
    nodo1->parte = nodo2->parte;
    nodo2->parte = temp;
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
	            glVertex3f(aux->coordenadas[0]*4, (aux->coordenadas[1]*4)+100,0.0);
        	}	
        glEnd();

    glColor3f(0.0,0.0,0.0);
    glLineWidth(5.0f);
    aux = raiz->parte;
    glBegin(GL_LINE_LOOP);
        	for(;aux != NULL; aux = aux->sig)
        	{
	            glVertex3f(aux->coordenadas[0]*4, (aux->coordenadas[1]*4)+100,0.0);
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
	ListinsertarParte(&((*raiz)->parte),13,13);

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
	ListinsertarParte(&((*raiz)->izq->parte),18,22);
	ListinsertarParte(&((*raiz)->izq->parte),18,23);

	insertarNodoArbol(&((*raiz)->izq->izq),aux,"Pieizq");
	ListinsertarParte(&((*raiz)->izq->izq->parte),16,6);
	ListinsertarParte(&((*raiz)->izq->izq->parte),16,2);
	ListinsertarParte(&((*raiz)->izq->izq->parte),17,1);
	ListinsertarParte(&((*raiz)->izq->izq->parte),18,1);
	ListinsertarParte(&((*raiz)->izq->izq->parte),19,2);
	ListinsertarParte(&((*raiz)->izq->izq->parte),19,8);


	// Partes auxiliares para la animacion
	insertarNodoArbol(&Aux,aux, "Null");

	insertarNodoArbol(&(Aux->izq),aux, "pieizqWalk");
	ListinsertarParte(&(Aux->izq->parte),16,6);
	ListinsertarParte(&(Aux->izq->parte),16,4);
	ListinsertarParte(&(Aux->izq->parte),17,4);
	ListinsertarParte(&(Aux->izq->parte),17,2);
	ListinsertarParte(&(Aux->izq->parte),18,2);
	ListinsertarParte(&(Aux->izq->parte),19,1);
	ListinsertarParte(&(Aux->izq->parte),21,1);
	ListinsertarParte(&(Aux->izq->parte),21,3);
	ListinsertarParte(&(Aux->izq->parte),20,4);
	ListinsertarParte(&(Aux->izq->parte),20,6);
	ListinsertarParte(&(Aux->izq->parte),19,6);
	ListinsertarParte(&(Aux->izq->parte),19,8);

	insertarNodoArbol(&(Aux->der),aux, "BrazoWalk");
	ListinsertarParte(&(Aux->der->parte),11,14);
	ListinsertarParte(&(Aux->der->parte),12,13);
	ListinsertarParte(&(Aux->der->parte),12,12);
	ListinsertarParte(&(Aux->der->parte),13,11);
	ListinsertarParte(&(Aux->der->parte),14,10);
	ListinsertarParte(&(Aux->der->parte),15,10);
	ListinsertarParte(&(Aux->der->parte),15,12);
	ListinsertarParte(&(Aux->der->parte),14,13);

	insertarNodoArbol(&Aux2,aux, "Null");

	insertarNodoArbol(&(Aux2->izq),aux, "CuerpoWalk");
	ListinsertarParte(&(Aux2->izq->parte),8,21);
	ListinsertarParte(&(Aux2->izq->parte),8,6);
	ListinsertarParte(&(Aux2->izq->parte),9,6);
	ListinsertarParte(&(Aux2->izq->parte),9,5);
	ListinsertarParte(&(Aux2->izq->parte),10,4);
	ListinsertarParte(&(Aux2->izq->parte),10,3);
	ListinsertarParte(&(Aux2->izq->parte),11,2);
	ListinsertarParte(&(Aux2->izq->parte),12,1);
	ListinsertarParte(&(Aux2->izq->parte),14,1);
	ListinsertarParte(&(Aux2->izq->parte),14,3);
	ListinsertarParte(&(Aux2->izq->parte),13,4);
	ListinsertarParte(&(Aux2->izq->parte),13,5);
	ListinsertarParte(&(Aux2->izq->parte),15,5);
	ListinsertarParte(&(Aux2->izq->parte),16,6);
	ListinsertarParte(&(Aux2->izq->parte),17,6);
	ListinsertarParte(&(Aux2->izq->parte),18,7);
	ListinsertarParte(&(Aux2->izq->parte),19,8);
	ListinsertarParte(&(Aux2->izq->parte),19,9);
	ListinsertarParte(&(Aux2->izq->parte),20,9);
	ListinsertarParte(&(Aux2->izq->parte),20,13);
	ListinsertarParte(&(Aux2->izq->parte),24,13);
	ListinsertarParte(&(Aux2->izq->parte),25,14);
	ListinsertarParte(&(Aux2->izq->parte),25,15);
	ListinsertarParte(&(Aux2->izq->parte),24,16);
	ListinsertarParte(&(Aux2->izq->parte),21,16);
	ListinsertarParte(&(Aux2->izq->parte),20,17);
	ListinsertarParte(&(Aux2->izq->parte),20,18);
	ListinsertarParte(&(Aux2->izq->parte),23,18);
	ListinsertarParte(&(Aux2->izq->parte),23,19);
	ListinsertarParte(&(Aux2->izq->parte),25,19);
	ListinsertarParte(&(Aux2->izq->parte),25,21);
	ListinsertarParte(&(Aux2->izq->parte),24,22);
	ListinsertarParte(&(Aux2->izq->parte),18,22);
	ListinsertarParte(&(Aux2->izq->parte),18,23);

	insertarNodoArbol(&(Aux2->der),aux, "BrazoWalk");
	ListinsertarParte(&(Aux2->der->parte),12,13);
	ListinsertarParte(&(Aux2->der->parte),11,12);
	ListinsertarParte(&(Aux2->der->parte),10,11);
	ListinsertarParte(&(Aux2->der->parte),10,9);
	ListinsertarParte(&(Aux2->der->parte),11,9);
	ListinsertarParte(&(Aux2->der->parte),12,10);
	ListinsertarParte(&(Aux2->der->parte),13,11);
	ListinsertarParte(&(Aux2->der->parte),14,12);


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

	aux[0]=0.33;
	aux[1]=0.15;
	aux[2]=0.456;
	insertarNodoArbol(&((*raiz)->der),aux,"sombreropico");
	ListinsertarParte(&((*raiz)->der->parte),7,21);
	ListinsertarParte(&((*raiz)->der->parte),7,26);
	ListinsertarParte(&((*raiz)->der->parte),6,26);
	ListinsertarParte(&((*raiz)->der->parte),6,27);
	ListinsertarParte(&((*raiz)->der->parte),5,27);
	ListinsertarParte(&((*raiz)->der->parte),5,28);
	ListinsertarParte(&((*raiz)->der->parte),4,28);
	ListinsertarParte(&((*raiz)->der->parte),4,27);
	ListinsertarParte(&((*raiz)->der->parte),3,27);
	ListinsertarParte(&((*raiz)->der->parte),2,28);
	ListinsertarParte(&((*raiz)->der->parte),1,28);
	ListinsertarParte(&((*raiz)->der->parte),2,29);
	ListinsertarParte(&((*raiz)->der->parte),4,30);
	ListinsertarParte(&((*raiz)->der->parte),6,31);
	ListinsertarParte(&((*raiz)->der->parte),7,31);
	ListinsertarParte(&((*raiz)->der->parte),9,30);
	ListinsertarParte(&((*raiz)->der->parte),11,29);
	ListinsertarParte(&((*raiz)->der->parte),11,28);
	ListinsertarParte(&((*raiz)->der->parte),12,28);
	ListinsertarParte(&((*raiz)->der->parte),12,27);
	ListinsertarParte(&((*raiz)->der->parte),13,27);
	ListinsertarParte(&((*raiz)->der->parte),13,26);
	ListinsertarParte(&((*raiz)->der->parte),14,26);
	ListinsertarParte(&((*raiz)->der->parte),15,25);

	insertarNodoArbol(&((*raiz)->der->der),aux,"sombrerobase");
	ListinsertarParte(&((*raiz)->der->der->parte),7,18);
	ListinsertarParte(&((*raiz)->der->der->parte),8,19);
	ListinsertarParte(&((*raiz)->der->der->parte),9,20);
	ListinsertarParte(&((*raiz)->der->der->parte),10,20);
	ListinsertarParte(&((*raiz)->der->der->parte),11,21);
	ListinsertarParte(&((*raiz)->der->der->parte),12,21);
	ListinsertarParte(&((*raiz)->der->der->parte),13,22);
	ListinsertarParte(&((*raiz)->der->der->parte),14,22);
	ListinsertarParte(&((*raiz)->der->der->parte),15,23);
	ListinsertarParte(&((*raiz)->der->der->parte),16,23);
	ListinsertarParte(&((*raiz)->der->der->parte),18,23);
	ListinsertarParte(&((*raiz)->der->der->parte),18,24);
	ListinsertarParte(&((*raiz)->der->der->parte),19,24);
	ListinsertarParte(&((*raiz)->der->der->parte),19,26);
	ListinsertarParte(&((*raiz)->der->der->parte),16,26);
	ListinsertarParte(&((*raiz)->der->der->parte),15,26);
	ListinsertarParte(&((*raiz)->der->der->parte),14,25);
	ListinsertarParte(&((*raiz)->der->der->parte),13,25);
	ListinsertarParte(&((*raiz)->der->der->parte),13,24);
	ListinsertarParte(&((*raiz)->der->der->parte),12,24);
	ListinsertarParte(&((*raiz)->der->der->parte),11,23);
	ListinsertarParte(&((*raiz)->der->der->parte),10,23);
	ListinsertarParte(&((*raiz)->der->der->parte),9,22);
	ListinsertarParte(&((*raiz)->der->der->parte),7,22);
	ListinsertarParte(&((*raiz)->der->der->parte),6,21);
	ListinsertarParte(&((*raiz)->der->der->parte),5,21);
	ListinsertarParte(&((*raiz)->der->der->parte),5,18);

	aux[0]=0.0;
	aux[1]=0.0;
	aux[2]=0.0;
	insertarNodoArbol(&((*raiz)->der->izq),aux,"pupila");
	ListinsertarParte(&((*raiz)->der->izq->parte),14,18);
	ListinsertarParte(&((*raiz)->der->izq->parte),15,18);
	ListinsertarParte(&((*raiz)->der->izq->parte),15,19);
	ListinsertarParte(&((*raiz)->der->izq->parte),14,19);

}

void initobstaculos(struct Cola *cola)
{
	//Escena 1
	enqueue(cola, 60, 350,130,0,caja);
	enqueue(cola, 60, 410,130,0,caja);
	enqueue(cola, 60, 410,190,0,caja);
	enqueue(cola, 60, 470,130,0,caja);
	enqueue(cola, 60, 470,190,0,caja);
	enqueue(cola, 60, 470,250,0,caja);
	enqueue(cola, 60, 530,130,0,caja);
	enqueue(cola, 60, 530,190,0,caja);
	enqueue(cola, 60, 530,250,0,caja);
	enqueue(cola, 60, 530,310,0,caja);
	enqueue(cola, 350,735,-75,0,gujero);
	enqueue(cola, 60, 710,370,0,caja);
	enqueue(cola, 60, 770,370,0,caja);
	enqueue(cola, 60, 830,370,0,caja);

	//Escena 2
	enqueue(cola, 900,450,-350,1,gujero);
	enqueue(cola, 60, 120,340,1,caja);
	enqueue(cola, 60, 180,340,1,caja);
	enqueue(cola, 60, 240,340,1,caja);
	enqueue(cola, 60, 400,280,1,caja);
	enqueue(cola, 60, 560,360,1,caja);
	enqueue(cola, 60, 790,200,1,caja);
	enqueue(cola, 60, 850,200,1,caja);

	enqueue(cola, 900,450,-350,2,gujero);
	enqueue(cola, 60, 80,300,2,caja);
	enqueue(cola, 60, 140,300,2,caja);
	enqueue(cola, 60, 200,300,2,caja);
	enqueue(cola, 60, 260,300,2,caja);
	enqueue(cola, 60, 320,300,2,caja);
	enqueue(cola, 60, 380,300,2,caja);
	enqueue(cola, 60, 440,300,2,caja);
	enqueue(cola, 60, 500,300,2,caja); 
	enqueue(cola, 60, 560,300,2,caja); 
	enqueue(cola, 60, 620,300,2,caja);
	enqueue(cola, 60, 680,300,2,caja);
	enqueue(cola, 60, 740,300,2,caja);
	enqueue(cola, 60, 800,300,2,caja);  
	enqueue(cola, 60, 800,360,2,tesoro); 


}

void dibujarobstaculos(struct Cola *cola)
{
	struct NodoCola *aux = cola->inicio;
	while(aux != NULL)
	{
		if(aux->scene == escena){
			glColor3f(1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, aux->textura);
		    glEnable(GL_TEXTURE_2D);
		    glBegin(GL_QUADS);
		        glTexCoord2f(0,0); glVertex2f(aux->coords[0].x,aux->coords[0].y);
		        glTexCoord2f(1,0); glVertex2f(aux->coords[1].x,aux->coords[1].y);
		        glTexCoord2f(1,1); glVertex2f(aux->coords[2].x,aux->coords[2].y);
		        glTexCoord2f(0,1); glVertex2f(aux->coords[3].x,aux->coords[3].y);
		    glEnd();
		    glBindTexture(GL_TEXTURE_2D, 0);
		    glDisable(GL_TEXTURE_2D);
		}
	    aux = aux->sig;
	}
}

static void reshape01(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); // establecer la vista a todo
   	glMatrixMode (GL_PROJECTION); // usar la matriz de proyección
   	glLoadIdentity (); // y cargarla
   	// establecer a toda la pantalla
   	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);

   	glMatrixMode(GL_MODELVIEW);  // Volver a MODELVIEW
    glLoadIdentity();
    
    glutPostRedisplay();  // Forzar redibujado
}

void CrearCamino(struct Camino *camino)
{
	int y=0;
	for(int x = 0; x <= 900; x+=3)
	{	if(x == 210)
			y = 10;

		if((x > 210 && y < 60)||(x > 270 && y < 120) || (x > 330 && y < 180) || (x > 390 && y < 240))
			y += 6;

		if(x < 645)
		{
			if(x > 560 && y > 300)
			{
				y -= 3;
			}else
				if(x > 500 && y < 360)
					y += 6;
		}

		if(x > 800 && y < 400)
			y += 5;

		insertarcamino(camino, x, y);
	}

	for(int x = 0; x<=900;x += 3)
	{	if(x > 0 && x < 200 && y > 270)
			y -=6;

		if(x > 200 && x < 220)
			y+= 6;	

		if(x > 220 &&  x < 300)
			y -=4;

		if(x >370 && x < 520 && y < 280)
			y+=3;

		if(x > 520 && x < 540)
			y+= 6;	

		if(x > 540 &&  x < 730)
			y -=3;

		if(x > 800 &&  x < 900)
			y += 3;

		insertarcamino(camino, x, y);
	}

	for(int x = 0; x<=900;x += 3)
	{
		if(x >= 750 && x < 753)
			{
				insertarcamino(camino, x, 306);
				insertarcamino(camino, x, 312);
				insertarcamino(camino, x, 318);
				insertarcamino(camino, x, 318);
				insertarcamino(camino, x, 312);
				insertarcamino(camino, x, 306);
			}
		if(x > 780)
			y += 6;	
		insertarcamino(camino, x, y);

	}

}

void display() 
{
    glClear(GL_COLOR_BUFFER_BIT); // Limpia el búfer de color

    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturafondo);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(0,700);
        glTexCoord2f(1,0); glVertex2f(900,700);
        glTexCoord2f(1,1); glVertex2f(900,0);
        glTexCoord2f(0,1); glVertex2f(0,0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturapiso);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(0,0);
        glTexCoord2f(1,0); glVertex2f(900,0);
        glTexCoord2f(1,1); glVertex2f(900,100);
        glTexCoord2f(0,1); glVertex2f(0,100);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    dibujarobstaculos(&Cola);

    glPushMatrix();
	glTranslatef(caminoaux->coord.x, caminoaux->coord.y, 0);
	dibujarpersonaje(Raiz);
	glPopMatrix();

    // intercambio de buffers (porque usamos GLUT_DOUBLE)
    glutSwapBuffers();
}

void texto(char *texto,int x, int y, float color[],void* font)
{
	glColor3f(color[0], color[1], color[2]);
	glRasterPos2f(x, y);
	for(; *texto != '\0'; texto++)
	glutBitmapCharacter(font, *texto);
}


void menu()
{
	glClear(GL_COLOR_BUFFER_BIT); // Limpia el búfer de color

	glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturafondo);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(0,700);
        glTexCoord2f(1,0); glVertex2f(900,700);
        glTexCoord2f(1,1); glVertex2f(900,0);
        glTexCoord2f(0,1); glVertex2f(0,0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    float aux[3] = {0.0,0.0,0.0};
    texto("LA AVENTURA DE GLEEP",300,600,aux,GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturapiso);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(250,300);
        glTexCoord2f(1,0); glVertex2f(650,300);
        glTexCoord2f(1,1); glVertex2f(650,400);
        glTexCoord2f(0,1); glVertex2f(250,400);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    aux[0]=1.0;
	aux[1]=1.0;
	aux[2]=1.0;

	texto("REPRODUCIR (enter)",340,340,aux,GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturapiso);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(250,100);
        glTexCoord2f(1,0); glVertex2f(650,100);
        glTexCoord2f(1,1); glVertex2f(650,200);
        glTexCoord2f(0,1); glVertex2f(250,200);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    texto("SALIR (S)",400,140,aux,GLUT_BITMAP_TIMES_ROMAN_24);

    // intercambio de buffers (porque usamos GLUT_DOUBLE)
    glutSwapBuffers();
}

void pausa()
{
	glClear(GL_COLOR_BUFFER_BIT); // Limpia el búfer de color

	glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturafondo);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(0,700);
        glTexCoord2f(1,0); glVertex2f(900,700);
        glTexCoord2f(1,1); glVertex2f(900,0);
        glTexCoord2f(0,1); glVertex2f(0,0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    float aux[3] ={1.0,1.0,1.0}; 

    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturapiso);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(250,500);
        glTexCoord2f(1,0); glVertex2f(650,500);
        glTexCoord2f(1,1); glVertex2f(650,600);
        glTexCoord2f(0,1); glVertex2f(250,600);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

	texto("CONTINUAR (C)",350,540,aux,GLUT_BITMAP_TIMES_ROMAN_24);

	glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturapiso);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(250,300);
        glTexCoord2f(1,0); glVertex2f(650,300);
        glTexCoord2f(1,1); glVertex2f(650,400);
        glTexCoord2f(0,1); glVertex2f(250,400);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

	texto("REINICIAR ANIMACION (R)",300,340,aux,GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texturapiso);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(250,100);
        glTexCoord2f(1,0); glVertex2f(650,100);
        glTexCoord2f(1,1); glVertex2f(650,200);
        glTexCoord2f(0,1); glVertex2f(250,200);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    texto("SALIR (S)",400,140,aux,GLUT_BITMAP_TIMES_ROMAN_24);

    // intercambio de buffers (porque usamos GLUT_DOUBLE)
    glutSwapBuffers();
}

static void init01(void) 
{
	glClearColor (1.0, 1.0, 1.0, 0.0);  // Limpiar a color RGB A
   	glShadeModel (GL_FLAT); // El valor default es GL_SMOOTH
   	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Cola.inicio = NULL;
	Cola.final = NULL;
	Camino.inicio = NULL;
	Camino.final = NULL;
    texturapiso = CargarTextura("vecteezy_stone-tiles-texture-in-cartoon-style_3678912.jpg");
    texturafondo = CargarTextura("png-clipart-cartoon-drawing-sky-cloud-clouds-cartoon-blue-atmosphere-thumbnail.png");
    caja = CargarTextura("g5q1_cfw8_210729-removebg-preview.png");
    gujero = CargarTextura("agujero.png");
    tesoro = CargarTextura("cofre.png");
}


GLuint CargarTextura(const char *ruta)
{
    int w, h, channels;
    unsigned char *img = stbi_load(ruta, &w, &h, &channels, 0);

    if (!img) {
        printf("Error al cargar imagen: %s\n", ruta);
        printf("Razon: %s\n", stbi_failure_reason());
        return 0;
    }

    printf("Textura cargada: %s - %dx%d, canales: %d\n", ruta, w, h, channels);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Determinar formato según canales
    GLenum formato, formatoInterno;
    if(channels == 4) {
        formato = GL_RGBA;
        formatoInterno = GL_RGBA;
    } else if(channels == 3) {
        formato = GL_RGB;
        formatoInterno = GL_RGB;
    } else {
        formato = GL_LUMINANCE;
        formatoInterno = GL_LUMINANCE;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, formatoInterno, w, h, 0, formato, GL_UNSIGNED_BYTE, img);

    // Parámetros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(img);
    
    printf("ID de textura generada: %u\n", texID);

    return texID;
}

void Animacion(int num) 
{
	if(ventana != 2)
	{
    switch(pasoAnim) {
        case 0:
            
            break;
            
        case 1:
            swapPartes(Raiz->izq->izq, Aux->izq);
            swapPartes(Raiz, Aux->der);
            break;
            
        case 2:
            swapPartes(Raiz->izq, Aux2->izq);
            swapPartes(Raiz, Aux2->der);
            break;
    }
    
    pasoAnim = (pasoAnim + 1) % 3;
    
    switch(pasoAnim) {
        case 0:
            break;
            
        case 1:
            swapPartes(Raiz->izq->izq, Aux->izq);
            swapPartes(Raiz, Aux->der);
            break;
            
        case 2:
            swapPartes(Raiz->izq, Aux2->izq);
            swapPartes(Raiz, Aux2->der);
            break;
    }
    caminoaux = caminoaux->sig;

    if(caminoaux == NULL)
		caminoaux = Camino.inicio->sig;

	if(caminoaux->coord.x == 900)
		escena =    (escena + 1) % 3;

    glutPostRedisplay();
    glutTimerFunc(200, Animacion, 0);
	}else
		glutTimerFunc(200, Animacion, 0);
}


void teclado(unsigned char tecla,int x, int y)
{
	if(tecla == '\r' && ventana == 0)// play a la animacion
		{
			ventana = 1;
			glutDisplayFunc(display);
			glutTimerFunc(200, Animacion, 0);
		}
	else 
	if(tecla == 'S' && (ventana == 0 || ventana == 2))
	{
		exit(1);
	}else 
	if(tecla == 'P' && ventana == 1)
	{
		glutDisplayFunc(pausa);
		ventana = 2;
		glutPostRedisplay();
	}else 
	if(tecla == 'C' && ventana == 2)
	{
		ventana = 1;
		glutDisplayFunc(display);
	}else 
	if(tecla == 'R' && ventana == 2)
	{
		ventana = 1;
		escena = 0;
		caminoaux = Camino.inicio;
		glutDisplayFunc(display);
	}

}	

int main(int argc, char** argv)
{
    glutInit(&argc, argv); // inicializa las bibliotecas de glut

    // configuración del modo de visualización de la pantalla
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(900, 700);
    glutInitWindowPosition(300, 50);
    glutCreateWindow("La Aventura de GLEEP");
    
    init01();
    iniciarpersonaje(&Raiz);
    initobstaculos(&Cola);
    CrearCamino(&Camino);
    caminoaux = Camino.inicio;

    glutDisplayFunc(menu);
    glutReshapeFunc(reshape01);
    glutKeyboardFunc(teclado);
    glutMainLoop(); // loop principal
    return 0;
}


//glutPostRedisplay();

