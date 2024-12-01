#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

// Konštruktor triedy Scene - inicializuje časovač na pridávanie stĺpov
Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
    // Inicializácia časovača na pravidelné pridávanie stĺpov
    setUpPillarTimer();
}

// Pridanie vtáčika do scény
void Scene::addBird()
{
    // Vytvorenie vtáčika s počiatočným obrázkom
    bird = new BirdItem(QPixmap(":/images/bird_up.png"));
    addItem(bird);
}

// Nastavenie časovača pre generovanie stĺpov
void Scene::setUpPillarTimer()
{
    // Vytvorenie časovača
    pillarTimer = new QTimer(this);

    // Prepojenie časovača s funkciou, ktorá vytvára nové stĺpy
    connect(pillarTimer, &QTimer::timeout,[=](){

        // Pri každom timeout-e vytvorí a pridá nový stĺp
        PillarItem * pillarItem = new PillarItem();
        addItem(pillarItem);
    });

    // Spustenie časovača - nový stĺp sa generuje každú 1 sekundu
    pillarTimer -> start(1000);
}

// Spracovanie stlačenia klávesy
void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event -> key() == Qt::Key_Space){
        bird -> shootUp(); // Po stlačení medzerníka vtáčik "vyskočí"
    }
    QGraphicsScene::keyPressEvent(event); // Volanie základnej implementácie
}

// Spracovanie stlačenia myši
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event -> button() == Qt::LeftButton){
        bird -> shootUp(); // Po stlačení myši vtáčik "vyskočí"
    }
    QGraphicsScene::mousePressEvent(event); // Volanie základnej implementácie
}
