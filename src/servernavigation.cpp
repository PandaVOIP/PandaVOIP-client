#include "servernavigation.h"
#include <QPushButton>
#include <QListWidget>
#include <QLayout>

ServerNavigation::ServerNavigation(QWidget *parent) : QWidget(parent)
{
    this->server_navigation = (server_node *)malloc(sizeof(struct server_node));
}

void ServerNavigation::newServerNode(const QString &node_name, QLayout *parent_layout, QWidget *parent) {
    createNode(node_name, parent_layout, parent);
    createUserList();
}

void ServerNavigation::createNode(const QString &node_name, QLayout *parent_layout, QWidget *parent) {
    //parent_layout->setAlignment(Qt::AlignTop);

    QWidget *node = new QWidget(parent);
    node->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QVBoxLayout *node_layout = new QVBoxLayout(node);

    QPushButton *join_node = new QPushButton(node_name, node);
    node_layout->addWidget(join_node);

    parent_layout->addWidget(node);

    this->server_navigation->node = node;
    this->server_navigation->node_layout = node_layout;
}

void ServerNavigation::createUserList() {
    QListWidget *user_list = new QListWidget(this->server_navigation->node);
    user_list->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

    this->server_navigation->node_layout->addWidget(user_list);

    this->server_navigation->user_list = user_list;
}

void ServerNavigation::addUser(const QString &username) {
    this->server_navigation->user_list->addItem(username);
}
