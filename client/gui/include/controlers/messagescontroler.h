#ifndef MESSAGESCONTROLER_H
#define MESSAGESCONTROLER_H

#include <QObject>

#include <include/models/messages_model.h>
#include <include/models/models_elements.h>
#include <include/guiadapter.h>


namespace m2 {
namespace gui {
namespace controler {


class MessagesControler : public QObject
{
    Q_OBJECT

public:
    explicit MessagesControler(QObject* parent = 0);
    static void declareQML();

public slots:
    Q_INVOKABLE void loadHistory();
    Q_INVOKABLE void sendNewMessage(const QString &text);
    Q_INVOKABLE void getChatData();


    void loadChat(QVector<ModelsElements::MessageData> messages);
    void receiveMessage(const ModelsElements::MessageData &message);
    void changeChat();

signals:
    void newMessage();
    void chatChanged(QString name, QString avatar);

private:
    MessagesModel* messagesModel;
    GuiAdapter* adapter;
};

}
}
}


#endif // MESSAGESCONTROLER_H
