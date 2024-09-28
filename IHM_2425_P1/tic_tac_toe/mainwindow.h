/**
 * @file     mainwindow.h
 *
 * @author   Marcos Belda Martinez' <mbelmar@etsinf.upv.es>
 * @date     September, 2024
 * @section  IHM-GIIROB
 * @brief    Definition of MainWindow class functions.
 */

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

#include <QMainWindow>


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


//-----[ MAINWINDOW CLASS ]---------------------------------------------------//

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        MainWindow(QWidget * parent = nullptr);
        ~MainWindow();

    private slots:

        void on_resetButton_clicked();
        void on_lineEdit_1_textEdited(const QString & arg);
        void on_lineEdit_2_textEdited(const QString & arg);
        void on_lineEdit_3_textEdited(const QString & arg);
        void on_lineEdit_4_textEdited(const QString & arg);
        void on_lineEdit_5_textEdited(const QString & arg);
        void on_lineEdit_6_textEdited(const QString & arg);
        void on_lineEdit_7_textEdited(const QString & arg);
        void on_lineEdit_8_textEdited(const QString & arg);
        void on_lineEdit_9_textEdited(const QString & arg);

    private:

        Ui::MainWindow * ui;
};

#endif /* MAINWINDOW_H */

/*** end of file ***/
