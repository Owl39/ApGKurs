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

private:
	QPoint GetVertexPoint(QPoint center, int radius, int vertexIndex);
	void paintEvent(QPaintEvent*);
	void PaintGraph(QPainter& painter);
	void PaintPath(QPainter& painter);
	void DrawVertex(QPainter& painter, QPoint center, int radius, int vertex);

	Ui::PaintWidget ui;
	QVector<QVector<int>> _matrix;
	Path* _path = nullptr;
};
