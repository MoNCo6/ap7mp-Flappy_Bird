#include "pillaritem.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QGraphicsScene>

// Konštruktor triedy PillarItem - nastavuje grafické objekty a ich pozíciu
PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillar_top.png"))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillar_bottom.png")))
{
    // Nastavenie pozície horného stĺpa
    // Horný stĺp je posunutý nahor o svoju výšku + 60 bodov, aby vytvoril medzeru
    topPillar -> setPos(QPointF(0,0) - QPointF(topPillar -> boundingRect().width()/2,
                                              topPillar->boundingRect().height() + 60));

    // Nastavenie pozície dolného stĺpa
    // Dolný stĺp je posunutý nadol o 60 bodov, čím sa zachováva medzera medzi stĺpmi
    bottomPillar -> setPos(QPointF(0,0) + QPointF(-bottomPillar -> boundingRect().width()/2,
                                                 60));

    // Pridanie stĺpov do skupiny objektov
    addToGroup(topPillar);
    addToGroup(bottomPillar);

    // Nastavenie náhodnej pozície stĺpov
    yPos = QRandomGenerator::global() -> bounded(150); // Vertikálna náhodnosť
    int xRandomizer = QRandomGenerator::global() -> bounded(200); // Horizontálny offset

    // Nastavenie počiatočnej pozície stĺpa mimo obrazovky
    setPos(QPointF(0,0) + QPointF(260 + xRandomizer, yPos));

    // Inicializácia animácie na posun stĺpov
    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation -> setStartValue(260 + xRandomizer);
    xAnimation -> setEndValue(-260);
    xAnimation -> setEasingCurve(QEasingCurve::Linear);
    xAnimation -> setDuration(1500);

    // Po ukončení animácie sa stĺp odstráni
    connect(xAnimation, &QPropertyAnimation::finished,[=](){
        scene() -> removeItem(this); // Odstránenie stĺpa zo scény
        delete this; // Deštrukcia stĺpa
    });

    xAnimation -> start(); // Spustenie animácie
}

// Destruktor triedy PillarItem
PillarItem::~PillarItem()
{
    qDebug() << "Deleting pillar";
}

// Getter pre hodnotu osi X
qreal PillarItem::x() const
{
    return m_x;
}

// Setter pre hodnotu osi X
void PillarItem::setX(qreal x)
{
    m_x = x;
    setPos(QPointF(0,0) + QPointF(x, yPos)); // Aktualizácia pozície stĺpov na scéne
}
