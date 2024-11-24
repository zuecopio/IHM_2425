/**
 * @file     mainwindow.cpp
 *
 * @author   Marcos Belda Martinez' <mbelmar@etsinf.upv.es>
 * @date     November, 2024
 * @section  IHM-GIIROB
 * @brief    Implementation of MainWindow class functions.
 */

//-----[ INCLUDES ]-----------------------------------------------------------//

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

#include <algorithm> // For std::shuffle, std::min and std::max
#include <random>    // For std::default_random_engine

#include <ctime> // std::time


//-----[ IMPLEMENTATION OF PUBLIC MAINWINDOW FUNCTIONS ]----------------------//

/******************************************************************************/
/*!
 * @brief  MainWindow class constructor implementation.
 * @param  parent  Used to set the parent widget of MainWindow
 */
MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , secondsElapsed(0)
    , moves(0)
    , stateIterator(0)
{
    /*-----------------------------------------------------+
     |     INITIALIZE APPLICATION VARIABLES                |
     +-----------------------------------------------------*/

    ui->setupUi(this);

    // Set welcome page as main
    ui->stackedWidget->setCurrentWidget(ui->welcomeView);

    // Load translation file
    if (translator.load(":/translations/8puzzle_es_ES"))
    {
        qDebug() << "Translation file loaded successfully: 8puzzle_es_ES";
        // Install the translator to the application
        qApp->installTranslator(&translator);
    }
    else
    {
        qDebug() << "Translation file not found: 8puzzle_es_ES";
    }

    // Load and apply stylesheet
    QFile file(":/styles/dark.qss"); // Path to the file in the resource
    if (file.open(QFile::ReadOnly))
    {
        QString styleSheet = QString::fromUtf8(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }

    // Set the text labels with the current language selection
    setLabelsText();

    // INITIALIZE MOVES COUNTER
    ui->movesValueLabel->setText(QString::number(moves));

    /*-----------------------------------------------------+
     |     INITIALIZE TIMER                                |
     +-----------------------------------------------------*/

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);

    // Set up timer display
    ui->timeValueLabel->setText("00:00");

    /*-----------------------------------------------------+
     |     CREATE GAME BOARD BUTTONS                       |
     +-----------------------------------------------------*/

    for (int i = 0; i < 8; i++)
    {
        buttons[i].button = new QPushButton(
            QString::number(i + 1),
            this);

        buttons[i].button->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding);

        buttons[i].button->setMinimumSize(100, 100);

        // Connect moveButton private slot function to each button
        connect(buttons[i].button, &QPushButton::clicked,
                this, &MainWindow::moveButton);
    }

    // Create empty button
    emptyButton.button = new QPushButton("", this);
    emptyButton.button->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding);
    emptyButton.button->setFlat(true);
    emptyButton.button->setEnabled(false);

    buttonsGridLayout = ui->buttonsGridLayout;

    /*-----------------------------------------------------+
     |     CREATE GAME INITIAL BOARD STATE                 |
     +-----------------------------------------------------*/

    positions.clear(); // Removes all the elements from the list

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            positions.append(qMakePair(row, col));
        }
    }

    // Assign buttons to random positions
    for (int i = 0; i < 8; ++i)
    {
        buttons[i].row = positions[i].first;
        buttons[i].col = positions[i].second;
        buttonsGridLayout->addWidget(buttons[i].button, buttons[i].row,
                                     buttons[i].col);
        buttons[i].button->setEnabled(false);
    }

    // Assign the position of the empty slot
    emptyButton.row = positions[8].first;
    emptyButton.col = positions[8].second;
    buttonsGridLayout->addWidget(emptyButton.button, emptyButton.row,
                                 emptyButton.col);

    // Disable buttons
    ui->resetButton->setEnabled(false);
    ui->undoButton->setEnabled(false);
    ui->redoButton->setEnabled(false);

    /*-----------------------------------------------------+
     |     CONNECT PRIVATE SLOT FUNCTIONS TO BUTTONS       |
     +-----------------------------------------------------*/

    // Connecting startButton private slot function to startGame
    connect(ui->startButton, &QPushButton::clicked,
            this, &MainWindow::startGame);

    // Connecting homeButton private slot function to comeBackToHome
    connect(ui->homeButton, &QPushButton::clicked,
            this, &MainWindow::comeBackToHome);

    // Connecting changeTheme private slot function to themeButton1
    connect(ui->themeButton1, &QPushButton::clicked,
            this, &MainWindow::changeTheme);

    // Connecting changeLanguage private slot function to languageButton
    connect(ui->languageButton, &QPushButton::clicked,
            this, &MainWindow::changeLanguage);


    // Connecting gameButton private slot function to playButton
    connect(ui->gameButton, &QPushButton::clicked,
            this, &MainWindow::playButton);

    // Connecting resetGame private slot function to resetButton
    connect(ui->resetButton, &QPushButton::clicked,
            this, &MainWindow::resetGame);

    // Connecting undoMove private slot function to undoButton
    connect(ui->undoButton, &QPushButton::clicked,
            this, &MainWindow::undoMove);

    // Connecting redoMove private slot function to redoButton
    connect(ui->redoButton, &QPushButton::clicked,
            this, &MainWindow::redoMove);

    // Connecting changeTheme private slot function to themeButton2
    connect(ui->themeButton2, &QPushButton::clicked,
            this, &MainWindow::changeTheme);

}   /* MainWindow() */

