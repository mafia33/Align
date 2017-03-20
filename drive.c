/*
 * Copyright ©2015 STEP CO.,LTD
 * Author Larry.Liu
 */
#include <stdio.h>
#include <stdlib.h>
#include "tool.h"
#include "line.h"

int main() {
	/* test data */
	/*
	{4000,    1,    {2000,  1000,  0,  0}},
	{5500,    1,    {3000,  2000,  0,  0}}


	*/
	CARDBOARD cardAandB[CARDBOARD_TYPE_NUM] = {
		{4000,    1,    {2000,  1000,  0,  0}},
		{5500,    1,    {3000,  2000,  0,  0}}
	};
	/* tool test, OK */
	int i;
	int CardboardNum;
	int ToolAreaNum;
	AREA ToolArea[6];
	int ValidToolAreaNum;
	int AreaIndex[6];
	TOOL ToolInfo[TOOL_NUM] = {(0, 0), (0, 0), (0, 0), (0, 0), (0, 0)};

	/* line test */
	int LineCoordinate[8] = {0};
	int LineTotalNum = 0;
	int LineGroup[4] = {0};
	int MiddleGroupSize;
	int MiddleGroupAreaNum;
	AREA LineArea[5];
	int ValidMiddleLineAreaNum;
	int LineAreaIndex[5];
	LINE MiddleLineInfo[LINE_MAX] = {(0, 0), (0, 0), (0, 0), (0, 0)};

	int BackGroupSize;
	int BackGroupAreaNum;
	int ValidBackLineAreaNum;
	LINE BackLineInfo[LINE_MAX] = {(0, 0), (0, 0), (0, 0), (0, 0)};

	printf("----------------Tool alinement test----------------");
	CardboardNum = GetCardboardNum(cardAandB, CARDBOARD_TYPE_NUM);
	printf("\nNumber of cardboard: %d", CardboardNum);

	ToolAreaNum = GetToolAreaNum(CardboardNum);
	printf("\nNumber of tool area: %d", ToolAreaNum);
	
	CalToolArea(cardAandB, CARDBOARD_TYPE_NUM, ToolArea, ToolAreaNum);
	printf("\nRight border of tool area : ");
	for (i = 0; i < ToolAreaNum; i++) {
		printf("%d ", ToolArea[i].Right);
	}
	printf("\nWidth of tool area: ");
	for (i = 0; i < ToolAreaNum; i++) {
		printf("%d ", ToolArea[i].Width);
	}
	
	ValidToolAreaNum = GetValidToolAreaNum(ToolArea, ToolAreaNum);
	printf("\nNumber of valid tool area: %d", ValidToolAreaNum);
	
	SortToolArea(ToolArea, ToolAreaNum, AreaIndex, ValidToolAreaNum);
	printf("\nValid tool area sorted(index): ");
	for (i = 0; i < 4; i++) {
		printf("%d ", AreaIndex[i]);
	}

	CalToolCoordinate(ToolArea, ToolAreaNum, ToolInfo, TOOL_NUM, AreaIndex, ValidToolAreaNum);
	printf("\nTool coordinate: ");
	for (i = 0; i < TOOL_NUM; i++) {
		printf("%d ", ToolInfo[i].Coordinate);
	}

	printf("\nTool selected: ");
	for (i = 0; i < TOOL_NUM; i++) {
		printf("%d ", ToolInfo[i].Active);
	}

	/* line test */
	printf("\n");
	printf("----------------Line alinement test----------------");
	for (i = 0; i < CARDBOARD_TYPE_NUM; i++) {
		printf("\nCardboard %c line total width: %d ", 'A'+i, GetLineTotalWidth(cardAandB[i]));
	}

	for (i = 0; i < CARDBOARD_TYPE_NUM; i++) {
		printf("\nCardboard %c line number: %d ", 'A'+i, GetLineNum(cardAandB[i]));
	}

	LineTotalNum = GetTotalLineNum(cardAandB, CARDBOARD_TYPE_NUM);
	printf("\nTotal line number: %d ", LineTotalNum);

	/* Middle line test */
	printf("\n");
	printf("----------------Middle line alinement test----------------");
	printf("\nMiddle line coordinate: ");
	LineInput2Coordinate(cardAandB, ToolArea, LineCoordinate);
	MiddleGroupSize = GetGroupSize(LineTotalNum, 0);
	GroupLine(LineCoordinate, LineTotalNum, 0, LineGroup, MiddleGroupSize);
	for (i = 0; i < MiddleGroupSize; i++) {
		printf("%d ", LineGroup[i]);
	}

	MiddleGroupAreaNum = GetLineAreaNum(MiddleGroupSize);
	printf("\nNumber of middle line area: %d", MiddleGroupAreaNum);
	
	CalLineArea(LineGroup, MiddleGroupSize, LineArea, MiddleGroupAreaNum);
	printf("\nRight border of middle line area : ");
	for (i = 0; i < MiddleGroupAreaNum; i++) {
		printf("%d ", LineArea[i].Right);
	}
	printf("\nWidth of middle line area: ");
	for (i = 0; i < MiddleGroupAreaNum; i++) {
		printf("%d ", LineArea[i].Width);
	}

	ValidMiddleLineAreaNum = GetValidLineAreaNum(LineArea, MiddleGroupAreaNum);
	printf("\nNumber of valid middle line area: %d", ValidMiddleLineAreaNum);

	SortLineArea(LineArea, MiddleGroupAreaNum, LineAreaIndex, ValidMiddleLineAreaNum);
	printf("\nValid middle line area sorted(index): ");
	for (i = 0; i < 3; i++) {
		printf("%d ", LineAreaIndex[i]);
	}

	CalLineCoordinate(LineArea, MiddleGroupAreaNum, MiddleLineInfo, LINE_MAX, LineAreaIndex, ValidMiddleLineAreaNum);
	printf("\nMiddle line coordinate: ");
	for (i = 0; i < LINE_MAX; i++) {
		printf("%d ", MiddleLineInfo[i].Coordinate);
	}

	printf("\nMiddle line selected: ");
	for (i = 0; i < LINE_MAX; i++) {
		printf("%d ", MiddleLineInfo[i].Active);
	}

	/* Back line test */
	printf("\n");
	printf("----------------Back line alinement test----------------");
	printf("\nBack line coordinate: ");
	BackGroupSize = GetGroupSize(LineTotalNum, 1);
	GroupLine(LineCoordinate, LineTotalNum, 1, LineGroup, BackGroupSize);
	for (i = 0; i < BackGroupSize; i++) {
		printf("%d ", LineGroup[i]);
	}

	BackGroupAreaNum = GetLineAreaNum(BackGroupSize);
	printf("\nNumber of back line area: %d", BackGroupAreaNum);
	
	CalLineArea(LineGroup, BackGroupSize, LineArea, BackGroupAreaNum);
	printf("\nRight border of back line area : ");
	for (i = 0; i < BackGroupAreaNum; i++) {
		printf("%d ", LineArea[i].Right);
	}
	printf("\nWidth of back line area: ");
	for (i = 0; i < BackGroupAreaNum; i++) {
		printf("%d ", LineArea[i].Width);
	}

	ValidBackLineAreaNum = GetValidLineAreaNum(LineArea, BackGroupAreaNum);
	printf("\nNumber of valid back line area: %d", ValidBackLineAreaNum);

	SortLineArea(LineArea, BackGroupAreaNum, LineAreaIndex, ValidBackLineAreaNum);
	printf("\nValid back line area sorted(index): ");
	for (i = 0; i < 3; i++) {
		printf("%d ", LineAreaIndex[i]);
	}

	CalLineCoordinate(LineArea, BackGroupAreaNum, BackLineInfo, LINE_MAX, LineAreaIndex, ValidBackLineAreaNum);
	printf("\nBack line coordinate: ");
	for (i = 0; i < LINE_MAX; i++) {
		printf("%d ", BackLineInfo[i].Coordinate);
	}

	printf("\nBack line selected: ");
	for (i = 0; i < LINE_MAX; i++) {
		printf("%d ", BackLineInfo[i].Active);
	}
}
