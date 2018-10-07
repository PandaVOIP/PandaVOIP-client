#ifndef SETTINGSUI_H
#define SETTINGSUI_H
#include <QWidget>

namespace Ui {
    class Settings;
}

class Settings : public QWidget {
    Q_OBJECT

    private:
        Ui::Settings *ui;

    public:
        explicit Settings(QWidget *parent = nullptr);
        ~Settings();
};

#endif // SETTINGSUI_H
