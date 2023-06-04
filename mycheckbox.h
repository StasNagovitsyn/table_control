/// данный класс предназначен для создания своего checkBox;
/// наследуюсь от QCheckBox

#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H
#include <QCheckBox>

class MycheckBox : public QCheckBox
{
    Q_OBJECT

public:
    MycheckBox();

    int id;             // номер строки в QTableWidget
};

#endif // MYCHECKBOX_H
