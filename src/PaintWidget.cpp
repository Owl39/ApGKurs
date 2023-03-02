//Project:		 Finding the way by the method of deepening
//Name:			"Tasks on graphs"
//Author:		 Svetlichnyi.S.S
//Date of
//development:	 15 June, 2022

#include "PaintWidget.h"
#include <QPainter>
#include <QMath.h>
#include <QVector2D>
#include <math.h>

const int VertexRadius = 8;

PaintWidget::PaintWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PaintWidget::~PaintWidget()
{
}

void drawText(QPainter& painter, qreal x, qreal y, Qt::Alignment flags,
	const QString& text, QRectF* boundingRect = 0)
{
	const qreal size = 32767.0;
	QPointF corner(x, y - size);
	if (flags & Qt::AlignHCenter) corner.rx() -= size / 2.0;
	else if (flags & Qt::AlignRight) corner.rx() -= size;
	if (flags & Qt::AlignVCenter) corner.ry() += size / 2.0;
	else if (flags & Qt::AlignTop) corner.ry() += size;
	else flags |= Qt::AlignBottom;
	QRectF rect{ corner.x(), corner.y(), size, size };
	painter.drawText(rect, flags, text, boundingRect);
}

void drawText(QPainter& painter, const QPointF& point, Qt::Alignment flags,
	const QString& text, QRectF* boundingRect = {})
{
	drawText(painter, point.x(), point.y(), flags, text, boundingRect);
}

QPoint PaintWidget::GetVertexPoint(QPoint center, int radius, int vertexIndex) {
	double offset = 360.0 / _matrix.size();
	double angle = qDegreesToRadians(offset * vertexIndex);

	QPoint  pt(center.x() + cos(angle) * radius, center.y() + sin(angle) * radius);
	return pt;
}

void PaintWidget::SetMatrix(QVector<QVector<int>>& matrix) {
	_matrix = matrix;
	ClearPath();
}

void PaintWidget::SetPath(Path* path) {
	_path = path;
}

void PaintWidget::ClearPath() {
	_path = nullptr;
}

void PaintWidget::paintEvent(QPaintEvent* event) {

	QPainter painter(this);

	int width = size().width();
	int height = size().height();

	painter.fillRect(0, 0, width, height, QColor(220, 220, 220));

	PaintGraph(painter);
	PaintPath(painter);
}

void PaintWidget::PaintGraph(QPainter& painter)
{
	int width = size().width();
	int height = size().height();

	int vertexes = _matrix.size();
	QPoint center(width / 2, height / 2);
	int radius = qMin(width, height) * 0.8 / 2;

	// рисование ребер
	painter.setPen(Qt::blue);
	painter.setBrush(Qt::blue);
	for (int i = 0; i < vertexes; i++) {
		for (int j = 0; j < vertexes; j++) {
			if (_matrix[i][j] != 0)
			{
				QPoint pt1 = GetVertexPoint(center, radius, i);
				QPoint pt2 = GetVertexPoint(center, radius, j);

				DrawBone(painter, pt1, pt2, _matrix[i][j]);
			}
		}
	}

	// рисование вершин 
	painter.setBrush(Qt::white);
	painter.setPen(Qt::red);
	for (int i = 0; i < vertexes; i++) {
		DrawVertex(painter, center, radius, i);
	}
}

void PaintWidget::PaintPath(QPainter& painter)
{
	if (_path == nullptr)
		return;

	int width = size().width();
	int height = size().height();

	QPoint center(width / 2, height / 2);
	int radius = qMin(width, height) * 0.8 / 2;

	// рисование ребер
	painter.setPen(QPen(Qt::yellow, 2));
	for (int i = 1; i < _path->line.size(); i++) {
		QPoint pt1 = GetVertexPoint(center, radius, _path->line[i - 1]);
		QPoint pt2 = GetVertexPoint(center, radius, _path->line[i]);
		DrawBone(painter, pt1, pt2, _matrix[_path->line[i - 1]][_path->line[i]]);
	}

	// рисование вершин 
	painter.setBrush(Qt::white);
	painter.setPen(QPen(Qt::green, 2));
	for (int i = 0; i < _path->line.size(); i++) {
		DrawVertex(painter, center, radius, _path->line[i]);
	}

	painter.setBrush(Qt::white);
	painter.setPen(QPen(Qt::blue, 2));
	DrawVertex(painter, center, radius, _path->from);
	DrawVertex(painter, center, radius, _path->to);
}

void PaintWidget::DrawVertex(QPainter& painter, QPoint center, int radius, int vertex) {
	QPoint pt = GetVertexPoint(center, radius, vertex);
	painter.drawEllipse(pt, VertexRadius * 2, VertexRadius * 2);
	QString label = QString::number(vertex + 1);
	drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, label);
}

void PaintWidget::DrawBone(QPainter& painter, QPoint pt1, QPoint pt2, int weight) {
	QVector2D vec((pt2 - pt1).y(), -(pt2 - pt1).x());
	if (vec.y() == 0)
		vec = QVector2D(-(pt2 - pt1).y(), (pt2 - pt1).x());
	vec.normalize();

	QLineF line(pt1 + vec.toPointF() * VertexRadius, pt2 + vec.toPointF() * VertexRadius);
	painter.drawLine(line);

	QTransform tran;
	tran.translate(line.pointAt(0.7f).x(), line.pointAt(0.7f).y());
	float ang = 180 - line.angle();
	tran.rotate(ang);
	painter.setTransform(tran);

	QPointF arPoint[3];
	arPoint[0] = QPoint(0, 0);
	arPoint[1] = QPoint(5, 5);
	arPoint[2] = QPoint(5, -5);
	painter.drawPolygon(arPoint, 3);

	painter.resetTransform();

	QPen pen = painter.pen();
	painter.setPen(Qt::blue);
	QString label = QString::number(weight);
	painter.drawText(line.center(), label);
	painter.setPen(pen);
}
