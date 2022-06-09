#include "ApGKurs.h"
#include "Algoritms.h"
#include <QPushButton>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>



ApGKurs::ApGKurs(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//ui.centralWidget->setAutoFillBackground(true);
	//ui.centralWidget->setBackgroundRole(QPalette::Highlight);

	connect(ui.btnReadFile, SIGNAL(clicked()), SLOT(OnReadFile()));
	connect(ui.btnTaskA, SIGNAL(clicked()), SLOT(OnFindShortestWay()));
	connect(ui.btnTaskB, SIGNAL(clicked()), SLOT(OnFindAllWays()));
	connect(ui.btnTaskC, SIGNAL(clicked()), SLOT(OnFindAllWaysShort()));
	connect(ui.btnTaskD, SIGNAL(clicked()), SLOT(OnFindNoWay()));
	connect(ui.listWidget, SIGNAL(itemSelectionChanged()), SLOT(OnListSelectionChanged()));

	_paintWidget = new PaintWidget(ui.centralWidget);
	ui.horizontalLayout_3->addWidget(_paintWidget, 1);

	ui.centralWidget->setLayout(ui.mainVerticalLayout);
}

void ApGKurs::OnReadFile()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);

	QStringList fileNames;
	if (dialog.exec())
	{
		fileNames = dialog.selectedFiles();

		QFile file(fileNames.at(0));

		if (!file.open(QIODevice::ReadOnly)) {
			ShowMessage("Cannot open file for reading.");
			return;
		}
		// Создаем входящий поток, из которого будут считываться данные, и связываем его с нашим файлом
		QTextStream in(&file);

		_matrix.clear();
		// Считываем файл строка за строкой
		while (!in.atEnd()) { // метод atEnd() возвращает true, если в потоке больше нет данных для чтения
			QString line = in.readLine(); // метод readLine() считывает одну строку из потока
			QVector<int> row = SplitMatrixLine(line);
			_matrix.append(row);
		}

		// Закрываем файл 
		file.close();

		if (ValidateMatrix() == false)
			_matrix.clear();

		QString str = PrintMatrix(_matrix);
		_paintWidget->SetMatrix(_matrix);
	}
}

void ApGKurs::ShowMessage(QString message) {
	QMessageBox msgBox;
	msgBox.setText(message);
	msgBox.exec();
}

QVector<int> ApGKurs::SplitMatrixLine(QString line) {
	QVector<int> row;
	QStringList list = line.split(' ');
	for (int i = 0; i < list.length(); i++)
		row.append(list[i].toInt());
	return row;
}

bool ApGKurs::ValidateMatrix() {

	// 1 - матрица не пуста 
	if (_matrix.length() == 0) {
		ShowMessage("Matrix is empty.");
		return false;
	}

	// 2 - все длины строк равны первой 
	for (int i = 0; i < _matrix.length(); i++) {
		if (_matrix[0].length() != _matrix[i].length()) {
			ShowMessage("Raws length isn`t equal.");
			return false;
		}
	}

	// 3 - кол-во строк равно кол-ву столбцов 
	if (_matrix[0].length() != _matrix.length()) {
		ShowMessage("Matrix isn`t square.");
		return false;
	}
	// 4 - главная диагональ нулевая 
	for (int i = 0; i < _matrix.length(); i++) {
		if (_matrix[i][i] != 0) {
			ShowMessage("The main diagonal is non-zero.");
			return false;
		}
	}

	return true;
}

QString ApGKurs::PrintMatrix(QVector<QVector<int>> matrix) {
	QString str;
	QTextStream out(&str);
	ui.tableWidget->setColumnCount(matrix.length());
	ui.tableWidget->setRowCount(matrix.length());

	for (int i = 0; i < matrix.length(); i++) {
		//ui.tableWidget->insertRow(i);
		for (int j = 0; j < matrix.length(); j++) {
			out << matrix[i][j] << "\t";
			ui.tableWidget->setItem(i, j,
				new QTableWidgetItem(QString::number(matrix[i][j])));
		}
		out << "\n";
	}
	return str;
}


