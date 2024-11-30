#include "birditem.h"
#include <QTimer>

// Konštruktor triedy BirdItem
BirdItem::BirdItem(QPixmap pixmap) :
    wingPosition(WingPosition::Up), // Počiatočná poloha krídel
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
    yAnimation -> setEasingCurve(QEasingCurve::InQuad); // Spomalené padanie
    yAnimation -> setDuration(1000); // Padanie trvá 1 sekundu

    // Spustenie animácie padania
    yAnimation -> start();

    // Inicializácia animácie rotácie
    rotationAnimation = new QPropertyAnimation(this, "rotation", this);
    rotateTo(90, 1200, QEasingCurve::InQuad); // Rotácia o 90° za 1,2 sekundy
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

// Setter pre rotáciu vtáčika
void BirdItem::setRotation(qreal rotation)
{
    m_rotation = rotation;

    // Výpočet transformácie podľa stredu objektu
    QPointF c = boundingRect().center(); // Stred vtáčika
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

// Setter pre vertikálnu pozíciu a aktualizácia pozície
void BirdItem::setY(qreal y)
{
    moveBy(0,y-m_y); // Posunutie o rozdiel medzi starou a novou pozíciou
    m_y = y; // Aktualizácia hodnoty pozície
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
