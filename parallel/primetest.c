#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#define MAXNUM 100
#define MAXSTR 50
#define MAXRESULTS 1000
#define CHUNK 32
#define NUM_THREADS 16

char firstHalf[MAXNUM][MAXSTR];
char secondHalf[MAXNUM][MAXSTR];
long int result[MAXRESULTS];
int numPrimes=0;

int isprime(long int value) {
  long int root;
  long int factor=2;
  int prime=1;
  
  root = sqrtl(value);
  
  while ((factor<=root) && (prime)) {
    prime = fmod((double)value,(double) factor) > 0.0;
		factor++;
  }

  return prime;
}

void quicksort(long int *primes,int first,int last) {
  int i, j, pivot;
  long int temp;

  if (first<last) {
    pivot=first;
    i=first;
    j=last;
    
    while(i<j) {
      while (primes[i]<=primes[pivot]&&i<last) 
        i++;

      while(primes[j]>primes[pivot])
        j--;

      if (i<j) {
        temp=primes[i];
        primes[i]=primes[j];
        primes[j]=temp;
      }
    }

    temp=primes[pivot];
    primes[pivot]=primes[j];
    primes[j]=temp;

    quicksort(primes,first,j-1);
    quicksort(primes,j+1,last);
  }
}

int main(int argc,char** argv) {
  FILE *primesFile;
  int i=0, j=0, numResults=0;

  primesFile=stdin;

  fscanf(primesFile,"%d\n",&numPrimes);

  for (i=0;i<numPrimes;i++)
    fscanf(primesFile,"%s\n",firstHalf[i]);

  for (i=0;i<numPrimes;i++)
    fscanf(primesFile,"%s\n",secondHalf[i]);

  fclose(primesFile);

  // ---- Paralelização ----
  //
  // O laço duplo (i,j) é 100% independente entre iterações, não existe nenhuma dependência de dados entre pares diferentes
  //
  //   1) Variáveis globais: `strToTest` e `primeToTest`
  //      Com várias threads escrevendo nelas ao mesmo tempo via strcpy/strcat, poderia criar um cenário de condição de corrida, impactando na corretude do algortimo
  //      Por isso passou a ser declarado dentro do laço, para que cada thread enxergue somente a sua cópia.
  //
  //   2) Condição de corrida para `result[numResults++] = ...`
  //      O incremento de `numResults` não é atômico, por isso cada thread deve acumular seus próprios primos encontrados num buffer privado (localResult)
  //      e só juntar tudo no result[] global ao final da região paralela.
  //
  // O schedule(dynamic, CHUNK) foi escolhido porque isprime() gera uma carga de trabalho desigual por índice (problema visto em prova, similar ao sleep(i))

  #pragma omp parallel num_threads(NUM_THREADS)
  {
    // Buffer privado desta thread para os primos que ela encontrar.
    // Cada thread tem sua própria instância dessas três variáveis.
    int localCapacity = 64;
    int localCount = 0;
    long int *localResult = malloc(localCapacity * sizeof(long int));

    #pragma omp for collapse(2) schedule(dynamic, CHUNK)
    for (i=0;i<numPrimes;i++)
      for (j=0;j<numPrimes;j++) {
        char strToTest[MAXSTR];
        long int primeToTest;

        strcpy(strToTest,firstHalf[i]);
        strcat(strToTest,secondHalf[j]);
        primeToTest=atol(strToTest);

        if (isprime(primeToTest)) {
          if (localCount==localCapacity) {
            localCapacity*=2;
            localResult=realloc(localResult,localCapacity*sizeof(long int));
          }
          localResult[localCount++]=primeToTest;
        }
      }

    // Merge: cada thread despeja seu buffer privado no result[] global.
    // Acontece uma vez por thread, não por resultado, então o overhead de sincronização é baixo.
    #pragma omp critical
    {
      int k;
      for (k=0;k<localCount;k++)
        result[numResults++]=localResult[k];
    }

    free(localResult);
  }

  quicksort(result,0,numResults-1);
  
  for (i=0;i<numResults;i++)
    printf("%ld\n",result[i]);	  

  return 0;
}