#include "include/guiadapter.h"

#include <QDebug>
#include <QQmlContext>

#include "core_dispatcher.h"
#include <functional>

using m2::core::CoreDispatcher;

namespace m2 {
namespace gui {
namespace controler {

using namespace ModelsElements;

GuiAdapter *GuiAdapter::getGuiAdapter() {
  static GuiAdapter adapter;
  return &adapter;
}

GuiAdapter::GuiAdapter() {
  messages = new MessagesModel();
  chats = new ChatsFilterProxyModel();
  contacts = new ContactsModel();
  qDebug() << "create gui adapter";
}

GuiAdapter::~GuiAdapter() {
  delete messages;
  delete chats;
  delete contacts;

  qDebug() << "delete guiadapter";
}

void GuiAdapter::setDispatcher(
    std::shared_ptr<m2::core::CoreDispatcher> dispatcher) {
  dispatcher_ = dispatcher;
}

void GuiAdapter::addModelsToEngineRoot(QQmlApplicationEngine *engine) {
  engine->rootContext()->setContextProperty("chatModel", chats);
  engine->rootContext()->setContextProperty("messagesModel", messages);
  engine->rootContext()->setContextProperty("contactsModel", contacts);
}

QString GuiAdapter::getMyUuid() const { return myUuid; }

void GuiAdapter::setMyUuidCallback(const std::string &value) {
  myUuid = QString::fromStdString(value);
}

QString GuiAdapter::getCurrentChatID() const { return currentChatID; }

void GuiAdapter::changeCurrentChat(const QString &ID) {
  currentChatID = ID;
  emit currentChatChanged(ID);
}

MessagesModel *GuiAdapter::getMessagesModel() const { return messages; }

ChatsFilterProxyModel *GuiAdapter::getChatsModel() const { return chats; }

ContactsModel *GuiAdapter::getContactsModel() const { return contacts; }

void GuiAdapter::getMyUuidCallback(std::string uuid) {
  myUuid.fromStdString(uuid);
}

void GuiAdapter::connectionLostCallback() {}

QStringList GuiAdapter::getServerList() {
  ///запросим у сервера список серверов
  std::list<std::string> list = dispatcher_.get()->GetServerList();
  //  list.push_back("neEbaServer");
  //  list.push_back("ebaServer");

  QStringList servers;
  foreach (auto item, list) { servers.append(QString::fromStdString(item)); }

  return servers;
}

void GuiAdapter::loginToServer() {
  m2::LoginHandler handler;
  handler.onCompletion = [&](std::string id) { loginSuccessed(); };
  handler.onError = [&](Error &&error) {
    std::cout << "You're a loser.";
    QString qstr = QString::fromStdString(error.message);
    loginFailed(qstr);
  };
  dispatcher_.get()->Login(handler);

  ///пинаем ядро
  // loginToServerCallback();
}

// void GuiAdapter::loginToServerCallback() {
//  // loginSuccessed();
//  ///а здесь пока заглушка
//  //  QTimer *timer = new QTimer();
//  //  connect(timer, SIGNAL(timeout()), this, SIGNAL(loginSuccessed()));
//  //  connect(timer, SIGNAL(timeout()), timer, SLOT(deleteLater()));
//  //  timer->start(2000);
//}

void GuiAdapter::registerToServer(const int &serverIndex) {
  // registerToServerCallback();
  m2::RegisterHandler handler;
  handler.onCompletion = [&]() { registrationSuccessed(); };
  handler.onError = [&](Error &&error) {
    std::cout << "You're a loser (registration).";
    QString qstr = QString::fromStdString(error.message);
    registrationFailed(qstr);
  };
  // FIXME: serverIndex
  dispatcher_.get()->RegisterUser("server 1", handler);
}

// void GuiAdapter::registerToServerCallback() {
//  ///а здесь пока заглушка
//  //  QTimer *timer = new QTimer();
//  //  connect(timer, SIGNAL(timeout()), this,
//  SIGNAL(registrationSuccessed()));
//  //  connect(timer, SIGNAL(timeout()), timer, SLOT(deleteLater()));
//  //  timer->start(2000);
//}

void GuiAdapter::sendMessage(ModelsElements::MessageData) const {}

void GuiAdapter::loadChatHistory() {
  //вызываем ядро, просим стартовую историю по текущему chatID
  //кастуем, отдаем контролеру

  QVector<ModelsElements::MessageData> messList;

  /// а здесь пока побудет заглушка
  if (currentChatID == 0) {
    messList.append(MessageData("0", "0", "Eba eto ya", "17:00"));
    messList.append(MessageData("0", "1", "Da", "18:00"));
  } else {
    messList.append(MessageData("1", "0", "Eba eto ti", "19:00"));
    messList.append(MessageData("1", "1", "Net, Eba eto ti", "20:00"));
  }

  emit messagesLoaded(messList);
}

void GuiAdapter::uploadChatHistory() const {
  ///здесь мы запрашиваем следующий кусок истории диалога
}

void GuiAdapter::receiveMessageCallback(ebucheeYadro::Message mess) {
  ///это коллбек для получения новых сообщений
  ///кастанем и эмитнем сигнал

  ///а пока тут заглушка
  MessageData message(currentChatID, "1", "EbaEba", "21:00");
  emit newMessage(message);
}

void GuiAdapter::loadChatHistoryCallback(std::vector<ebucheeYadro::Message>) {
  /// это коллбек для получения стартовой истории сообщений для чата
}

void GuiAdapter::uploadChatHistoryCallback(std::vector<ebucheeYadro::Message>) {
  /// это коллбек для получения продолжения истории сообщений для чата
}

void GuiAdapter::deleteChat(const QString &chatID) {
  /// скажем ядру, что мы удалим диалог
  /// потом уберем его из модели через контролер
}

void GuiAdapter::createChat(const QString &uuid) {
  /// кинем ядру запрос на новый диалог
  /// вообще по идее эта функция должна возвращать результат добавления
}

QHash<QString, ModelsElements::ChatData> GuiAdapter::loadChats() {
  /// кидаем ядру запрос на список диалогов
  auto chatMap = dispatcher_.get()->GetChats();

  /// кастуем в наши структуры

  /// а пока тут заглушка

  QHash<QString, ModelsElements::ChatData> chatList;
  chatList.insert("0",
                  ChatData("0", MessageData("0", "0", "Eba eto ya", "17:00"),
                           "Eba1", "", 1));

  chatList.insert("0",
                  ChatData("1", MessageData("0", "0", "Eba eto ti", "19:00"),
                           "Eba2", "", 1));

  return chatList;
}

void GuiAdapter::addChatCallback(ebucheeYadro::Chat chat) {
  ///коллбек когда создается новый диалог с нами
}

void GuiAdapter::loadChatsCallback(
    std::unordered_map<std::string, ebucheeYadro::Chat> chatsTable) {
  ///коллбек для загрузки списка чатов
}

QHash<QString, ModelsElements::ContactData> GuiAdapter::loadContacts() {
  /// подтянем список контактов, кастанем и вернем
  /// список контактов так же содержит нас самих

  QHash<QString, ModelsElements::ContactData> contactList;
  contactList.insert("0", ContactData("0", "Ne Eba", "/demo/dsa.jpg"));
  contactList.insert("1", ContactData("1", "Eba", "/demo/asd.jpg"));

  return contactList;
}

void GuiAdapter::loadContactsCallback(
    std::unordered_map<std::string, ebucheeYadro::Contact> contactsTable) {
  ///коллбек для загрузки списка контактов
}
}
}
}
