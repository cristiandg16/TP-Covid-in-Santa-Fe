#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include "dataset.h"


unsigned long funcion_hash_localidades(char *clave_localidad) {
    unsigned long hash = 11;
    int c;
    while (c = *clave_localidad++)
        hash = ((hash << 3) + 3*hash) + c; /* hash * 33 + c */ 
    
    return hash;
}


//Iniciliaza una localidad vacia.
Localidad init_localidad(){
    Localidad nueva = malloc(sizeof(Localidad));
    nueva->nombre_localidad = malloc(sizeof(char)*100);
    nueva->registro_localidad = crear_registro();
    return nueva;
}

//Determina si una localidad es vacia.
int empty_localidad(Localidad local){
   return (registro_vacio(local->registro_localidad) == 1);
}

tabla_Localidades* crear_tabla_localidades(unsigned int capacidad,Funcion_Hash_Localidades hash_local){
    tabla_Localidades* tabla_localidades = malloc(sizeof(tabla_Localidades));
    tabla_localidades->total_localidades = capacidad;
    tabla_localidades->arr_localidades = malloc(sizeof(Localidad)*capacidad);
    tabla_localidades->hash_local = hash_local;
    tabla_localidades->localidades_afectadas = 0;
   
    for(unsigned long index = 0;index < tabla_localidades->total_localidades;index++){
        tabla_localidades->arr_localidades[index]=init_localidad();
    }
    
    return tabla_localidades;
}

void insertar_registro_tabla_localidad(tabla_Localidades *tabla_loc,char* nombre_localidad,char* fecha,unsigned int casos_conf,unsigned int casos_desc,unsigned int casos_estudio,unsigned int notificaciones){
    unsigned long index = tabla_loc->hash_local(nombre_localidad);
    index = index%(tabla_loc->total_localidades);

    while(!registro_vacio(tabla_loc->arr_localidades[index]->registro_localidad) && strcmp(tabla_loc->arr_localidades[index]->nombre_localidad,nombre_localidad) != 0){
        index++;
        index = index%(tabla_loc->total_localidades);
    }

    if(empty_localidad(tabla_loc->arr_localidades[index])){
        strcpy(tabla_loc->arr_localidades[index]->nombre_localidad,nombre_localidad);
        tabla_loc->arr_localidades[index]->registro_localidad = carga_registro_por_fecha(tabla_loc->arr_localidades[index]->registro_localidad,fecha,casos_conf,casos_desc,casos_estudio,notificaciones);
        tabla_loc->localidades_afectadas++;
    }
    else{
        if(existe_registro_por_fecha(tabla_loc->arr_localidades[index]->registro_localidad,fecha)){
            tabla_loc->arr_localidades[index]->registro_localidad = eliminar_registro_por_fecha(tabla_loc->arr_localidades[index]->registro_localidad,fecha);
            tabla_loc->arr_localidades[index]->registro_localidad = carga_registro_por_fecha(tabla_loc->arr_localidades[index]->registro_localidad,fecha,casos_conf,casos_desc,casos_estudio,notificaciones);
        }
        else{
            tabla_loc->arr_localidades[index]->registro_localidad = carga_registro_por_fecha(tabla_loc->arr_localidades[index]->registro_localidad,fecha,casos_conf,casos_desc,casos_estudio,notificaciones);
            tabla_loc->localidades_afectadas++;
        }
    }

   
}

void eliminar_registro_tabla_localidad(tabla_Localidades *tabla_loc,char* nombre_localidad,char* fecha){
    unsigned long index = tabla_loc->hash_local(nombre_localidad);
    index = index%(tabla_loc->total_localidades);

    if(!empty_localidad(tabla_loc->arr_localidades[index])){
        if(existe_registro_por_fecha(tabla_loc->arr_localidades[index]->registro_localidad,fecha)){
            tabla_loc->arr_localidades[index]->registro_localidad = eliminar_registro_por_fecha(tabla_loc->arr_localidades[index]->registro_localidad,fecha);
            if(registro_vacio(tabla_loc->arr_localidades[index]->registro_localidad)){
                free(tabla_loc->arr_localidades[index]->nombre_localidad);
                tabla_loc->localidades_afectadas--;
            }
        }
    }
}

Localidad buscar_localidad_tabla_localidad(tabla_Localidades *tabla_loc,char* nombre_localidad_buscar){
    unsigned long index = tabla_loc->hash_local(nombre_localidad_buscar);
    index = index%tabla_loc->total_localidades;

    if(!empty_localidad(tabla_loc->arr_localidades[index])){
        return tabla_loc->arr_localidades[index];
    }

    return NULL;
}

void destruir_tabla_localidad(tabla_Localidades* tabla_loc){
    for(unsigned long index = 0;index < tabla_loc->total_localidades;index++){
        free(tabla_loc->arr_localidades[index]->nombre_localidad);
        destruir_registro(tabla_loc->arr_localidades[index]->registro_localidad);
        free(tabla_loc->arr_localidades[index]);   
    }

    free(tabla_loc->arr_localidades);
    free(tabla_loc);
}

