#include "scene.h"

// Konštruktor triedy Scene
Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
    // Inicializácia časovača na pravidelné pridávanie stĺpov
    setUpPillarTimer();
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
