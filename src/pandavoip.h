#ifndef PandaVOIP_H
#define PandaVOIP_H

#include <QMainWindow>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QListWidget>
#include <QString>
#include "voipcontroller.h"

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
        void updateVoiceUsers(std::vector<QString>);
        void updateChatUsers(std::vector<QString>);

    private:
        void setup_PandaVOIP();

        Ui::PandaVOIP *ui;
        QListWidget* general_chat_list;
        QListWidget* general_voice_list;
        VoipController* voipController;
};

#endif // PandaVOIP_H