void imprimir_tabla(tabla_Localidades* tabla_loc){
    for(unsigned long index = 0;index < tabla_loc->total_localidades;index++){
        if(!empty_localidad(tabla_loc->arr_localidades[index])){
            printf("Localidad: %s\n",tabla_loc->arr_localidades[index]->nombre_localidad);
            recorrer_registro(tabla_loc->arr_localidades[index]->registro_localidad);
        }
        puts("----------------------------- LOC ");
    }
}

//------------------------------- DEPARTAMENTOS ------------------------------------------


Departamento depto_init(){
    Departamento nuevo = malloc(sizeof(Departamento));
    nuevo->nombre_departamento = malloc(sizeof(char)*100);
    nuevo->localidades_depto = crear_tabla_localidades(127,funcion_hash_localidades);
    return nuevo;
}

tabla_Deptos* crear_tabla_deptos(unsigned int cantidad_deptos,Funcion_Hash_Departamentos fun_hash){
    tabla_Deptos *tabla_dep = malloc(sizeof(tabla_Deptos));
    tabla_dep->total_deptos = cantidad_deptos;
    tabla_dep->arr_departamentos = malloc(sizeof(Departamento)*cantidad_deptos);
    tabla_dep->hash_depto = fun_hash;
    tabla_dep->deptos_afectados = 0;

    for(unsigned int index = 0;index < tabla_dep->total_deptos; index++){
        tabla_dep->arr_departamentos[index] = depto_init();
    }

    return tabla_dep;

}


int empty_tabla_localidad(tabla_Localidades* tabla_loc){
    if(tabla_loc->localidades_afectadas == 0){
        return 1;
    }
    else return 0;
}


void insertar_registro_depto_localidad(tabla_Deptos* tabla_dep,char* nombre_depto,char* nombre_localidad,char* fecha,unsigned int casos_conf,unsigned int casos_desc,unsigned int casos_estudio,unsigned int notificaciones){
    unsigned long index_depto = tabla_dep->hash_depto(nombre_depto);
    index_depto = index_depto%tabla_dep->total_deptos;

    while(!empty_tabla_localidad(tabla_dep->arr_departamentos[index_depto]->localidades_depto) && strcmp(tabla_dep->arr_departamentos[index_depto]->nombre_departamento,nombre_depto) != 0){
        index_depto++;
        index_depto = index_depto%tabla_dep->total_deptos;
    }

    if(empty_tabla_localidad(tabla_dep->arr_departamentos[index_depto]->localidades_depto)){
        strcpy(tabla_dep->arr_departamentos[index_depto]->nombre_departamento,nombre_depto);
        insertar_registro_tabla_localidad(tabla_dep->arr_departamentos[index_depto]->localidades_depto,nombre_localidad,fecha,casos_conf,casos_desc,casos_estudio,notificaciones);
        tabla_dep->deptos_afectados++;
    }
    else{
        insertar_registro_tabla_localidad(tabla_dep->arr_departamentos[index_depto]->localidades_depto,nombre_localidad,fecha,casos_conf,casos_desc,casos_estudio,notificaciones);

    }

    
}


void eliminar_registro_depto_localidad(tabla_Deptos* tabla_dep,char* nombre_depto,char* nombre_localidad,char* fecha){
    unsigned long index_depto = tabla_dep->hash_depto(nombre_depto);
    index_depto = index_depto % tabla_dep->total_deptos;

    eliminar_registro_tabla_localidad(tabla_dep->arr_departamentos[index_depto]->localidades_depto,nombre_localidad,fecha);
}


void destruir_tabla_depto(tabla_Deptos* tabla_dep){
    for(unsigned int index = 0;index < tabla_dep->total_deptos;index++){
        free(tabla_dep->arr_departamentos[index]->nombre_departamento);
        destruir_tabla_localidad(tabla_dep->arr_departamentos[index]->localidades_depto);
        free(tabla_dep->arr_departamentos[index]);
    }

    free(tabla_dep->arr_departamentos);
    free(tabla_dep);
}

void imprimir_tabla_deptos(tabla_Deptos* tabla_dep){
    for(unsigned long index = 0;index < tabla_dep->total_deptos;index++){
        if(!empty_tabla_localidad(tabla_dep->arr_departamentos[index]->localidades_depto)){
            printf("Depto: %s\n",tabla_dep->arr_departamentos[index]->nombre_departamento);
            imprimir_tabla(tabla_dep->arr_departamentos[index]->localidades_depto);
        }
        puts("----------------------------- DEP ");
    }
}


Departamento buscar_depto(tabla_Deptos *tabla_dep,char* nombre_depto_buscar){
    unsigned long index = tabla_dep->hash_depto(nombre_depto_buscar);
    index = index%tabla_dep->total_deptos;

    if(!empty_tabla_localidad(tabla_dep->arr_departamentos[index]->localidades_depto)){
        return tabla_dep->arr_departamentos[index];
    }

    return NULL;
}