#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->installEventFilter(this);

    // устанавливаю цвета когда человек оповещен и не оповещен
    color_yes.setRgb(0,230,0);                           // цвет, когда человек оповещен
    color_no.setRgb(255,36,0);                           // цвет, когда человек не оповещен
    color_absent.setRgb(255,255,255);                    // цвет, когда человек отсутствует по уважительной причине

    // инициализируем переменные, подсчет людей
    all_human = 0;      // по списуку
    notified = 0;       // оповещены
    not_notified = 0;   // не оповещены
    ill = 0;            // болен
    reason_1 = 0;       // причина №1
    vacation = 0;       // отпуск
    trip = 0;           // командировка
    reason_2 = 0;       // причина №2

    // беру файл Excele. Открываю страницу со списком людей и отделов
    Document xlsx("vedomost_controly.xlsx");
    xlsx.selectSheet("Работники предприятия");

    bool condition_1 = false, condition_2 = false; // если хотя бы одна из ячеек не пуста, то обрабатываем файл дальше
    int count = 1;  // начинаем обработку с первой строки

    int num_list = 1;   // для формирования порядкового номера человека

    int id = 0; // это порядковый номер для формирования Vector-ов - MyComboBox; MyCheckBox

    // этим циклом пробегаюсь по файлу Exele и заполняю QTableWidget фамилиями
    while ((condition_1==false) || (condition_2==false)) {
        QString str1 = xlsx.read(count,1).toString();   // наименование отдела
        QString str2 = "";                              // разряд

        if(str1=="") {
            condition_1=true;  // первая ячейка пуста
            str2 = xlsx.read(count,2).toString();

            if(str2=="") condition_2=true;  // вторая ячейка пуста
            else {

                ui->tableWidget->setRowCount(count);    // каждый цикл добавляю строку

                condition_2 = false;
                QString str3 = xlsx.read(count,3).toString();   // ФИО

                // разряд
                QTableWidgetItem *ptwi = new QTableWidgetItem();
                ptwi->setText(str2);
                ptwi->setBackground(color_no);
                ui->tableWidget->setItem(count-1,1, ptwi);

                // ФИО
                QTableWidgetItem *ptwi2 = new QTableWidgetItem();
                ptwi2->setText(str3);
                ptwi2->setBackground(color_no);
                ui->tableWidget->setItem(count-1,2, ptwi2);

                // порядковый номер
                QTableWidgetItem *ptwi3 = new QTableWidgetItem();
                str3 = QString::number(num_list);
                ptwi3->setText(str3);
                ptwi3->setBackground(color_no);
                ui->tableWidget->setItem(count-1,0, ptwi3);
                num_list ++;

                // Время оповещения и прибытия
                QString time = QTime::currentTime().toString("HH:mm");
                QTableWidgetItem *ptwi4 = new QTableWidgetItem();
                ptwi4->setText(time);
                ptwi4->setBackground(color_no);
                ptwi4->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(count-1,5, ptwi4);                

                QTableWidgetItem *ptwi5 = new QTableWidgetItem();
                ptwi5->setText(time);
                ptwi5->setBackground(color_no);
                ptwi5->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(count-1,7, ptwi5);

                // Оповещение
                // Создаём виджет, который будет содержать в себе чекбокс
                QWidget *checkBoxWidget = new QWidget();
                MycheckBox *checkBox = new MycheckBox();      // объявляем и инициализируем чекбокс
                // устанавливаю имя ObjectName. Он же является номером строки QtableWidget
                checkBox->setObjectName(QString::number(count-1));
                checkBox->id = id;  // номер по порядку. для работы с Vector
                // заполняю вектор моих MycheckBox полученными значениями ОПОВЕЩЕНИЕ
                VMyCheckBox_first.push_back(checkBox);
                QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget); // создаём слой с привязкой к виджету
                layoutCheckBox->addWidget(checkBox);            // Устанавливаем чекбокс в слой
                layoutCheckBox->setAlignment(Qt::AlignCenter);  // Отцентровываем чекбокс
                layoutCheckBox->setContentsMargins(0,0,0,0);    // Устанавливаем нулевые отступы
                // Устанавливаем чекбокс во вторую колонку
                ui->tableWidget->setCellWidget(count-1, 3, checkBoxWidget);

                // Прибытие
                // Создаём виджет, который будет содержать в себе чекбокс
                QWidget *checkBoxWidget1 = new QWidget();
                MycheckBox *checkBox1 = new MycheckBox();      // объявляем и инициализируем чекбокс
                // устанавливаю имя ObjectName.
                checkBox1->setObjectName(QString::number(count-1));
                checkBox1->id = id;  // номер по порядку. для работы с Vector
                QHBoxLayout *layoutCheckBox1 = new QHBoxLayout(checkBoxWidget1); // создаём слой с привязкой к виджету
                layoutCheckBox1->addWidget(checkBox1);            // Устанавливаем чекбокс в слой
                layoutCheckBox1->setAlignment(Qt::AlignCenter);  // Отцентровываем чекбокс
                layoutCheckBox1->setContentsMargins(0,0,0,0);    // Устанавливаем нулевые отступы
                // Устанавливаем чекбокс в шестую колонку
                ui->tableWidget->setCellWidget(count-1, 6, checkBoxWidget1);
                // заполняю вектор моих MycheckBox полученными значениями ПРИБЫТИЕ
                VMyCheckBox_second.push_back(checkBox1);

                // статус
                MyComboBox *pmcb = new MyComboBox();
                pmcb->setObjectName(QString::number(count-1));
                pmcb->id = id; // номер по порядку. для работы с Vector
                ui->tableWidget->setCellWidget(count-1, 4, pmcb);
                VMyComboBox.push_back(pmcb);

                id++; // делаем id на единицу больше

                // запускаем все connect-ы
                // изменяем QtableWidget с помощью состояния MyComboBox
                connect(pmcb, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changeQTableWiget);
                // изменяю стату прибытия человека
                connect(checkBox1, &MycheckBox::clicked, this, &MainWindow::changeArrival);
                // соединяю первый MycheckBox и MyComboBox
                connect(checkBox, &MycheckBox::clicked, this, &MainWindow::ChangeMyComboBox);
            }
        }
        else {
            ui->tableWidget->setRowCount(count);    // каждый цикл добавляю строку

            condition_1 = false;

            QTableWidgetItem *ptwi = new QTableWidgetItem();
            ptwi->setText(str1);
            ptwi->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(count-1,0, ptwi);

            // регистрирую названия отделов в вектор
            listDepartment.push_back(str1);

            ui->tableWidget->setSpan(count-1,0, 1,8);
        }
        count ++;
    }

    // всего людей по списку
    all_human = num_list-1;


    // разместить окно
    // доработать
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
//    ui->centralwidget->setGeometry(screenGeometry);

    // настраиваю QTableWidget - список людей
    ui->tableWidget->setColumnWidth(0,60);      // № п/п
    ui->tableWidget->setColumnWidth(1,120);      // должность
    ui->tableWidget->setColumnWidth(2,230);     // ФИО
    ui->tableWidget->setColumnWidth(3,120);     // Оповещён
    ui->tableWidget->setColumnWidth(4,130);     // Статус
    ui->tableWidget->setColumnWidth(5,115);     // Время опов.
    ui->tableWidget->setColumnWidth(6,110);     // Прибытие
    ui->tableWidget->setColumnWidth(7,115);      // Время приб.
    ui->tableWidget->setMinimumHeight(screenGeometry.height()-200);
    // настраиваю QTableWidget - результаты оповещения
    ui->tableWidget_2->setColumnWidth(0,80);    // пустой заголовок
    ui->tableWidget_2->setColumnWidth(1,110);   // Оповещено
    ui->tableWidget_2->setColumnWidth(2,123);   // Не оповещено
    ui->tableWidget_2->setColumnWidth(3,93);    //  по списку
    ui->tableWidget_2->setColumnWidth(4,75);    //  на лицо
    ui->tableWidget_2->setColumnWidth(5,115);    // причина №2
    ui->tableWidget_2->setColumnWidth(6,135);   // командировка
    ui->tableWidget_2->setColumnWidth(7,75);    // отпуск
    ui->tableWidget_2->setColumnWidth(8,75);    // болен
    ui->tableWidget_2->setColumnWidth(9,115);   // причина №1

    //заполняю в таблице оповещения две строки
    QTableWidgetItem *ptwi = new QTableWidgetItem();
    ptwi->setText("Итого:");
    ui->tableWidget_2->setItem(0,0,ptwi);

    // подсчитываю людей    
    MainWindow::counting(notified, not_notified, ill, reason_1, vacation, trip, reason_2, VMyComboBox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::counting(int &notified, int &not_notified, int &ill, int &reason_1, int &vacation, int &trip, int &reason_2, QVector<MyComboBox*> &VMyComboBox)
{       // ф-я для подсчета людей

    // СНАЧАЛА ОБНУЛЯЮ
    notified = 0;
    not_notified = 0;
    ill = 0;
    reason_1 = 0;
    vacation = 0;
    trip = 0;
    reason_2 = 0;

    // потом делаю подсчет
    for(auto& it : VMyComboBox)
    {
        switch (it->currentIndex()) {
            case 0: {   // не оповещен
                not_notified ++;
                break;
            }
            case 1: {   // оповещен
                notified ++;
                break;
            }
            case 2: {   // болен
                ill ++;
                break;
            }
            case 3: {   // причина №1
                reason_1 ++;
                break;
            }
            case 4: {   // причиан №2
                reason_2 ++;
                break;
            }
            case 5: {   // командировка
                trip ++;
                break;
            }
            case 6: {   // отпуск
                vacation++;
                break;
            }
        }
    }

    // после подсчёта заполняю таблицу QTableWidget_2
    QTableWidgetItem *ptwi01  = new QTableWidgetItem(); // оповещен
    ptwi01->setText(QString::number(notified));
    ptwi01->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 1, ptwi01);
    QTableWidgetItem *ptwi02  = new QTableWidgetItem(); // не оповещен
    ptwi02->setText(QString::number(all_human-notified));
    ptwi02->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 2, ptwi02);
    QTableWidgetItem *ptwi03  = new QTableWidgetItem(); // по списку
    ptwi03->setText(QString::number(all_human));
    ptwi03->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 3, ptwi03);
    QTableWidgetItem *ptwi04  = new QTableWidgetItem(); // на лицо
    ptwi04->setText(QString::number(all_human-reason_2-trip-vacation-ill-reason_2));
    ptwi04->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 4, ptwi04);
    QTableWidgetItem *ptwi05  = new QTableWidgetItem(); // причина №2
    ptwi05->setText(QString::number(reason_2));
    ptwi05->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 5, ptwi05);
    QTableWidgetItem *ptwi06  = new QTableWidgetItem(); // командировка
    ptwi06->setText(QString::number(trip));
    ptwi06->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 6, ptwi06);
    QTableWidgetItem *ptwi07  = new QTableWidgetItem(); // отпуск
    ptwi07->setText(QString::number(vacation));
    ptwi07->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 7, ptwi07);
    QTableWidgetItem *ptwi08  = new QTableWidgetItem(); // болен
    ptwi08->setText(QString::number(ill));
    ptwi08->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 8, ptwi08);
    QTableWidgetItem *ptwi09  = new QTableWidgetItem(); // причина №1
    ptwi09->setText(QString::number(reason_1));
    ptwi09->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget_2->setItem(0, 9, ptwi09);
}


