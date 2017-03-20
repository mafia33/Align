/*
 * Copyright ©2015 STEP CO.,LTD
 * Author Larry.Liu
 */
#include "tool.h"

static void Process1Cardboard(AREA*, int, TOOL*, int, int*, int);
static void Process2Cardboard(AREA*, int, TOOL*, int, int*, int);
static void Process3Cardboard(AREA*, int, TOOL*, int, int*, int);
static void Process4Cardboard(AREA*, int, TOOL*, int, int*, int);

int GetCardboardNum(const CARDBOARD* Cardboard, int CardboardTypeNum) {
	int CardboardNum = 0;
	int i;
	for (i = 0; i < CardboardTypeNum; i++) {
		CardboardNum += Cardboard[i].Num;
	}
	return CardboardNum;
}

int GetToolAreaNum(int CardboardNum) {
	return CardboardNum + 2;
}

void CalToolArea(const CARDBOARD* Cardboard, int CardboardTypeNum, AREA* ToolArea, int ToolAreaNum) {
	int WidthTotal = 0;
	int ValidAreaWidth = TOOL_MINIMAL_DISTANCE << 1;//Valid Area: one more tool could be located in
	int i, j;
	for (i = 0; i < CardboardTypeNum; i++) {
		WidthTotal += Cardboard[i].Width * Cardboard[i].Num;
	}
	ToolArea[0].Right  = (RACK_LENGTH - WidthTotal) >> 1;
	ToolArea[0].Width = ToolArea[0].Right;
	ToolArea[0].SizeLevel = ToolArea[0].Right >= ValidAreaWidth ? 0 : -1;
	ToolArea[0].Valid = ToolArea[0].Width >= ValidAreaWidth ? 1 : 0;
	for (i = 0; i < CardboardTypeNum; i++) {
		for (j = 0; j < Cardboard[i].Num; j++) {
			ToolArea[i + j + 1].Right = ToolArea[i + j].Right + Cardboard[i].Width;
			ToolArea[i + j + 1].Width = Cardboard[i].Width;
			ToolArea[i + j + 1].SizeLevel = Cardboard[i].Width >= ValidAreaWidth ? 0 : -1;
			ToolArea[i + j + 1].Valid = Cardboard[i].Width >= ValidAreaWidth ? 1 : 0;
		}
	}
	ToolArea[ToolAreaNum - 1].Right  = RACK_LENGTH;
	ToolArea[ToolAreaNum - 1].Width = ToolArea[ToolAreaNum - 1].Right - ToolArea[ToolAreaNum - 2].Right;
	ToolArea[ToolAreaNum - 1].SizeLevel = ToolArea[ToolAreaNum - 1].Right - ToolArea[ToolAreaNum - 2].Right >= ValidAreaWidth ? 0 : -1;
	ToolArea[ToolAreaNum - 1].Valid = ToolArea[ToolAreaNum - 1].Width >= ValidAreaWidth ? 1 : 0;
}

int GetValidToolAreaNum(const AREA* ToolArea, int ToolAreaNum) {
	int ValidToolAreaNum = 0;
	int i;
	for (i = 0; i < ToolAreaNum; i++) {
		if (ToolArea[i].Valid == 1)
			ValidToolAreaNum++;
	}
	return ValidToolAreaNum;
}

void SortToolArea(AREA* ToolArea, int ToolAreaNum, int* AreaIndex, int ValidToolAreaNum) {
	int i, j, k;
	int MaxAreaWidth;
	for (j = 0; j < ValidToolAreaNum; j++) {
		for (i = 0; i < ToolAreaNum; i++) {
			if (ToolArea[i].SizeLevel == 0)
			{
				MaxAreaWidth = ToolArea[i].Width;
				k = i;
				break;
			}
		}
		for (i = 0; i < ToolAreaNum; i++) {
			if (ToolArea[i].SizeLevel == 0 && ToolArea[i].Width > MaxAreaWidth) {
				MaxAreaWidth = ToolArea[i].Width;
				k = i;
			}
		}
		ToolArea[k].SizeLevel = j + 1;
		AreaIndex[j] = k;
	}
}

