/*
 * Copyright ©2016 STEP CO.,LTD
 * Author Larry.Liu
 */
#include "tool.h"
#include "line.h"

static void Process1Line(AREA*, int, LINE*, int, int*, int);
static void Process2Line(AREA*, int, LINE*, int, int*, int);
static void Process3Line(AREA*, int, LINE*, int, int*, int);
static void Process4Line(AREA*, int, LINE*, int, int*, int);

int GetLineTotalWidth(CARDBOARD Cardboard) {
	int LineTotalWidth = 0, i;
	for (i = 0; i < LINE_MAX_PER_CARDBOARD; i++)
		LineTotalWidth += Cardboard.Line[i];
	return LineTotalWidth;
}

int GetLineNum(CARDBOARD Cardboard) {
	int LineNum = 0, i;
	for (i = 0; i < LINE_MAX_PER_CARDBOARD; i++)
		if (Cardboard.Line[i] != 0)
			LineNum++;
	return LineNum;
}

int GetTotalLineNum(CARDBOARD* pCardboard, int CardboardTypeNum) {
	int LineTotalNum = 0, i, LineNum;
	for (i = 0; i < CardboardTypeNum; i++) {
		LineNum = GetLineNum(*(pCardboard + i));
		LineTotalNum += (pCardboard + i)->Num * LineNum;
	}
	return LineTotalNum;
}

void CopyLineInput(CARDBOARD Cardboard, int* LineInput, int LineInputNum) {
	int i, j = 0;
	for (i = 0; i < LineInputNum; i++) {
		if (Cardboard.Line[i] != 0) {
			LineInput[j++] = Cardboard.Line[i];
		}
	}
}

void LineInput2Coordinate(CARDBOARD* pCardboard, AREA* ToolArea, int* LineCoordinate) {
	int LineNum;
	int LineInput[LINE_MAX_PER_CARDBOARD];
	int SumLine = 0;
	int t, i, j, k = 0, n = 0;
	for (t = 0; t < CARDBOARD_TYPE_NUM; t++)
	{
		LineNum = GetLineNum(*(pCardboard + t));
		CopyLineInput(*(pCardboard + t), LineInput, LINE_MAX);
		for (i = 0; i < (pCardboard + t)->Num; i++)
		{
			for (j = 0; j < LineNum; j++)
			{
				SumLine += LineInput[j];
				LineCoordinate[i + j + n] += ToolArea[i + k].Right + SumLine;
			}
			SumLine = 0;
			n = i + j;
		}
		k = i;
	}
}

int GetGroupSize(int LineTotalNum, int GroupType) {
	int GroupSize;
	if (GroupType == 0)
		GroupSize = LineTotalNum % 2 == 0 ? LineTotalNum / 2 : LineTotalNum / 2 + 1;
	else
		GroupSize = LineTotalNum / 2;
	return GroupSize;
}

void GroupLine(int* LineCoordinate, int LineTotalNum, int GroupType, int* LineGroup, int GroupSize) {
	int i, j;
	if (GroupType == 0) {
		for (i = 0, j = 0; i < GroupSize; i++, j += 2) {
			LineGroup[i] = LineCoordinate[j];
		}
	} else {
		for (i = 0, j = 1; i < GroupSize; i++, j += 2) {
			LineGroup[i] = LineCoordinate[j];
		}
	}
}

int GetLineAreaNum(int GroupSize) {
	return GroupSize + 1;
}

void CalLineArea(int* LineGroup, int GroupSize, AREA* LineArea, int LineAreaNum) {
	int PreviousLine = LINE_LEFT_LIMIT;
	int ValidAreaWidth = LINE_MINIMAL_DISTANCE << 1;//Valid Area: one more Line could be located in
	int i;
	for (i = 0; i < GroupSize; i++) {
		LineArea[i].Right = LineGroup[i];
		LineArea[i].Width = LineGroup[i] - PreviousLine;
		PreviousLine = LineGroup[i];
		LineArea[i].SizeLevel = LineArea[i].Width >= ValidAreaWidth ? 0 : -1;
		LineArea[i].Valid = LineArea[i].Width >= ValidAreaWidth ? 1 : 0;
	}
	LineArea[i].Right = RACK_LENGTH;
	LineArea[i].Width = RACK_LENGTH - PreviousLine;
	LineArea[i].SizeLevel = LineArea[i].Width >= ValidAreaWidth ? 0 : -1;
	LineArea[i].Valid = LineArea[i].Width >= ValidAreaWidth ? 1 : 0;
}

