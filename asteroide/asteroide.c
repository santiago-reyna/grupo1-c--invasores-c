#include "asteroide.h"

static NodoAsteroide* cabeza = NULL;
static int asteroidesEliminados = 0;
static int cantidadPorOleada = 5;

static void AgregarAsteroide(int anchoPantalla)
 {
    NodoAsteroide* nuevo = (NodoAsteroide*)malloc(sizeof(NodoAsteroide));
    
    if (nuevo == NULL) return;

    nuevo->tamanio = (Vector2){ 50.0f, 50.0f };
    float posX = (float)GetRandomValue(0, anchoPantalla - (int)nuevo->tamanio.x);
    nuevo->posicion = (Vector2){ posX, (float)(-GetRandomValue(50, 400)) };
    nuevo->velocidad = (float)GetRandomValue(150, 300);

    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}

static void GenerarMasAsteroides(int anchoPantalla, int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        AgregarAsteroide(anchoPantalla);
    }
}

void InicializarAsteroides(int anchoPantalla) 
{
    NodoAsteroide* actual = cabeza;
    while (actual != NULL) {
        NodoAsteroide* aux = actual;
        actual = actual->siguiente;
        free(aux);
    }
    cabeza = NULL;
    asteroidesEliminados = 0;
    cantidadPorOleada = 5;

    GenerarMasAsteroides(anchoPantalla, cantidadPorOleada);
}

void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo) {
    NodoAsteroide* actual = cabeza;

    while (actual != NULL)
     {
        actual->posicion.y += actual->velocidad * deltaTiempo;

        if (actual->posicion.y > altoPantalla) {
            actual->posicion.x = (float)GetRandomValue(0, anchoPantalla - (int)actual->tamanio.x);
            actual->posicion.y = -50.0f;
            actual->velocidad = (float)GetRandomValue(150, 300);
        }

        actual = actual->siguiente;
    }
}

void DibujarAsteroides(Texture2D texAsteroide)
 {
    NodoAsteroide* actual = cabeza;

    while (actual != NULL) 
    {
        if (texAsteroide.id > 0)
         {
            Rectangle dest =
             { 
                actual->posicion.x, 
                actual->posicion.y, 
                actual->tamanio.x, 
                actual->tamanio.y 
            };
            Rectangle src = { 0, 0, (float)texAsteroide.width, (float)texAsteroide.height };
            Vector2 origin = { 0, 0 };

            DrawTexturePro(texAsteroide, src, dest, origin, 0.0f, WHITE);
        } 
        else
         {
            DrawRectangleV(actual->posicion, actual->tamanio, RED);
        }

        actual = actual->siguiente;
    }
}

NodoAsteroide* ObtenerListaAsteroides(void)
 {
    return cabeza;
}

void EliminarAsteroide(NodoAsteroide* objetivo, int anchoPantalla) {
    if (cabeza == NULL || objetivo == NULL) return;

    NodoAsteroide* actual = cabeza;
    NodoAsteroide* anterior = NULL;

    while (actual != NULL) {
        if (actual == objetivo) {
            if (anterior == NULL) {
                cabeza = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }

            free(actual);

            asteroidesEliminados++;

            // Si eliminamos 10, subimos la cantidad por oleada y rellenamos
            if (asteroidesEliminados >= 10) {
                asteroidesEliminados = 0;
                cantidadPorOleada += 2;
                GenerarMasAsteroides(anchoPantalla, cantidadPorOleada);
            } 
            // Si la lista quedó vacía antes de llegar a 10, forzamos generación inmediata
            else if (cabeza == NULL) {
                GenerarMasAsteroides(anchoPantalla, cantidadPorOleada);
            }
            // Mantenemos siempre asteroides en pantalla agregando uno nuevo arriba por cada baja
            else {
                AgregarAsteroide(anchoPantalla);
            }
            break;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}