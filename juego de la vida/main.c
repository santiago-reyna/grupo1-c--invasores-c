#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define FILAS 50
#define COLUMNAS 50
#define TAM_CELDA 16

int tablero_actual[FILAS][COLUMNAS];
int tablero_siguiente[FILAS][COLUMNAS];

int contar_vecinos(int f, int c)
{
    int vecinos = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0) continue;

            int fila_vecina = (f + i + FILAS) % FILAS;
            int col_vecina = (c + j + COLUMNAS) % COLUMNAS;

            vecinos += tablero_actual[fila_vecina][col_vecina];
        }
    }
    return vecinos;
}

void actualizar_generacion(void)
{
    for (int f = 0; f < FILAS; f++)
    {
        for (int c = 0; c < COLUMNAS; c++)
        {
            int vecinos = contar_vecinos(f, c);
            int estado = tablero_actual[f][c];

            if (estado == 0 && vecinos == 3)
            {
                tablero_siguiente[f][c] = 1;
            }
            else if (estado == 1 && (vecinos < 2 || vecinos > 3))
            {
                tablero_siguiente[f][c] = 0;
            }
            else
            {
                tablero_siguiente[f][c] = estado;
            }
        }
    }

    for (int f = 0; f < FILAS; f++)
    {
        for (int c = 0; c < COLUMNAS; c++)
        {
            tablero_actual[f][c] = tablero_siguiente[f][c];
        }
    }
}

void cargar_configuracion(int opcion)
{
    for (int f = 0; f < FILAS; f++)
        for (int c = 0; c < COLUMNAS; c++)
            tablero_actual[f][c] = 0;

    int centro_f = FILAS / 2;
    int centro_c = COLUMNAS / 2;

    if (opcion == 1)
    {
        tablero_actual[centro_f][centro_c - 1] = 1;
        tablero_actual[centro_f][centro_c] = 1;
        tablero_actual[centro_f][centro_c + 1] = 1;
    }
    else if (opcion == 2)
    {
        tablero_actual[centro_f][centro_c] = 1;
        tablero_actual[centro_f][centro_c + 1] = 1;
        tablero_actual[centro_f + 1][centro_c] = 1;
        tablero_actual[centro_f + 1][centro_c + 1] = 1;
    }
    else if (opcion == 3)
    {
        tablero_actual[centro_f - 1][centro_c] = 1;
        tablero_actual[centro_f][centro_c + 1] = 1;
        tablero_actual[centro_f + 1][centro_c - 1] = 1;
        tablero_actual[centro_f + 1][centro_c] = 1;
        tablero_actual[centro_f + 1][centro_c + 1] = 1;
    }
}

void guardar_archivo(const char *nombre)
{
    FILE *archivo = fopen(nombre, "w");
    if (archivo == NULL) return;

    for (int f = 0; f < FILAS; f++)
    {
        for (int c = 0; c < COLUMNAS; c++)
        {
            fprintf(archivo, "%d ", tablero_actual[f][c]);
        }
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

void cargar_archivo(const char *nombre)
{
    FILE *archivo = fopen(nombre, "r");
    if (archivo == NULL) return;

    for (int f = 0; f < FILAS; f++)
    {
        for (int c = 0; c < COLUMNAS; c++)
        {
            fscanf(archivo, "%d", &tablero_actual[f][c]);
        }
    }
    fclose(archivo);
}

int main(void)
{
    const int anchoVentana = COLUMNAS * TAM_CELDA;
    const int altoVentana = FILAS * TAM_CELDA;

    InitWindow(anchoVentana, altoVentana, "Juego de la Vida - Conway");
    SetTargetFPS(60);

    bool pausado = true;
    float tiempo_acumulado = 0.0f;
    float intervalo = 0.15f;

    cargar_configuracion(1);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE)) pausado = !pausado;
        if (IsKeyPressed(KEY_ONE))   cargar_configuracion(1);
        if (IsKeyPressed(KEY_TWO))   cargar_configuracion(2);
        if (IsKeyPressed(KEY_THREE)) cargar_configuracion(3);
        if (IsKeyPressed(KEY_G))     guardar_archivo("partidaJDLV.txt");
        if (IsKeyPressed(KEY_C))     cargar_archivo("partidaJDLV.txt");

        Vector2 posMouse = GetMousePosition();
        int colMouse = posMouse.x / TAM_CELDA;
        int filaMouse = posMouse.y / TAM_CELDA;

        if (colMouse >= 0 && colMouse < COLUMNAS && filaMouse >= 0 && filaMouse < FILAS)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))  tablero_actual[filaMouse][colMouse] = 1;
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) tablero_actual[filaMouse][colMouse] = 0;
        }

        if (!pausado)
        {
            tiempo_acumulado += GetFrameTime();
            if (tiempo_acumulado >= intervalo)
            {
                actualizar_generacion();
                tiempo_acumulado = 0.0f;
            }
        }

        BeginDrawing();
        ClearBackground((Color){ 25, 25, 25, 255 });

        for (int f = 0; f < FILAS; f++)
        {
            for (int c = 0; c < COLUMNAS; c++)
            {
                int x = c * TAM_CELDA;
                int y = f * TAM_CELDA;

                if (tablero_actual[f][c] == 1)
                {
                    DrawRectangle(x, y, TAM_CELDA, TAM_CELDA, RAYWHITE);
                }

                DrawRectangleLines(x, y, TAM_CELDA, TAM_CELDA, (Color){ 50, 50, 50, 255 });
            }
        }

        DrawText(pausado ? "ESTADO: PAUSADO (ESPACIO para reanudar)" : "ESTADO: SIMULANDO", 10, 10, 18, RED);
        DrawText("1: Palo | 2: Bloque | 3: Glider | G: Guardar | C: Cargar", 10, 35, 14, LIGHTGRAY);
        DrawText("Click Izq: Crear celda | Click Der: Borrar celda", 10, 55, 14, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}