int GetValidLineAreaNum(const AREA* LineArea, int LineAreaNum) {
	int ValidLineAreaNum = 0;
	int i;
	for (i = 0; i < LineAreaNum; i++) {
		if (LineArea[i].Valid == 1)
			ValidLineAreaNum++;
	}
	return ValidLineAreaNum;
}

void SortLineArea(AREA* LineArea, int LineAreaNum, int* AreaIndex, int ValidLineAreaNum) {
	int i, j, k;
	int MaxAreaWidth;
	for (j = 0; j < ValidLineAreaNum; j++) {
		for (i = 0; i < LineAreaNum; i++) {
			if (LineArea[i].SizeLevel == 0)
			{
				MaxAreaWidth = LineArea[i].Width;
				k = i;
				break;
			}
		}
		for (i = 0; i < LineAreaNum; i++) {
			if (LineArea[i].SizeLevel == 0 && LineArea[i].Width > MaxAreaWidth) {
				MaxAreaWidth = LineArea[i].Width;
				k = i;
			}
		}
		LineArea[k].SizeLevel = j + 1;
		AreaIndex[j] = k;
	}
}

void Process0Line(LINE* LineInfo, int LineNum) {
	int i;
	int temp = (RACK_LENGTH - LINE_LEFT_LIMIT - LINE_RIGHT_LIMIT) / 5;
	for (i = 0 ; i < LineNum; i++) {
		if (i == 0)
			LineInfo[i].Coordinate = LINE_LEFT_LIMIT + temp;
		else
			LineInfo[i].Coordinate = LineInfo[i - 1].Coordinate + temp;
		LineInfo[i].Active = 0;
	}
}

void Process1Line(AREA* LineArea, int LineAreaNum, LINE* LineInfo, int LineNum, int* AreaIndex, int ValidLineAreaNum) {
	if (ValidLineAreaNum == 2) {
		int AreaRatio = LineArea[AreaIndex[0]].Width / LineArea[AreaIndex[1]].Width;
		int i = 0, j = 0;
		if (AreaRatio >= 3) {
			while (i < LineNum) {
				if (AreaIndex[0] == i) {
					int temp = LineArea[AreaIndex[0]].Width >> 2;
					LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp * 3);
					LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp << 1);
					LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - temp;
					LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right;
					LineInfo[i].Active = 1;
				}
				else {
					LineInfo[i].Coordinate = LineArea[j].Right;
					LineInfo[i].Active = 1;
				}
				//LineInfo[i].Active = 1;
				i++;
				j++;
			}
		}
		else {
			while (i < LineNum) {
				if (AreaIndex[0] == i) {
					int temp = LineArea[AreaIndex[0]].Width / 3;
					LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp << 1);
					LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - temp;
					LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right;
					LineInfo[i].Active = 1;
				}
				else if (AreaIndex[1] == i) {
					LineInfo[i++].Coordinate = LineArea[AreaIndex[1]].Right - (LineArea[AreaIndex[1]].Width >> 1);
					LineInfo[i].Coordinate = LineArea[AreaIndex[1]].Right;
					LineInfo[i].Active = 1;
				}
				else {
					LineInfo[i].Coordinate = LineArea[j].Right;
					LineInfo[i].Active = 1;
				}
				//LineInfo[i].Active = 1;
				i++;
				j++;
			}
		}
	}
	else {
		int i = 0, j = 0;
		while (i < LineNum) {
			if (AreaIndex[0] == i) {
				int temp = LineArea[AreaIndex[0]].Width >> 2;
				LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp * 3);
				LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp << 1);
				LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - temp;
				LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right;
				LineInfo[i].Active = 1;
			}
			else {
				LineInfo[i].Coordinate = LineArea[j].Right;
				LineInfo[i].Active = 1;
			}
			//LineInfo[i].Active = 1;
			i++;
			j++;
		}
	}
}

