#include "widget.h"
#include "./ui_widget.h"
#include <QGraphicsPixmapItem>

// Konštruktor triedy Widget - inicializuje UI a grafickú scénu
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget), settingsButton(new QPushButton(this)),
    sidebar(new QWidget(this))
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

    // Initialize UI components
    initializeSettingsButton();
    initializeSidebar();
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

    sidebar->hide();
}

void Widget::toggleSidebar()
{
    if (sidebar->isVisible()) {
        sidebar->hide();
    } else {
        sidebar->show();
        sidebar->raise();
    }
}

// Initializes the settings button
void Widget::initializeSettingsButton()
{
    settingsButton->setGeometry(20, 20, 50, 50);  // Add gap to both x and y

    // Load and set the gear icon
    QPixmap pixmap(":/images/gear.png");
    QIcon gearIcon(pixmap);
    settingsButton->setIcon(gearIcon);  // Set the icon for the button
    settingsButton->setIconSize(QSize(50, 50));  // Match the button size

    // Make the button transparent (remove borders and background)
    settingsButton->setFlat(true);  // Removes button borders and background
    settingsButton->setStyleSheet(
        "background: transparent;"  // Transparent background
        "border: none;"             // No border
        );

    // Connect button signal to the sidebar toggle function
    connect(settingsButton, &QPushButton::clicked, this, &Widget::toggleSidebar);
}

// Initializes the sidebar and its contents
void Widget::initializeSidebar()
{
    sidebar->setGeometry(width() - 200, 0, 200, height());
    sidebar->setStyleSheet("background-color: black; border: 1px solid black;");
    sidebar->hide();

    // Sidebar layout
    sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setAlignment(Qt::AlignTop);

    // Add settings label
    settingsLabel = new QLabel("Settings", sidebar);
    settingsLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px; color: white;");
    sidebarLayout->addWidget(settingsLabel);

    // Add sound toggle button
    QLabel *soundLabel = new QLabel("Sound Control:", sidebar);
    soundLabel->setStyleSheet("font-size: 14px; color: white; margin-bottom: 5px;");
    sidebarLayout->addWidget(soundLabel);

    QPushButton *soundToggleButton = new QPushButton("OFF", sidebar);
    soundToggleButton->setCheckable(true);
    soundToggleButton->setStyleSheet(
        "QPushButton {"
        "    background-color: red;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    border-radius: 15px;"
        "    padding: 5px;"
        "}"
        "QPushButton:checked {"
        "    background-color: green;"
        "    color: white;"
        "}"
        );
    sidebarLayout->addWidget(soundToggleButton);
    connect(soundToggleButton, &QPushButton::toggled, this, [soundToggleButton](bool checked) {
        soundToggleButton->setText(checked ? "ON" : "OFF");
        qDebug() << "Sound toggled:" << (checked ? "ON" : "OFF");
    });

    // Add volume slider
    QLabel *volumeLabel = new QLabel("Music Volume:", sidebar);
    volumeLabel->setStyleSheet("font-size: 14px; color: white; margin-top: 10px; margin-bottom: 5px;");
    sidebarLayout->addWidget(volumeLabel);

    volumeSlider = new QSlider(Qt::Horizontal, sidebar);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);
    sidebarLayout->addWidget(volumeSlider);
    connect(volumeSlider, &QSlider::valueChanged, this, [](int value) {
        qDebug() << "Volume set to:" << value;
    });
}

QPushButton *Widget::getSettingsButton() const
{
    return settingsButton;
}
