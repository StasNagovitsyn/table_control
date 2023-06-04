#include "mycombobox.h"


MyComboBox::MyComboBox()
{
    QStringList status_list;
    status_list << "не оповещен" << "оповещен" <<  "болен" << "причина №1" <<  "причина №2" << "командировка" <<  "отпуск";
    this->addItems(status_list);

    id = 0; // поумолчанию номер строки 0
}

void MyComboBox::wheelEvent(QWheelEvent *e)
{   // здесь ничего не делаю, чтобы колесо мыши не меняло состояние MyComboBox

}





