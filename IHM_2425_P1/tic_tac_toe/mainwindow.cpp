/**
 * @file     mainwindow.cpp
 *
 * @author   Marcos Belda Martinez' <mbelmar@etsinf.upv.es>
 * @date     September, 2024
 * @section  IHM-GIIROB
 * @brief    Implementation of MainWindow class functions.
 */

//-----[ INCLUDES ]-----------------------------------------------------------//

#include "mainwindow.h"
#include "./ui_mainwindow.h"


//-----[ IMPLEMENTATION OF PUBLIC MAINWINDOW FUNCTIONS ]----------------------//

/******************************************************************************/
/*!
 * @brief  MainWindow class constructor implementation.
 */
MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}   /* MainWindow() */

/******************************************************************************/
/*!
 * @brief  MainWindow class destructor implementation.
 */
MainWindow::~MainWindow(void)
{
    delete ui;

}   /* ~MainWindow() */


//-----[ IMPLEMENTATION OF PRIVATE SLOTS MAINWINDOW FUNCTIONS ]---------------//

/******************************************************************************/
/*!
 * @brief  Method to reset each lineEdit from gridLayout.
 * @param  void
 * @return void
 */
void MainWindow::on_resetButton_clicked(void)
{
    ui->lineEdit_1->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");

}   /* on_resetButton_clicked() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_1.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_1_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_1->setText(upperArg);
    }
    else
    {
        ui->lineEdit_1->setText("");
    }

}   /* on_lineEdit_1_textEdited() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_2.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_2_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_2->setText(upperArg);
    }
    else
    {
        ui->lineEdit_2->setText("");
    }

}   /* on_lineEdit_2_textEdited() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_3.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_3_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_3->setText(upperArg);
    }
    else
    {
        ui->lineEdit_3->setText("");
    }

}   /* on_lineEdit_3_textEdited() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_4.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_4_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_4->setText(upperArg);
    }
    else
    {
        ui->lineEdit_4->setText("");
    }

}   /* on_lineEdit_4_textEdited() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_5.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_5_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_5->setText(upperArg);
    }
    else
    {
        ui->lineEdit_5->setText("");
    }

}   /* on_lineEdit_5_textEdited() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_6.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_6_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_6->setText(upperArg);
    }
    else
    {
        ui->lineEdit_6->setText("");
    }

}   /* on_lineEdit_6_textEdited() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_7.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_7_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_7->setText(upperArg);
    }
    else
    {
        ui->lineEdit_7->setText("");
    }

}   /* on_lineEdit_7_textEdited() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_8.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_8_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_8->setText(upperArg);
    }
    else
    {
        ui->lineEdit_8->setText("");
    }

}   /* on_lineEdit_8_textEdited() */

/******************************************************************************/
/*!
 * @brief  Method to set only 'O' or 'X' as posible values on lineEdit_9.
 * @param  const QString & arg
 * @return void
 */
void MainWindow::on_lineEdit_9_textEdited(const QString & arg)
{
    QString upperArg = arg.toUpper();

    if (upperArg == "O" || upperArg == "X")
    {
        ui->lineEdit_9->setText(upperArg);
    }
    else
    {
        ui->lineEdit_9->setText("");
    }

}   /* on_lineEdit_9_textEdited() */

/*** end of file ***/





