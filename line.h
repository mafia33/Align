/*
 * Copyright ©2016 STEP CO.,LTD
 * Author Larry.Liu
 */
#ifndef GUARD_LINE_H
#define GUARD_LINE_H

#include "tool.h"

#define LINE_LEFT_LIMIT             500
#define LINE_RIGHT_LIMIT            0
#define LINE_MAX_PER_CARDBOARD      4
#define LINE_MINIMAL_DISTANCE       1550

typedef struct LINE_STRU {
	int Coordinate;
	int Active;
} LINE;

int GetLineTotalWidth(CARDBOARD);
int GetLineNum(CARDBOARD);
int GetTotalLineNum(CARDBOARD*, int);
void LineInput2Coordinate(CARDBOARD*, AREA*, int*);
int GetGroupSize(int, int);
void GroupLine(int*, int, int, int*, int);
int GetLineAreaNum(int);
void CalLineArea(int*, int, AREA*, int);
int GetValidLineAreaNum(const AREA*, int);
void SortLineArea(AREA*, int, int*, int);
void CalLineCoordinate(AREA*, int, LINE*, int, int*, int);

#endif
