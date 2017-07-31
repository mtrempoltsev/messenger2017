#include "../include/guid.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

std::string m2::Uuid::toString() const { return boost::uuids::to_string(u_); }

void m2::Uuid::set(const std::string &uuid) { u_ = boost::lexical_cast<boost::uuids::uuid>(uuid); }