void Process1Cardboard(AREA* ToolArea, int ToolAreaNum, TOOL* ToolInfo, int ToolNum, int* AreaIndex, int ValidToolAreaNum) {
	if (ValidToolAreaNum == 3) {
		int AreaRatio = ToolArea[AreaIndex[0]].Width / ToolArea[AreaIndex[1]].Width;
		int i = 0, j = 0;
		if (AreaRatio >= 3) {
			while (i < ToolNum) {
				if (AreaIndex[0] == i) {
					int temp = ToolArea[AreaIndex[0]].Width >> 2;
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp * 3);
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
					ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
				}
				else {
					ToolInfo[i].Coordinate = ToolArea[j].Right;
				}
				ToolInfo[i].Active = 1;
				i++;
				j++;
			}
		}
		else if (AreaRatio == 2) {
			while (i < ToolNum) {
				if (AreaIndex[0] == i) {
					int temp = ToolArea[AreaIndex[0]].Width / 3;
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
					ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
				}
				else if (AreaIndex[1] == i) {
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[1]].Right - (ToolArea[AreaIndex[1]].Width >> 1);
					ToolInfo[i].Coordinate = ToolArea[AreaIndex[1]].Right;
				}
				else {
					ToolInfo[i].Coordinate = ToolArea[j].Right;
				}
				ToolInfo[i].Active = 1;
				i++;
				j++;
			}
		}
		else if (AreaRatio == 1) {
			int AreaRatio1 = ToolArea[AreaIndex[0]].Width / ToolArea[AreaIndex[2]].Width;
			if (AreaRatio1 == 1) {
				while (i < ToolNum) {
					if (AreaIndex[0] == i) {
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (ToolArea[AreaIndex[0]].Width >> 1);
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
					}
					else if (AreaIndex[1] == i) {
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[1]].Right - (ToolArea[AreaIndex[1]].Width >> 1);
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[1]].Right;
					}
					else if (AreaIndex[2] == i) {
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[2]].Right - (ToolArea[AreaIndex[2]].Width >> 1);
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[2]].Right;
					}
					else {
						ToolInfo[i].Coordinate = ToolArea[j].Right;
					}
					ToolInfo[i].Active = 1;
					i++;
					j++;
				}
			}
			else {
				while (i < ToolNum) {
					if (AreaIndex[0] == i) {
						int temp = ToolArea[AreaIndex[0]].Width / 3;
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
					}
					else if (AreaIndex[1] == i) {
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[1]].Right - (ToolArea[AreaIndex[1]].Width >> 1);
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[1]].Right;
					}
					else {
						ToolInfo[i].Coordinate = ToolArea[j].Right;
					}
					ToolInfo[i].Active = 1;
					i++;
					j++;
				}
			}
		}
	}
	else if (ValidToolAreaNum == 2) {
		int AreaRatio = ToolArea[AreaIndex[0]].Width / ToolArea[AreaIndex[1]].Width;
		int i = 0, j = 0;
		if (AreaRatio >= 3) {
			while (i < ToolNum) {
				if (AreaIndex[0] == i) {
					int temp = ToolArea[AreaIndex[0]].Width >> 2;
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp * 3);
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
					ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
				}
				else {
					ToolInfo[i].Coordinate = ToolArea[j].Right;
				}
				ToolInfo[i].Active = 1;
				i++;
				j++;
			}
		}
		else {
			while (i < ToolNum) {
				if (AreaIndex[0] == i) {
					int temp = ToolArea[AreaIndex[0]].Width / 3;
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
					ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
				}
				else if (AreaIndex[1] == i) {
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[1]].Right - (ToolArea[AreaIndex[1]].Width >> 1);
					ToolInfo[i].Coordinate = ToolArea[AreaIndex[1]].Right;
				}
				else {
					ToolInfo[i].Coordinate = ToolArea[j].Right;
				}
				ToolInfo[i].Active = 1;
				i++;
				j++;
			}
		}
	}
	else if (ValidToolAreaNum == 1) {
		int i = 0, j = 0;
		while (i < ToolNum) {
			if (AreaIndex[0] == i) {
				int temp = ToolArea[AreaIndex[0]].Width >> 2;
				ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp * 3);
				ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
				ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
				ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
			}
			else {
				ToolInfo[i].Coordinate = ToolArea[j].Right;
			}
			ToolInfo[i].Active = 1;
			i++;
			j++;
		}
	}
}

