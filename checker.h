#ifndef CHECKER_H
#define CHECKER_H
#include "unit.h"
#include <QList>


class checker
{
public:
    checker(QList<unit*> &allunits, QList<unit*> &tempunits, QList<unit*> &errorunits, QList<unit*> &passunits);
    int **checker_map;

};

#endif // CHECKER_H
