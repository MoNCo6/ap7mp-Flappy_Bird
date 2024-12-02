#include "widget.h"
#include "./ui_widget.h"
#include <QGraphicsPixmapItem>

// Konštruktor triedy Widget - inicializuje UI a grafickú scénu
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this); // Nastavenie používateľského rozhrania.

    // Inicializácia hernej scény
    scene = new Scene(this); // Vytvorenie novej inštancie hernej scény
    scene -> setSceneRect(-250,-300,500,600); // Nastavenie rozmerov scény

    // Pridanie pozadia do scény
    QGraphicsPixmapItem * pixItem = new QGraphicsPixmapItem(QPixmap(":/images/background.jpg"));
    scene -> addItem(pixItem); // Pridanie obrázku pozadia do scény

    // Vycentrovanie pozadia
    pixItem -> setPos(QPointF(0,0) - QPointF(pixItem->boundingRect().width()/2,
                                            pixItem->boundingRect().height()/2));

    // Pridanie pomocných čiar na testovanie scény
    // scene -> addLine(-400,0,400,0,QPen(Qt::blue)); // Horizontálna čiara
    // scene -> addLine(0,-400,0,400,QPen(Qt::blue)); // Vertikálna čiara

    ui -> graphicsView -> setScene(scene); // Prepojenie scény s grafickým pohľadom.

    // Pridanie vtáčika do scény
    scene -> addBird();
}

// Destruktor triedy Widget
Widget::~Widget()
{
    delete ui;
}

// Funkcia pre spracovanie kliknutia na tlačidlo "Start Game"
void Widget::on_startGameButton_clicked()
{
    scene -> startGame(); // Spustenie herného cyklu.
}

