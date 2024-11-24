# IHM_2425_E1
*Entrega del ejercicio del 8 puzzle*

## Ficheros principales desarrollados

### ```main.cpp```
En este fichero se define la función ```main```. Dentro de esta función se crea una instancia de ```QApplication``` para gestionar la interfaz gráfica y una instancia de ```MainWindow```, que representa la ventana principal de la aplicación.

### ```mainwindow.h```
Este fichero define la clase ```MainWindow```, que hereda de ```QMainWindow``` y representa la ventana principal de la aplicación. Incluye declaraciones de funciones públicas y privadas, así como variables para gestionar la interfaz de usuario, botones, un temporizador y un traductor de idiomas entre otros (los idiomas disponibles son inglés y español). La clase maneja la lógica del juego, incluyendo iniciar el juego, cambiar temas (los temas disponibles son el claro y oscuro) y lenguajes, y gestionar los movimientos de los botones. También se definen estructuras para almacenar información sobre los botones y sus posiciones en el tablero.

### ```mainwindow.cpp```
Implementación de las funciones de la clase ```MainWindow```.

### ```mainwindow.ui```
En este archivo se encuentra la interfaz gráfica de la aplicación. Cabe destacar el uso de la clase ```QStackedWidget```, que permite crear múltiples páginas con diferentes interfaces dentro de una sola ventana. Para alternar entre las páginas, se utiliza la función ```setCurrentWidget()```, que toma como argumento el ```QWidget``` que se desea mostrar en ese momento.

## Referencias del código desarrollado 

Para la elaboración del código se ha recopilado información de diversas fuentes. Estos códigos han sido utilizados como referencia para la implementación de la aplicación, por lo que se pueden observar similitudes, ya que el código ha sido adaptado al caso específico.

A continuación, dichas fuentes:

- **Documentación oficial de Qt:** (https://doc.qt.io/) búsqueda de información sobre funciones específicas y sus ejemplos de implementación.
- **Ecosia AI Chat:** (https://www.ecosia.org/chat) implementación del traductor, sugerencia de utilización de ```QStackedWidget```, dudas en general y resolución de algunos problemas de compilación.
