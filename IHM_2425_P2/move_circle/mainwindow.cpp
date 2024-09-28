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
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>


//-----[ IMPLEMENTATION OF PUBLIC MAINWINDOW FUNCTIONS ]----------------------//

/******************************************************************************/
/*!
 * @brief  MainWindow class constructor implementation.
 */
MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cols(2)
    , rows(2)
    , currentCol(1)
    , currentRow(1)
    , isDragging(false)
    , isDraggingCircle(false)
    , dragStartPos()
    , dragCurrentPos()
{
    ui->setupUi(this);

}   /* MainWindow() */

/******************************************************************************/
/*!
 * @brief  MainWindow class destructor implementation.
 */
MainWindow::~MainWindow()
{
    delete ui;

}   /* ~MainWindow() */


//-----[ IMPLEMENTATION OF PRIVATE MAINWINDOW FUNCTIONS ]---------------------//

/******************************************************************************/
/*!
 * @brief  Method that modifies the current value of rows and cols.
 * @param  deltaRow  This number indicates how far you want to move in the rows.
 * @param  deltaCol  This number indicates how far you want to move in the cols.
 * @return void
 */
void MainWindow::updatePosition(int deltaRow, int deltaCol)
{
    // Update position with wrapper (envoltura)
    currentRow = (currentRow + deltaRow + rows) % rows;
    currentCol = (currentCol + deltaCol + cols) % cols;

    update(); // Request a repaint

}   /* updatePosition() */

/******************************************************************************/
/*!
 * @brief  Auxiliar function to obtain the cell from a position.
 * @param  pos  Mouse position coordinates.
 * @param  row  Changes its value depending y mouse position coordinate.
 * @param  col  Changes its value depending x mouse position coordinate.
 * @return Returns true if there is a cell for the position and false if not.
 */
bool MainWindow::getCellFromPosition(const QPoint & pos, int & row, int & col)
{
    bool result = true;

    // Obtain available drawing area (excluding slider and label)
    int controlHeight = 40; // 40 px of margin
    controlHeight += ui->gridSizeSlider->height() + ui->gridSizeLabel->height();
    int drawAreaHeight = height() - controlHeight;

    // Calculate cells size
    int cellWidth = width() / cols;
    int cellHeight = drawAreaHeight / rows;

    // Check if the position is within the drawing area
    if (pos.y() < 0 || pos.y() > drawAreaHeight || pos.x() < 0 || pos.x() > width())
    {
        row = -1;
        col = -1;
        result = false;
    }
    else
    {
        // Calculate row and column
        row = pos.y() / cellHeight;
        col = pos.x() / cellWidth;

        // Check if the values are within the limits
        if (row >= rows)
        {
            row = rows - 1;
        }

        if (col >= cols)
        {
            col = cols - 1;
        }
    }

    return result;

}   /* getCellFromPosition() */

/******************************************************************************/
/*!
 * @brief  Auxiliar function to know if the click is inside the circle.
 * @param  pos  Mouse position coordinates.
 * @param  row  Current Row value.
 * @param  col  Current Col value.
 * @return Returns true if the click is inside the circle and false if not.
 */
bool MainWindow::isPosInsideCircle(const QPoint & pos, int row, int col,
                       int cellWidth, int CellHeight, int diameter)
{
    int centerX = col * cellWidth + cellWidth / 2;
    int centerY = row * CellHeight + CellHeight / 2;
    double distance = qSqrt(qPow(pos.x() - centerX, 2) + qPow(pos.y() - centerY, 2));

    return distance <= (diameter / 2.0);

}   /* isPosInsideCircle() */


//-----[ IMPLEMENTATION OF PRIVATE SLOTS MAINWINDOW FUNCTIONS ]---------------//

/******************************************************************************/
/*!
 * @brief  Updates the size of the grid acording to the slider value.
 * @param  value  Current position of the slider (between 2 and 20).
 * @return void
 */
void MainWindow::on_gridSizeSlider_valueChanged(int value)
{
    // Update the size of the grid
    rows = value;
    cols = value;

    // Adjust the current position to maintain it inside the new limits
    currentRow = currentRow % rows;
    currentCol = currentCol % cols;

    // Update the label text
    ui->gridSizeLabel->setText(QString("Grid size: %1 x %1").arg(rows));

    update(); // Request a repaint

}   /* on_gridSizeSlider_valueChanged() */


