#include "pillaritem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include "birditem.h"
#include "scene.h"

// Konštruktor triedy PillarItem - nastavuje grafické objekty a ich pozíciu
PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillar_top.png"))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillar_bottom.png"))),
    pastBird(false) // Zaznamenáva, či vtáčik prešiel stĺpom
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

    // Nastavenie náhodnej vertikálnej pozície stĺpov (y-ová náhodnosť)
    yPos = QRandomGenerator::global() -> bounded(150);

    // Pridanie horizontálneho posunu (x-ová náhodnosť)
    int xRandomizer = QRandomGenerator::global() -> bounded(200);

    // Nastavenie počiatočnej pozície stĺpa mimo obrazovky (vpravo)
    setPos(QPointF(0,0) + QPointF(260 + xRandomizer, yPos));

    // Inicializácia animácie pohybu stĺpov
    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation -> setStartValue(260 + xRandomizer); // Začiatok animácie (pravá strana)
    xAnimation -> setEndValue(-260); // Koniec animácie (ľavá strana)
    xAnimation -> setEasingCurve(QEasingCurve::Linear); // Nastavenie lineárneho pohybu
    xAnimation -> setDuration(1500); // Trvanie animácie (1,5 sekundy)

    // Prepojenie ukončenia animácie s odstránením stĺpa
    connect(xAnimation, &QPropertyAnimation::finished,[=](){
        scene() -> removeItem(this); // Odstránenie stĺpa zo scény
        delete this; // Deštrukcia stĺpa
    });

    xAnimation -> start(); // Spustenie animácie stĺpa
}

// Destruktor triedy PillarItem
PillarItem::~PillarItem()
{
    delete topPillar;
    delete bottomPillar;
}

// Getter pre hodnotu osi X
qreal PillarItem::x() const
{
    return m_x; // Vráti aktuálnu hodnotu osi X
}

// Setter pre hodnotu osi X - aktualizuje pozíciu stĺpov
void PillarItem::setX(qreal x)
{
    m_x = x; // Aktualizácia hodnoty X

    // Skontroluje, či vtáčik preletel stĺpom
    if(x < 0 && !pastBird){
        pastBird = true; // Označuje, že vtáčik prešiel
        QGraphicsScene * mScene = scene(); // Získa aktuálnu scénu
        Scene * myScene = dynamic_cast<Scene *>(mScene); // Skonvertuje na typ Scene
        if(myScene){
            myScene -> incrementScore(); // Zvýšenie skóre
        }
    }

    // Skontroluje kolíziu stĺpa s vtáčikom
    if(collidesWithBird()){
        emit collideFail(); // Vydá signál kolízie
    }

    // Nastaví novú pozíciu stĺpa na scéne
    setPos(QPointF(0,0) + QPointF(x, yPos));
}

// Zastavenie animácie stĺpa
void PillarItem::freezeInPlace()
{
    xAnimation -> stop(); // Zastavenie animácie
}

// Kontrola kolízie stĺpa s vtáčikom
bool PillarItem::collidesWithBird()
{
    // Získanie všetkých objektov, ktoré kolidujú s horným a dolným stĺpom
    QList<QGraphicsItem*> collidingItems = topPillar -> collidingItems();
    collidingItems.append(bottomPillar -> collidingItems());

    // Skontroluje, či medzi kolidujúcimi objektmi je vtáčik
    foreach (QGraphicsItem * item, collidingItems) {
        BirdItem * birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem){
            return true; // Ak došlo ku kolízii, vráti true
        }
    }
    return false; // Žiadna kolízia
}
