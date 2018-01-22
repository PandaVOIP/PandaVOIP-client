#ifndef PandaVOIP_H
#define PandaVOIP_H

#include <QMainWindow>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QListWidget>
#include <QString>
#include "lib/voipcontroller.h"
#include "account.h"
#include "settings.h"

namespace Ui{
    class PandaVOIP;
}

class PandaVOIP : public QMainWindow{
    Q_OBJECT

    private:
        void setupPandaVOIP();
        void loginPopup();

        Ui::PandaVOIP *ui;
        QListWidget* general_list;
        VoipController* voipController;
        Account *account = NULL;
        Settings *settings = NULL;

    public:
        explicit PandaVOIP(QWidget *parent = 0);
        ~PandaVOIP();
        void newMessage(QString, QString);

    public slots:
        void onMessageBoxReturned();
        void onSettingsClicked();
        void onCloseClicked();
        void onMaximizeClicked();
        void onMinimizeClicked();
        void updateVoiceUsers(std::vector<QString>);
        void updateChatUsers(std::vector<QString>);
};

#endif // PandaVOIP_H