/******************************************************************************/
/*!
 * @brief  Implementation of the MainWindow class destructor.
 * @param  void
 */
MainWindow::~MainWindow(void)
{
    // Delete the button associated with the empty button
    delete emptyButton.button;

    // Loop through the first 8 buttons and delete each button
    for (int i = 0; i < 8; i++)
    {
        delete buttons[i].button; // Delete the button at index i
    }

    // Delete the user interface (UI) object
    delete ui;

}   /* ~MainWindow() */


//-----[ IMPLEMENTATION OF PRIVATE SLOTS MAINWINDOW FUNCTIONS ]---------------//

/******************************************************************************/
/*!
 * @brief   This function starts the game and displays instructions.
 * @param   void
 * @return  void
 */
void MainWindow::startGame(void)
{
    // Set the game page as the current view
    ui->stackedWidget->setCurrentWidget(ui->gameView);

    // Create an informational message box for game instructions
    QMessageBox infoMessage;
    infoMessage.setWindowTitle(tr("Game Instructions"));

    // Prepare the instructions message
    QString msg =
        tr("Welcome to the game!")           + "\n\n" +
        tr("Instructions:")                  + "\n" +
        tr("1. Select the pieces with the left"
           " mouse button to move them.")    + "\n" +
        tr("2. Complete the puzzle to win.") + "\n" +
        tr("3. Have fun!");

    // Set the message text and button options
    infoMessage.setText(msg);
    infoMessage.setStandardButtons(QMessageBox::Ok);
    infoMessage.setIcon(QMessageBox::Information);

    // Show the informational message and wait for user to close it
    if (infoMessage.exec() == QMessageBox::Ok)
    {
        // Start the game when the message box is closed
        resetGame();
    }

}   /* startGame() */

/******************************************************************************/
/*!
 * @brief   This function handles returning to the home menu.
 * @param   void
 * @return  void
 */
void MainWindow::comeBackToHome(void)
{
    // Create a warning message explaining how to play
    QMessageBox warningMessage;
    warningMessage.setWindowTitle(tr("Confirmation needed"));

    // Set the warning message text
    warningMessage.setText(
        tr("Are you sure you want to return to the welcome"
           " menu? The current game will not be saved."));

    // Define the standard buttons for the warning message
    warningMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    warningMessage.setIcon(QMessageBox::Warning);

    QPushButton * yesButton = qobject_cast<QPushButton *>(warningMessage.button(
        QMessageBox::Yes));

    yesButton->setText(tr("Yes")); // Change button text

    QPushButton * noButton = qobject_cast<QPushButton *>(warningMessage.button(
        QMessageBox::No));

    noButton->setText(tr("No")); // Change button text

    // Show the warning message and wait for user response
    if (warningMessage.exec() == QMessageBox::Yes)
    {
        // Set welcome page as the current view
        ui->stackedWidget->setCurrentWidget(ui->welcomeView);

        // Check if the timer is active
        if (timer->isActive())
        {
            // Pause the game
            ui->gameButton->setChecked(false);
            playButton(); // Stop the timer and disable the board

            // Reset the timer display
            ui->timeValueLabel->setText("00:00");
        }
    }

}   /* comeBackToHome() */

