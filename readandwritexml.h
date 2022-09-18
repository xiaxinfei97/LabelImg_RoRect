#include<QString>
#include "object.h"

#ifndef READANDWRITEXML_H
#define READANDWRITEXML_H

void WriteXml(QString xmlPath, std::vector<Object> objectSet, int w, int h ,int depth, QString suffix);

int ReadXml(QString xmlPath, std::vector<Object>& objectSet, const QVector<QString>& classlist, const QVector<QColor>& colorlist);
#endif // READANDWRITEXML_H
