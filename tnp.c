// compilar: gcc -o tnp tnp.c lib/matriz.c
// executar: ./tnp input.ppm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib/matriz.h"

unsigned char tratarOverflow(float valor)
{
    if (valor > 255)
        return 255;
    else if (valor < 0)
        return 0;
    else
        return (unsigned char)valor;
}

int tratarFimDeLinha(int inicio, int fim)
{
    if (inicio >= fim - 1)
        return 0;
    else
        return 1;
}

void main(int argc, char **argv)
{
    int i, j, l, h;
    unsigned char type, cmax, caractere;
    FILE *fp;

    fp = fopen(argv[1], "r");
    while ((caractere = getc(fp)) == '#')
        while ((caractere = getc(fp)) != '\n')
            ;
    ungetc(caractere, fp);

    fscanf(fp, "P%hhu\n", &type);
    while ((caractere = getc(fp)) == '#')
        while ((caractere = getc(fp)) != '\n')
            ;
    ungetc(caractere, fp);

    fscanf(fp, "%u %u\n%hhu\n", &l, &h, &cmax);

    unsigned char(**imagem)[3];

    int jsize = l * sizeof(char *);
    imagem = malloc(jsize);

    jsize = h * 3;
    for (i = 0; i < l; i++)
        imagem[i] = malloc(jsize);

    if (type == 3)
    {
        for (j = 0; j < h; j++)
            for (i = 0; i < l; i++)
                fscanf(fp, "%hhu %hhu %hhu", &imagem[i][j][0], &imagem[i][j][1], &imagem[i][j][2]);
        fclose(fp);
    }
    else if (type == 6)
    {
        for (j = 0; j < h; j++)
            for (i = 0; i < l; i++)
                fscanf(fp, "%c%c%c", &imagem[i][j][0], &imagem[i][j][1], &imagem[i][j][2]);
        fclose(fp);
    }
    else
    {
        printf("Formato inválido!");
        fclose(fp);
        exit(0);
    }

    int L = 0, H = 0;
    unsigned char(**transformada)[3];
    L = l;
    H = h;
    int jsize_transformada = L * sizeof(char *);
    transformada = malloc(jsize_transformada);

    jsize_transformada = H * 3;
    for (i = 0; i < L; i++)
        transformada[i] = malloc(jsize_transformada);

    for (i = 0; i < L; i++)
        for (j = 0; j < H; j++)
            transformada[i][j][0] = transformada[i][j][1] = transformada[i][j][2] = 255;

    Matriz3x3 M_A = {
        {{0, L, 0},
         {0, 0, H},
         {1, 1, 1}}};
    Matriz3x1 M_A4 = {
        {{L},
         {H},
         {1}}};
    Matriz3x3 M_AI = inversa3x3(M_A);
    Matriz3x1 abc = multiplicar3x3Por3x1(M_AI, M_A4);
    Matriz3x3 M_abcI = {
        {
            {1.0 / abc.elemento[0][0], 0, 0},
            {0, 1.0 / abc.elemento[1][0], 0},
            {0, 0, 1.0 / abc.elemento[2][0]},
        },
    };
    Matriz3x3 M_B = {
        {{-778, 835, -778},
         {-320, -317, 979},
         {1, 1, 1}}};
    Matriz3x1 M_B4 = {
        {{835},
         {982},
         {1}}};
    Matriz3x3 M_BI = inversa3x3(M_B);
    Matriz3x1 alfaBetaGama = multiplicar3x3Por3x1(M_abcI, multiplicar3x3Por3x1(M_BI, M_B4));
    Matriz3x3 M_alfaBetaGama = {
        {
            {alfaBetaGama.elemento[0][0], 0, 0},
            {0, alfaBetaGama.elemento[1][0], 0},
            {0, 0, alfaBetaGama.elemento[2][0]},
        },
    };
    Matriz3x3 M_BF = multiplicar3x3Por3x3(M_B, M_alfaBetaGama);
    Matriz3x3 T = multiplicar3x3Por3x3(M_BF, M_AI);
    Matriz3x3 M_I = inversa3x3(T);
    int X_int, Y_int;

    for (j = 0; j < H; j++)
    {
        for (i = 0; i < L; i++)
        {
            float X = M_I.elemento[0][0] * i + M_I.elemento[0][1] * j + M_I.elemento[0][2];
            float Y = M_I.elemento[1][0] * i + M_I.elemento[1][1] * j + M_I.elemento[1][2];
            float Z = M_I.elemento[2][0] * i + M_I.elemento[2][1] * j + M_I.elemento[2][2];
            X /= Z;
            Y /= Z;
            X_int = (int)X;
            Y_int = (int)Y;
            float X_frac = X - X_int;
            float Y_frac = Y - Y_int;

            if (X_int >= L || Y_int >= H || X_int < 0 || Y_int < 0)
                continue;

            unsigned char R = tratarOverflow(imagem[X_int][Y_int][0] + X_frac * (imagem[X_int + tratarFimDeLinha(X_int, L)][Y_int][0] - imagem[X_int][Y_int][0]) + Y_frac * (imagem[X_int][Y_int + tratarFimDeLinha(Y_int, H)][0] - imagem[X_int][Y_int][0]));
            unsigned char G = tratarOverflow(imagem[X_int][Y_int][1] + X_frac * (imagem[X_int + tratarFimDeLinha(X_int, L)][Y_int][1] - imagem[X_int][Y_int][1]) + Y_frac * (imagem[X_int][Y_int + tratarFimDeLinha(Y_int, H)][1] - imagem[X_int][Y_int][1]));
            unsigned char B = tratarOverflow(imagem[X_int][Y_int][2] + X_frac * (imagem[X_int + tratarFimDeLinha(X_int, L)][Y_int][2] - imagem[X_int][Y_int][2]) + Y_frac * (imagem[X_int][Y_int + tratarFimDeLinha(Y_int, H)][2] - imagem[X_int][Y_int][2]));

            transformada[i][j][0] = R;
            transformada[i][j][1] = G;
            transformada[i][j][2] = B;
        }
    }

    fp = fopen("output.ppm", "w");
    fprintf(fp, "P6\n");
    fprintf(fp, "%u %u\n255\n", L, H);
    for (j = 0; j < H; j++)
        for (i = 0; i < L; i++)
            fprintf(fp, "%c%c%c", transformada[i][j][0], transformada[i][j][1], transformada[i][j][2]);
    fclose(fp);

    for (i = 0; i < l; i++)
        free(imagem[i]);
    free(imagem);

    for (i = 0; i < L; i++)
        free(transformada[i]);
    free(transformada);
}