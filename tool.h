/*
 * Copyright ©2015 STEP CO.,LTD
 * Author Larry.Liu
 */
#ifndef GUARD_TOOL_H
#define GUARD_TOOL_H

#define LINE_MAX               4
#define TOOL_NUM			   5
#define RACK_LENGTH            18500
#define TOOL_MINIMAL_DISTANCE  1600
#define CARDBOARD_TYPE_NUM     2

typedef struct CARDBOARD_STRU {
	int Width;
	int Num;
	int Line[LINE_MAX];
} CARDBOARD;

typedef struct AREA_STRU {
	int Right;
	int Width;
	int SizeLevel;
	int Valid;
} AREA;

typedef struct TOOL_STRU {
	int Coordinate;
	int Active;
} TOOL;

int GetCardboardNum(const CARDBOARD*, int);
int GetToolAreaNum(int);
void CalToolArea(const CARDBOARD*, int, AREA*, int);
int GetValidToolAreaNum(const AREA*, int);
void SortToolArea(AREA*, int, int*, int);
void CalToolCoordinate(AREA*, int, TOOL*, int, int*, int);

#endif