/******************************************************************************/
/*!
 * @brief   This function changes the application theme based on user selection.
 * @param   void
 * @return  void
 */
void MainWindow::changeTheme(void)
{
    // Get the button that triggered this function
    QPushButton * clickedButton = qobject_cast<QPushButton *>(sender());

    QString theme; // Variable to hold the theme name

    // Determine the theme based on the button state
    if (clickedButton->isChecked())
    {
        theme = "light"; // Set theme to light if checked
    }
    else
    {
        theme = "dark"; // Set theme to dark if unchecked
    }

    // Create a file path for the corresponding QSS file in resources
    QFile file(":/styles/" + theme + ".qss");
    if (file.open(QFile::ReadOnly)) // Try to open the file for reading
    {
        // Read the stylesheet from the file
        QString styleSheet = QString::fromUtf8(file.readAll());
        this->setStyleSheet(styleSheet); // Apply the stylesheet to the app

        // Debug output for new style sheet selection
        qDebug() << "Theme changed to" << theme << "mode";

        file.close(); // Close the file after reading
    }

    setLabelsText();

}   /* changeTheme() */

/******************************************************************************/
/*!
 * @brief   This function changes the application language based on user input.
 * @param   void
 * @return  void
 */
void MainWindow::changeLanguage(void)
{
    // Get the button that triggered this function
    QPushButton * clickedButton = qobject_cast<QPushButton *>(sender());

    QString language; // Variable to hold the language code
    QTranslator translator; // Translator object for language loading

    // Determine the language based on the button state
    if (clickedButton->isChecked())
    {
        language = "8puzzle_en_GB"; // Set language to English if checked

        // Debug output for English selection
        qDebug() << "Language changed to English";
    }
    else
    {
        language = "8puzzle_es_ES"; // Set language to Spanish if unchecked

        // Debug output for Spanish selection
        qDebug() << "Language changed to Spanish";
    }

    // Load the appropriate translation file
    if (translator.load(":/translations/" + language + ".qm"))
    {
        // Install the translator to the application
        qApp->installTranslator(&translator);
    }

    // Update the text labels to reflect the new language
    setLabelsText();

}   /* changeLanguage() */

/******************************************************************************/
/*!
 * @brief   This function updates the timer by incrementing the elapsed time.
 * @param   void
 * @return  void
 */
void MainWindow::updateTimer(void)
{
    secondsElapsed++; // Increment the elapsed seconds

    // Calculate minutes and seconds from the elapsed time
    int minutes = secondsElapsed / 60; // Get total minutes
    int seconds = secondsElapsed % 60;  // Get remaining seconds

    // Update the label to display the elapsed time
    ui->timeValueLabel->setText(
        QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0')) // Format minutes
        .arg(seconds, 2, 10, QChar('0'))); // Format seconds

}   /* updateTimer() */

/******************************************************************************/
/*!
 * @brief   This function handles the play/pause button functionality.
 * @param   void
 * @return  void
 */
