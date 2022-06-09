#pragma once
#include "Path.h"
#include <QVector>

#define INFINITE 0x0fffffff

void ShortestWay(QVector<QVector<int>>& matrix, int start, int end, QVector<int>& weight, QVector<bool>& visited);
void FindPathes(QVector<QVector<int>>& matrix, int start, int end, QVector<Path>& pathes);