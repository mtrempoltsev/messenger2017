#pragma once

#include <string>

namespace m2 {
namespace server {

template <ResponseType>
class make_response
{
public:

  std::string get_response ()
  {
    return "nigga" + get_specified_data ();
  }
    ;
  virtual std::string get_response ();

  std::string get_specified_data ();
private:

};

template<>
std::string make_response<createEncrypted>::get_specified_data ()
{
  return "createEncrypted";
}

template<>
std::string make_response<registerUser>::get_specified_data ()
{
  return "registerUser";
}

}  // namespace server
}  // namespace m2
