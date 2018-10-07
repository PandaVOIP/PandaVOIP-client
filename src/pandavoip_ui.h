#ifndef PANDAVOIPUI_H
#define PANDAVOIPUI_H
#include "account_ui.h"
#include "settings_ui.h"
#include "pandavoip.h"
#include <QMainWindow>
#include <QAudioInput>
#include <QAudioOutput>
#include <QShowEvent>
#include <QBuffer>
#include <QListWidget>
#include <QString>

namespace Ui {
    class PandaVOIPUI;
}

struct channel_widget {
    int id;
    QString channel_name;
    QWidget *widget;
    QLayout *layout;
    QListWidget *user_list;
};

class PandaVOIPUI : public QMainWindow {
    Q_OBJECT

    private:
        Ui::PandaVOIPUI *ui;
        QListWidget* general_list;
        Account *account;
        Settings *settings;
        map<QString, struct channel_widget> channels;

        void setupPandaVOIP();
        void createUserList(QString channel_name);

    public:
        PandaVOIP *panda_voip;

        explicit PandaVOIPUI(QWidget *parent = nullptr);
        ~PandaVOIPUI();

    public slots:
        void onMessageBoxReturned();
        void onSettingsClicked();
        void openLoginPopup();
        void onCloseClicked();
        void onMaximizeClicked();
        void onMinimizeClicked();
        void updateVoiceUsers(std::vector<QString>);
        void updateChatUsers(std::vector<QString>);
        void onNewMessage(QString, QString);
        void createChannelWidget(QString channel_name);
        void addUser(QString channel_name, QString username);
};

#endif // PANDAVOIPUI_H