//-----[ IMPLEMENTATION OF PROTECTED MAINWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief  Method to blablabla.
 * @param  event  Contains event parameters for pain events.
 * @return void
 */
void MainWindow::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Obtain available drawing area (excluding slider and label)
    int controlHeight = 40; // 40 px of margin
    controlHeight += (ui->gridSizeSlider->height() + ui->gridSizeLabel->height());
    int drawAreaHeight = height() - controlHeight;

    // Cells height and width
    int cellHeight = drawAreaHeight / rows;;
    int cellWidth = width() / cols;

    // Drawing grid
    painter.setPen(Qt::darkMagenta);
    for (int i = 0; i <= rows; i++)
    {
        painter.drawLine(0, cellHeight * i, width(), cellHeight * i);
    }

    for (int j = 0; j <= cols; j++)
    {
        painter.drawLine(cellWidth * j, 0, j * cellWidth, drawAreaHeight);
    }

    // Drawing circle
    if (isDragging)
    {
        painter.setPen(Qt::darkMagenta);
        painter.setBrush(Qt::darkMagenta);
    }
    else
    {
        painter.setPen(Qt::magenta);
        painter.setBrush(Qt::magenta);
    }

    int diameter = qMin(cellHeight, cellWidth) * 0.8;
    int x, y;

    x = currentCol * cellWidth + (cellWidth - diameter) / 2;
    y = currentRow * cellHeight + (cellHeight - diameter) / 2;

    painter.drawEllipse(x, y, diameter, diameter);

}   /* paintEvent() */

/******************************************************************************/
/*!
 * @brief  Method to analyze the key that has been pressed.
 * @param  event  Pointer to the class that describes a key event.
 * @return void
 */
void MainWindow::keyPressEvent(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        updatePosition(-1, 0);
        update(); // Request a repaint
        break;

    case Qt::Key_Down:
        updatePosition(1, 0);
        update(); // Request a repaint
        break;

    case Qt::Key_Left:
        updatePosition(0, -1);
        update(); // Request a repaint
        break;

    case Qt::Key_Right:
        updatePosition(0, 1);
        update(); // Request a repaint
        break;

    default:
        QMainWindow::keyPressEvent(event);
    }

}   /* keyPressEvent() */

/******************************************************************************/
/*!
 * @brief  Method to analyze the key that has been pressed.
 * @param  event  Contains parameters that describe a mouse event.
 * @return void
 */
void MainWindow::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Obtain available drawing area (excluding slider and label)
        int controlHeight = 40; // 40 px of margin
        controlHeight += ui->gridSizeSlider->height() + ui->gridSizeLabel->height();
        int drawAreaHeight = height() - controlHeight;

        // Calculate cells size
        int cellWidth  = width() / cols;
        int cellHeight = drawAreaHeight / rows;

        // Calculate diameter
        int diameter = qMin(cellHeight, cellWidth) * 0.8;

        if (isPosInsideCircle(event->pos(), currentRow, currentCol,
                              cellWidth, cellHeight, diameter))
        {
            // Starts circle dragging
            isDragging       = true;
            isDraggingCircle = true;
            dragStartPos     = event->pos();
            dragCurrentPos   = event->pos();
        }

        update(); // Request a repaint
    }

}   /* mousePressEvent() */

/******************************************************************************/
/*!
 * @brief  Method to analyze the key that has been pressed.
 * @param  event  Contains parameters that describe a mouse event.
 * @return void
 */
void MainWindow::mouseMoveEvent(QMouseEvent * event)
{
    if (isDragging && isDraggingCircle)
    {
        // Update the curret pose while dragging
        dragCurrentPos = event->pos();

        int row, col;
        if (getCellFromPosition(event->pos(), row, col))
        {
            // Move the circle to the selected cell
            currentRow = row;
            currentCol = col;
        }

        update(); // Request a repaint
    }

}   /* mouseMoveEvent() */

/******************************************************************************/
/*!
 * @brief  Method to analyze the key that has been pressed.
 * @param  event  Contains parameters that describe a mouse event.
 * @return void
 */
void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && isDragging && isDraggingCircle)
    {
        isDragging = false;
        isDraggingCircle = false;
        dragCurrentPos = event->pos();

        int row, col;
        if (getCellFromPosition(event->pos(), row, col))
        {
            // Move the circle to the selected cell
            currentRow = row;
            currentCol = col;

            update(); // Request a repaint
        }
    }

}   /* mouseReleaseEvent() */

/*** end of file ***/

