#pragma once

#include "Cell.h"
#include <math.h>
#include <stdlib.h>

#define MAX_WORLD 10

VOID HeapSort(CELL *cHead, INT iMaxChild, POINT ptGoal, BOOL CompareMethod(CELL * cell1, CELL * cell2, POINT ptGoal));
VOID Heapify(CELL *cHead, INT iNode, INT iMaxChild, POINT ptGoal, BOOL CompareMethod(CELL * cell1, CELL * cell2, POINT ptGoal));
VOID BulidHeap(CELL *cHead, INT iMaxChild, POINT ptGoal, BOOL CompareMethod(CELL * cell1, CELL * cell2, POINT ptGoal));
VOID swap(CELL* cell1, CELL* cell2);