#ifndef CHATSCONTROLER_H
#define CHATSCONTROLER_H

#include <QObject>
#include <include/guiadapter.h>
#include <include/models/chats_filter_proxy_model.h>

namespace m2 {
namespace gui {
namespace controler {


class ChatsControler : public QObject
{
    Q_OBJECT
public:
    explicit ChatsControler(QObject *parent = nullptr);
    static void declareQML();

    Q_INVOKABLE void changeCurrentChat(const int &index);
    Q_INVOKABLE void addNewChat(const QString &uuid);

public slots:
    void newMessage(ModelsElements::MessageData message);
    void newChat(ModelsElements::ChatData chat);
    void chatsLoaded(QHash<QString, ModelsElements::ChatData> chats);

private:
    GuiAdapter* adapter;
    ChatsFilterProxyModel* chatsModel;
};

}
}
}

#endif // CHATSCONTROLER_H
