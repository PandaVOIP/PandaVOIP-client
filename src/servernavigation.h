#ifndef SERVERNAVIGATION_H
#define SERVERNAVIGATION_H

#include <QWidget>
#include <QListWidget>

struct server_node {
    QWidget *node;
    QLayout *node_layout;
    QListWidget *user_list;
};

class ServerNavigation : public QWidget
{
    Q_OBJECT

    private:
        void create_node(const QString &node_name, QLayout *parent_layout = nullptr, QWidget *parent = nullptr);
        void create_user_list();

        server_node *server_navigation;

    public:
        explicit ServerNavigation(QWidget *parent = nullptr);
        void new_server_node(const QString &node_name, QLayout *parent_layout = nullptr, QWidget *parent = nullptr);
        void add_user(const QString &username);
};

#endif // SERVERNAVIGATION_H
