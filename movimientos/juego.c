#include "juego.h"

void inicializar_nave(Nave *nave, int ancho_pantalla, int alto_pantalla) {
    nave->posicion = (Vector2){ancho_pantalla / 2.0f, alto_pantalla - 50.0f}; // Posición inicial en el centro inferior
    nave->tamanio = (Vector2){50.0f, 20.0f}; // Tamaño de la nave
    nave->velocidad = 5.0f; // Velocidad de movimiento
    nave->color = BLUE; // Color de la nave
}   

//Función A: Control de movimiento y validación de bordes
void actualizar_nave(Nave *nave, int ancho_pantalla) {
    // Detección de entrada por teclado
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        nave->posicion.x -= nave->velocidad; // Mover a la izquierda
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        nave->posicion.x += nave->velocidad; // Mover a la derecha
    }
    //2. Control de límite izquierdo
    if (nave->posicion.x < 0) {
        nave->posicion.x = 0;
    }
    //3. Control de límite derecho
    if (nave->posicion.x + nave->tamanio.x > ancho_pantalla) {
        nave->posicion.x = ancho_pantalla - nave->tamanio.x;
    }
}
    //Función B: Renderizado de la nave en pantalla
void renderizar_nave(Nave nave) {
    DrawRectangleV(nave.posicion, nave.tamanio, nave.color); // Dibuja la nave como un rectángulo
}

