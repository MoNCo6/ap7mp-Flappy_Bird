#include "widget.h"
#include "./ui_widget.h"
#include <QGraphicsPixmapItem>
#include "birditem.h"

// Konštruktor triedy Widget
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    // Inicializácia hernej scény
    scene = new Scene(this); // Vytvorenie novej inštancie hernej scény
    scene -> setSceneRect(-250,-300,500,600); // Nastavenie rozmerov scény

    // Pridanie pozadia do scény
    QGraphicsPixmapItem * pixItem = new QGraphicsPixmapItem(QPixmap(":/images/background.jpg"));
    scene -> addItem(pixItem);

    // Vycentrovanie pozadia
    pixItem -> setPos(QPointF(0,0) - QPointF(pixItem->boundingRect().width()/2,
                                            pixItem->boundingRect().height()/2));

    // Pridanie pomocných čiar na testovanie scény
    scene -> addLine(-400,0,400,0,QPen(Qt::blue)); // Horizontálna čiara
    scene -> addLine(0,-400,0,400,QPen(Qt::blue)); // Vertikálna čiara

    // Pridanie vtáčika do scény
    BirdItem * bird = new BirdItem(QPixmap(":/images/bird_up.png"));
    scene -> addItem(bird);

    ui -> graphicsView -> setScene(scene);
}

// Destruktor triedy Widget
Widget::~Widget()
{
    delete ui;
}
