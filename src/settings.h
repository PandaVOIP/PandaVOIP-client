#ifndef SETTINGS_H
#define SETTINGS_H

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

#endif // SETTINGS_H
