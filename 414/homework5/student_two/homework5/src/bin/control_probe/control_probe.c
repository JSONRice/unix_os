#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "control_probe.h"
#include "socket_utils.h"
#include "shared_mem.h"
#include "log_mgr.h"

/*
 * Name: Student 2
 * Proj: HW #5
 * Date: 
 * Desc: Program explores the cavern by communicating 
 *       with a probe.  It stores the cavern in to 
 *       shared memory.  The program uses displayed
 *       program to show the current map of the cavern.
*/

Graph g;
int sock;

typedef char MAP [2 * MAXROW][2 * MAXCOL];
int shmKey = -1;
MAP *map;
Coordinate currLoc;
PROBE_MESSAGES response;

main(int argc, char *argv[]) {
    int numRead, counter = 0, moveBackResult, backUpResult;
    int c;
    int portNumber = 10000;
    int *cavernNumber = malloc(sizeof (int));
    long oldFlag;
    Coordinate nextLoc;
    Coordinate newLoc;
    BOOLEAN notProcess;
    DIRECTIONS direction = MOVE_NORTH, oldDirection;
    Vertex *curr, *hasUnvisitedVertex;

    PROBE_MESSAGES message;

    /* command line option argument processing */
    opterr = 0;             /* suppress getopt(3) generated messages */
    while ((c = getopt(argc, argv, "m:P:c:")) != -1) {
        switch(c) {
            case 'm':
                shmKey = atoi(optarg); break;
            case 'P':
                portNumber = atoi(optarg); break;
            case 'c':
                cavernNumber = (int *) atoi(optarg); break;
            default:
                printf("c = %d\n", c); break;
        }
    }

    if (shmKey == -1) {
        printf("Usage: control_probe -m <shmKey> -P <portNumber> -c <cavern id>\n");
        log_event(FATAL, "Usage: control_probe -m <shmKey> -P <portNumber> -c <cavern id>");
        exit(1);
    }

    /* doing some initialization work */
    g.head = NULL;

    map = (MAP *)connect_shm(shmKey, sizeof(MAP));

    initializeMap(map);

    if ((sock = setup_client(portNumber)) == ERROR) {
        printf("failed to establish connection.\n");
        log_event(FATAL, "failed to establish connection.");
        exit(ERROR);
    }

    printf("Connection to port (%d) on host (localhost).\n", portNumber);
    log_event(INFO, "Connection to port (%d) on host (localhost).", portNumber);

    message.msg_type = MSG_INSERT_PROBE;
    message.msg.insert.cavern = *cavernNumber;
    sendMessage(&message);
 
    currLoc.row = MAXROW - 1;
    currLoc.col = MAXCOL - 1;

    (*map)[currLoc.row][currLoc.col] = PATH;
    curr = addVertex(currLoc);
 

    printf("Exploring the cavern\n");
    log_event(INFO, "Exploring the cavern");

    /* main loop that explores the cavern */
    do {
        message.msg_type = MSG_MOVE;
        message.msg.move.move = direction;
        sendMessage(&message);
        numRead = getResponse(&response);
       
        newLoc = updateLocation(direction, currLoc);

        /* it's a wall */
        if (response.msg.mv_response.success == NO) {
            (*map)[newLoc.row][newLoc.col] = WALL;
        }

        else {
            (*map)[newLoc.row][newLoc.col] = PATH;
            curr = addVertex(newLoc);
            addNeighbors(curr);
            currLoc = newLoc;
        }

        hasUnvisitedVertex = getPath(curr);
        /* current still has unvisited neighbor */
        if (hasUnvisitedVertex == curr) {
            getNextDirection(&direction);
        }
        /* no more unvisited neighbor, move to next location */
        else if (hasUnvisitedVertex != NULL) {
            moveBackResult = moveBack(hasUnvisitedVertex);
            if (moveBackResult == OK) {
                curr = hasUnvisitedVertex;
                getNextDirection(&direction);
            }
            /* problem with moving back */
            else {
                curr = NULL;
            }
        }
        /* all path has been visited */
        else {
            curr = NULL;
        }

        fcntl(sock, F_GETFL, oldFlag);
        fcntl(sock, F_SETFL, O_NONBLOCK);
        numRead = read(sock, &response, sizeof(response));

        /* handling of MSG_RESULTS */
        if (numRead != -1 && numRead == sizeof(response) && response.msg_type == MSG_RESULTS) { 
            printf("MSG_RESULTS received.\n");
            log_event(FATAL, "MSG_RESULTS received");
            finalCleanUp();
            exit(1);
        }
        fcntl(sock, F_SETFL, oldFlag);
    } while (curr != NULL);


    (*map)[currLoc.row][currLoc.col] = ')';

    message.msg_type = MSG_EXPLODE;
    sendMessage(&message);
    numRead = getResponse(&response);
    printStat();
    finalCleanUp();
}

