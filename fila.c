#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ultimo = 0;
char fila[5][100]= {"0", "0",
    "0", "0","0"};




void push(char *nome)
{

    if(ultimo==0)
    {
        strcpy(fila[0],nome);
    }
    if(ultimo==1)
    {        
        strcpy(fila[1],fila[0]);
        strcpy(fila[0],nome);
    }
    if(ultimo==2)
    {        
        strcpy(fila[2],fila[1]);
        strcpy(fila[1],fila[0]);
        strcpy(fila[0],nome);
    }
    if(ultimo==3)
    {        
        strcpy(fila[3],fila[2]);
        strcpy(fila[2],fila[1]);
        strcpy(fila[1],fila[0]);
        strcpy(fila[0],nome);
    }
    if(ultimo==4)
    {        
        strcpy(fila[4],fila[3]);
        strcpy(fila[3],fila[2]);
        strcpy(fila[2],fila[1]);
        strcpy(fila[1],fila[0]);
        strcpy(fila[0],nome);
    }
    if(ultimo==5)
    {        
        strcpy(fila[5],fila[4]);        
        strcpy(fila[4],fila[3]);
        strcpy(fila[3],fila[2]);
        strcpy(fila[2],fila[1]);
        strcpy(fila[1],fila[0]);
        strcpy(fila[0],nome);
    }
   if(ultimo<5)
        ultimo++;
    return;
};

char* pegaNome(int posicao)
{
    if(ultimo<5 && posicao>ultimo)
        return 0;
    else
        return fila[posicao];
};

