#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

// Konštruktor triedy Scene - inicializuje časovač na pridávanie stĺpov
Scene::Scene(QObject *parent) : QGraphicsScene(parent), gameOn(false), score(0), bestScore(0)
{
    // Inicializácia časovača na pravidelné pridávanie stĺpov
    setUpPillarTimer();
}

// Pridanie vtáčika do scény
void Scene::addBird()
{
    // Vytvorenie vtáčika s počiatočným obrázkom
    bird = new BirdItem(QPixmap(":/images/bird_up.png"));
    addItem(bird); // Pridanie vtáčika do scény
}

//Metóda pre spustenie hry
void Scene::startGame()
{
    // Spustenie letu vtáčika
    bird -> startFlying();

    // Spustenie časovača na pridávanie stĺpov
    if(!pillarTimer -> isActive()){
        cleanPillars(); // Vymazanie existujúcich stĺpov
        setGameOn(true); // Nastavenie hry ako aktívnej
        setScore(0); // Vynulovanie skóre
        hideGameOverGraphics(); // Skrytie grafiky pre koniec hry
        pillarTimer -> start(1000); // Časovač sa spúšťa každú sekundu
    }
}

// Nastavenie časovača pre generovanie stĺpov
void Scene::setUpPillarTimer()
{
    // Vytvorenie časovača
    pillarTimer = new QTimer(this);

    // Prepojenie časovača s funkciou na pridávanie stĺpov
    connect(pillarTimer, &QTimer::timeout,[=](){

        // Pri každom timeout-e vytvorí a pridá nový stĺp
        PillarItem * pillarItem = new PillarItem();
        connect(pillarItem, &PillarItem::collideFail,[=](){
            pillarTimer -> stop(); // Zastavenie časovača
            freezeBirdAndPillarsInPlace(); // Zastavenie pohybu vtáčika a stĺpov
            setGameOn(false); // Nastavenie hry ako neaktívnej
            showGameOverGraphics(); // Zobrazenie grafiky pre koniec hry
        });
        addItem(pillarItem); // Pridanie nového stĺpu do scény
    });
}

// Zastavenie pohybu vtáčika a stĺpov
void Scene::freezeBirdAndPillarsInPlace()
{
    // Zastavenie vtáčika
    bird -> freezeInPlace();

    // Zastavenie stĺpov
    QList<QGraphicsItem *> sceneItem = items();
    foreach(QGraphicsItem *item, sceneItem){
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar){
            pillar ->freezeInPlace(); // Zastavenie pohybu stĺpa
        }
    }
}

// Nastavenie skóre
void Scene::setScore(int newScore)
{
    score = newScore; // Aktualizácia aktuálneho skóre
}

// Získanie stavu hry
bool Scene::getGameOn() const
{
    return gameOn; // Vráti, či je hra aktívna
}

// Nastavenie stavu hry
void Scene::setGameOn(bool value)
{
    gameOn = value; // Nastavenie aktívneho/ neaktívneho stavu hry
}

// Zvýšenie skóre a aktualizácia najlepšieho skóre
void Scene::incrementScore()
{
    score++; // Zvýšenie aktuálneho skóre
    if(score > bestScore){
        bestScore = score; // Aktualizácia najlepšieho skóre
    }
}

// Spracovanie stlačenia klávesy
void Scene::keyPressEvent(QKeyEvent *event)
{
    // Ak je stlačený medzerník
    if(event -> key() == Qt::Key_Space){
        if(gameOn){
            bird -> shootUp(); // Vtáčik "vyskočí" po stlačení medzerníka
        }
    }
    QGraphicsScene::keyPressEvent(event); // Volanie základnej implementácie
}

// Spracovanie stlačenia myši
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Ak je stlačené ľavé tlačidlo myši
    if(event -> button() == Qt::LeftButton){
        if(gameOn){
            bird -> shootUp(); // Vtáčik "vyskočí" po kliknutí myšou
        }
    }
    QGraphicsScene::mousePressEvent(event); // Volanie základnej implementácie
}

// Zobrazenie grafiky pre koniec hry
void Scene::showGameOverGraphics()
{
    // Vytvorenie grafického objektu pre obrázok konca hry
    gameOverPix = new QGraphicsPixmapItem(QPixmap(":/images/game_over.png"));
    addItem(gameOverPix); // Pridanie grafiky do scény

    // Nastavenie pozície obrázku na stred scény
    gameOverPix -> setPos(QPointF(0,0) - QPointF(gameOverPix -> boundingRect().width()/2,
                                                gameOverPix->boundingRect().height()/2));

    // Vytvorenie textového objektu pre zobrazenie skóre
    scoreTextItem = new QGraphicsTextItem();

    // Vytvorenie HTML reťazca, ktorý zobrazuje aktuálne skóre a najlepšie skóre
    QString htmlString = "<p> Score: " + QString::number(score) + " </p>" + "<p> Best Score: " + QString::number(bestScore) + " </p>";

    // Nastavenie fontu a štýlu textového objektu
    QFont mFont("Consolas", 30, QFont::Bold);
    scoreTextItem -> setHtml(htmlString); // Zobrazenie skóre a najlepšieho skóre
    scoreTextItem -> setFont(mFont);
    scoreTextItem -> setDefaultTextColor(Qt::black);
    addItem(scoreTextItem); // Pridanie textu do hernej scény

    // Nastavenie pozície textu pod obrázok "Game Over"
    scoreTextItem -> setPos(QPointF(0,0) - QPointF(scoreTextItem -> boundingRect().width()/2, -gameOverPix -> boundingRect().height()/2));
}

// Skrytie grafiky pre koniec hry
void Scene::hideGameOverGraphics()
{
    // Odstránenie a uvoľnenie objektu grafiky "Game Over", ak existuje
    if(gameOverPix){
        removeItem(gameOverPix); // Odstránenie zo scény
        delete gameOverPix; // Uvoľnenie pamäte
        gameOverPix = nullptr; // Resetovanie ukazovateľa
    }
    // Odstránenie a uvoľnenie textového objektu skóre, ak existuje
    if(scoreTextItem){
        removeItem(scoreTextItem); // Odstránenie zo scény
        delete scoreTextItem; // Uvoľnenie pamäte
        scoreTextItem = nullptr; // Resetovanie ukazovateľa
    }
}

// Vymazanie všetkých stĺpov zo scény
void Scene::cleanPillars()
{
    // Získanie všetkých objektov aktuálne umiestnených v scéne
    QList<QGraphicsItem *> sceneItem = items();

    // Prechádzanie všetkých objektov a hľadanie stĺpov (PillarItem)
    foreach(QGraphicsItem *item, sceneItem){
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);

        // Ak je objekt stĺpom, odstráni ho zo scény a uvoľní pamäť
        if(pillar){
            removeItem(pillar); // Odstránenie stĺpu zo scény
            delete pillar; // Uvoľnenie pamäte
        }
    }
}
