/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"

#define INITIALEDGES 32

struct edge;

/* Helper function to update edges if a new lower cost edge for that vertice 
exist */
void updateLYNKED(struct graph *g, int curr, struct edge *LYNKED[]);

/* Helper function to initialise an array of "struct edge pointers" */
void initLYNKEDLINKED(struct graph *g, int LINKED[], 
		struct edge *LYNKED[]);

/* Helper function to feed relevent edges into the priority queue */
void enqueupdate(struct graph *g, struct pq *prique, 
		int LINKED[], struct edge *LYNKED[]);

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
  int cost;
};

struct graph *newGraph(int numVertices){
  struct graph *g = (struct graph *) malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost){
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if((g->numEdges + 1) > g->allocedEdges){
    if(g->allocedEdges == 0){
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **) realloc(g->edgeList,
      sizeof(struct edge *) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *) malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;
  newEdge->cost = cost;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
  int i;
  for(i = 0; i < g->numEdges; i++){
    free((g->edgeList)[i]);
  }
  if(g->edgeList){
    free(g->edgeList);
  }
  free(g);
}

/* Helper function which finds the solution to the problems */
struct solution *graphSolve(struct graph *g, enum problemPart part,
  int antennaCost, int numHouses){
  	
	/* shared variables between two problems */
    int LINKED[g->numVertices];
    int counter = 0, tally = 0, curr=0, i;
    struct pq *prique;
    struct edge *LYNKED[g->numVertices];
    struct edge *temp;
    
    /* variabels unique to part c */
    int addantenna = 0;
    
	struct solution *solution = (struct solution *)
    	malloc(sizeof(struct solution));
    assert(solution);
    
    if(part == PART_A){
    	/* IMPLEMENT 2A SOLUTION HERE */ 	
    	
    	/* initialise both arrays which help keep track */
    	initLYNKEDLINKED(g, LINKED, LYNKED);
    	
    	/* use mechanism to find shortest path to each edge, and then select
    	shortest path overall (prims' algorithm using given data structures) */
    	while (counter < g->numVertices-1){
			updateLYNKED(g, curr, LYNKED);
			
			prique = newPQ();
			enqueupdate(g, prique, LINKED, LYNKED);
			
			temp = (struct edge*)deletemin(prique);
			LINKED[temp->end] = 0;
			tally += temp->cost;
			curr = temp->end;
			counter++;
			
			free(prique);
		}
		
		/* free all the things that were created inside this function */
		for (i=0; i<g->numVertices; i++){
    		free(LYNKED[i]);
    	}
		
    	/* realy the final results back to the calling function */
    	solution->antennaTotal = numHouses * antennaCost;
    	solution->cableTotal = tally;
    	
    } else {
    	/* IMPLEMENT 2C SOLUTION HERE */
    	
    	/* initialise both arrays which help keep track */
    	initLYNKEDLINKED(g, LINKED, LYNKED);
    	
    	/* use mechanism to find shortest path to each edge, and then select
    	shortest path overall (prims' algorithm using given data structures) */
    	while (counter < g->numVertices-1){
			updateLYNKED(g, curr, LYNKED);

			prique = newPQ();
			enqueupdate(g, prique, LINKED, LYNKED);
			
			temp = (struct edge*)deletemin(prique);
			LINKED[temp->end] = 0;
			curr = temp->end;
			counter++;
			
			/* add to cost only if the path cost less than the antenna, 
			otherwise more cheap to build an antenna at that house and don't 
			build the path (but still keep traverse it as if we are doing
			prim's) */
			if (temp->cost < antennaCost) {
				tally += temp->cost;
			} else {
				addantenna++;
			}
			
			free(prique);
			
		}
		
		/* free all the things that were created inside this function */
		for (i=0; i<g->numVertices; i++){
    		free(LYNKED[i]);
    	}
    	
    	/* realy the final results back to the calling function */
    	solution->mixedTotal = tally + addantenna * antennaCost;
    }
    return solution;
}

/* Helper function to update edges if a new lower cost edge for that vertice 
exist */
void
updateLYNKED(struct graph *g, int curr, struct edge *LYNKED[]){
	int i;
	
	/* walk through all edges, inefficient but does the job */
	for (i=0; i<(g->numEdges); i++){
		/* if either the start or end matches the curr vertex (as if doing 
		prim's algo manually) */
		if (g->edgeList[i]->start == curr || g->edgeList[i]->end == curr){
			/* two diff cases - with 'opposite' actions but ultimately doing
			the same job */
			if (g->edgeList[i]->start == curr){
				/* if current cost is higher than this edge's cost */
				if (g->edgeList[i]->cost < 
						LYNKED[g->edgeList[i]->end]->cost){
				
					LYNKED[g->edgeList[i]->end]->start = curr;
					LYNKED[g->edgeList[i]->end]->end = g->edgeList[i]->end;
					LYNKED[g->edgeList[i]->end]->cost = 
						g->edgeList[i]->cost;
				}
			} else {
				/* if current cost is higher than this edge's cost */
				if (g->edgeList[i]->cost < 
						LYNKED[g->edgeList[i]->start]->cost){
				
					LYNKED[g->edgeList[i]->start]->start = curr;
					LYNKED[g->edgeList[i]->start]->end = 
						g->edgeList[i]->start;
					LYNKED[g->edgeList[i]->start]->cost = 
						g->edgeList[i]->cost;
				}
			}
		}
	}
}

/* Helper function to initialise an array of "struct edge pointers" */
void
initLYNKEDLINKED(struct graph *g, int LINKED[], struct edge *LYNKED[]){	
	int i;
	struct edge *temp;
	
	for (i=0; i<g->numVertices; i++){
   		temp = (struct edge *) malloc(sizeof(struct edge));
   		assert(temp);
   		temp->start = 0;
   		temp->end = 0;
   		temp->cost = INT_MAX; /* so that the first cost is always recorded */
   		LYNKED[i] = temp;
   		LINKED[i] = 1; /* all verticies are yet to be connected */
    }
    LINKED[0] = 0;/*so that the data centre is not considered for connection*/
}

/* Helper function to feed relevent edges into the priority queue */
void
enqueupdate(struct graph *g, struct pq *prique, 
		int LINKED[], struct edge *LYNKED[]){
	
	int i;
	
	/* add all verticies after the current round of prim's into the priority 
	queue so the minimum of them can be outputted later */
	for (i=0; i<g->numVertices; i++){
		if (LINKED[i] == 1){
			enqueue(prique, LYNKED[i], LYNKED[i]->cost);	
		}
	}	
}