#include "asteroide.h"
#include "../graficos/Grafico.h" 

static NodoAsteroide* cabeza = NULL;
static int asteroidesEliminados = 0;

// --- CONFIGURACIÓN DE CANTIDAD EN PANTALLA ---
#define CANTIDAD_ASTEROIDES 25

// --- DIFICULTAD GRADUAL ---
static float temporizadorDificultad = 0.0f;
static float velocidadExtra = 0.0f;

void ActualizarDificultadAsteroides(float deltaTiempo) {
    temporizadorDificultad += deltaTiempo;
    if (temporizadorDificultad >= 10.0f) {
        if (velocidadExtra < 150.0f) {
            velocidadExtra += 20.0f; 
        }
        temporizadorDificultad = 0.0f;
    }
}

static void AgregarAsteroide(int anchoPantalla) {
    NodoAsteroide* nuevo = (NodoAsteroide*)malloc(sizeof(NodoAsteroide));
    if (nuevo == NULL) return;

    nuevo->tamanio = (Vector2){ 50.0f, 50.0f };
    float posX = (float)GetRandomValue(0, anchoPantalla - (int)nuevo->tamanio.x);
    // Aparecen arriba a distintas alturas para que no caigan todos pegados
    nuevo->posicion = (Vector2){ posX, (float)(-GetRandomValue(50, 600)) };
    
    nuevo->velocidad = (float)GetRandomValue(150, 300) + velocidadExtra;

    nuevo->siguiente = cabeza;
    cabeza = nuevo;
}

void InicializarAsteroides(int anchoPantalla) {
    NodoAsteroide* actual = cabeza;
    while (actual != NULL) {
        NodoAsteroide* aux = actual;
        actual = actual->siguiente;
        free(aux);
    }
    cabeza = NULL;
    asteroidesEliminados = 0;
    temporizadorDificultad = 0.0f;
    velocidadExtra = 0.0f;

    // Generamos exactamente los 25 asteroides de entrada
    for (int i = 0; i < CANTIDAD_ASTEROIDES; i++) {
        AgregarAsteroide(anchoPantalla);
    }
}

void GenerarYMoverAsteroides(int anchoPantalla, int altoPantalla, float deltaTiempo) {
    NodoAsteroide* actual = cabeza;

    while (actual != NULL) {
        actual->posicion.y += actual->velocidad * deltaTiempo;

        // Si el asteroide sale por abajo de la pantalla, recicla su posición arriba
        if (actual->posicion.y > altoPantalla) {
            actual->posicion.x = (float)GetRandomValue(0, anchoPantalla - (int)actual->tamanio.x);
            actual->posicion.y = (float)(-GetRandomValue(50, 200));
            actual->velocidad = (float)GetRandomValue(150, 300) + velocidadExtra;
        }

        actual = actual->siguiente;
    }
}

void DibujarAsteroides(Texture2D texAsteroide) {
    NodoAsteroide* actual = cabeza;

    while (actual != NULL) {
        if (texAsteroide.id > 0) {
            Rectangle dest = { 
                actual->posicion.x, 
                actual->posicion.y, 
                actual->tamanio.x, 
                actual->tamanio.y 
            };
            Rectangle src = { 0, 0, (float)texAsteroide.width, (float)texAsteroide.height };
            Vector2 origin = { 0, 0 };

            DrawTexturePro(texAsteroide, src, dest, origin, 0.0f, WHITE);
        } else {
            DrawRectangleV(actual->posicion, actual->tamanio, RED);
        }

        actual = actual->siguiente;
    }
}

NodoAsteroide* ObtenerListaAsteroides(void) {
    return cabeza;
}

void EliminarAsteroide(NodoAsteroide* objetivo, int anchoPantalla, Nave* nave) {
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

            // Mejora de disparo al eliminar 15
            if (asteroidesEliminados >= 15 && nave != NULL) {
                if (nave->nivelDisparo < 3) {
                    nave->nivelDisparo++; 
                }
            }

            // Mantenemos la cantidad fija responiendo 1 nuevo asteroide por cada 1 destruido
            AgregarAsteroide(anchoPantalla);
            break;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
}