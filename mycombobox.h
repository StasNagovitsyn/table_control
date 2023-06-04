/// данный класс предназначен для создания своего ComboBox;
/// наследуюсь от QComboBox
///
#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>
#include "mycheckbox.h"

class MyComboBox : public QComboBox
{
    Q_OBJECT

public:
    MyComboBox();

    // убираю прикрутку колёсиком мыши вариантов MyComboBox
    void wheelEvent(QWheelEvent *e) override;

    int id;             // номер строки в QTableWidget

};

#endif // MYCOMBOBOX_H
