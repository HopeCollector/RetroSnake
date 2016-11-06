#include "AStar.h"

extern INT NEIGHBORE[4][2] = {
	0, 1,
	0, -1,
	1, 0,
	-1, 0
};

BOOL Compare(CELL * cell1, CELL * cell2, POINT ptGoal/*, POINT ptC1Comfrom, POINT ptC2Comfrom*/)
{/*1.�ҳ������پ�����С��
   2. ��1.�Ҳ����������ҳ�������С��_____����Ȳ���
   3. ��2.�Ҳ����������ҳ������յ������*/
	BOOL flag = FALSE;
	INT imDistance_C1, imDistance_C2;
	//INT iCost_C1, iCost_C2;
	FLOAT fDistance_C1, fDistance_C2;

	imDistance_C1 = abs(cell1->pt.x - ptGoal.x) + abs(cell1->pt.y - ptGoal.y) + cell1->iDistance;
	imDistance_C2 = abs(cell2->pt.x - ptGoal.x) + abs(cell2->pt.y - ptGoal.y) + cell2->iDistance;
	if (imDistance_C1 < imDistance_C2)
		flag = TRUE;
	else {
		fDistance_C1 = Distance(cell1->pt, ptGoal);
		fDistance_C2 = Distance(cell2->pt, ptGoal);
		if (fDistance_C1 < fDistance_C2)
			flag = TRUE;
	}
	return flag;
}

VOID FindPath(POINT ptStart, POINT ptGoal, BOOL Unavalible(POINT* Body, INT BodyLength, POINT pt), POINT* Body, INT BodyLength, POINT* ptReturn)
{
	CELL CellHeap[MAX_WORLD * MAX_WORLD];
	CELL Old_CellHeap[MAX_WORLD * MAX_WORLD];
	CELL nowProcessCell;
	CELL* cell =(CELL*) malloc(sizeof(CELL));
	INT iMaxChild = 0, iOldHeapLength = 0;
	INT i, j = 1;
	POINT ptNext;
	FLOAT fDistance_Start;
	BOOL flag = FALSE;

	CellHeap->bIsJudged = TRUE;
	CellHeap->iDistance = 0;
	CellHeap->pt = ptStart;
	CellHeap->ptComfrom = ptStart;
	iMaxChild++;

	while (iMaxChild != 0)
	{
		nowProcessCell = CellHeap[--iMaxChild];
		Old_CellHeap[iOldHeapLength++] = CellHeap[iMaxChild];
		if (nowProcessCell.pt.x == ptGoal.x && nowProcessCell.pt.y == ptGoal.y)
		{
			flag = TRUE;
			break;
		}

		for (i = 0; i < 4; i++)
		{
			ptNext.x = nowProcessCell.pt.x + NEIGHBORE[i][0];
			ptNext.y = nowProcessCell.pt.y + NEIGHBORE[i][1];
			if (!Unavalible(Body, BodyLength, ptNext))
			{
				cell->pt = ptNext;
				fDistance_Start = (FLOAT)(abs(NEIGHBORE[i][0]) + abs(NEIGHBORE[i][1]) + nowProcessCell.iDistance);
				GetCellByPt(CellHeap, iMaxChild, ptNext, cell);
				if (cell->iDistance == -1)
				{
					GetCellByPt(Old_CellHeap, iOldHeapLength, ptNext, cell);
					if (cell->iDistance == -1)
						cell->pt = ptNext;
				}
				else {
					cell->pt = ptNext;
				}
				if					(!cell->bIsJudged)
				{
					cell->bIsJudged = TRUE;
					cell->iDistance = (INT)fDistance_Start;
					cell->ptComfrom = nowProcessCell.pt;

					CellHeap[iMaxChild++] = *cell;
					HeapSort(CellHeap, iMaxChild, ptGoal, Compare);
				}
				else if (fDistance_Start < cell->iDistance)
				{
					cell->iDistance = (INT)fDistance_Start;
					cell->ptComfrom = nowProcessCell.pt;

					HeapSort(CellHeap, iMaxChild, ptGoal, Compare);
				}
			}
		}
	}
	if (flag)
	{
		GetCellByPt(Old_CellHeap, iOldHeapLength, ptGoal, cell);
		do {
			ptReturn[j++] = cell->pt;
			GetCellByPt(Old_CellHeap, iOldHeapLength, cell->ptComfrom, cell);
		} while (cell->pt.x != ptStart.x || cell->pt.y != ptStart.y);

		ptReturn[0].x = --j;
		ptReturn[0].y = -1;
	}
	else
	{
		ptReturn[0].x = -1;
	}
	free(cell);
}