/* performs the final clean up before the program exits */
void finalCleanUp() {
    close(sock);
    freeGraph();
    destroy_shm(shmKey);
}

void printStat() {    
    int cavernSize = response.msg.results.cavern_size;
    int amtMapped = response.msg.results.amt_mapped;
    int lawsuits = response.msg.results.lawsuits;
    int moves = response.msg.results.moves;
    float movesRatio = ((float) moves) / cavernSize;

    printf("Explored %d out of %d in %d moves (%d lawsuits)\n", amtMapped, cavernSize, moves, lawsuits); 
    printf("Move per space ratio = %.3f\n", movesRatio);
    log_event(INFO, "Explored %d out of %d in %d moves (%d lawsuits)", amtMapped, cavernSize, moves, lawsuits); 
    log_event(INFO, "Move per space ratio = %.3f", movesRatio);
}

BOOLEAN getNextDirection(DIRECTIONS *direction) {
    DIRECTIONS newDirection, oldDirection;
    BOOLEAN found = FALSE;
    int i;

    oldDirection = *direction;

    for(i = 0; i < 4; i++)
    { 
        switch(oldDirection) {
        case MOVE_NORTH:
            newDirection = MOVE_EAST; 
            if ((*map)[currLoc.row][currLoc.col + 1] == UNKNOWN) {
                found = TRUE; 
            }
            break;
        case MOVE_EAST:
            newDirection = MOVE_SOUTH;
            if ((*map)[currLoc.row + 1][currLoc.col] == UNKNOWN) {
                found = TRUE; 
            }
            break;
        case MOVE_SOUTH:
            newDirection = MOVE_WEST;
            if ((*map)[currLoc.row][currLoc.col - 1] == UNKNOWN) {
                found = TRUE; 
            }
            break;
        case MOVE_WEST:
            newDirection = MOVE_NORTH;
            if ((*map)[currLoc.row - 1][currLoc.col] == UNKNOWN) {
                found = TRUE; 
            }
            break;
        default:
            newDirection = MOVE_NORTH; break;
        }

        if (found) {
            break;
        }
        oldDirection = newDirection;
    }

    /* assign if the newdirection is found */
    if (found) {
        *direction = newDirection;
    }

    return found;
}

Coordinate updateLocation(DIRECTIONS direction, Coordinate loc) {
    Coordinate tempLoc = loc;
       
    switch (direction) {
        case MOVE_NORTH: tempLoc.row--; break;
        case MOVE_SOUTH: tempLoc.row++; break;
        case MOVE_EAST:  tempLoc.col++; break;
        case MOVE_WEST:  tempLoc.col--; break;
    }
    return tempLoc;
}

int getResponse(PROBE_MESSAGES *response) {
    int nbytes;
    int running_total = 0;
    int to_read;
    int num_read;

    nbytes = sizeof(PROBE_MESSAGES);
    to_read = nbytes;
    running_total = 0;

    do {
        num_read = read(sock, &(response[running_total]), to_read);
        if (num_read > 0) {
            running_total += num_read;
        }
        to_read = nbytes - running_total;

    } while (running_total < nbytes);

    return nbytes;
}

