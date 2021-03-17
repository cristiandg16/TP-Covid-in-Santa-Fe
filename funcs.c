#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include "dataset.h"




void interprete(){
    tabla_Deptos *tabla = crear_tabla_deptos(200,funcion_hash_localidades);

    char entrada[1000];

    scanf(" %[^\n]",entrada);

    int cargado = 0;
    while(strcmp(entrada,"salir") != 0){
        char *command = NULL;
        command = strtok(entrada," ");
        

        if(strcmp(command,"cargar_dataset")==0 && cargado == 0){
            command = strtok(NULL," ");
            cargado = entrada_datos(command,tabla);
            
        }
        else if(strcmp(command,"imprimir_dataset")==0 && cargado == 1){
            command = strtok(NULL," ");
            imprime_data(command,tabla);
        }
        else if(strcmp(command,"imprimir_dataset")==0 && cargado == 0){
            printf("No hay ningun dataset cargado.\n");
        }
        else if(strcmp(command,"agregar_registro")==0 && cargado == 1){
            char fecha[100];
            char depto[100];
            char localidad[100];
            char casos_conf[100];
            char casos_desc[100];
            char casos_estudio[100];

            command = strtok(NULL,",");

            
            strcpy(fecha,command);
            
                
                command = strtok(NULL,",");
                strcpy(depto,command);
                command = strtok(NULL,",");
                strcpy(localidad,command);
                command = strtok(NULL,",");
                strcpy(casos_conf,command);
                command = strtok(NULL,",");
                strcpy(casos_desc,command);
                command = strtok(NULL," ");
                strcpy(casos_estudio,command);

                if(strcmp(casos_conf,"0")==0 || atoi(casos_conf)>0){
                    if(strcmp(casos_desc,"0")==0 || atoi(casos_desc)>0){
                        if(strcmp(casos_estudio,"0")==0 || atoi(casos_estudio)>0){
                            insertar_registro_depto_localidad(tabla,depto,localidad,fecha,atoi(casos_conf),atoi(casos_desc),atoi(casos_estudio),atoi(casos_conf)+atoi(casos_desc)+atoi(casos_estudio));
                            printf("El registro se ha insertado exitosamente.\n");
                        }
                        else{
                            printf("Casos en estudio erroneos. Reingrese un nuevo registro.\n");
                        }
                    }
                    else{
                        printf("Casos descartados erroneos. Reingrese un nuevo registro.\n");
                    }
                }
                else{
                    printf("Casos confirmados erroneos. Reingrese un nuevo registro.\n");
                }
            
            
            

        }
        else if(strcmp(command,"eliminar_registro")==0 && cargado == 1){
            char fecha[100];
            char depto[100];
            char localidad[100];

            command = strtok(NULL," ");

            strcpy(fecha,command);
            command = strtok(NULL," ");
            strcpy(depto,command);
            command = strtok(NULL," ");
            strcpy(localidad,command);

            eliminar_registro_depto_localidad(tabla,depto,localidad,fecha);

        }
        else if(strcmp(command,"buscar_pico")==0 && cargado == 1){
            char depto[100];
            char localidad[100];

            command = strtok(NULL,",");

            strcpy(depto,command);
            command = strtok(NULL,",");
            strcpy(localidad,command);

            Departamento busqueda = buscar_depto(tabla,depto);
            
            if(busqueda != NULL){
                Localidad busqueda2 = buscar_localidad_tabla_localidad(busqueda->localidades_depto,localidad);
                if(busqueda2 != NULL){
                    pico_localidad(busqueda2->registro_localidad);
                }
                else{
                    printf("No se ha encontrado la localidad recibida.\n");
                }
            }
            else{
                printf("No se ha encontrado la localidad recibida.\n");
            }

        }
        else if(strcmp(command,"casos_acumulados")==0 && cargado == 1){
            char depto[100];
            char localidad[100];
            char fecha[100];

            command = strtok(NULL,",");
            strcpy(fecha,command);
            command = strtok(NULL,",");
            strcpy(depto,command);
            command = strtok(NULL,",");
            strcpy(localidad,command);

            Departamento busqueda = buscar_depto(tabla,depto);
            
            if(busqueda != NULL){
                Localidad busqueda2 = buscar_localidad_tabla_localidad(busqueda->localidades_depto,localidad);
                if(busqueda2 != NULL){
                   casos_acum_hasta_fecha(busqueda2->registro_localidad,fecha);
                }
                else{
                    printf("No se ha encontrado la localidad recibida.\n");
                }
            }
            else{
                printf("No se ha encontrado la localidad recibida.\n");
            }

        }
        else if(strcmp(command,"tiempo_duplicacion")==0 && cargado == 1){
            char depto[100];
            char localidad[100];
            char fecha[100];

            command = strtok(NULL,",");
            strcpy(fecha,command);
            command = strtok(NULL,",");
            strcpy(depto,command);
            command = strtok(NULL,",");
            strcpy(localidad,command);

            Departamento busqueda = buscar_depto(tabla,depto);
            
            if(busqueda != NULL){
                Localidad busqueda2 = buscar_localidad_tabla_localidad(busqueda->localidades_depto,localidad);
                if(busqueda2 != NULL){
                   dias_duplicacion_casos(busqueda2->registro_localidad,fecha);
                }
                else{
                    printf("No se ha encontrado la localidad recibida.\n");
                }
            }
            else{
                printf("No se ha encontrado la localidad recibida.\n");
            }
        }
        else if(strcmp(command,"graficar")==0 && cargado == 1){
            char depto[100];
            char localidad[100];
            char fecha_init[100];
            char fecha_end[100];

            command = strtok(NULL,",");
            strcpy(fecha_init,command);
            command = strtok(NULL,",");
            strcpy(fecha_end,command);
            command = strtok(NULL,",");
            strcpy(depto,command);
            command = strtok(NULL,",");
            strcpy(localidad,command);

            Departamento busqueda = buscar_depto(tabla,depto);
            
            if(busqueda != NULL){
                Localidad busqueda2 = buscar_localidad_tabla_localidad(busqueda->localidades_depto,localidad);
                if(busqueda2 != NULL){
                   plot_casos_por_dia(busqueda2->registro_localidad,fecha_init,fecha_end);
                }
                else{
                    printf("No se ha encontrado la localidad recibida.\n");
                }
            }
            else{
                printf("No se ha encontrado la localidad recibida.\n");
            }

        }
        else if(cargado == 0){
            printf("Comando no reconocido, por favor vuelva a ingresar.\n");
        }
        else{
            printf("Ya existe un dataset cargado.\n");
        }
        scanf(" %[^\n]",entrada);	
    }

    destruir_tabla_depto(tabla);
    
}