void MainWindow::changeQTableWiget(int index)
{   // меняю цвета QTableWidget при изменении состояния MyComboBox

    // беру номер строки
    int id_row = dynamic_cast<MyComboBox*>(sender())->objectName().toInt();

    // порядковый номер
    QTableWidgetItem *ptwi  = ui->tableWidget->item(id_row, 0);
    // разряд
    QTableWidgetItem *ptwi1 = ui->tableWidget->item(id_row, 1);
    // ФИО
    QTableWidgetItem *ptwi2 = ui->tableWidget->item(id_row, 2);
    // Время оповещения
    QTableWidgetItem *ptwi5 = ui->tableWidget->item(id_row, 5);
    // Время прибытия
    QTableWidgetItem *ptwi7 = ui->tableWidget->item(id_row, 7);

    // беру id MyComboBox, у MyCheckBox он такойже. они на одной строке
    int Vindex = dynamic_cast<MyComboBox*>(sender())->id;

    // все MyCheckBox данной строки изначально делаю в положение false
    // (внутри в зависимости от состояния человека меняю)
    VMyCheckBox_second[Vindex]->setChecked(false);
    VMyCheckBox_first[Vindex]->setChecked(false);
    // делаю не достпным MyCheckBox данной строки
    VMyCheckBox_second[Vindex]->setEnabled(false);
    VMyCheckBox_first[Vindex]->setEnabled(false);

    switch (index) {
        // не оповещен
        case 0:
        {
            // устанавливаю цвет ячеек
            ptwi->setBackground(color_no);
            ptwi1->setBackground(color_no);
            ptwi2->setBackground(color_no);
            ptwi5->setBackground(color_no);
            ptwi7->setBackground(color_no);

            // устанавливаю галку ОПОВЕЩЕНИЧ в false
            VMyCheckBox_first[Vindex]->setChecked(false);

            // делаю достпным MyCheckBox данной строки
            VMyCheckBox_first[Vindex]->setEnabled(true);
            VMyCheckBox_second[Vindex]->setEnabled(true);

            // ставлю в положение false
            VMyCheckBox_second[Vindex]->setChecked(false);
            VMyCheckBox_first[Vindex]->setChecked(false);

            // делаю доступной ячеку времени оповещения и прибытия данной строки
            ptwi7->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled); //QFlags<Qt::ItemFlag>(ItemIsSelectable|ItemIsEditable|ItemIsDragEnabled|ItemIsDropEnabled|ItemIsUserCheckable|ItemIsEnabled)
            ptwi5->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
            break;
        }

        // оповещен
        case 1:
        {
            // устанавливаю цвет ячеек
            ptwi->setBackground(color_yes);
            ptwi1->setBackground(color_yes);
            ptwi2->setBackground(color_yes);
            ptwi5->setBackground(color_yes);

            // устанавливаю галку ОПОВЕЩЕНИЧ в true
            VMyCheckBox_first[Vindex]->setChecked(true);
            // ставлю в положение false
            VMyCheckBox_second[Vindex]->setChecked(false);

            // устанавливаем текущее время
            QString time = QTime::currentTime().toString("HH:mm");
            ptwi5->setText(time);
            ptwi5->setTextAlignment(Qt::AlignCenter);

            // делаю достпным MyCheckBox данной строки
            VMyCheckBox_first[Vindex]->setEnabled(true);
            VMyCheckBox_second[Vindex]->setEnabled(true);
            // делаю доступной ячеку времени прибытия оповещения и данной строки
            ptwi7->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
            ptwi5->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
            break;
        }

        // болен
        case 2:
        {
            // устанавливаю цвет ячеек
            ptwi->setBackground(color_absent);
            ptwi1->setBackground(color_absent);
            ptwi2->setBackground(color_absent);
            ptwi5->setBackground(color_absent);
            ptwi7->setBackground(color_absent);

            // ставлю в положение false
            VMyCheckBox_second[Vindex]->setChecked(false);
            VMyCheckBox_first[Vindex]->setChecked(false);

            // делаю недоступной ячеку времени оповещения и прибытия данной строки
            ptwi7->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            ptwi5->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            break;
        }

        // причина №1
        case 3:
        {
            // устанавливаю цвет ячеек
            ptwi->setBackground(color_absent);
            ptwi1->setBackground(color_absent);
            ptwi2->setBackground(color_absent);
            ptwi5->setBackground(color_absent);
            ptwi7->setBackground(color_absent);

            // ставлю в положение false
            VMyCheckBox_second[Vindex]->setChecked(false);
            VMyCheckBox_first[Vindex]->setChecked(false);
            qDebug() << "tuta";

            // делаю недоступной ячеку времени оповещения и прибытия данной строки
            ptwi7->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            ptwi5->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            break;
        }

        // причина №2
        case 4:
        {
            // устанавливаю цвет ячеек
            ptwi->setBackground(color_absent);
            ptwi1->setBackground(color_absent);
            ptwi2->setBackground(color_absent);
            ptwi5->setBackground(color_absent);
            ptwi7->setBackground(color_absent);

            // ставлю в положение false
            VMyCheckBox_second[Vindex]->setChecked(false);
            VMyCheckBox_first[Vindex]->setChecked(false);

            // делаю недоступной ячеку времени оповещения и прибытия данной строки
            ptwi7->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            ptwi5->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            break;
        }
        // командировка
        case 5:
        {
            // устанавливаю цвет ячеек
            ptwi->setBackground(color_absent);
            ptwi1->setBackground(color_absent);
            ptwi2->setBackground(color_absent);
            ptwi5->setBackground(color_absent);
            ptwi7->setBackground(color_absent);

            // ставлю в положение false
            VMyCheckBox_second[Vindex]->setChecked(false);
            VMyCheckBox_first[Vindex]->setChecked(false);

            // делаю недоступной ячеку времени оповещения и прибытия данной строки
            ptwi7->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            ptwi5->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            break;
        }

        // отпуск
        case 6:
        {
            // устанавливаю цвет ячеек
            ptwi->setBackground(color_absent);
            ptwi1->setBackground(color_absent);
            ptwi2->setBackground(color_absent);
            ptwi5->setBackground(color_absent);
            ptwi7->setBackground(color_absent);

            // ставлю в положение false
            VMyCheckBox_second[Vindex]->setChecked(false);
            VMyCheckBox_first[Vindex]->setChecked(false);

            // делаю недоступной ячеку времени оповещения и прибытия данной строки
            ptwi7->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
            ptwi5->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);

            break;
        }
    }

    // подсчитываю людей
    MainWindow::counting(notified, not_notified, ill, reason_1, vacation, trip, reason_2, VMyComboBox);
}


