#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include "dataset.h"


int entrada_datos(char* nombre_archivo,tabla_Deptos* tabla1){
    FILE *archivo_entrada = NULL;
    archivo_entrada = fopen(nombre_archivo,"r");
    if(archivo_entrada != NULL){
        char lectura_temp[150];
        char fecha[100],depto[100],localidad[100],casos_conf[100],casos_desc[100],casos_estudio[100],notificaciones[100];
        char *traza = NULL;
        do{
            memset(lectura_temp,0,150);
            fgets(lectura_temp,150,archivo_entrada);
            
            traza = strtok(lectura_temp,",");
            if(traza != NULL){
                strcpy(fecha,traza);
                traza = strtok(NULL,",");
                strcpy(depto,traza);
                traza = strtok(NULL,",");
                strcpy(localidad,traza);
                traza = strtok(NULL,",");
                strcpy(casos_conf,traza);
                traza = strtok(NULL,",");
                strcpy(casos_desc,traza);
                traza = strtok(NULL,",");
                strcpy(casos_estudio,traza);
                traza = strtok(NULL,",");
                strcpy(notificaciones,traza);
        
                insertar_registro_depto_localidad(tabla1,depto,localidad,fecha,atoi(casos_conf),atoi(casos_desc),atoi(casos_estudio),atoi(notificaciones));

            }
           
        }while(feof(archivo_entrada)==0);
        fclose(archivo_entrada);
        printf("Datos cargados correctamente.\n");
        return 1;
    }
    else{
        printf("Error leyendo el archivo. Intente nuevamente.\n");
    }
    return 0;
}



void imprime_data(char *nombre_archivo,tabla_Deptos *tabla){
    FILE *archivo_salida = NULL;
    archivo_salida = fopen(nombre_archivo,"w");

    if(archivo_salida!=NULL){
        for(unsigned int index_1 = 0;index_1 < tabla->total_deptos;index_1++){
            if(!empty_tabla_localidad(tabla->arr_departamentos[index_1]->localidades_depto)){
                for(unsigned int index_2 = 0;index_2 < tabla->arr_departamentos[index_1]->localidades_depto->total_localidades;index_2++){
                    if(!registro_vacio(tabla->arr_departamentos[index_1]->localidades_depto->arr_localidades[index_2]->registro_localidad)){
                        Registro aux = tabla->arr_departamentos[index_1]->localidades_depto->arr_localidades[index_2]->registro_localidad;
                        for(;aux!=NULL;aux = aux->siguiente){
                            fprintf(archivo_salida,"%s,%s,%s,%d,%d,%d,%d\n",aux->fecha,
                                                tabla->arr_departamentos[index_1]->nombre_departamento,
                                                tabla->arr_departamentos[index_1]->localidades_depto->arr_localidades[index_2]->nombre_localidad,
                                                aux->casos_conf_acumm,
                                                aux->casos_desc_acumm,
                                                aux->casos_estudio_acumm,
                                                aux->noti_acumm);
                        }
                    } 
                }
            }
        }
        fclose(archivo_salida);
    }
    else{
        printf("Error en la creación del nuevo archivo.\n");
    }
}