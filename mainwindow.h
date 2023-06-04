#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QStyle>
#include <QTime>
#include <QCheckBox>
#include <QComboBox>
#include <QFont>
#include "mycombobox.h"
#include "mycheckbox.h"
#include <QPainter>
#include <QAxObject>

#include <QtCore>
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"
#include "xlsxworksheet.h"
#include <QScreen>
#include <QTableWidgetItem>

QTXLSX_USE_NAMESPACE

using namespace QXlsx;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Document xlsx;

    QVector<MycheckBox*> VMyCheckBox_second;    // вектор MycheckBox первого столбца
    QVector<MycheckBox*> VMyCheckBox_first;     // вектор MycheckBox второго столбца
    QVector<MyComboBox*> VMyComboBox;           // вектор MyComboBox
    QColor color_yes;                           // цвет, когда человек оповещен
    QColor color_no;                            // цвет, когда человек не оповещен
    QColor color_absent;                        // цвет, когда человек отсутствует
    QVector<QString> listDepartment;            // список отделов учереждения

    int all_human, notified, not_notified, ill, reason_1, vacation, trip, reason_2;    // по списку, оповещены, не оповещены, болен, причина №1, отпуск, командировка, причина №2

    void counting(int &notified, int &not_notified, int &ill, int &reason_1, int &vacation, int &trip,int &reason_2, QVector<MyComboBox*> &VMyComboBox);        // ф-я для подсчета людей



private slots:
    void changeQTableWiget(int index); // слот для изменения QTableWidget по MyComboBox
    void changeArrival();              // изменяю цвет прибытия человека   
    void ChangeMyComboBox();            // изменяю MyComboBox по нажатию первого MyCheckBox


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
