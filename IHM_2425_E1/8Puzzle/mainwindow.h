/**
 * @file     mainwindow.h
 *
 * @author   Marcos Belda Martinez' <mbelmar@etsinf.upv.es>
 * @date     November, 2024
 * @section  IHM-GIIROB
 * @brief    Definition of MainWindow class functions.
 */

//-----[ DEFINE TO PREVENT RECURSIVE INCLUSIONS ]-----------------------------//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//-----[ INCLUDES ]-----------------------------------------------------------//

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>
#include <QTranslator>


//-----[ TYPEDEF STRUCTS ]----------------------------------------------------//

typedef struct
{
    QPushButton * button;
    int row, col;

} buttonType_t;


//-----[ QT NAMESPACE ]-------------------------------------------------------//

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


//-----[ MAINWINDOW CLASS ]---------------------------------------------------//

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        /*-----------------------------------------------------+
         |     DECLARATION OF PUBLIC FUNCTIONS                 |
         +-----------------------------------------------------*/

        MainWindow(QWidget * parent = nullptr);
        ~MainWindow();

    private slots:

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE SLOT FUNCTIONS           |
         +-----------------------------------------------------*/

        void startGame(void);
        void comeBackToHome(void);
        void changeTheme(void);
        void changeLanguage(void);

        void updateTimer(void);

        void playButton(void);
        void undoMove(void);
        void redoMove(void);
        void resetGame(void);
        
        void moveButton(void);

    private:

        /*-----------------------------------------------------+
         |     DECLARIATION OF PRIVATE VARIABLES               |
         +-----------------------------------------------------*/
        
        // VARIABLES FOR THE APPLICATION

        Ui::MainWindow * ui;
        QTranslator translator; // Translator for language (ES or EN)

        // VARIABLES FOR THE TIMER

        QTimer * timer;     // Timer to keep track of elapsed time
        int secondsElapsed; // To keep track of the elapsed seconds

        // VARIABLES FOR THE MOVES

        int moves; // Variable that stores the number of moves made

        // VARIABLES FOR THE BUTTONS

        buttonType_t buttons[8];  // Array to hold button types
        buttonType_t emptyButton; // Variable to represent the empty button

        QGridLayout * buttonsGridLayout;  // Reference to the grid layout for buttons
        QVector<QList<QPair<int, int>>> boardStates; // Vector to store different board states
        QList<QPair<int, int>> positions; // List to store positions of buttons

        int stateIterator; // Iterator to navigate through board states

        /*-----------------------------------------------------+
         |     DECLARATION OF PRIVATE FUNCTIONS                |
         +-----------------------------------------------------*/
        
        // FUNCTIONS FOR THE LABELS

        void setLabelsText(void);
        void updateMoves(void);

        // FUNCTIONS FOR THE BUTTONS

        void randomizeButtons(void);
        bool isSolvable(void);
        bool canBeMoved(buttonType_t * A, buttonType_t * B);
        bool isResolved(void);
};

#endif /* MAINWINDOW_H */

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
