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
#include <QKeyEvent>
#include <QMouseEvent>


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

    private:

        Ui::MainWindow * ui;
        int rows;
        int cols;
        int currentRow;
        int currentCol;
        bool isDragging;
        bool isDraggingCircle;
        QPoint dragStartPos;
        QPoint dragCurrentPos;
        void updatePosition(int deltaRow, int deltaCol);
        bool getCellFromPosition(const QPoint & pos, int & row, int & col);
        bool isPosInsideCircle(const QPoint & pos, int row, int col,
                               int cellWidth, int CellHeight, int diameter);

    private slots:

        void on_gridSizeSlider_valueChanged(int value);

    protected:

        void paintEvent(QPaintEvent * event) override;
        void keyPressEvent(QKeyEvent * event) override;
        void mousePressEvent(QMouseEvent * event) override;
        void mouseMoveEvent(QMouseEvent * event) override;
        void mouseReleaseEvent(QMouseEvent * event) override;
};

#endif /* MAINWINDOW_H */

/*** end of file ***/