void sendMessage(PROBE_MESSAGES *message) {
    int             n;
    int             nwritten;
    char           *inet_ntoa();
    int             nbytes;

    if (sock == 0)
        return;

    nbytes = sizeof(struct msg_generic);
    nwritten = 0;
    while (nwritten < nbytes) {
        n = write(sock, message + nwritten, nbytes - nwritten);
        if (n < 0) {
            if (errno == EPIPE) {
                close(sock);
                break;
            }
            printf("nwritten is %d, nbytes is %d", nwritten, nbytes);
            if (errno == EWOULDBLOCK) {
                printf("sleeping");
                sleep(1);
            }
        }
        else {
            nwritten += n;
        }
    }
    if (nwritten != nbytes) {
        printf("send_to_connect: wrote %d bytes of %d byte buffer", nwritten, nbytes);
        log_event(WARNING, "send_to_connect: wrote %d bytes of %d byte buffer", nwritten, nbytes);
    }
}

int moveBack(Vertex *v) {
    long oldFlag;
    int numRead;
    int i, counter, result = OK; 
    int size = v->depth;
    DIRECTIONS direction;
    Vertex *curr;
    Coordinate loc;
    Coordinate wayBack[MAXROW * MAXCOL];

    PROBE_MESSAGES message;

    curr = v;
    counter = size;

    /* reversing the path to the right order */
    while (curr != NULL && counter >= 0) {
        wayBack[counter--] = curr->loc;
        curr = curr->parent;
    }

    message.msg_type = MSG_MOVE;

    if (wayBack[0].row != currLoc.row || wayBack[0].col != currLoc.col) {
        printf("Error with moveBack\n"); 
        log_event(FATAL, "Error with moveBack"); 
        return ERROR;
    }

    for (i = 1; i <= size; i++) {
        direction = findDirection(currLoc, wayBack[i]);
        message.msg.move.move = direction;
        loc = updateLocation(direction, currLoc);
        if (loc.row != wayBack[i].row || loc.col != wayBack[i].col) {
            printf("Error with moveBack\n");
            log_event(FATAL, "Error with moveBack"); 
            return ERROR;
        }
        sendMessage(&message);
        numRead = getResponse(&response);

        /* it's a wall */
        if (response.msg.mv_response.success == NO) {
           result = ERROR;
        }

        else {
            currLoc = loc;
            result = OK;
        }

        fcntl(sock, F_GETFL, oldFlag);
        fcntl(sock, F_SETFL, O_NONBLOCK);
        numRead = read(sock, &response, sizeof(response));
        
        /* handling of MSG_RESULTS */
        if (numRead != -1 && numRead == sizeof(response) && response.msg_type == MSG_RESULTS) { 
            printf("MSG_RESULTS received\n");
            log_event(FATAL, "MSG_RESULTS received");
            finalCleanUp();
            exit(1);
        }
        fcntl(sock, F_SETFL, oldFlag);
    }
    return result;
}

DIRECTIONS findDirection(Coordinate from, Coordinate to) {
    DIRECTIONS direction;
    if (from.row < to.row) {
        direction = MOVE_SOUTH;
    }
    else if (from.row > to.row) {
        direction = MOVE_NORTH;
    }
    else if (from.col < to.col) {
        direction = MOVE_EAST;
    }
    else if (from.col > to.col) {
        direction = MOVE_WEST;
    }
    return direction;
}

Vertex * getPath(Vertex *curr) {
    Vertex *v;
    Vertex *temp;
    Coordinate tempLoc;
    int numUnvisited;

    initializeGraph();

    /* before search for path, make sure all neighbors of curr has been visited */
    numUnvisited = getNumUnvisited(curr->loc);
    if (numUnvisited != 0) {
        v = curr;
    }
    /* all the neighbors of curr vertex have been visited, search for a path */
    else {
        dfs(curr);
        v = findMinDepth();
    }
    return v;
}

