//Project:		 Finding the way by the method of deepening
//Name:			"Tasks on graphs"
//Author:		 Svetlichnyi.S.S
//Date of
//development:	 15 June, 2022

#include "ApGKurs.h"
#include "Algoritms.h"
#include "CurrentPath.h"
#include <iostream>
#include <QVector>


void ShortestWay(QVector<QVector<int>>& matrix, int start, int end, QVector<int>& weight, QVector<bool>& visited)
{
	if (start == end)
		return;

	visited[start] = true;

	for (int i = 0; i < matrix.length(); i++)
	{
		if (matrix[start][i] == 0)
			continue;

		if (weight[start] + matrix[start][i] < weight[i])
			weight[i] = matrix[start][i] + weight[start];
	}

	for (int i = 0; i < matrix.length(); i++)
	{
		if (matrix[start][i] == 0 || visited[i] == true)
			continue;
		ShortestWay(matrix, i, end, weight, visited);
	}
}

void FindPathesRecursive(QVector<QVector<int>>& matrix, int start, int end, Path currentPath, QVector<Path>& pathes, QVector<bool> visited)
{
	
	if (start == end)
	{
		pathes.append(currentPath);
		return;
	}

	visited[start] = true;

	for (int i = 0; i < matrix.length(); i++)
	{
		if (matrix[start][i] == 0 || visited[i] == true)
			continue;

		Path path = currentPath;
		path.line.append(i);
		path.length += matrix[start][i];
		FindPathesRecursive(matrix, i, end, path, pathes, visited);
	}
}

void FindPathes(QVector<QVector<int>>& matrix, int start, int end, QVector<Path>& pathes)
{
	Path currentPath;
	currentPath.from = start;
	currentPath.to = end;
	currentPath.line.append(start);
	QVector<bool> visited(matrix.length(), false);
	FindPathesRecursive(matrix, start, end, currentPath, pathes, visited);
}
