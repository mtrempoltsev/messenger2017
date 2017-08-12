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

    Q_INVOKABLE void sendNewMessage(const QString &text);
    Q_INVOKABLE void loadHistory();


public slots:
    void loadChat(QVector<ModelsElements::MessageData> messages);
    void receiveMessage(ModelsElements::MessageData message);

signals:
    void newMessage();


private:
    MessagesModel* messagesModel;

    GuiAdapter* adapter;
};

}
}
}


#endif // MESSAGESCONTROLER_H
