#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAXSIZE 8
#define NUM_THREADS 4

int Vet[MAXSIZE] = { 1, 12 , 4 , 5 , 2 , 6 , 8 , 7};
int count = 0;


//utilização do algoritmo padrao de merge, com algumas modificações para se adequar ao pedido
void merge(int comeco, int meio, int fim){
    int i=0, j=0;
    int ind1 = 0; // indice para a primeira metade
    int ind2 = 0; // indice da segunda metade
    int k = comeco; // indice para unir 
    int tam1 = meio - comeco + 1;
    int tam2 = fim - meio;
 
//vetores auxiliares temporarios    
    int VetAuxL[tam1], VetAuxR[tam2];
 
//salvando os numeros nos vetores

    for (i = 0; i < tam1; i++){
        VetAuxL[i] = Vet[comeco + i];
    }
    for (j = 0; j < tam2; j++){
        VetAuxR[j] = Vet[meio + 1 + j];
    }
 

   
    
    int k = comeco; // indice para a uniao
    
    while (ind1 < tam1 && ind2 < tam2) {
        if (VetAuxL[ind1] <= VetAuxR[ind2]) {
            Vet[k] = VetAuxL[ind1];
            ind1++;
        }
        else {
            Vet[k] = VetAuxR[ind2];
            ind2++;
        }
        k++;
    }
 
//copiando o resto dos elementos da esquerda e da direita que nao entraram na condição passada
    while (ind1 < tam1) {
        Vet[k] = VetAuxL[ind1];
        ind1++;
        k++;
    }

    while (ind2 < tam2) {
        Vet[k] = VetAuxR[ind2];
        ind2++;
        k++;
    }
}
 
// implementação padrão do merge_sort
void merge_sort(int comeco, int fim){
    int meio = comeco + (fim - comeco) / 2;
    if (comeco < fim) {
        merge_sort(comeco, meio);
        merge_sort(meio + 1, fim);
        merge(comeco, meio, fim);
    }
}
 

//Função que utilizara as threads dividindo para que cada uma trabalhe com um lado
//utilizando o id da thread como elemento para fazer uma rotação entre elas
void* merge_sort_thread(void* threadid)
{

    int tid = *((int*)threadid);
    int low = tid * (MAXSIZE / 2);
    int high = (tid + 1) * (MAXSIZE / 2) - 1;
 
    
    int meio = low + (high - low) / 2;
    
    if (low < high) {
        
        merge_sort(low, meio);                         //Juntando as subdivisoes
        merge_sort(meio + 1, high);
        merge(low, meio, high);
    }
    pthread_exit(NULL);
}
 
int main(){
    int i; pthread_t threads[NUM_THREADS];int *threadid[NUM_THREADS];

    //criação padrao de threads seguindo o valor NUM_thread
    for(i = 0; i<NUM_THREADS; i++){                                               //Utilizar o id para deixar a escolha do lado aleatoria
        threadid[i]= (int*) malloc(sizeof(int));
        *threadid[i] = i;
        pthread_create(&threads[i],NULL, merge_sort_thread,(void*)threadid[i]);
        
    }
    for(i = 0; i<NUM_THREADS; i++){
        pthread_join(threads[i],NULL);
    }
 
    
    //juntando a parte de uma thread a outra
    merge(0,(MAXSIZE - 1)/2,MAXSIZE - 1);
    for(int i=0;i<MAXSIZE;i++){
        printf("%d ",Vet[i]);
    }
    printf("\n");

    pthread_exit(NULL);
}
    