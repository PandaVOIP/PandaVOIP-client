#ifndef PandaVOIP_H
#define PandaVOIP_H

#include <QMainWindow>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QListWidget>
#include <QString>
#include "voipcontroller.h"
#include "account.h"
#include "settings.h"

namespace Ui{
    class PandaVOIP;
}

class PandaVOIP : public QMainWindow{
    Q_OBJECT

    public:
        explicit PandaVOIP(QWidget *parent = 0);
        ~PandaVOIP();
        void new_message(QString, QString);

    public slots:
        void on_message_box_returned();
        void on_settings_clicked();
        void on_close_clicked();
        void updateVoiceUsers(std::vector<QString>);

    private:
        void setup_PandaVOIP();
        void login_popup();

        Ui::PandaVOIP *ui;
        QListWidget* general_list;
        VoipController* voipController;
        Account *account = NULL;
        Settings *settings = NULL;
};

#endif // PandaVOIP_H
