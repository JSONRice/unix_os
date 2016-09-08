#ifndef _CONTROL_PROBE_H_
#define _CONTORL_PROBE_H_
#include "messages.h"

#define MAXROW      24 
#define MAXCOL      80
#define MINROW      0
#define MINCOL      0
#define WHITE       0   
#define GREY        1
#define BLACK       2

typedef enum {
 	WALL = '#',
 	PATH = '.',
	UNKNOWN = 0
} LOCTYPE;

typedef enum {
	TRUE		= 1,
	FALSE		= 0
} BOOLEAN;

typedef struct coordinate {
    int row;
    int col;
} Coordinate;


typedef struct vertex {
    Coordinate loc;
    struct vertex *edges[4];
    struct vertex *next;
    int visited;
    int depth;
    struct vertex *parent;
} Vertex;

typedef struct graph {
    Vertex *head;
} Graph;

int getResponse(PROBE_MESSAGES *);
void sendMessage(PROBE_MESSAGES *);
Vertex *findMinDepth();
void addNeighbors(Vertex *);
BOOLEAN isNeighbor(Coordinate, Coordinate);
void initializeMap();
BOOLEAN isInBound(Coordinate);
int moveBack(Vertex *);
Vertex * getPath(Vertex *);
DIRECTIONS findDirection(Coordinate, Coordinate);
BOOLEAN getNextDirection(DIRECTIONS *);
Coordinate updateLocation(DIRECTIONS, Coordinate);
int getNumUnvisited(Coordinate);
Vertex * addVertex(Coordinate);
Vertex * findVertex(Vertex *, Coordinate);
void addEdge(Vertex **, Vertex *);
void freeGraph();
void dfs(Vertex *);
void initializeGraph();
void finalCleanUp();
void printStat();
BOOLEAN isWall(Coordinate loc);

#endif
