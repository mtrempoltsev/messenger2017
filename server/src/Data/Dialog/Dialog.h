#ifndef M2_SERVER_ADIALOG_H
#define M2_SERVER_ADIALOG_H

#include <list>

#include "../stdafx.h"
#include "../Message/Message.h"
#include "../Index/IndexManager.hpp"
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
     * chunk message loading::
     * 0                        on creation
     * 0 - 1                    next chank read
     * 0 - 1 - 2                next chank read
     *     1 - 2 - 3            0-th was removed
     *         2 - 3 - 4        1-th was removed
     *             3 - 4 - 5    2-d  was removed
     *
     * *****
     * chunk index reading:: - for example: in case of 1k msg/day and usage float as data type : 1.4 mb on chat * 200 chats * 5000
     *
     * what if make index as routing node, containig a time of chunk's first record.
     * 01.02.2017 23.05.15
     *      01.02.2017 23.05.15     | chunk : 01.02.2017 23.05.15
     *      01.02.2017 23.05.25     |
     *      01.02.2017 23.15.25     | chank could be difference lenght
     *      02.02.2017 12.18.00     |
     *      ...                     |
     *      05.02.2017 08.56.13     |
     * 08.02.2017 23.05.15
     *      08.02.2017 23.05.15
     *      ...
     * *****
     * >> get all messages since DATE (or all, DATE = 0)
     *      -> goto Main Index
     *      -> get list of required once
     *      -> itr:: load index
     *          -> itr:: create message
     *              -> add to cash
     *              -> searialaize to ostream
     *          ->close index
     *
     *  *
     * *****
     * >> write message
     *      -> add to cash
     *      -> add to index
     *
     * *****
     * create iterators
     *
     * create a 2-lvl index system
     */
    class ADialog
            : boost::enable_shared_from_this<ADialog>
    //        , boost::noncopyable
    {
    public:

        typedef std::shared_ptr<ADialog>   ptr;

        typedef TCashManager<AMessage>     LMessages;
        typedef TIndexManager<std::string> LIndexManager;

    protected: /************| Construction |***************/

        ADialog(const std::string&  Root, uuids::uuid Uid);
        ADialog(      std::string&& Root, uuids::uuid Uid);

    public:

        static ptr Create(const std::string&  Root, uuids::uuid Uid);
        static ptr Create(      std::string&& Root, uuids::uuid Uid);

    public: /***************| Interface |***************/
        // main function
        void Serialize    (std::ostream& os, std::string since);
        void AddMessage   (std::istream& is);
        void AddMessage   (const std::string& time, const std::string& text);
        void DeleteMessage(const std::string& time);

    protected: /************| Members |***************/

        uuids::uuid uid;
        std::string root;

        LIndexManager index;
        LMessages     messages;

    public:
              uuids::uuid  Uid()  const { return uid;  }
        const std::string& Root() const { return root; }

    public: /***************| operators |***************/

        // send ALL messages
        std::ostream& operator<<(std::ostream& os);

        bool operator==(const uuids::uuid& Uid) const;
    };



}}}


#endif //M2_SERVER_ADIALOG_H
