#ifndef M2_SERVER_AMESSAGE_H
#define M2_SERVER_AMESSAGE_H

#include <chrono> // how to explain time?

#include "../stdafx.h"



namespace m2      {
namespace data    {
namespace message {

    /**
     * A message is a class,
     * wrapping a file on a disk
     *
     * file name looks like: time^author
     */
    class AMessage
    {
    public: /***************| Construction |***************/
        // new message
        AMessage(std::istream& is);

        // open existing message
        AMessage(const std::string&  File);
        AMessage(      std::string&& File);

    public: /***************| Interface |***************/

        AMessage& Serialize(std::ostream& os);

        AMessage& Storage();

    protected: /************| Members |***************/


        std::string author; // or uuids::uuid ?
        std::string time;

        std::string text;

    public:
        std::string& Author();
        const std::string& Author() const;

        std::string& Time();
        const std::string& Time() const;

        std::string& Text();
        const std::string& Text() const;

    public: /***************| operators |***************/

        std::ostream& operator<<(std::ostream& os);
    };


}}}


#endif //M2_SERVER_AMESSAGE_H
