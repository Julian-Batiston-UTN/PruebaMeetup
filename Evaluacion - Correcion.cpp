/*En este ejercicio intentaré poner en acción todas las buenas prácticas de programación
que conozco para poder hacer de la lectura y funcionamiento del código lo mejor posible
agradezco su lectura con detenimiento y la valorización de mi esfuerzo en este trabajo sencillo*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int valorInv=-1; /*Valor utilizado para reiniciar las var, Opc1 y Opc2 luego de ser usadas*/

#include "evaluacion - correcion.h" //Lo primero que hice fué mover las funciones y las struct a un archivo .h para limpiar un poco el código

int main() {
	
	/*Cambie el nombre de las variables n y m por Opc1 y Opc2 que son nombres más representativo dado a que las variables númericas son utilizadas
	para seleccionar opciones en los distintos menus*/
	
	int Opc1,NumDatos=0;
	
	struct dato *ArrDat=NULL;
	
	printf("BIENVENIDO!!");
	
	
	do{
		
		Opc1=valorInv; 
		
		/*Luego de ingresar un valor válido (distinto de 3, dado a que ese es el valor de salir) el programa ejecuta la opción correspondiente 
		a ese valor y mantiene Opc1 en ese valor, por lo que a la hora de re ingreasar por el bucle do-while, el valor previamente cargado sigue en Opc1
		haciendo que se salte la validación (esto si se ingresa un caracter no numérico) y re ingresando en la opción valida previamente cargada.
		Como esto ocurre otra vez más adelante en el código opté por usar un valor "const int global" negativo, dado a que por las características
		intrinsecas del programa los número negativos siempre son inválidos :)*/
		
		//Menú de inico
		printf("\n\nIngrese una opción:");
		printf("\n\n(1) - Ingreso de datos\n(2) - Mostrar datos\n(3) - SALIR:\nOPCION: ");
		
		//Validación de valor ingresado
		fflush(stdin);
		scanf("%i",&Opc1);
		if(Opc1<1 or Opc1>3){
			
			printf("ERROR!!: El número de opción es inválido.");
			
		}
		
		switch (Opc1){
		case 1:{
			
			archivo_maquinas(ArrDat,NumDatos);
			NumDatos=cargar_maq(&ArrDat,NumDatos);
			archivo_maquinas(ArrDat,NumDatos);
			break;
		}
		case 2:{
				
				if(NumDatos!=0){
					
					int Opc2;
					Opc2=valorInv;
					
					/*Agregué un arreglo de dátos temporal para poder gestionar los datos aparte a la hora de comparar Tensiones y Corrientes
					evitando así que el orden de ingreso original sea afectado*/
					
					struct dato *ArrDatTemp = (struct dato *)malloc(NumDatos * sizeof(struct dato));
					
					for(int cont=0;cont<NumDatos;cont++){
						
						*(ArrDatTemp+cont)=*(ArrDat+cont);
						
					}
					
					printf("Ingrese una opción a mostrar:\n\n");
					
					printf("	(1) - Tensión máxima.\n	(2) - Corriente máxima.\n	(3) - Buscar maquina.\n\n	OPCION:	");
					
					do{
						
						fflush(stdin);
						scanf("%i",&Opc2);
						
						if(Opc2<1 or Opc2>3)
							printf("\n\nERROR!!: El número de opción es inválido.\n\n");
						
						
					}while(Opc2<1 or Opc2>3);
					
					switch(Opc2){
						
					case 1:{
						
						ordena_T(ArrDatTemp,NumDatos);
						
						//puts(ArrDat->NombreMaquina);
						printf("Tensión máxima:%i",ArrDatTemp->T);
						FILE *Ten;
						Ten=fopen("TensionMax.txt","wb");
						
						fwrite(ArrDatTemp->NombreMaquina, sizeof(char),strlen(ArrDatTemp->NombreMaquina),Ten);
						
						fclose(Ten);
						break;
					}
					case 2:{
						
						ordena_C(ArrDatTemp,NumDatos);
						
						//puts(ArrDat->NombreMaquina);
						printf("Corriente máxima:%i",ArrDatTemp->C);
						FILE *Corr;
						Corr=fopen("CorrienteMax.txt","wb");
						
						fwrite(ArrDatTemp->NombreMaquina, sizeof(char),strlen(ArrDatTemp->NombreMaquina),Corr);
						
						fclose(Corr);
						break;
					}
					case 3:{
						
					   char NombMaqBuscar[40];
					   
						printf("\nIngrese el nombre de la máquina a buscar:\n");
						fflush(stdin);
						gets(NombMaqBuscar);
						
						buscar_maquinas(NombMaqBuscar);
						
						break;
						
					}
						
						
					}
					
					free(ArrDatTemp);//Libero la variable temporal.
					
				}else{
					
					printf("!!ERROR: No ha cargado datos.");
					
				}
				
				
			}
			
		}
		
		
		
		
	}while(Opc1!=3);
	
	
	
}

