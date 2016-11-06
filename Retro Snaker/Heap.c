#include "Heap.h"

VOID Heapify(CELL * cHead, INT iNode, INT iMaxChild, POINT ptGoal, BOOL CompareMethod(CELL * cell1, CELL * cell2, POINT ptGoal))
{//CompareMethod要保证输入的第一个值大的时候能返回FALSE
	INT iLeftIndex;
	CELL cSwap;

	cSwap = cHead[iNode];
	iLeftIndex = iNode * 2 + 1;
	while (iLeftIndex < iMaxChild)
	{
		if (iLeftIndex + 1 < iMaxChild &&
			CompareMethod(&cHead[iLeftIndex + 1], &cHead[iLeftIndex], ptGoal))
		{
			iLeftIndex++;
		}
		if (CompareMethod(&cHead[iNode], &cHead[iLeftIndex], ptGoal))
		{
			break;
		}
		cHead[iNode] = cHead[iLeftIndex];
		cHead[iLeftIndex] = cSwap;
		iNode = iLeftIndex;
		iLeftIndex = 2 * iNode + 1;
	}
}

VOID BulidHeap(CELL * cHead, INT iMaxChild, POINT ptGoal, BOOL CompareMethod(CELL * cell1, CELL * cell2, POINT ptGoal))
{
	INT i;
	for (i = (iMaxChild % 2 == 1 ? iMaxChild + 1 : iMaxChild) / 2 - 1; i >= 0; i--)
	{
		Heapify(cHead, i, iMaxChild, ptGoal, CompareMethod);
	}
}

VOID HeapSort(CELL * cHead, INT iMaxChild, POINT ptGoal, BOOL CompareMethod(CELL * cell1, CELL * cell2, POINT ptGoal))
{
	INT i;
	BulidHeap(cHead, iMaxChild, ptGoal, CompareMethod);
	for (i = iMaxChild - 1; i >= 0; i--)
	{
		swap(&cHead[0], &cHead[i]);
		Heapify(cHead, 0, i, ptGoal, CompareMethod);
	}
}

VOID swap(CELL * cell1, CELL * cell2)
{
	CELL cSwap = *cell1;
	*cell1 = *cell2;
	*cell2 = cSwap;
}