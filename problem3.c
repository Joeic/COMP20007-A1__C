/*
problem3.c

Driver function for Problem 3.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

/* Constants */
#define OLDCHIP 0
#define NEWCHIP 1
#define MAXNUMERATOR 100
#define MAXDENOMINATOR 100

/* Used to store all the statistics for a single chip. */
struct statistics;

/* Used to store all the statistics for both chips for each problem. */
struct chipStatistics;

struct statistics {
  int operations;
  int instances;
  int minOperations;
  double avgOperations;
  int maxOperations;
};

struct chipStatistics {
  struct statistics oldChipEuclid;
  struct statistics newChipEuclid;
  struct statistics oldChipSieve;
  struct statistics newChipSieve;
};

/* Set all statistics to 0s */
void initialiseStatistics(struct statistics *stats);

/* Collects the minimum, average and maximum operations from running all
combinations of numerators from 1 to the given maxNumerator and 1 to the given
maxDenominator. */
void collectStatistics(struct chipStatistics *chipStats, int maxNumerator,
  int maxDenominator);

/* Divides the number of operations by the number of instances. */
void calculateAverage(struct statistics *stats);

/* Prints out the minimum, average and maximum operations from given
statistics. */
void printStatistics(struct statistics *stats);

/* Calculates the number of operations required for Euclid's algorithm given the
numerator and denominator when running on the given chip type (one of OLDCHIP
and NEWCHIP) by moving through the steps of the algorithm and counting each
pseudocode operation. */
void euclid(int numerator, int denominator, int chip, struct statistics *s);

/* Calculates the number of operations required for the sieve of Eratosthenes
given the numerator and denominator when running on the given chip type (one of
OLDCHIP and NEWCHIP) by moving through the steps of the algorithm and counting
each pseudocode operation. */
void eratosthenes(int numerator, int denominator, int chip,
  struct statistics *s);

int min(int a, int b);

int main(int argc, char **argv){
  struct chipStatistics summaryStatistics;

  collectStatistics(&summaryStatistics, MAXNUMERATOR, MAXDENOMINATOR);

  printf("Old chip (Euclid):\n");
  printStatistics(&(summaryStatistics.oldChipEuclid));
  printf("\n");
  printf("New chip (Euclid)\n");
  printStatistics(&(summaryStatistics.newChipEuclid));
  printf("\n");
  printf("Old chip (Sieve)\n");
  printStatistics(&(summaryStatistics.oldChipSieve));
  printf("\n");
  printf("New chip (Sieve)\n");
  printStatistics(&(summaryStatistics.newChipSieve));
  printf("\n");

  return 0;
}

void collectStatistics(struct chipStatistics *chipStats, int maxNumerator,
  int maxDenominator){
  int numerator, denominator;
  /* Initialise all statistics */
  initialiseStatistics(&(chipStats->oldChipEuclid));
  initialiseStatistics(&(chipStats->newChipEuclid));
  initialiseStatistics(&(chipStats->oldChipSieve));
  initialiseStatistics(&(chipStats->newChipSieve));

  for(numerator = 1; numerator <= maxNumerator; numerator++){
    for(denominator = 1; denominator <= maxDenominator; denominator++){
      /* Run algorithms for all combinations of numerator and denominator. */
      euclid(numerator, denominator, OLDCHIP,
        &(chipStats->oldChipEuclid));
      euclid(numerator, denominator, NEWCHIP,
        &(chipStats->newChipEuclid));
      eratosthenes(numerator, denominator, OLDCHIP,
        &(chipStats->oldChipSieve));
      eratosthenes(numerator, denominator, NEWCHIP,
        &(chipStats->newChipSieve));
    }
  }
  calculateAverage(&(chipStats->oldChipEuclid));
  calculateAverage(&(chipStats->newChipEuclid));
  calculateAverage(&(chipStats->oldChipSieve));
  calculateAverage(&(chipStats->newChipSieve));
}

void calculateAverage(struct statistics *stats){
  stats->avgOperations = (double) stats->operations / stats->instances;
}