void buscar_maquinas(const char *nombreBuscado) {
    FILE *archivo = fopen("Maquinas.txt", "r");
    if (archivo == NULL) {
        printf("ERROR: No se pudo abrir el archivo.\n");
        return;
    }

    char nombre[40];
    int tension, corriente;
    bool encontrada = false;

    printf("Buscando máquinas con el nombre '%s'...\n", nombreBuscado);

    while (fscanf(archivo, "%s %d %d", nombre, &tension, &corriente) != EOF) {
        if (strcmp(nombre, nombreBuscado) == 0) {
            printf("Máquina encontrada:\n");
            printf("Nombre: %s\n", nombre);
            printf("Tensión: %d\n", tension);
            printf("Corriente: %d\n", corriente);
            printf("-------------------------\n");
            encontrada = true;
        }
    }

    if (!encontrada) {
        printf("No se encontraron máquinas con el nombre '%s'.\n", nombreBuscado);
    }

    fclose(archivo);
}



void archivo_maquinas(struct dato *p, int CantDat){
	
	/*Cada vez que se ejecuta el programa el archivo ya existente con los datos de ejecuciones previas se vacia 
	para evitar error en la lógica del programa, en una exigencia más compleja y que se pida conservar estos datos
	y no perderlos se necesitaría otra lógica de programa lo que supone cambiar rotundamente la lógica inicial de
	este programa.*/
	
	FILE *Maquinas = fopen("Maquinas.txt","rb");
	
	if(Maquinas==NULL){
		printf("\n\n!!ERROR: No se pudo habrir el archivo.\n\n");
		return;
	}
	
	fseek(Maquinas,0,SEEK_END);
	
	long ArchVac=ftell(Maquinas);
	fclose(Maquinas);
	
	if(ArchVac!=0 and CantDat==0){
		
		FILE *Maquinas = fopen("Maquinas.txt","wb");
		fclose(Maquinas);
		
	}
	
	if(CantDat!=0){
		
		FILE *Maquinas;
		Maquinas= fopen("Maquinas.txt","wb+");
		
		if(Maquinas==NULL)
			printf("\n\n!!ERROR: No se pudo crear, abrir o editar el archivo solicitado\n\n");
		
		for (int cont = 0; cont < CantDat; cont++) {
        // Escribimos los datos en el archivo en el orden T, C, NombreMaquina
        fprintf(Maquinas, "%s %d %d\n", (p+cont)->NombreMaquina, (p+cont)->T, (p+cont)->C);
		}
		
		
		fclose(Maquinas);
		
		
	}

}
	
