/* defines useful characters for the chess program */

// board coordinate related defs
#define ASC_A		65
#define ASC_H		72
#define ASC_1		49
#define ASC_8		56
#define START		0
#define FIN		8

#define _RANK 		0
#define _FILE		1
#define LOC_SIZE	2

// chess pieces codes (only used to initialise/reset ChessBoard)
#define	BK		"\u265A"
#define	BQ		"\u265B"
#define	BR		"\u265C"
#define	BB		"\u265D"
#define	BN		"\u265E"
#define	BP		"\u265F"
#define	WK		"\u2654"
#define	WQ		"\u2655"
#define	WR		"\u2656"
#define	WB		"\u2657"
#define	WN		"\u2658"
#define	WP		"\u2659"

// event codes
#define CONTINUE		0
#define INVALID_COORDINATES	1
#define DEAL_WITH_CHECK		2
#define INVALID_MOVE		3
#define NO_PIECE		4
#define KING_SIDE		5
#define QUEEN_SIDE		6
#define NO_CASTLE		7
#define EN_PASSANT		8
#define NO_EN_PASSANT		9
#define BAD_CASTLE		10
#define WRONG_TURN		11

// rook positions for castling
#define K_ROOK_START		7
#define Q_ROOK_START		0
#define K_ROOK_CASTLE		5
#define Q_ROOK_CASTLE		3
