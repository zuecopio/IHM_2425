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
#include <QToolBar>
#include <QColor>
#include <QColorDialog>


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
    , isFilled(true)
    , circleColor(Qt::magenta)
    , diameterRatio(0.8)
{
    ui->setupUi(this);

    // Connecting sliderRows private slot function to gridSizeSlider
    connect(ui->gridSizeSlider, & QSlider::valueChanged,
            this, & MainWindow::sliderRows);

    // Connecting toggleFill private slot function to actionToggleFill
    connect(ui->actionToggleFill, & QAction::toggled,
            this, & MainWindow::toggleFill);

    // Connecting chooseColor private slot function to actionToggleFill
    connect(ui->actionChooseColor, & QAction::triggered,
            this, & MainWindow::chooseColor);

    // Creating a QSlider to control the circle size (diameter)
    QSlider * toolbar_slider = new QSlider(Qt::Horizontal);
    toolbar_slider->setMinimum(5);        // Minimum value
    toolbar_slider->setMaximum(95);       // Maximum value
    toolbar_slider->setValue(80);         // Initial value
    toolbar_slider->setFixedSize(32, 32); // Slider size

    // Adding the slider to the toolbar
    ui->mainToolBar->addWidget(toolbar_slider);

    // Connecting normalizing function to toolbar_slider
    connect(toolbar_slider, & QSlider::valueChanged, this,
            [=](int value)
            {
                diameterRatio = value / 100.0; // Normalize between 0 and 1
                update(); // Request a repaint
            });

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
 * @brief  Method that modifies the current value of rows and cols. (okay)
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
 * @brief  Auxiliar function to obtain the cell from a position. (okay)
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
    controlHeight += (ui->gridSizeSlider->height() + ui->gridSizeLabel->height());

    int drawAreaWidthMin = ui->centralwidget->pos().x();
    int drawAreaWidthMax = drawAreaWidthMin + ui->centralwidget->width();

    int drawAreaHeightMin = ui->centralwidget->pos().y();
    int drawAreaHeightMax = (drawAreaHeightMin + ui->centralwidget->height())
                            - controlHeight;

    // Cells height and width
    int cellHeight = (drawAreaHeightMax - drawAreaHeightMin) / rows;
    int cellWidth  = (drawAreaWidthMax - drawAreaWidthMin) / cols;

    // Check if the position is within the drawing area
    if (pos.x() < drawAreaWidthMin || pos.x() > drawAreaWidthMax ||
        pos.y() < drawAreaHeightMin || pos.y() > drawAreaHeightMax)
    {
        row = -1;
        col = -1;
        result = false;
    }
    else
    {
        // Calculate row and column
        row = (pos.y() - drawAreaHeightMin) / cellHeight;
        col = (pos.x() - drawAreaWidthMin) / cellWidth;

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
 * @brief  Auxiliar function to know if the click is inside the circle. (okay)
 * @param  pos  Mouse position coordinates.
 * @param  row  Current Row value.
 * @param  col  Current Col value.
 * @return Returns true if the click is inside the circle and false if not.
 */
bool MainWindow::isPosInsideCircle(const QPoint & pos, int row, int col,
                       int cellWidth, int cellHeight, int diameter)
{
    int drawAreaWidthMin = ui->centralwidget->pos().x();
    int drawAreaHeightMin = ui->centralwidget->pos().y();

    int centerX = drawAreaWidthMin + (col * cellWidth) + cellWidth / 2;
    int centerY = drawAreaHeightMin + (row * cellHeight) + cellHeight / 2;
    double distance = qSqrt(qPow(pos.x() - centerX, 2) + qPow(pos.y() - centerY, 2));

    return distance <= (diameter / 2.0);

}   /* isPosInsideCircle() */


//-----[ IMPLEMENTATION OF PRIVATE SLOTS MAINWINDOW FUNCTIONS ]---------------//

/******************************************************************************/
/*!
 * @brief  Updates the size of the grid acording to the slider value. (okay)
 * @param  value  Current position of the slider (between 2 and 20).
 * @return void
 */
void MainWindow::sliderRows(int value)
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

/******************************************************************************/
/*!
 * @brief  This function toggles the fill of the circle when it is painted. (okay)
 * @param  checked  True or false value.
 * @return void
 */
void MainWindow::toggleFill(bool checked)
{
    isFilled = !checked;
    update(); // Request a repaint

}   /* toggleFill() */

/******************************************************************************/
/*!
 * @brief  Function to choose the color of the circle. (okay)
 * @param  void
 * @return void
 */
void MainWindow::chooseColor(void)
{
    QColor color = QColorDialog::getColor(circleColor, this,
                                          "Select the Circle Color");
    if (color.isValid())
    {
        circleColor = color;
        update(); // Request a repaint to reflect the color change
    }

}   /* chooseColor() */


//-----[ IMPLEMENTATION OF PROTECTED MAINWINDOW FUNCTIONS ]-------------------//

/******************************************************************************/
/*!
 * @brief  Method to blablabla. (okay?)
 * @param  event  Contains event parameters for pain events.
 * @return void
 */
void MainWindow::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Pens configuration
    QPen normal_pen(circleColor);
    QPen dragging_pen(circleColor.darker(200));
    normal_pen.setWidth(5);
    dragging_pen.setWidth(5);

    // Obtain available drawing area (excluding slider and label)
    int controlHeight = 40; // 40 px of margin
    controlHeight += (ui->gridSizeSlider->height() + ui->gridSizeLabel->height());

    int drawAreaWidthMin = ui->centralwidget->pos().x();
    int drawAreaWidthMax = drawAreaWidthMin + ui->centralwidget->width();

    int drawAreaHeightMin = ui->centralwidget->pos().y();
    int drawAreaHeightMax = (drawAreaHeightMin + ui->centralwidget->height()) -
                             controlHeight;

    // Cells height and width
    int cellHeight = (drawAreaHeightMax - drawAreaHeightMin) / rows;
    int cellWidth  = (drawAreaWidthMax - drawAreaWidthMin) / cols;

    // Drawing grid
    painter.setPen(Qt::lightGray);
    for (int i = 0; i <= rows; i++)
    {
        painter.drawLine(drawAreaWidthMin, drawAreaHeightMin + (cellHeight * i),
                         drawAreaWidthMax, drawAreaHeightMin + (cellHeight * i));
    }

    for (int j = 0; j <= cols; j++)
    {
        painter.drawLine(drawAreaWidthMin + (cellWidth * j), drawAreaHeightMin,
                         drawAreaWidthMin + (j * cellWidth), drawAreaHeightMax);
    }

    int diameter = qMin(cellHeight, cellWidth) * diameterRatio;
    int x, y;

    // Drawing circle    
    if (isDragging)
    {
        painter.setPen(dragging_pen);
        if (isFilled)
        {
            painter.setBrush(circleColor.darker(200));
        }

        x = dragCurrentPos.x() - (diameter / 2);
        y = dragCurrentPos.y() - (diameter / 2);
    }
    else
    {
        painter.setPen(normal_pen);
        if (isFilled)
        {
            painter.setBrush(circleColor);
        }

        x = drawAreaWidthMin +
            (currentCol * cellWidth) +
            (cellWidth - diameter) / 2;

        y = drawAreaHeightMin +
            (currentRow * cellHeight) +
            (cellHeight - diameter) / 2;
    }

    painter.drawEllipse(x, y, diameter, diameter);

}   /* paintEvent() */

/******************************************************************************/
/*!
 * @brief  Method to analyze the key that has been pressed. (okay)
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
 * @brief  Method to analyze the key that has been pressed. (okay)
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

        int drawAreaWidthMin = ui->centralwidget->pos().x();
        int drawAreaWidthMax = drawAreaWidthMin + ui->centralwidget->width();

        int drawAreaHeightMin = ui->centralwidget->pos().y();
        int drawAreaHeightMax = (drawAreaHeightMin + ui->centralwidget->height())
                                - controlHeight;

        // Calculate cells size
        int cellHeight = (drawAreaHeightMax - drawAreaHeightMin) / rows;
        int cellWidth  = (drawAreaWidthMax - drawAreaWidthMin) / cols;

        // Calculate diameter
        int diameter = qMin(cellHeight, cellWidth) * diameterRatio;

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
 * @brief  Method to analyze the key that has been pressed. (okay)
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
 * @brief  Method to analyze the key that has been pressed. (okay)
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
        }

        update(); // Request a repaint
    }

}   /* mouseReleaseEvent() */

/*** end of file ***/

