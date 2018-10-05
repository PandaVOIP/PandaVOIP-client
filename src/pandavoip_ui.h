#ifndef PandaVOIP_H
#define PandaVOIP_H
#include <QMainWindow>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QListWidget>
#include <QString>
#include "voipcontroller.h"
#include "account_ui.h"
#include "settings_ui.h"

namespace Ui {
    class PandaVOIPUI;
}

class PandaVOIPUI : public QMainWindow {
    Q_OBJECT

    private:
        void setupPandaVOIP();
        void loginPopup();

        Ui::PandaVOIPUI *ui;
        QListWidget* general_list;
        Account *account = NULL;
        Settings *settings = NULL;

    public:
        explicit PandaVOIPUI(QWidget *parent = 0);
        ~PandaVOIPUI();

    public slots:
        void onMessageBoxReturned();
        void onSettingsClicked();
        void onLoginPopup();
        void onCloseClicked();
        void onMaximizeClicked();
        void onMinimizeClicked();
        void updateVoiceUsers(std::vector<QString>);
        void updateChatUsers(std::vector<QString>);
        void onNewMessage(QString, QString);
};

#endif // PandaVOIP_H
