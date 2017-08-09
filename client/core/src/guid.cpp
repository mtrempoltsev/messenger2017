#include "../include/guid.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

m2::Uuid::Uuid(const std::string &uuid)
{
    Set(uuid);
}

void m2::Uuid::Generate()
{
    u_ = boost::uuids::random_generator()();
}

std::string m2::Uuid::ToString() const
{
    return boost::uuids::to_string(u_);
}

bool m2::Uuid::Set(const std::string &uuid)
{
	bool result = false;
    try
    {
		u_ = boost::lexical_cast<boost::uuids::uuid>(uuid);
		result = true;
    }
    catch (boost::bad_lexical_cast b)
    {
        //write a message to a log
		//do we have a log?
    }
    return result;
}