void MainWindow::playButton(void)
{
    // Get the button that triggered this slot
    QPushButton * button = qobject_cast<QPushButton *>(sender());

    // Check if the button is currently checked (Play is selected)
    if (button->isChecked()) 
    {
        ui->gameButton->setToolTip(tr("Pause game")); // Set tooltip for play

        // Check if this is a new game (timer is at 0)
        if (secondsElapsed == 0) 
        {
            resetGame(); // Start a new game
        }
        else // If not a new game, enable buttons
        {
            for (int i = 0; i < 8; ++i)
            {
                buttons[i].button->setEnabled(true); // Enable each button
            }
        }

        // Start or resume the timer
        timer->start(1000); // Timer ticks every second
    }
    else // Pause is selected
    {
        ui->gameButton->setToolTip(tr("Play game")); // Set tooltip for pause

        // Pause the game by stopping the timer
        timer->stop();

        // Disable all buttons
        for (int i = 0; i < 8; ++i)
        {
            buttons[i].button->setEnabled(false); // Disable each button
        }
    }

}   /* playButton() */

/******************************************************************************/
/*!
 * @brief   This function undoes the last move in the game.
 * @param   void
 * @return  void
 */
void MainWindow::undoMove(void)
{
    // Check if there are previous states to undo and if we can move back
    if ((boardStates.size() > 1) && 
        (stateIterator < (boardStates.size() - 1)))
    {
        stateIterator += 1; // Move forward in the state history
    }

    // Check if we can undo a move
    if (stateIterator > 0)
    {
        // Update button positions based on the current state
        for (int i = 0; i < 8; ++i)
        {
            buttons[i].row = boardStates[stateIterator][i].first; // Row
            buttons[i].col = boardStates[stateIterator][i].second; // Column
            // Add button to the grid layout at the new position
            buttonsGridLayout->addWidget(buttons[i].button, 
                                          buttons[i].row, 
                                          buttons[i].col);
        }

        // Update the position of the empty slot
        emptyButton.row = boardStates[stateIterator][8].first; // Row
        emptyButton.col = boardStates[stateIterator][8].second; // Column
        // Add empty button to the grid layout
        buttonsGridLayout->addWidget(emptyButton.button, 
                                      emptyButton.row, 
                                      emptyButton.col);

        // Enable the redo button if there are enough states
        if (boardStates.size() > 2)
        {
            ui->redoButton->setEnabled(true);
        }

        moves -= 1; // Decrement the move counter
        updateMoves(); // Update the display of moves
    }

    // Disable the undo button if we are at the latest state
    if (stateIterator == boardStates.size() - 1)
    {
        ui->undoButton->setEnabled(false);
    }

}   /* undoMove() */

/******************************************************************************/
/*!
 * @brief   This function handles the redo action in the game.
 * @param   void
 * @return  void
 */
void MainWindow::redoMove(void)
{
    // Enable the undo button
    ui->undoButton->setEnabled(true);

    // Check if there are more than one board state and if we can redo
    if ((boardStates.size() > 1) && (stateIterator > 0))
    {
        // Move the iterator back to the previous state
        stateIterator -= 1;
    }

    // If the state iterator is valid, update the board
    if (stateIterator >= 0)
    {
        // Assign button positions based on the current board state
        for (int i = 0; i < 8; ++i)
        {
            buttons[i].row = boardStates[stateIterator][i].first;
            buttons[i].col = boardStates[stateIterator][i].second;
            buttonsGridLayout->addWidget(buttons[i].button, 
                                          buttons[i].row, 
                                          buttons[i].col);
        }

        // Assign the position of the empty slot on the board
        emptyButton.row = boardStates[stateIterator][8].first;
        emptyButton.col = boardStates[stateIterator][8].second;
        buttonsGridLayout->addWidget(emptyButton.button, 
                                      emptyButton.row, 
                                      emptyButton.col);

        // Increment the move counter
        moves += 1;
        updateMoves();
    }

    // Disable the redo button if we are at the first state
    if (stateIterator == 0)
    {
        ui->redoButton->setEnabled(false);
    }

}   /* redoMove() */

/******************************************************************************/
/*!
 * @brief   This function generates a resolvable initial state for the game.
 *          It utilizes the randomizeButtons() and isSolvable() methods.
 * @param   void
 * @return  void
 */
