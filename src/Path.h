#pragma once
#include <QVector>
#include <QTextStream>

struct Path
{
	int length = 0;
	int from = 0;
	int to = 0;
	QVector<int> line;


	QString toString()
	{
		QString path;
		QTextStream out(&path);
		if (line.length() > 0) {

			for (int i = 0; i < line.length(); i++) {
				if (i != 0)
					out << "->";
				out << QString::number(line[i] + 1);
			}
			out << " (" << QString::number(length) << ")";
		}

		else if (length > 0)
			out << "Way between " << from + 1 << " and " << to + 1 << "=" << QString::number(length);
		else {
			if (from != to)
				out << "No way between " << from + 1 << " and " << to + 1;
			else
				out << "No way from " << from + 1;
		}
		return path;
	}
};



