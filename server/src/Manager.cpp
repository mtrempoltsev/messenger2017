#include "Manager.h"
using namespace m2::server;

std::string Manager::createError(const std::string message)
{
    pt::ptree tree;
    std::stringstream stream;
    tree.put("error_msg", message);
    boost::property_tree::write_json(stream, tree);
    return stream.str();
}
Manager::Manager(ManagerController* controller): db(controller->getDB())
{

}
