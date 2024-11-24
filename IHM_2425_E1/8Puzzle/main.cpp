/**
 * @file     main.cpp
 *
 * @author   Marcos Belda Martinez' <mbelmar@etsinf.upv.es>
 * @date     November, 2024
 * @section  IHM-GIIROB
 * @brief    Implementation of main function for the Qt framework application.
 */

//-----[ INCLUDES ]-----------------------------------------------------------//

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>


//-----[ PRIVATE FUNCTIONS ]--------------------------------------------------//

/******************************************************************************/
/*!
 * @brief  This function is the entry point for the Qt framework application.
 * @param  argc  Number of arguments passed to the application.
 * @param  argv  Contains the arguments passed to the application.
 * @return When the event loop terminates, exec() returns an exit code.
 */
int main(int argc, char * argv[])
{
    // Create an instance of QApplication to manage the GUI application
    QApplication a(argc, argv);

    // Create an instance of MainWindow, representing the main window
    MainWindow w;
    
    // Show the main window
    w.show();
    
    // Start the application's event loop
    return (a.exec());

}   /* main() */

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
