#ifndef M2_SERVER_ADIALOG_H
#define M2_SERVER_ADIALOG_H

#include <list>

#include "../stdafx.h"
#include "../Message/Message.h"
#include "../Index/IndexManager.h"
#include "../Index/CashManager.hpp"



namespace m2     {
namespace data   {
namespace dialog {

    using namespace message;
    using namespace indices;


    /**
     * the class is a wrapper on a user folder
     * >> take required messages
     * >> collect messages
     * ** archive old messages
     *
     *
     * . It creates a file in a root of it's folder
     *      taking a task of a search index
     * . In takes a time of synchronisation
     *
     * *****
     * create iterators
     *
     */
    class ADialog
            : boost::enable_shared_from_this<ADialog>
    //        , boost::noncopyable
    {
    public:

        typedef std::shared_ptr<ADialog> ptr;

        typedef std::list<AMessage> Messages; // how to use a cash manager?

    protected: /************| Construction |***************/

        ADialog(const std::string&  Root, uuids::uuid Uid);
        ADialog(      std::string&& Root, uuids::uuid Uid);

        ADialog(std::istream& is, const std::string&  Root);
        ADialog(std::istream& is,       std::string&& Root);

    public:

        static ptr Create(const std::string&  Root, uuids::uuid Uid);
        static ptr Create(      std::string&& Root, uuids::uuid Uid);

        static ptr Create(std::istream& is, const std::string&  Root);
        static ptr Create(std::istream& is,       std::string&& Root);

    public: /***************| Interface |***************/

        void Serialize(std::ostream& os, std::string since);

    protected: /************| Members |***************/

        uuids::uuid uid;
        std::string root;

        Messages    messages;

    public:
              uuids::uuid  Uid()  const;
        const std::string& Root() const;

    public: /***************| operators |***************/

        // send ALL messages
        std::ostream& operator<<(std::ostream& os);

        bool operator==(const uuids::uuid& Uid) const;
    };



}}}


#endif //M2_SERVER_ADIALOG_H
