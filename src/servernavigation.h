#ifndef SERVERNAVIGATION_H
#define SERVERNAVIGATION_H

#include <QWidget>
#include <QListWidget>

struct server_node {
    QWidget *node;
    QLayout *node_layout;
    QListWidget *user_list;
};

class ServerNavigation : public QWidget {
    Q_OBJECT

    private:
        void createNode(const QString &node_name, QLayout *parent_layout = nullptr, QWidget *parent = nullptr);
        void createUserList();

        server_node *server_navigation;

    public:
        explicit ServerNavigation(QWidget *parent = nullptr);
        void newServerNode(const QString &node_name, QLayout *parent_layout = nullptr, QWidget *parent = nullptr);
        void addUser(const QString &username);
};

#endif // SERVERNAVIGATION_H
