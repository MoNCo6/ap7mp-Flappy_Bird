#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>
#include "scene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

    QPushButton *getSettingsButton() const;

private slots:
    void on_startGameButton_clicked();

    void toggleSidebar();  // Slot to show/hide the sidebar

private:
    void initializeSettingsButton(); // Modular function to initialize settings button

    void initializeSidebar();        // Modular function to initialize sidebar

    Ui::Widget *ui;

    Scene * scene;

    QPushButton *settingsButton;  // Button for opening the sidebar
    QWidget *sidebar;             // Sidebar widget
    QVBoxLayout *sidebarLayout;      // Layout for sidebar content
    QLabel *settingsLabel;           // Title label
    QCheckBox *soundCheckBox;        // Option for enabling/disabling sound
    QSlider *volumeSlider;           // Slider to adjust volume
};
#endif // WIDGET_H
