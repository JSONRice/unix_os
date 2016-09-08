#define PROBE_PORT		10000

typedef enum {

	MSG_INSERT_PROBE =	0,
	MSG_MOVE =		1,
	MSG_MV_RESPONSE =	2,
	MSG_EXPLODE =		3,
	MSG_RESULTS =		4,
	MSG_DISCONNECT =	5,
	MSG_PRINT_MAZE =	6,
	MSG_PID =		7,
	MSG_BAD	=		8
} COMMANDS;

#define MAX_MSG_SIZE		256

typedef enum {
	MOVE_NORTH =		0,
	MOVE_SOUTH =		1,
	MOVE_EAST =		2,
	MOVE_WEST =		3
} DIRECTIONS;

typedef enum {
	YES =			1,
	NO =			0
} MOVE_RESPONSES;

typedef struct _msg_pid {
	int pid;
} PROBE_PID_MSGS;

typedef struct _msg_insert {
	int cavern;
} PROBE_INSERT_MSGS;

typedef struct _msg_move {
	DIRECTIONS	move;
} PROBE_MOVE_MSGS;

typedef struct _msg_mv_response {
	MOVE_RESPONSES	success;
} MOVE_RESPONSE_MSGS;

typedef struct _msg_results {
	int	cavern_size;
	int	amt_mapped;
	int	lawsuits;
	int	moves;
} RESULT_MSGS;

typedef struct msg_generic {
	char msg_type;		/* should be of type COMMANDS */
	union {
		RESULT_MSGS		results;
		MOVE_RESPONSE_MSGS	mv_response;
		PROBE_MOVE_MSGS		move;
		PROBE_INSERT_MSGS	insert;
	} msg;
} PROBE_MESSAGES;
