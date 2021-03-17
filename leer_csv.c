/*
	web:		https://programaciontrabajosescolares.blogspot.com/
	e-mail: 	ProgramacionTrabajosEscolares@gmail.com
	facebook:	https://www.facebook.com/ProgramacionTrabajosEscolares
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char temporal[100];

char ***valores = NULL;	//Valores va a contener todos los valores leidos
int *campos_por_linea = NULL;

char *pivote = NULL;
int lineas = 0;
int campos = 0;
int longitud = 0;
int i,j;

int main()	{
	FILE *archivo = NULL;
	archivo = fopen("query_result_2020-11-23T15_59_05.333634-03_00.csv","r");
	if(archivo != NULL)	{
		do	{
			memset(temporal,0,100);
			fgets(temporal,100,archivo);
			lineas++;
			valores = realloc(valores,lineas*sizeof(char*));
			campos_por_linea = realloc(campos_por_linea,lineas*sizeof(int));
			valores[lineas-1] = NULL;
			pivote = strtok(temporal,",");
			campos = 0;
			while (pivote != NULL)	{
				campos++;
				valores[lineas-1] = realloc(valores[lineas-1],sizeof(char*)*campos);
				longitud = strlen(pivote);
				valores[lineas-1][campos -1] = calloc(longitud+1,sizeof(char));
				strncpy(valores[lineas-1][campos -1],pivote,longitud);
				pivote = strtok(NULL, ",");
			}
			campos_por_linea[lineas-1] = campos;
		}while(feof(archivo) == 0);
		fclose(archivo);
		printf("Mostrando valores leidos\n");
		i = 0;
		while(i < lineas)	{
			j = 0;
			printf("Linea %i\n",i+1);
			while(j < campos_por_linea[i])	{	// Aqui es donde se procesarian los campos para transformarlos a valores enteres flotantes etc....
				printf("Campo %i: %s\n",j+1,valores[i][j]);	//Actualmente todos los campos son char*
				j++;
			}
			i++;
		}	
	}
	else	{
		printf("Error leyendo el archivo\n");
	}
}