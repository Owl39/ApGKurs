#include "PaintWidget.h"
#include <QPainter>
#include <QMath.h>
#include <QVector2D>

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

void PaintWidget::paintEvent(QPaintEvent*) {

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
	for (int i = 0; i < vertexes; i++) {
		for (int j = 0; j < vertexes; j++) {
			if (_matrix[i][j] != 0)
			{
				QPoint pt1 = GetVertexPoint(center, radius, i);
				QPoint pt2 = GetVertexPoint(center, radius, j);
				QLine line(pt2, pt1);
				painter.drawLine(pt1, pt2);

				//QVector2D v1(pt1);
				//QVector2D v2(pt2);
				//QVector2D v = v2 - v1;

				QString label = QString::number(_matrix[i][j]);
				painter.drawText(line.center(), label);
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
		QPoint pt1 = GetVertexPoint(center, radius, _path->line[i]);
		QPoint pt2 = GetVertexPoint(center, radius, _path->line[i - 1]);
		QLine line(pt2, pt1);
		painter.drawLine(pt1, pt2);

		//QVector2D v1(pt1);
		//QVector2D v2(pt2);
		//QVector2D v = v2 - v1;

		//QString label = QString::number(_matrix[i][j]);
		//painter.drawText(line.center(), label);
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
	painter.drawEllipse(pt, 15, 15);
	QString label = QString::number(vertex + 1);
	drawText(painter, pt, Qt::AlignVCenter | Qt::AlignHCenter, label);
}