void Process2Cardboard(AREA* ToolArea, int ToolAreaNum, TOOL* ToolInfo, int ToolNum, int* AreaIndex, int ValidToolAreaNum) {
	if (ValidToolAreaNum >= 2) {
		int AreaRatio = ToolArea[AreaIndex[0]].Width / ToolArea[AreaIndex[1]].Width;
		int i, j;
		if (AreaRatio >= 2) {
			for (i = 0, j = 0; i < ToolNum; i++, j++) {
				if (AreaIndex[0] == i) {
					int temp = ToolArea[AreaIndex[0]].Width / 3;
					if (ToolArea[AreaIndex[0]].Right != RACK_LENGTH) {
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
					}
					else {
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
					}	
				}
				else {
					ToolInfo[i].Coordinate = ToolArea[j].Right;
				}
				ToolInfo[i].Active = 1;
			}
		}
		else if (AreaRatio == 1) {
			for (i = 0, j = 0; i < ToolNum; i++, j++) {
				if (AreaIndex[0] == j) {
					if (ToolArea[AreaIndex[0]].Right != RACK_LENGTH) {
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (ToolArea[AreaIndex[0]].Width >> 1);
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
					}
					else {
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right - (ToolArea[AreaIndex[0]].Width >> 1);
					}
				}
				else if (AreaIndex[1] == j) {
					if (ToolArea[AreaIndex[1]].Right != RACK_LENGTH) {
						ToolInfo[i++].Coordinate = ToolArea[AreaIndex[1]].Right - (ToolArea[AreaIndex[1]].Width >> 1);
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[1]].Right;
					}
					else {
						ToolInfo[i].Coordinate = ToolArea[AreaIndex[1]].Right - (ToolArea[AreaIndex[1]].Width >> 1);
					}
				}
				else {
					ToolInfo[i].Coordinate = ToolArea[j].Right;
				}
				ToolInfo[i].Active = 1;
			}
		}
	}
	else if (ValidToolAreaNum == 1) {
		int i, j;
		for (i = 0, j = 0; i < ToolNum; i++, j++) {
			if (AreaIndex[0] == i) {
				int temp = ToolArea[AreaIndex[0]].Width / 3;
				if (ToolArea[AreaIndex[0]].Right != RACK_LENGTH) {
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
					ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
				}
				else {
					ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (temp << 1);
					ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right - temp;
				}
			}
			else {
				ToolInfo[i].Coordinate = ToolArea[j].Right;
			}
			ToolInfo[i].Active = 1;
		}
	}
}

void Process3Cardboard(AREA* ToolArea, int ToolAreaNum, TOOL* ToolInfo, int ToolNum, int* AreaIndex, int ValidToolAreaNum) {
	int i = 0, j = 0;
	while (i < ToolNum) {
		if (AreaIndex[0] == i) {
			ToolInfo[i++].Coordinate = ToolArea[AreaIndex[0]].Right - (ToolArea[AreaIndex[0]].Width >> 1);
			ToolInfo[i].Coordinate = ToolArea[AreaIndex[0]].Right;
		}
		else {
			ToolInfo[i].Coordinate = ToolArea[j].Right;
		}
		ToolInfo[i].Active = 1;
		i++;
		j++;
	}
}

void Process4Cardboard(AREA* ToolArea, int ToolAreaNum, TOOL* ToolInfo, int ToolNum, int* AreaIndex, int ValidToolAreaNum) {
	int i, j = 0;
	for (i = 0; i < ToolNum; i++, j++) {
		ToolInfo[i].Coordinate = ToolArea[j].Right;
		ToolInfo[i].Active = 1;
	}
}

void CalToolCoordinate(AREA* ToolArea, int ToolAreaNum, TOOL* ToolInfo, int ToolNum, int* AreaIndex, int ValidToolAreaNum) { 
	switch (ToolAreaNum) {
	case 3:
		Process1Cardboard(ToolArea, ToolAreaNum, ToolInfo, ToolNum, AreaIndex, ValidToolAreaNum);
		break;
			
	case 4:
		Process2Cardboard(ToolArea, ToolAreaNum, ToolInfo, ToolNum, AreaIndex, ValidToolAreaNum);
		break;
		
	case 5:
		Process3Cardboard(ToolArea, ToolAreaNum, ToolInfo, ToolNum, AreaIndex, ValidToolAreaNum);
		break;
			
	case 6:
		Process4Cardboard(ToolArea, ToolAreaNum, ToolInfo, ToolNum, AreaIndex, ValidToolAreaNum);
		break;
		
	default:
		break;
	}
}
