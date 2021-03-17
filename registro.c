#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include "dataset.h"

//Retorna 1 si fecha 1 es anterior a fecha2, 0 si es al reves y 2 si las fechas coinciden.
// AAAA-MM-DD formato de fecha.

int comparar_fecha(char* fecha1,char* fecha2){
    int anio1,mes1,dia1;
    int anio2,mes2,dia2;

    char* copia_fecha1 = malloc(sizeof(char)*100);
    strcpy(copia_fecha1,fecha1);

    char* copia_fecha2 = malloc(sizeof(char)*100);
    strcpy(copia_fecha2,fecha2);

    char* fecha_1 = strtok(copia_fecha1,"T");
    char* fecha_2 = strtok(copia_fecha2,"T");

    char* tok_fecha_1 = strtok(fecha_1,"-");

    anio1 = atoi(tok_fecha_1);
    tok_fecha_1 = strtok(NULL,"-");
    mes1 = atoi(tok_fecha_1);
    tok_fecha_1 = strtok(NULL,"");
    dia1 = atoi(tok_fecha_1);

    //---------------------------

    char* tok_fecha_2 = strtok(fecha_2,"-");

    anio2 = atoi(tok_fecha_2);    
    tok_fecha_2 = strtok(NULL,"-");
    mes2 = atoi(tok_fecha_2);
    tok_fecha_2 = strtok(NULL,"");
    dia2 = atoi(tok_fecha_2);

    free(copia_fecha1);
    free(copia_fecha2);

    if(anio1 < anio2){
        return 1;
    }
    else if(anio1 == anio2){
        if(mes1 < mes2){
            return 1;
        }
        else if(mes1 == mes2){
            if(dia1 < dia2){
                return 1;
            }
            else if(dia1 == dia2){
                return 2;
            }
            else return 0;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }

}

//Crea e inicializa un registro vacio
Registro crear_registro(){
    return NULL;
}

//Determina si un registro es vacio.
int registro_vacio(Registro registro){
    return (registro == NULL);
}

//Retorna 1 si existe el registro con tal fecha, 0 sino.
int existe_registro_por_fecha(Registro registro,char* fecha_a_buscar){
    if(!registro_vacio(registro)){
        Registro auxiliar = registro;
        while(!registro_vacio(auxiliar) && comparar_fecha(auxiliar->fecha,fecha_a_buscar)!= 2){
            auxiliar = auxiliar->siguiente;
        }
        if(!registro_vacio(auxiliar)){
            return 1;
        }
    }

    return 0;
}

//Elimina un registro por fecha.
Registro eliminar_registro_por_fecha(Registro registro,char* fecha_eliminar){
    if(!registro_vacio(registro)){
        Registro auxiliar = registro;
        
        for(;!registro_vacio(auxiliar) && comparar_fecha(auxiliar->fecha,fecha_eliminar) != 2;auxiliar = auxiliar->siguiente);
        
        if(registro_vacio(auxiliar)){
            return registro;
        }
        else if(auxiliar->anterior == NULL && comparar_fecha(auxiliar->fecha,fecha_eliminar) == 2){
            if(auxiliar->siguiente == NULL){
                free(auxiliar->fecha);
                free(auxiliar);
                return NULL;
            }
            registro = registro->siguiente;
            registro->anterior = NULL;
            free(auxiliar->fecha);
            free(auxiliar);
        }
        else if(auxiliar->siguiente == NULL && comparar_fecha(auxiliar->fecha,fecha_eliminar) == 2){
            auxiliar->anterior->siguiente = NULL;
            free(auxiliar->fecha);
            free(auxiliar);
        }
        else{
            Registro a_eliminar = auxiliar;
            auxiliar = a_eliminar->siguiente;
            auxiliar->anterior = a_eliminar->anterior;
            a_eliminar->anterior->siguiente = auxiliar;
            free(a_eliminar->fecha);
            free(a_eliminar);
            
        }
        
    }
    return registro;
}

//Ingresa un registro en la fecha pasada como argumento.
Registro carga_registro_por_fecha(Registro registro,char* fecha,unsigned int casos_conf,unsigned int casos_desc,unsigned int casos_estudio,unsigned int notificaciones){
    Registro nuevo = malloc(sizeof(Day));
    nuevo->fecha = malloc(sizeof(char)*100);
    strcpy(nuevo->fecha,fecha);
    nuevo->casos_conf_acumm = casos_conf;
    nuevo->casos_desc_acumm = casos_desc;
    nuevo->casos_estudio_acumm = casos_estudio;
    nuevo->noti_acumm = notificaciones;
    

    if(registro_vacio(registro)){
        nuevo->siguiente = crear_registro();
        nuevo->anterior = crear_registro();
        nuevo->casos_conf_dia = casos_conf;
        return nuevo;
    }
    else{
        Registro aux = registro;
        Registro aux_ant = crear_registro();
        
        while(!registro_vacio(aux) && comparar_fecha(aux->fecha,fecha)!=0){
            aux_ant = aux;
            aux = aux->siguiente;
        }
        if(registro_vacio(aux_ant) && comparar_fecha(aux->fecha,fecha)==0){
            nuevo->siguiente = registro;
            nuevo->anterior = NULL;
            registro->anterior = nuevo;
            nuevo->siguiente->casos_conf_dia = nuevo->siguiente->casos_conf_acumm - nuevo->casos_conf_acumm;
            return nuevo;
        }
        else if(registro_vacio(aux)){
            nuevo->siguiente = NULL;
            nuevo->anterior = aux_ant;
            aux_ant->siguiente = nuevo;
            nuevo->casos_conf_dia = nuevo->casos_conf_acumm - nuevo->anterior->casos_conf_acumm;
        }
        else{
            aux_ant->siguiente = nuevo;
            nuevo->siguiente = aux;
            nuevo->anterior = aux_ant;
            aux->anterior = nuevo;
            nuevo->casos_conf_dia = nuevo->casos_conf_acumm - nuevo->anterior->casos_conf_acumm;
        }
    }

    return registro;
}


//Recorre un registro mostrando los datos correspondientes.
void recorrer_registro(Registro registro){
    if(registro_vacio(registro)) return;

    Registro aux = registro;
    while(!registro_vacio(aux)){
        printf("%s Confirmados Acumulados: %d  Confirmados Diarios: %d  En estudio: %d  Descartados %d Notis: %d ->\n",aux->fecha,aux->casos_conf_acumm,aux->casos_conf_dia,
        aux->casos_estudio_acumm,aux->casos_desc_acumm,aux->noti_acumm);
        aux = aux->siguiente;
    }
    printf("\n");
}

//Destruye un registro liberando la memoria utilizada.
void destruir_registro(Registro registro){
    if(!registro_vacio(registro)){
        Registro temp = registro;
        Registro a_eliminar;
        while(!registro_vacio(temp)){
            a_eliminar = temp;
            temp = temp->siguiente;
            free(a_eliminar->fecha);
            free(a_eliminar);
        }
    }
}



//Retorna la cantidad de casos acumulados hasta la fecha, 0 si no existe la fecha a buscar.
void casos_acum_hasta_fecha(Registro registro,char* fecha_buscar){
    if(registro_vacio(registro)){
        printf("No se encuentra registro para la fecha solicitada.\n");
        return;
    }
    Registro aux = registro;
    for(;aux != NULL;aux = aux->siguiente){
        if(comparar_fecha(aux->fecha,fecha_buscar)==2){
            printf("Casos confirmados acumulados hasta el %s : %d\n",fecha_buscar,aux->casos_conf_acumm); //poner en el interprete
            return;
        }
    }
    printf("No se encuentra registro para la fecha solicitada.\n"); //poner en el interprete
}

Registro fecha_nodo(Registro registro,char* fecha_buscar){
    Registro aux = registro;
    for(;aux != NULL;aux = aux->siguiente){
        if(comparar_fecha(aux->fecha,fecha_buscar)==2){
            return aux;
        }
    }
    
    return NULL;
}


//Devuelve el pico de casos confirmados en un dia en un registro.
void pico_localidad(Registro registro){
    if(registro_vacio(registro)){
        printf("No hay registro para la localidad buscada.\n");
        return;
    }
    
    int pico = 0;
    char fecha_pico[100];
    Registro aux = registro;
    

    for(;aux != NULL;aux = aux->siguiente){
        if(pico < aux->casos_conf_dia){
            pico = aux->casos_conf_dia;
            strcpy(fecha_pico,aux->fecha);
        }
    }
    printf("Pico de contagios = %d, en el dia %s\n",pico,fecha_pico);
}


//Imprime en pantalla los dias que tardaron para duplicarse los datos hasta la fecha solicitada.
void dias_duplicacion_casos(Registro registro,char* fecha_duplicacion){
    Registro aux1 = fecha_nodo(registro,fecha_duplicacion);
    if(!registro_vacio(aux1)){
        int casos_confirmados = aux1->casos_conf_acumm;
        int dias = 0;
        while(aux1!=NULL && (casos_confirmados/2) <= aux1->casos_conf_acumm){
            dias++;
            aux1 = aux1->anterior;
        }
        if(!registro_vacio(aux1)){
            printf("Dias transcurridos hasta la duplicacion : %d \n",dias);
        }
        else{
            printf("No se han duplicados los casos hasta el dia de la fecha.\n");
        }
        
    }
    else{
        printf("No se encontraron registros para esa fecha.\n");
    }
    
}

//Grafica los casos por dia dados entre las fechas recibidas como argumentos.
//En caso de que la fecha init no se encuentre pero la final si, se toma como init
//la primera subsiguiente del registro.
//Si la fecha_end no se encuentra, no se imprime debido a que puede haber fallas
//en los datos. 
void plot_casos_por_dia(Registro registro,char* fecha_init,char* fecha_end){
    if(!registro_vacio(registro)){
        if(existe_registro_por_fecha(registro,fecha_end)){
            if(comparar_fecha(fecha_init,fecha_end) == 1){
                Registro nodo_init = fecha_nodo(registro,fecha_init);
                int cant_dias = 1;
                if(registro_vacio(nodo_init)){  //Si la fecha_init no esta en el registro, comenzar desde el primer dia.
                    Registro aux = registro;
                    for(;comparar_fecha(aux->fecha,fecha_end) != 2;aux = aux->siguiente){
                        cant_dias++;
                    }
                    nodo_init = registro;

                }
                else{
                    Registro aux1 = nodo_init;
                
                    for(;comparar_fecha(aux1->fecha,fecha_end) != 2;aux1 = aux1->siguiente){
                        cant_dias++;
                    }
                }
                
                
                char **x_fechas = malloc(sizeof(char*)*cant_dias);
                for(int i = 0;i<cant_dias;i++){
                    x_fechas[i] = malloc(sizeof(char)*100);
                }

                int y_casos_fecha_diarios[cant_dias];
                int y_casos_fecha_acumm[cant_dias];
                int j = 0;
                
                while(j < cant_dias && comparar_fecha(nodo_init->fecha,fecha_end)!=2){
                    char fecha[100];
                    strcpy(fecha,nodo_init->fecha);
                    char *fecha_aux = strtok(fecha,"T");
                    
                    strcpy(x_fechas[j],fecha_aux);
                    y_casos_fecha_diarios[j] = nodo_init->casos_conf_dia;
                    y_casos_fecha_acumm[j] = nodo_init->casos_conf_acumm;
                    nodo_init = nodo_init->siguiente;
                    j++;
                    
                }
                printf("%s\n",nodo_init->fecha);
                char fecha[100];
                strcpy(fecha,nodo_init->fecha);
                char *fecha_aux = strtok(fecha,"T");
                    
                strcpy(x_fechas[j],fecha_aux);
                y_casos_fecha_diarios[j] = nodo_init->casos_conf_dia;
                y_casos_fecha_acumm[j] = nodo_init->casos_conf_acumm;

                FILE * temp = fopen("data_tp.temp", "w");
                FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
                int index;
                for (index=0; index < cant_dias; index++){
                    fprintf(temp, "%s , %d , %d \n", x_fechas[index], y_casos_fecha_diarios[index],  y_casos_fecha_acumm[index]); //Escribe los datos en un archivo temporal
                }

                char * commandsForGnuplot[] = {"set title 'Casos confirmados por fecha'",
                                               "set ylabel 'Cantidad de casos confirmados'",
                                               "set xlabel 'Fecha'",
                                               "set xdata time",
                                               "set timefmt '%Y-%m-%d'",
                                               "set format x '%Y-%m-%d'",
                                               "set datafile sep ','",
                                               "set grid",
                                               "set autoscale x",
                                               "set xtics auto",
                                               "plot 'data_tp.temp' using 1:2 lt rgb 'red' w l title 'Casos confirmados diarios'",
                                               "replot 'data_tp.temp' using 1:3 lt rgb 'violet' w l title 'Casos confirmados acumulados' "
                                              };
                
                int index2;
                for (index2=0; index2 < 12; index2++){
                    fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[index2]); //Envia los comandos a gnuplot uno por uno.
                }
                
                fclose(temp);
                pclose(gnuplotPipe);                
                
                for(int k = 0;k<cant_dias;k++){
                    free(x_fechas[k]);
                }
                free(x_fechas);
            }
            else{
                printf("La fecha inicial no es anterior a la fecha final ingresada. Por favor, reingrese nuevamente.\n");
                return;
            }
        }
        else{
            printf("Fecha Final no registrada, por favor reingrese una nueva fecha.\n");
            return;
        }
    }
    
}




