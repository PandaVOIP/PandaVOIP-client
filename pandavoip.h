#ifndef PandaVOIP_H
#define PandaVOIP_H

#include <QMainWindow>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>

namespace Ui{
    class PandaVOIP;
}

class PandaVOIP : public QMainWindow{
    Q_OBJECT

    public:
        explicit PandaVOIP(QWidget *parent = 0);
        ~PandaVOIP();

    public slots:
        void on_message_box_returned();

    private:
        Ui::PandaVOIP *ui;

        void setup_PandaVOIP();
};

#endif // PandaVOIP_H