void MainWindow::resetGame(void)
{
    // Reset the timer display to 00:00
    ui->timeValueLabel->setText("00:00");

    // Initialize the timer and start counting
    secondsElapsed = 0;
    timer->start(1000);

    // Generate a random board configuration until it is solvable
    do
    {
        randomizeButtons(); // Randomize the button positions
    } while (!isSolvable()); // Repeat until the board is solvable

    // Clear previous board states
    boardStates.clear();

    // Store the initial positions in the board states
    boardStates.push_front(positions);

    // Reset the move counter and state iterator
    moves = 0;
    stateIterator = 0;
    updateMoves(); // Update the displayed move count

    // Disable undo and redo buttons, enable reset button
    ui->undoButton->setEnabled(false);
    ui->redoButton->setEnabled(false);
    ui->resetButton->setEnabled(true);
    ui->gameButton->setChecked(true); // Set the game button as checked

}   /* resetGame() */

/******************************************************************************/ 
/*! 
 * @brief   This function handles the movement of buttons in the game.
 * @param   void 
 * @return  void 
 */ 
void MainWindow::moveButton(void) 
{ 
    // Cast the sender to a QPushButton
    QPushButton * clickedButton = qobject_cast<QPushButton *>(sender()); 

    // Get the number associated with the clicked button
    int clickedButtonNum = clickedButton->text().toInt(); 

    bool stopLoop = false; 

    // Iterate through the buttons array
    for (int i = 0; (i < 8) && (stopLoop == false); i++) 
    { 
        // Check if the current button's number matches the clicked button's number
        if (buttons[i].button->text().toInt() == clickedButtonNum) 
        { 
            stopLoop = true; 

            // Check if there is a contact with the empty button
            if (canBeMoved(&(buttons[i]), &(emptyButton)))
            { 
                // Store the current button's row and column
                int rowAux = buttons[i].row; 
                int colAux = buttons[i].col; 

                // Swap the positions of the clicked button and the empty button
                buttons[i].row = emptyButton.row; 
                buttons[i].col = emptyButton.col; 
                emptyButton.row = rowAux; 
                emptyButton.col = colAux; 

                // Update the grid layout with the new button positions
                buttonsGridLayout->addWidget(buttons[i].button, 
                                              buttons[i].row, 
                                              buttons[i].col); 
                buttonsGridLayout->addWidget(emptyButton.button, 
                                              emptyButton.row, 
                                              emptyButton.col); 

                // Clear the positions list and update it with new positions
                positions.clear(); 
                for (int i = 0; i < 8; i++) 
                { 
                    positions.append(qMakePair(buttons[i].row, buttons[i].col)); 
                } 
                positions.append(qMakePair(emptyButton.row, emptyButton.col)); 

                // Update the board states
                for (int i = 0; i < stateIterator; i++) 
                { 
                    boardStates.pop_front(); 
                } 
                boardStates.push_front(positions); 

                // Maintain a maximum size for board states
                if (boardStates.size() > 5) 
                { 
                    boardStates.pop_back(); 
                } 

                stateIterator = 0; 
                // Enable the undo button and disable the redo button
                ui->undoButton->setEnabled(true); 
                ui->redoButton->setEnabled(false); 

                moves += 1; 
                updateMoves(); 

                // Print the current capacity of board states
                printf("capacity: %lld\n", boardStates.size()); 
            } 
        } 
    } 

    // Check if the game is resolved
    if (isResolved()) 
    { 
        ui->undoButton->setEnabled(false); 
        ui->redoButton->setEnabled(false); 
        ui->gameButton->setChecked(true); 
        ui->gameButton->setEnabled(false); 
        playButton(); 
    } 
}   /* moveButton() */


//-----[ IMPLEMENTATION OF PRIVATE MAINWINDOW FUNCTIONS ]---------------------//

/******************************************************************************/ 
/*! 
 * @brief   This function sets the text for various UI labels and buttons.
 * @param   void 
 * @return  void 
 */ 