int main(){
    setlocale(LC_ALL,"");
   /*
    Registro nuevo = crear_registro();
    nuevo = carga_registro_por_fecha(nuevo,"2020-12-01T00:00:00-03:00",100,35,2,157);
    nuevo = carga_registro_por_fecha(nuevo,"2020-12-02T00:00:00-03:00",145,39,10,194);
    nuevo = carga_registro_por_fecha(nuevo,"2020-12-03T00:00:00-03:00",166,48,22,236);
    nuevo = carga_registro_por_fecha(nuevo,"2020-12-04T00:00:00-03:00",200,78,41,319);
    nuevo = carga_registro_por_fecha(nuevo,"2020-12-05T00:00:00-03:00",240,120,51,411);
    
    recorrer_registro(nuevo);

    
    if(existe_registro_por_fecha(nuevo,"2020-12-04T00:00:00-03:00")){
        puts("OPCION1");
        nuevo = eliminar_registro_por_fecha(nuevo,"2020-12-04T00:00:00-03:00");
        nuevo = carga_registro_por_fecha(nuevo,"2020-12-04T00:00:00-03:00",195,56,33,284);
    }
    else{
        puts("OPCION2");
        nuevo = carga_registro_por_fecha(nuevo,"2020-12-04T00:00:00-03:00",195,56,33,284);
    }
   
    

    pico_localidad(nuevo);

    dias_duplicacion_casos(nuevo,"2020-12-03T00:00:00-03:00");
   // plot_casos_por_dia(nuevo,"2020-12-03T00:00:00-03:00","2020-12-05T00:00:00-03:00");
    plot_casos_por_dia(nuevo,"2020-12-02T00:00:00-03:00","2020-12-04T00:00:00-03:00");
    
  // nuevo = eliminar_registro_por_fecha(nuevo,"2020-12-01T00:00:00-03:00");
   //recorrer_registro(nuevo);
   // printf("%d\n",registro_vacio(nuevo));
    destruir_registro(nuevo);
     


    tabla_Localidades* tabla1 = crear_tabla_localidades(127,funcion_hash_localidades);

    insertar_registro_tabla_localidad(tabla1,"Rosario","2020-12-03T00:00:00-03:00",145,39,10,194);
    insertar_registro_tabla_localidad(tabla1,"Rosario","2020-12-04T00:00:00-03:00",165,49,20,234);
    insertar_registro_tabla_localidad(tabla1,"Rosario","2020-12-05T00:00:00-03:00",200,69,33,302);
    insertar_registro_tabla_localidad(tabla1,"Funessaas","2020-12-05T00:00:00-03:00",5,9,2,16);
    
    imprimir_tabla(tabla1);

    eliminar_registro_tabla_localidad(tabla1,"Rosario","2020-12-03T00:00:00-03:00");
    puts("SKEREEEEEEEEEEE");
    imprimir_tabla(tabla1);

    destruir_tabla_localidad(tabla1);
    

    tabla_Deptos* tabla1 = crear_tabla_deptos(200,funcion_hash_localidades);
    insertar_registro_depto_localidad(tabla1,"9 de julio","ANTONIO PINI","2020-12-06T00:00:00-03:00",145,39,10,194);
     insertar_registro_depto_localidad(tabla1,"9 de julio","CASANOVA","2020-12-03T00:00:00-03:00",15,9,10,34);
     insertar_registro_depto_localidad(tabla1,"ROSARIO","PEREZ","2020-12-04T00:00:00-03:00",23,5,10,38);
    insertar_registro_depto_localidad(tabla1,"9 de julio","ANTONIO PINI","2020-12-04T00:00:00-03:00",200,50,10,260);
    insertar_registro_depto_localidad(tabla1,"9 de julio","ANTONIO PINI","2020-12-03T00:00:00-03:00",200,50,10,260);
    insertar_registro_depto_localidad(tabla1,"9 de julio","ANTONIO PINI","2020-12-05T00:00:00-03:00",200,50,10,260);
    
    entrada_datos("query_result_2020-11-23T15_59_05.333634-03_00.csv",tabla1);
    plot_casos_por_dia(nuevo,"2020-12-02T00:00:00-03:00","2020-12-04T00:00:00-03:00");
    imprimir_tabla_deptos(tabla1);
    destruir_tabla_depto(tabla1);
    */

    interprete();
    return 0;
}