void Process2Line(AREA* LineArea, int LineAreaNum, LINE* LineInfo, int LineNum, int* AreaIndex, int ValidLineAreaNum) {
	if (ValidLineAreaNum >= 2) {
		int AreaRatio = LineArea[AreaIndex[0]].Width / LineArea[AreaIndex[1]].Width;
		int i, j;
		if (AreaRatio >= 2) {
			for (i = 0, j = 0; i < LineNum; i++, j++) {
				if (AreaIndex[0] == i) {
					int temp = LineArea[AreaIndex[0]].Width / 3;
					if (LineArea[AreaIndex[0]].Right != RACK_LENGTH) {
						LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp << 1);
						LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - temp;
						LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right;
						LineInfo[i].Active = 1;
					}
					else {
						LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp << 1);
						LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right - temp;
					}	
				}
				else {
					LineInfo[i].Coordinate = LineArea[j].Right;
					LineInfo[i].Active = 1;
				}
				//LineInfo[i].Active = 1;
			}
		}
		else if (AreaRatio == 1) {
			for (i = 0, j = 0; i < LineNum; i++, j++) {
				if (AreaIndex[0] == j) {
					if (LineArea[AreaIndex[0]].Right != RACK_LENGTH) {
						LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (LineArea[AreaIndex[0]].Width >> 1);
						LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right;
						LineInfo[i].Active = 1;
					}
					else {
						LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right - (LineArea[AreaIndex[0]].Width >> 1);
					}
				}
				else if (AreaIndex[1] == j) {
					if (LineArea[AreaIndex[1]].Right != RACK_LENGTH) {
						LineInfo[i++].Coordinate = LineArea[AreaIndex[1]].Right - (LineArea[AreaIndex[1]].Width >> 1);
						LineInfo[i].Coordinate = LineArea[AreaIndex[1]].Right;
						LineInfo[i].Active = 1;
					}
					else {
						LineInfo[i].Coordinate = LineArea[AreaIndex[1]].Right - (LineArea[AreaIndex[1]].Width >> 1);
					}
				}
				else {
					LineInfo[i].Coordinate = LineArea[j].Right;
					LineInfo[i].Active = 1;
				}
				//LineInfo[i].Active = 1;
			}
		}
	}
	else if (ValidLineAreaNum == 1) {
		int i, j;
		for (i = 0, j = 0; i < LineNum; i++, j++) {
			if (AreaIndex[0] == i) {
				int temp = LineArea[AreaIndex[0]].Width / 3;
				if (LineArea[AreaIndex[0]].Right != RACK_LENGTH) {
					LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp << 1);
					LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - temp;
					LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right;
					LineInfo[i].Active = 1;
				}
				else {
					LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (temp << 1);
					LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right - temp;
				}
			}
			else {
				LineInfo[i].Coordinate = LineArea[j].Right;
				LineInfo[i].Active = 1;
			}
			//LineInfo[i].Active = 1;
		}
	}
}

void Process3Line(AREA* LineArea, int LineAreaNum, LINE* LineInfo, int LineNum, int* AreaIndex, int ValidLineAreaNum) {
	int i = 0, j = 0;
	while (i < LineNum) {
		if (AreaIndex[0] == i) {
			LineInfo[i++].Coordinate = LineArea[AreaIndex[0]].Right - (LineArea[AreaIndex[0]].Width >> 1);
			LineInfo[i].Coordinate = LineArea[AreaIndex[0]].Right;
			LineInfo[i].Active = 1;
		}
		else {
			LineInfo[i].Coordinate = LineArea[j].Right;
			LineInfo[i].Active = 1;
		}
		//LineInfo[i].Active = 1;
		i++;
		j++;
	}
}

void Process4Line(AREA* LineArea, int LineAreaNum, LINE* LineInfo, int LineNum, int* AreaIndex, int ValidLineAreaNum) {
	int i, j = 0;
	for (i = 0; i < LineNum; i++, j++) {
		LineInfo[i].Coordinate = LineArea[j].Right;
		LineInfo[i].Active = 1;
	}
}

void CalLineCoordinate(AREA* LineArea, int LineAreaNum, LINE* LineInfo, int LineNum, int* AreaIndex, int ValidLineAreaNum) { 
	switch (LineAreaNum) {
	case 2:
		Process1Line(LineArea, LineAreaNum, LineInfo, LineNum, AreaIndex, ValidLineAreaNum);
		break;
			
	case 3:
		Process2Line(LineArea, LineAreaNum, LineInfo, LineNum, AreaIndex, ValidLineAreaNum);
		break;
		
	case 4:
		Process3Line(LineArea, LineAreaNum, LineInfo, LineNum, AreaIndex, ValidLineAreaNum);
		break;
			
	case 5:
		Process4Line(LineArea, LineAreaNum, LineInfo, LineNum, AreaIndex, ValidLineAreaNum);
		break;
		
	default:
		break;
	}
}
