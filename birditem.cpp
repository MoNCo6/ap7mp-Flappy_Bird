#include "birditem.h"
#include <QTimer>
#include <QGraphicsScene>

// Konštruktor triedy BirdItem - inicializuje vtáčika a jeho animácie
BirdItem::BirdItem(QPixmap pixmap) :
    wingPosition(WingPosition::Up), // Nastavenie krídel do počiatočnej polohy
    wingDirection(0) // Počiatočný smer animácie krídel
{
    // Nastavenie počiatočného obrázka vtáčika
    setPixmap(pixmap);

    // Inicializácia časovača na animáciu krídel
    QTimer * birdWingsTimer = new QTimer(this);
    connect(birdWingsTimer, &QTimer::timeout,[=](){
        updatePixmap(); // Simulácia pohybu krídel zmenou obrázkov
    });

    // Spustenie časovača na pravidelné aktualizácie (každých 80 ms)
    birdWingsTimer -> start(80);

    // Nastavenie počiatočnej pozície vtáčika voči "zemi"
    groundPosition = scenePos().y() + 290;

    // Animácia padania vtáčika smerom k zemi
    yAnimation = new QPropertyAnimation(this, "y", this);
    yAnimation -> setStartValue(scenePos().y());
    yAnimation -> setEndValue(groundPosition);
    yAnimation -> setEasingCurve(QEasingCurve::InQuad); // Hladká animácia zrýchlenia
    yAnimation -> setDuration(1000); // Trvanie animácie pádu (1 sekunda)

    // Inicializácia animácie rotácie
    rotationAnimation = new QPropertyAnimation(this, "rotation", this);
}

// Metóda na aktualizáciu obrázkov vtáčika (pohyb krídel)
void BirdItem::updatePixmap()
{
    if(wingPosition == WingPosition::Middle){
        if(wingDirection){
            // Nastavenie obrázka krídel hore
            setPixmap(QPixmap(":/images/bird_up.png"));
            wingPosition = WingPosition::Up;
            wingDirection = 0;
        }else{
            // Nastavenie obrázka krídel dole
            setPixmap(QPixmap("::/images/bird_down.png"));
            wingPosition = WingPosition::Down;
            wingDirection = 1;
        }
    }else{
        // Nastavenie obrázka krídel do stredovej pozície
        setPixmap(QPixmap(":/images/bird_middle.png"));
        wingPosition = WingPosition::Middle;
    }
}

// Getter pre aktuálny uhol rotácie
qreal BirdItem::rotation() const
{
    return m_rotation;
}

// Setter pre uhol rotácie vtáčika
void BirdItem::setRotation(qreal rotation)
{
    m_rotation = rotation;

    // Výpočet transformácie podľa stredu objektu
    QPointF c = boundingRect().center(); // Stred grafického objektu
    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(rotation); // Rotácia
    t.translate(-c.x(), -c.y());
    setTransform(t); // Aplikácia transformácie
}

// Getter pre vertikálnu pozíciu
qreal BirdItem::y() const
{
    return m_y;
}

// Setter pre vertikálnu pozíciu
void BirdItem::setY(qreal y)
{
    moveBy(0,y-m_y); // Posunutie o rozdiel medzi starou a novou pozíciou
    m_y = y; // Aktualizácia hodnoty pozície
}

// Pohyb vtáčika smerom nahor
void BirdItem::shootUp()
{
    yAnimation -> stop(); // Zastavenie aktuálnej vertikálnej animácie
    rotationAnimation -> stop(); // Zastavenie rotácie

    qreal curPosY = y();  // Získanie aktuálnej vertikálnej pozície

    // Nastavenie novej vertikálnej animácie pre skok
    yAnimation -> setStartValue(curPosY);
    yAnimation -> setEndValue(curPosY - scene()->sceneRect().height()/8); // Skok nahor
    yAnimation -> setEasingCurve(QEasingCurve::OutQuad); // Plynulé spomalenie
    yAnimation -> setDuration(285); // Trvanie skoku (285 ms)

    // Po ukončení skoku skontroluje, či je potrebné spustiť pád
    connect(yAnimation,&QPropertyAnimation::finished,[=](){
        fallToGroundIfNecessary();
    });

    // Spustenie animácie skoku
    yAnimation -> start();

    // Nastavenie animácie rotácie počas skoku
    rotateTo(-20, 200, QEasingCurve::OutCubic); // Rotácia do mierneho uhla nahor
}

// Spustenie animácie letu vtáčika
void BirdItem::startFlying()
{
    yAnimation -> start(); // Spustenie animácie padania
    rotateTo(90, 1200, QEasingCurve::InQuad); // Rotácia o 90° za 1,2 sekundy
}

// Zastavenie pohybu vtáčika
void BirdItem::freezeInPlace()
{
    yAnimation -> stop(); // Zastavenie vertikálneho pohybu
    rotationAnimation -> stop(); // Zastavenie rotácie
}


// Metóda na plynulú animáciu rotácie vtáčika
void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation -> setStartValue(rotation()); // Počiatočný uhol
    rotationAnimation -> setEndValue(end); // Koncový uhol
    rotationAnimation -> setEasingCurve(curve); // Typ animácie (krivka)
    rotationAnimation -> setDuration(duration); // Trvanie animácie

    rotationAnimation ->start(); // Spustenie animácie
}

// Kontrola a spracovanie pádu vtáčika na zem, ak je to potrebné
void BirdItem::fallToGroundIfNecessary()
{
    // Kontrola, či vtáčik ešte nie je na zemi
    if(y() < groundPosition){
        rotationAnimation -> stop(); // Zastavenie prebiehajúcej rotácie

        // Nastavenie novej animácie pádu na zem
        yAnimation -> setStartValue(y()); // Počiatočná pozícia je aktuálna výška
        yAnimation -> setEasingCurve(QEasingCurve::InQuad);
        yAnimation -> setEndValue(groundPosition); // Konečná pozícia je zem
        yAnimation -> setDuration(1200);
        yAnimation -> start(); // Spustenie animácie pádu

        // Rotácia vtáčika pri páde na zem
        rotateTo(90, 1100, QEasingCurve::InCubic); // Rotácia o 90° počas 1,1 sekundy
    }
}
