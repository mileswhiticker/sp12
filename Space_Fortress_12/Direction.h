#ifndef DIRECTION_H
#define DIRECTION_H

/*
NORTH 1		//+Z	UNIT_Z
SOUTH 2		//-Z	NEGATIVE_UNIT_Z
EAST 4		//+X	UNIT_X
WEST 8		//-X	NEGATIVE_UNIT_X
UP 16		//+Y	UNIT_Y
DOWN 32		//-Y	NEGATIVE_UNIT_Y
*/

#define MINDIR NORTH
#define NORTH 1		//+Z
#define SOUTH 2		//-Z
#define EAST 4		//+X
#define WEST 8		//-X
#define UP 16		//+Y
#define DOWN 32		//-Y
#define MAXDIR DOWN
#define ALLDIRS (NORTH|SOUTH|EAST|WEST|UP|DOWN)

#endif	DIRECTION_H