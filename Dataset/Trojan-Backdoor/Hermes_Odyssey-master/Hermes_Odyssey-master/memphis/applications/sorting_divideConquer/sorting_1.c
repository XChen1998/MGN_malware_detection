/*

                                (0)
                    *(1)                     (2)
                (3)     (4)             (5)     (6)
              (7) (8) (9)(10)        (11)(12) (13)(14)

    Iaçanã Ianiski Weber
    - iacanaw@gmail.com
    06/2019
*/
#include <api.h>
#include <stdlib.h>
#include "sorting.h"

Message msg;

int main(){
    int vector[HALF_VECTOR_SIZE];
    int half1[FOURTH_VECTOR_SIZE];
	int half2[FOURTH_VECTOR_SIZE];
    int i, j, k, aux, tamanho, halfTamanho;

    Echo("Aguardando o vetor...");
    Receive(&msg, sorting_0); // AQUI
    Echo("Recebendo o vetor:");
    Echo("--> Tamanho do vetor:");
    Echo(itoa(msg.length));
    Echo("--> Vetor:");
    tamanho = msg.length; 
    for(i=0; i<tamanho; i++){
        vector[i] = msg.msg[i];
        Echo(itoa(vector[i]));
    }
    Echo("Vetor recebido com sucesso!");

    // Define o primeiro flit contendo o tamanho do vetor que está sendo enviado
	Echo("Iniciando o envio das mensgaens de cada ramo...");
    halfTamanho = tamanho/2;
    aux = 0;
    Echo("Ramo esquerdo:");
    for(i=0; i<halfTamanho; i++){
        msg.msg[i] = vector[aux];
        Echo(itoa(msg.msg[i]));
        aux++;
    }
    msg.length = halfTamanho;
    Send(&msg, sorting_3); // AQUI
    Echo("Ramo direito:");
    for(i=0; i<halfTamanho; i++){
        msg.msg[i] = vector[aux];
        Echo(itoa(msg.msg[i]));
        aux++;
    }
    msg.length = halfTamanho;
    Send(&msg, sorting_4); // AQUI

    // Aguarda o recebimento dos vetores organizados
    Echo("Aguardando recebimento dos ramos:");
    Echo("Ramo esquerdo:");
    Receive(&msg, sorting_3); // AQUI
    for(i=0; i<msg.length; i++){
        half1[i] = msg.msg[i];
        Echo(itoa(msg.msg[i]));
    }
    Echo("Ramo direito:");
    Receive(&msg, sorting_4); // AQUI
    for(i=0; i<msg.length; i++){
        half2[i] = msg.msg[i]; 
        Echo(itoa(msg.msg[i]));
    }

    Echo("Realizando o merge:");
    // Realiza o merge dos vetores
    j = 0;
    k = 0;
    for(i=0; i<tamanho; i++){
        if(j == halfTamanho){
            vector[i] = half2[k];
            k++;
        }
        else if(k == halfTamanho){
            vector[i] = half1[j];
            j++;
        }
        else if(half1[j]<half2[k] ){
            vector[i] = half1[j];
            j++;
        }  
        else{
            vector[i] = half2[k];
            k++;
        }
        // Imprime o vetor organizado
        Echo(itoa(vector[i]));
        msg.msg[i] = vector[i];
    }
    // Retorna o vetor para o pai
    Echo("Retornando o vetor para a raiz:");
    msg.length = tamanho;
    Send(&msg, sorting_0); // AQUI
    
	exit();

}