void MainWindow::setLabelsText(void) 
{ 
    // Set the main window title
    this->setWindowTitle(tr("8 Puzzle Game"));

    // Set text for the title label
    ui->titleLabel->setText(tr("8 PUZZLE GAME"));

    // Set text for the author label
    ui->authorLabel->setText(tr("~ made by Marcos :P"));

    // Set text for the puzzle solution label
    ui->puzzleSolLabel->setText(tr("Try to solve 8 puzzle this way"));

    // Set text for the start button
    ui->startButton->setText(tr("Start game"));

    // Set tooltips for the welcome buttons
    if (ui->themeButton1->isChecked()) // Light theme is selected
    {
        ui->themeButton1->setToolTip(tr("Switch to dark theme"));
    }
    else // Dark theme is selected
    {
        ui->themeButton1->setToolTip(tr("Switch to light theme"));
    }

    ui->languageButton->setToolTip(tr("Change language"));

    // Set text for the time label
    ui->timeLabel->setText(tr("Time")); 
    
    // Set text for the moves label
    ui->movesLabel->setText(tr("Moves"));

    // Set tooltips for the game buttons
    if (ui->gameButton->isChecked()) // Play is selected
    {
        ui->gameButton->setToolTip(tr("Pause game"));
    }
    else // Pause is selected
    {
        ui->gameButton->setToolTip(tr("Play game"));
    }

    ui->undoButton->setToolTip(tr("Undo move")); 
    ui->redoButton->setToolTip(tr("Redo move")); 
    ui->resetButton->setToolTip(tr("Reset game"));

    if (ui->themeButton2->isChecked()) // Light theme is selected
    {
        ui->themeButton2->setToolTip(tr("Switch to dark theme"));
    }
    else // Dark theme is selected
    {
        ui->themeButton2->setToolTip(tr("Switch to light theme"));
    }

}   /* setLabelsText() */

/******************************************************************************/ 
/*! 
 * @brief   This function updates the displayed number of moves.
 * @param   void 
 * @return  void 
 */ 
void MainWindow::updateMoves(void) 
{ 
    // Set the text of the moves value label to the current number of moves
    ui->movesValueLabel->setText(QString::number(moves)); 

}   /* updateMoves() */

/******************************************************************************/
/*!
 * @brief   This function randomizes the positions of the buttons on the grid.
 * @param   void
 * @return  void
 */
void MainWindow::randomizeButtons(void)
{
    // Clear the list of possible positions on the grid
    positions.clear();

    // Loop through each row and column to populate possible positions
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            positions.append(qMakePair(row, col)); // Add position to list
        }
    }

    // Initialize random number generator
    std::random_device rd; // Obtiene un número aleatorio del hardware
    std::default_random_engine rng(rd()); // Crea un generador de números aleatorios

    // Shuffle the positions randomly to mix them up
    std::shuffle(positions.begin(), positions.end(), rng);

    // Assign buttons to the first 8 random positions
    for (int i = 0; i < 8; ++i)
    {
        buttons[i].row = positions[i].first; // Set button row
        buttons[i].col = positions[i].second; // Set button column
        buttonsGridLayout->addWidget(buttons[i].button, 
                                      buttons[i].row, 
                                      buttons[i].col); // Add button to layout

        buttons[i].button->setEnabled(true); // Enable the button
    }

    // Assign the position of the empty slot (the last position)
    emptyButton.row = positions[8].first; // Set empty button row
    emptyButton.col = positions[8].second; // Set empty button column
    buttonsGridLayout->addWidget(emptyButton.button, 
                                  emptyButton.row, 
                                  emptyButton.col); // Add empty button to layout

}   /* randomizeButtons() */

/******************************************************************************/
/*!
 * @brief   This function checks if the puzzle is solvable.
 * @param   void
 * @return  true if solvable, false otherwise
 */