/*Como el case 1 se me hizo muy largo trabajé el ingreso de datos en una función general de carga de datos que parte de la base del Case 1 de la evaluacion pero
le he agregado funcionalidades explicadas en la función*/
int cargar_maq(struct dato **p, int CantDat){
	
	int Opc2, cont;
	
	if(CantDat==0){
		
		Opc2=valorInv; //Para evitar validaciones no adecuadas(Explicado arriba en el caso Opc1=valorInv)
		
		do{
			printf("\n\nIngrese el número de datos a guardar o pulse 0 para salir:\n\n");
			//Agregué la opcón salir
			fflush(stdin);
			scanf("%i",&Opc2);
			
			//El bloque de validación cambio debido a el agregado de la opción salir.
			if(Opc2==0){
				break;
			}else{
				if(Opc2<1 or Opc2>30)
				printf("ERROR!!: Valor ingresado inválido, debe ser un número entre 1 y 30, o 0 para salir.");
			}
			
		}while(Opc2<1 or Opc2>30);
		
		CantDat=Opc2; //Le paso a CantDat el valor de Opc2 para determinar si veuelve a ingresar al if (caso en el que el usuario pulsa 0) o salte al else (caso en el que ingreso al menos 1 dato).
		*p = (struct dato *)realloc(*p, Opc2 * sizeof(struct dato)); //ACLARACIÓN PERSONAL: No necesitaría agregar 1 elemento más al final, ya que eso solo se hace para cadenas de caracteres
		
		
		/*Solicito los datos al usuario y los carga en el arreglo de structs.*/
		for(cont=0;cont<Opc2;cont++){
			
			fflush(stdin);
			printf("\nIngrese el nombre de la maquina %i:\n",(cont+1));
			
			gets((*p+cont)->NombreMaquina);
			
			//puts((ArrDat+i)->NombreMaquina);
			
			do{
				
				printf("\nIngrese el valor de la tensión de esa maquina:\n");
				
				(*p+cont)->T=0;
				
				fflush(stdin);
				scanf("%i",&(*p+cont)->T);
				
				if((*p+cont)->T<1 or (*p+cont)->T>1000)
					printf("\n\n!!ERROR: Valor de la tensión inválido.\n\n");
				
			}while((*p+cont)->T<1 or (*p+cont)->T>1000);
			
			do{
				printf("\nIngrese el valor de la corriente de esa maquina:\n");
				
				(*p+cont)->C=0;
				fflush(stdin);
				scanf("%i",&(*p+cont)->C);
				
				if((*p+cont)->C<1 or (*p+cont)->C>1000)
					printf("\n\n!!ERROR: Valor de la corriente inválido.\n\n");
				
			}while((*p+cont)->C<1 or (*p+cont)->C>1000);
		}
		
		
	}else{
		
		/*En este else trabajo el caso en el que el usuario ya ingresó dátos y quiere ingeresar nuevos o borrar los que ya tenía*/
		
		int DatRest=30, Opc3;
		
		DatRest=DatRest-CantDat;
		
		Opc3=valorInv;
		Opc2=valorInv;
		
		printf("\n\nCantidad de maquinas cargadas previamente: %i\n\n",CantDat);
		
		printf("	Opciones:\n		1 - Cargar más datos(%i espacios restantes).\n		2 - Borrar todos los datos previos.",DatRest);
		
		do{
			fflush(stdin);
			scanf("%i",&Opc3);
			
			if(Opc3<1 or Opc3>2){
				
				printf("!!ERROR: Opción inválida, pulse 1 o 2");
				
			}
			
		}while(Opc3<1 or Opc3>2);
		
		switch(Opc3){
			
		case 1:{
			
			int CantDatNuev=0;
			
			printf("\n\nCúantos dátos quiere agregar?\n\n	Cantidad:  ");
			
			do{
				
				fflush(stdin);
				scanf("%i",&CantDatNuev);
					
				if(CantDatNuev<1 or CantDatNuev>DatRest)
					printf("\n\n!!ERROR: debe ingresar 1 dato al menos y no más de %i\n\n",DatRest);
				
				
			}while(CantDatNuev<1 or CantDatNuev>DatRest);
			
			*p = (struct dato *)realloc(*p, (CantDat+CantDatNuev) * sizeof(struct dato));
			
			for(cont=CantDat; cont<(CantDat+CantDatNuev) ;cont++){
				
				fflush(stdin);
				printf("\nIngrese el nombre de la maquina %i:\n",(cont+1));
				
				gets((*p+cont)->NombreMaquina);
				
				//puts((ArrDat+i)->NombreMaquina);
				
				printf("\nIngrese el valor de la tensión de esa maquina:\n");
				
				scanf("%i",&(*p+cont)->T);
				//printf("%i",(ArrDat+i)->T);
				
				printf("\nIngrese el valor de la corriente de esa maquina:\n");
				
				scanf("%i",&(*p+cont)->C);
				//printf("%i",(ArrDat+i)->C);
				
			}
			
			CantDat=CantDat+CantDatNuev;
			break;
		}
		case 2:{
			
			free(*p);
			*p=NULL;
			printf("Se eliminaron los datos correctamente");
			CantDat=0;
			
			FILE *Maquinas;
			Maquinas= fopen("Maquinas.txt","wb");
			
			if(Maquinas==NULL)
				printf("\n\n!!ERROR: No se pudo crear, abrir o editar el archivo solicitado\n\n");
			
			fclose(Maquinas);
		}
		}
	}
	
	return CantDat;


}

void ordena_T(struct dato *p,int n){
	
	struct dato aux;
	
	int cont1,cont2;
	
	for(cont1=0;cont1<n-1;cont1++){
		
		for(cont2=0;cont2<n-1;cont2++){
			
			if((p+cont2)->T  <  (p+cont2+1)->T){
				
				aux= *(p+cont2);
				*(p+cont2)=*(p+cont2+1);
				*(p+cont2+1)=aux;
				
			}
			
		}
		
	}
	
	
	
	
}
void ordena_C(struct dato *p,int n){
	
	struct dato aux;
	
	int i,j;
	
	for(i=0;i<n-1;i++){
		
		for(j=0;j<n-1;j++){
			
			if((p+j)->C  <  (p+j+1)->C){
				
				aux= *(p+j);
				*(p+j)=*(p+j+1);
				*(p+j+1)=aux;
				
			}
			
		}
		
	}
	
	
	
	
}
