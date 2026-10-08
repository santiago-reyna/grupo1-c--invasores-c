#ifndef GRAFICO_H
#define GRAFICO_H

#include "raylib.h"

#define MAX_ASTEROIDES 10


typedef enum { 
    ESTADO_MENU, 
    ESTADO_JUGANDO, 
    ESTADO_GAME_OVER 
} EstadoJuego;



typedef struct {
    Vector2 posicion;   
    Vector2 tamanio;    
    float velocidad;     
    Color color;        
} Nave;



typedef struct {
    Vector2 posicion;   
    Vector2 tamanio;    
    float velocidad;    
    bool activo;        
} Asteroide;




void InicializarJuego(Nave *nave, Asteroide asteroides[], int *puntaje); 


void ActualizarJugador(Nave *nave); 


void GenerarYMoverAsteroides(Asteroide asteroides[]);   


bool ComprobarColision(Nave nave, Asteroide asteroides[]); 


void DibujarJuego(EstadoJuego estado, Nave nave, Asteroide asteroides[], int puntaje, Texture2D texNave, Texture2D texAsteroide, Texture2D texFondo);


#endif 