void dfs(Vertex *v) {
    int counter = 0;
    Vertex *curr, *neighbor;
    curr = v;
    
    if (curr != NULL && curr->visited == WHITE) {
        curr->visited = GREY;
    }
    /* visiting all the neighbor */
    while (curr != NULL && counter < 4 && (neighbor = curr->edges[counter]) != NULL) {
        /* only continue on if the neighbor has not been visited */
        if (neighbor->visited == WHITE) {
            neighbor->parent = curr;
            neighbor->depth = curr->depth + 1;
            dfs(neighbor);
        }
        counter++;
    }
    curr->visited = BLACK;
}

void addNeighbors(Vertex *curr) {
    Coordinate currLoc;
    Coordinate tempLoc;
    Vertex *v;

    if (curr != NULL) {
        currLoc = curr->loc;
    }

    /* check for neighbor in north*/      
    tempLoc = currLoc;
    tempLoc.row--;
    if (isInBound(tempLoc) == TRUE && isNeighbor(currLoc, tempLoc)) {
        v = findVertex(g.head, tempLoc);
        if (v != NULL) {
            addEdge(&(curr), v);
            addEdge(&v, curr);
        }
    }

    /* check for neighbor in the south */
    tempLoc = currLoc;
    tempLoc.row++;
    if (isInBound(tempLoc) == TRUE && isNeighbor(currLoc, tempLoc)) {
        v = findVertex(g.head, tempLoc);
        if (v != NULL) {
            addEdge(&(curr), v);
            addEdge(&v, curr);
        }
    }

    /* check for neighbor in the east */
    tempLoc = currLoc;
    tempLoc.col++;
    if (isInBound(tempLoc) == TRUE && isNeighbor(currLoc, tempLoc)) {
        v = findVertex(g.head, tempLoc);
        if (v != NULL) {
            addEdge(&(curr), v);
            addEdge(&v, curr);
        }
    }

    /* check for neighbor in the west */
    tempLoc = currLoc;
    tempLoc.col--;
    if (isInBound(tempLoc) == TRUE && isNeighbor(currLoc, tempLoc)) {
        v = findVertex(g.head, tempLoc);
        if (v != NULL) {
            addEdge(&(curr), v);
            addEdge(&v, curr);
        }
    }
}

BOOLEAN isNeighbor(Coordinate a, Coordinate b) {
    int rowDiff;
    int colDiff;

    rowDiff = abs(a.row - b.row);
    colDiff = abs(a.col - b.col);
       
    /* it's only neighbor if the diff between row/col is one and it has been
        visited and not a wall */
    if ((rowDiff == 0 && colDiff == 1) || 
        (rowDiff == 1 && colDiff == 0)) {
        if ((*map)[b.row][b.col] != UNKNOWN &&
            (*map)[b.row][b.col] != WALL) {
            return TRUE;
	}
    }       
    return FALSE;
}

BOOLEAN isWall(Coordinate loc) {
    if ((*map)[loc.row][loc.col] == WALL) 
        return TRUE;
    return FALSE;
}

BOOLEAN isInBound(Coordinate loc) {

    if (loc.row >= 2 * MAXROW || loc.row < MINROW || loc.col >= 2 * MAXCOL || loc.col < MINCOL)
    {
        return FALSE;
    }

    return TRUE;
}

void initializeMap() {
    int row;
    int col;

    for (row = 0; row < 2 * MAXROW; row++) {
        for (col = 0; col < 2 * MAXCOL; col++) {
            (*map)[row][col] = UNKNOWN;
        }
    }
}