void initialiseStatistics(struct statistics *stats){
  stats->operations = 0;
  stats->instances = 0;
  stats->minOperations = INT_MAX;
  stats->avgOperations = 0;
  stats->maxOperations = 0;
}

/* actual euclid code not commented - just follows spec's pseudocode */
void 
euclid(int numerator, int denominator, int chip, struct statistics *s){
	int b = numerator, a = denominator, temp;
	int counter=2; /* initial two assignments +2 */
	
	counter++; /* counting for the final (failed) while +1 */
	while (b != 0){
		counter++; /* while loop +1 */
		
		temp = b;
		counter++; /* assignment +1*/
		
		b = a % b;
		counter++; /* assignment +1 */
		counter+=5; /* modulus +5 */
		
		a = temp;
		counter++; /* assignment +1 */
	}

	counter += 10; /* final 2 divisions before output +10 */
	
	s->instances += 1;
	s->operations += counter;
	
	if (s->maxOperations < counter){
		s->maxOperations = counter;
	} 
	if (s->minOperations > counter){
		s->minOperations = counter;
	}
}

/* actual eratosthenes code not commented - just follows spec's pseudocode */
void 
eratosthenes(int numerator, int denominator, int chip,
  struct statistics *s){
	int counter = 3;
	int numCandidates = min(numerator, denominator), i, j;
	int primes[numCandidates+1];
	
	for (i=1; i<=numCandidates; i++){
		primes[i] = 1;
	}
	counter++; /* assigning for the whole array +1 */
		
	i = 1;
	counter++; /* assigning +1 */
	
	counter++; /* final (failed) while +1 */
	while (i < numCandidates){
		counter++; /* while loop +1 */
		
		i++;
		counter++; /* assigning after addition +1 */
		
		counter++; /* evaluating an if - for below +1 */
		if (primes[i]){
			j=i+i;
			if (chip == OLDCHIP) {
				/* part of line 13 so only needed for old chip */
				counter++; /* assigning the j +1 */
			}
				
			counter++; /* final (failed) while +1 AND ALSO the only cost
			addition for whole line 13 for new chip */
			while (j <= numCandidates){
				if (chip == OLDCHIP) {
					/* only for old chip! */
					counter++; /* evaluating one while loop above +1 */
					counter++; /* evaluating an if for below +1 */
					counter += 5; /* evaluating a division +5 */
					counter += 5; /* evaluating a mod +5 */
				} 
				if (j/i > 1 && j%i == 0) {
					primes[j] = 0;
					if (chip == OLDCHIP){
						counter++; /* assigning +1 but only for old chip */
						counter++; /* j+=i assign below +1 but only for old
										chip */
					}
				}
				j += i;
				
			}
			
			counter++; /* final (failed) while below +1 */
			counter+=5; /* final mod (failed) while below +5 */
			counter+=5; /* final mod (failed) while below +5 */
			while (numerator % i == 0 && denominator % i == 0){
				counter++; /* evaluating while +1 */
				counter+=5; /* evaluating a division +5 */
				counter+=5; /* evaluating a mod +5 */

				numerator = numerator/i;
				counter++; /* assigning +1 */
				counter+=5; /* evaluating a division +5 */
				
				denominator = denominator/i;
				counter++; /* assigning +1 */
				counter+=5; /* evaluating a mod +5 */
			}
		}
	}
	
	s->instances += 1;
	s->operations += counter;
	
	if (s->maxOperations < counter){
		s->maxOperations = counter;
	} 
	if (s->minOperations > counter){
		s->minOperations = counter;
	}
}

/* helper function to return minimum of two numbers */
int
min(int a, int b){
	if (a > b){
		return b;
	} else {
		return a;
	}
}

void printStatistics(struct statistics *stats){
  printf("Minimum operations: %d\n", stats->minOperations);
  printf("Average operations: %f\n", stats->avgOperations);
  printf("Maximum operations: %d\n", stats->maxOperations);
}