void ApGKurs::OnListSelectionChanged() {
	QList<QListWidgetItem*> list = ui.listWidget->selectedItems();
	if (list.size() == 0) {
		_paintWidget->ClearPath();
		_paintWidget->repaint();
	}
	else {
		int index = ui.listWidget->row(list[0]);
		_paintWidget->SetPath(&_results[index]);
		_paintWidget->repaint();
	}
}

// a) найкоротший шлях з А в Б
void ApGKurs::OnFindShortestWay() {

	int start, end;
	if (!GetStartEnd(start, end))
	{
		ShowMessage("Wrong values.");
		return;
	}

	ui.listWidget->clear();
	_results.clear();


	QVector<int> weight(_matrix.length(), INFINITE);
	QVector<bool> visited(_matrix.length(), false);
	//ShortestWay(_matrix, start, end, weight, visited);

	QVector<Path> pathes;
	Path path;
	FindPathes(_matrix, start, end, pathes);
	if (pathes.length() > 0) {
		struct {
			bool operator()(const Path& a, const Path& b) const {
				return a.length < b.length;
			}
		} customLess;
		std::sort(pathes.begin(), pathes.end(), customLess);
		path = pathes[0];
	}
	else {

		path.from = start;
		path.to = end;
	}

	int index = _results.size();
	_results.append(path);
	ui.listWidget->insertItem(index, path.toString());
}


// б) всі можливі шляхи з А в Б
void ApGKurs::OnFindAllWays() {
	ui.listWidget->clear();
	_results.clear();

	int start, end;
	if (!GetStartEnd(start, end)) {
		ShowMessage("Wrong values.");
		return;
	}

	QVector<Path>pathes;
	FindPathes(_matrix, start, end, pathes);

	for (int i = 0; i < pathes.length(); i++) {
		int index = _results.size();
		_results.append(pathes[i]);
		ui.listWidget->insertItem(index, pathes[i].toString());
	}
}

// в) всі міста, не пов'язані між собою дорогами
void ApGKurs::OnFindAllWaysShort() {
	ui.listWidget->clear();
	_results.clear();

	int start;
	if (!GetStart(start)) {
		ShowMessage("Wrong values.");
		return;
	}

	int minLength = INFINITE;
	int endVertex = start;
	for (int i = 0; i < _matrix.length(); i++) {
		if (i == start)
			continue;
		QVector<int> weight(_matrix.length(), INFINITE);
		weight[start] = 0;
		QVector<bool> visited(_matrix.length(), false);
		ShortestWay(_matrix, start, i, weight, visited);
		if (weight[i] != INFINITE)
		{
			if (weight[i] < minLength) {
				minLength = weight[i];
				endVertex = i;
			}
		}
	}
	Path path;
	path.from = start;
	path.to = endVertex;
	if (minLength < INFINITE)
		path.length = minLength;

	int index = _results.size();

	_results.append(path);
	ui.listWidget->insertItem(index, path.toString());
}

// г) всі міста, не пов'язані між собою дорогами
void ApGKurs::OnFindNoWay() {
	ui.listWidget->clear();
	_results.clear();

	for (int i = 0; i < _matrix.length(); i++) {
		for (int j = 0; j < _matrix.length(); j++) {
			if (i == j)
				continue;

			QVector<int> weight(_matrix.length(), INFINITE);
			weight[i] = 0;
			QVector<bool> visited(_matrix.length(), false);
			ShortestWay(_matrix, i, j, weight, visited);
			if (weight[j] == INFINITE)
			{
				Path path;
				path.from = i;
				path.to = j;
				int index = _results.size();
				_results.append(path);
				ui.listWidget->insertItem(index, path.toString());
			}
		}
	}
}

bool ApGKurs::GetStart(int& start)
{
	bool ok = false;
	start = ui.txtVertexFrom->text().toInt(&ok);
	start--;
	return ok && start >= 0 && start < _matrix.length();
}

bool ApGKurs::GetStartEnd(int& start, int& end)
{
	bool okStart = false;
	bool okEnd = false;
	start = ui.txtVertexFrom->text().toInt(&okStart);
	end = ui.txtVertexTo->text().toInt(&okEnd);
	start--;
	end--;
	return okStart && start >= 0 && start < _matrix.length() &&
		okEnd&& end >= 0 && end < _matrix.length() &&
		end != start;
}