bool MainWindow::isSolvable(void)
{
    // Create a list to hold the numbers of the pieces, excluding the empty slot
    QList<int> list;
    
    // Loop through the grid to populate the list with button numbers
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; j++)
        {
            // Get item at (i, j)
            QLayoutItem *item = buttonsGridLayout->itemAtPosition(i, j);

            // Check if the item has an associated widget
            if (item->widget())
            {
                // Get the widget from the item
                QWidget *widget = item->widget();

                // Check if the widget is a QPushButton
                if (QPushButton *button = qobject_cast<QPushButton *>(widget))
                {
                    // Exclude the empty button from the list
                    if (button != emptyButton.button)
                    {
                        // Add button number to list
                        list.append(button->text().toInt());
                    }
                }
            }
        }
    }

    // Calculate the number of inversions in the list
    int inversions = 0;
    for (int i = 0; i < list.size() - 1; ++i)
    {
        for (int j = i + 1; j < list.size(); ++j)
        {
            // Count inversions where a larger number precedes a smaller one
            if (list[i] > list[j])
            {
                inversions++;
            }
        }
    }

    // A puzzle is solvable if the number of inversions is even
    return (inversions % 2 == 0);

}   /* isSolvable() */

/******************************************************************************/
/*!
 * @brief   This function checks if button A can be moved next to button B.
 * @param   A  Pointer to the first button
 * @param   B  Pointer to the second button
 * @return  true if A can be moved next to B, false otherwise
 */
bool MainWindow::canBeMoved(buttonType_t * A, buttonType_t * B)
{
    bool answer = false; // Initialize answer to false

    // Calculate the row and column differences between buttons A and B
    int rowDifference = std::max(A->row, B->row) - std::min(A->row, B->row);
    int colDifference = std::max(A->col, B->col) - std::min(A->col, B->col);

    // Check if the buttons are adjacent (rowDifference + colDifference <= 1)
    if ((rowDifference + colDifference) <= 1)
    {
        answer = true; // Set answer to true if they are adjacent
    }

    return answer; // Return the result

}   /* canBeMoved() */

/******************************************************************************/
/*!
 * @brief   This function checks if the puzzle is resolved.
 * @param   void
 * @return  true if resolved, false otherwise
 */
bool MainWindow::isResolved(void)
{
    bool resolved = true; // Initialize resolved status to true

    // Check the positions of the buttons in the grid
    for (int row = 0, i = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            // Only check the first 8 buttons (excluding the empty slot)
            if (i < 8)
            {
                // Check if the button is in the correct position
                if (!((buttons[i].row == row) && (buttons[i].col == col)))
                {
                    // Set resolved to false if any button is out of place
                    resolved = false;
                }

                i++; // Move to the next button
            }
        }
    }

    // If the puzzle is resolved, show a victory message
    if (resolved)
    {
        QMessageBox msgBox; // Create message box
        msgBox.setWindowTitle(tr("Victory!")); // Set title
        msgBox.setText(
            tr("Congratulations! You have successfully"
               " completed the puzzle.")); // Set text

        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);

        QPushButton * okButton = qobject_cast<QPushButton *>(msgBox.button(
            QMessageBox::Ok));

        okButton->setText(tr("Continue")); // Change button text

        timer->stop(); // Stop the timer
        msgBox.exec(); // Show the message box
    }

    return resolved; // Return the resolved status

}   /* isResolved() */

/*** end of file ***/

/*/
 *   ⠀⠀⠀ ⣤⣶⠿⢖⣢⣴⣶⣦⣤⣄
 *  ⠀⠀⠀⢸⡟⢁⣼⠟⣩⢤⣤⠤⣉⠻⣇
 *   ⢀⣾⣿⡈⡇⢸⡏⣼⣇⡑⢀⠇⣸⢆⢿⢸⣿⣦
 *   ⣸⣿⣿⡇⠁⣀⡘⠿⠿⠿⢣⡾⢏⣼⠈⣼⣿⣿
 *   ⣿⣿⣿⣄⠾⣿⣿⣿⣶⣶⣤⣶⣿⡟⠀⣿⣿⠏
 *   ⠈⠙⠿⣿⣦⡈⢿⣿⣿⣿⣿⡿⠋⣠⣾⣿⣷
 *   ⠀⠀⠀⠈⠉⠉⢀⣉⣉⣩⣥⣶⣿⣿⡿⠟⠋
 *     ⠀⠀⠀⠀⠀ ⠻⣿⣿⣿⠟⠉⠀⠀ 
/*/
