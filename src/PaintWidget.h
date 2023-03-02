//Project:		 Finding the way by the method of deepening
//Name:			"Tasks on graphs"
//Author:		 Svetlichnyi.S.S
//Date of
//development:	 15 June, 2022

#pragma once

#include <QWidget>
#include "Path.h"
#include "ui_PaintWidget.h"

class PaintWidget : public QWidget
{
	Q_OBJECT

public:
	PaintWidget(QWidget *parent = Q_NULLPTR);
	~PaintWidget();
	void SetMatrix(QVector<QVector<int>>& matrix);
	void SetPath(Path* path);
	void ClearPath();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QPoint GetVertexPoint(QPoint center, int radius, int vertexIndex);
	void PaintGraph(QPainter& painter);
	void PaintPath(QPainter& painter);
	void DrawVertex(QPainter& painter, QPoint center, int radius, int vertex);
	void DrawBone(QPainter& painter, QPoint pt1, QPoint pt2, int weight);

	Ui::PaintWidget ui;
	QVector<QVector<int>> _matrix;
	Path* _path = nullptr;
};
