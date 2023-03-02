//Project:		 Finding the way by the method of deepening
//Name:			"Tasks on graphs"
//Author:		 Svetlichnyi.S.S
//Date of
//development:	 15 June, 2022

#pragma once

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QVector>
#include "ui_ApGKurs.h"
#include "Path.h"
#include "PaintWidget.h"
#include "CurrentPath.h"

class ApGKurs : public QMainWindow
{
    Q_OBJECT

public:
    ApGKurs(QWidget *parent = Q_NULLPTR);

private:
    QVector<QVector<int>> _matrix;
    QVector<Path> _results;
    QVector<CurrentPath> _pathes;
    PaintWidget* _paintWidget;
    Ui::ApGKursClass ui;
    void ShowMessage(QString);
    QVector<int> SplitMatrixLine(QString);
    bool ValidateMatrix();
    void PrintMatrix(QVector<QVector<int>> matrix);
    bool GetStart(int& start);
    bool GetStartEnd(int& start, int& end);
 
private slots:
    void OnReadFile();
    void OnListSelectionChanged();
    void OnFindShortestWay();
    void OnFindAllWays();
    void OnFindAllWaysShort();
    void OnFindNoWay();
};