void MainWindow::changeArrival()
{   // меняю цвет в QTableWidget - прибыл человек или нет

    // забираю строчку QTableWidget
    int row_id = dynamic_cast<MycheckBox*>(sender())->objectName().toInt();

    // забираю состояние MyCheckBox
    bool state = dynamic_cast<MycheckBox*>(sender())->isChecked();

    // беру нужную ячейку ПРИБЫТИЯ
    QTableWidgetItem* ptwi = new QTableWidgetItem();
    ui->tableWidget->setItem(row_id, 7, ptwi);
    // устанавливаю текущее время прибытия
    QString time = QTime::currentTime().toString("HH:mm");
    ptwi->setText(time);
    ptwi->setTextAlignment(Qt::AlignCenter);

    if(state) { // если человек прибыл        

        ptwi->setBackground(color_yes);
    }
    else {  // если человек не прибыл       

        ptwi->setBackground(color_no);
    }
}

void MainWindow::ChangeMyComboBox()
{   // изменяю MyComboBox по нажатию первого MyCheckBox
    bool state = dynamic_cast<MycheckBox*>(sender())->isChecked();

    int id = dynamic_cast<MycheckBox*>(sender())->id;

    if(state) VMyComboBox[id]->setCurrentIndex(1);
    else      VMyComboBox[id]->setCurrentIndex(0);
}