VOID FindFarPoint(POINT ptCenter, POINT ptGoal, POINT* ptReturn, BOOL Unavalible(POINT* Body, INT BodyLength, POINT pt), POINT* Body, INT BodyLength)
{//����һ��С����
	INT i, j;
	INT iMax_Index = 0;
	INT LogicBodyLength;
	BOOL flag[4] = {0};
	FLOAT fMax_Distance = 99999;
	FLOAT fDistance[4];
	POINT ptNext;
	POINT Path[MAX_WORLD * MAX_WORLD];
	POINT LogicTail;
	POINT LogicBody[MAX_WORLD * MAX_WORLD];

	for (i = 0; i < 4; i++)
	{
		ptNext.x = ptCenter.x + NEIGHBORE[i][0];
		ptNext.y = ptCenter.y + NEIGHBORE[i][1];
		
		if (Body[BodyLength - 1].x != 0 && Body[BodyLength - 1].y != 0)
		{
			LogicBodyLength = BodyLength - 1;
			if (Unavalible(Body, LogicBodyLength, ptNext))
			{
				fDistance[i] = -Distance(ptNext, ptGoal);
				continue;
			}
			else
				fDistance[i] = Distance(ptNext, ptGoal);
		}
		else//����ǸճԵ�ʳ��ͱ�����������ԭ��
		{
			LogicBodyLength = BodyLength - 2;
			if (Unavalible(Body, LogicBodyLength + 1, ptNext))
			{
				fDistance[i] = -Distance(ptNext, ptGoal);
				continue;
			}
			else
				fDistance[i] = Distance(ptNext, ptGoal);

		}
		for (j = 0; j < BodyLength - 1; j++)
			LogicBody[j + 1] = Body[j];
		LogicBody[0] = ptNext;		
		FindPath(ptNext, Body[LogicBodyLength], Unavalible, LogicBody, LogicBodyLength + 1, Path);
		if (Path->x != -1)
			flag[i] = TRUE;
		/*for (j = 0; j < 4; j++)//�ж��ܷ��ҵ�β��
		{
			LogicTail.x = Body[BodyLength - 2].x + NEIGHBORE[j][0];
			LogicTail.y = Body[BodyLength - 2].y + NEIGHBORE[j][1];
			if (!Unavalible(Body, BodyLength - 1, LogicTail))
			{
				FindPath(ptNext, LogicTail, Unavalible, Body, BodyLength, Path[j]);
				if (Path[j]->x != -1)
				{
					flag[i] = TRUE;
				}
			}
		}*/
		//FindPath(ptNext, Body[BodyLength - 2], Unavalible, Body, BodyLength, Path[i]);
		//if (Path[i]->x != -1) //�ж��ܷ��ҵ�β��
			//flag = TRUE;		
	}
	if (flag[0] ||
		flag[1] ||
		flag[2] ||
		flag[3])//������ҵ�β��
	{
		for (i = 0; i < 4; i++)
		{
			if (flag[i] == TRUE)//�����ҵ�β�͵ĵ����ҵ���ʳ����Զ��
			{
				if (fDistance[i] >= 0)
				{
					if (fMax_Distance == 99999)//ΪMax����ֵ
					{
						fMax_Distance = fDistance[i];
						iMax_Index = i;
					}
					else if (fMax_Distance < fDistance[i])
					{
						fMax_Distance = fDistance[i];
						iMax_Index = i;
					}
				}
				else
					continue;
			}
		}
	}
	else {//����Ҳ���β�ͣ�����Щ�����ҵ���ʳ����Զ�ĵ�
		for (i = 0; i < 4; i++)
		{
			if (fDistance[i] >= 0)
			{
				if (fMax_Distance == 99999)//ΪMax����ֵ
				{
					fMax_Distance = fDistance[i];
					iMax_Index = i;
				}
				else if (fMax_Distance < fDistance[i])
				{
					fMax_Distance = fDistance[i];
					iMax_Index = i;
				}
			}
			else
				continue;
		}
	}

	ptReturn[0].x = 1;
	ptReturn[0].y = -2;
	ptReturn[1].x = ptCenter.x + NEIGHBORE[iMax_Index][0];
	ptReturn[1].y = ptCenter.y + NEIGHBORE[iMax_Index][1];
}

FLOAT Distance(POINT pt1, POINT pt2)
{
	return sqrt(pow(pt1.x - pt2.x, 2) + pow(pt1.y - pt2.y, 2));
}