Vertex * findMinDepth() {
    int minDepth = -1;
    int numUnvisited = 0;
    int minNumUnvisited = 5;
    Coordinate tempLoc;
    Vertex *curr, *minDepthVertex = NULL;

    curr = g.head;

    while (curr != NULL) {
        /* first time through loop so initialize depth */
        numUnvisited = getNumUnvisited(curr->loc);
        if (numUnvisited != 0) {
            if (curr->depth != 0 && minDepth == -1 && !isWall(curr->loc)) {
                minDepth = curr->depth;
                minDepthVertex = curr;
                minNumUnvisited = numUnvisited;
            }

            /* depth is lesser or equal to minDepth */
            if (curr->depth <= minDepth && curr->depth != 0 && !isWall(curr->loc)) {
                /* to depths are the same, pick one with least unvisited */
                if (curr->depth == minDepth) {
                    if (numUnvisited != 0 && numUnvisited <= minNumUnvisited) {
                        minDepth = curr->depth;
                        minDepthVertex = curr;
                        minNumUnvisited = numUnvisited;
                    }
                }
                else {
                    minDepth = curr->depth;
                    minDepthVertex = curr;
                    minNumUnvisited = numUnvisited;
                }
            }
        }
        curr = curr->next;
    }

    return minDepthVertex;
}

int getNumUnvisited(Coordinate loc) {
    int count = 0;
    Coordinate tempLoc = loc;

    /* look south */
    tempLoc.row++;
    if (isInBound(tempLoc) == TRUE && (*map)[tempLoc.row][tempLoc.col] == UNKNOWN) {
        count++;
    }
    /* look north */
    tempLoc = loc;
    tempLoc.row--;
    if (isInBound(tempLoc) == TRUE && (*map)[tempLoc.row][tempLoc.col] == UNKNOWN) {
        count++;
    }
    /* look east */
    tempLoc = loc;
    tempLoc.col++;
    if (isInBound(tempLoc) == TRUE && (*map)[tempLoc.row][tempLoc.col] == UNKNOWN) {
        count++;
    }
    /* look west */
    tempLoc = loc;
    tempLoc.col--;
    if (isInBound(tempLoc) == TRUE && (*map)[tempLoc.row][tempLoc.col] == UNKNOWN) {
        count++;
    }
    return count;
}

Vertex * addVertex(Coordinate loc) {
    Vertex *v, *curr;
    int i;

    v = (Vertex *) malloc(sizeof(Vertex));

    if (v != NULL) {
        v->next = NULL;
        for (i = 0; i < 4; i++) {
            v->edges[i] = NULL;
        }

        v->parent = NULL;
        v->loc = loc;
        v->depth = 0;
        v->visited = WHITE;
    }

    /* empty list */
    if (g.head == NULL) {
        g.head = v;
    }

    /* add to end of list */
    else {
        curr = g.head;
        while (curr != NULL && curr->next != NULL)
            curr = curr->next;

        curr->next = v;
    }

    return v;
}

/* returns a pointer to the vertex with the passed in location */
Vertex * findVertex(Vertex *head, Coordinate loc) {
    Vertex *curr;

    curr = head;

    while (curr != NULL) {
        if (curr->loc.row == loc.row && curr->loc.col == loc.col) {
            return curr;
        }
        curr = curr->next;
    }

    /* gone through the list and could find the vertex, return null */
    return curr;
}

/* adds an edge to the passed in vertex */
void addEdge(Vertex **sourceVertex, Vertex *connectTo) {

    int counter;

    for (counter = 0; counter < 4; counter++) {
        if ((*sourceVertex)->edges[counter] == NULL) {
            (*sourceVertex)->edges[counter] = connectTo;
            break;
        }
        
    }
}

/* frees the memory allocated to the graph */
void freeGraph() {
    Vertex *curr, *oldCurr;

    curr = g.head;

    while (curr != NULL) {
        oldCurr = curr;
        curr = curr->next;
        free(oldCurr);
    }
}

/* initializes the graph for depth first search */
void initializeGraph() {
    Vertex *curr;
    
    curr = g.head;
    while (curr != NULL) {
        curr->visited = WHITE;
        curr->depth = 0;
        curr->parent = NULL;
        curr = curr->next;
    }
}
