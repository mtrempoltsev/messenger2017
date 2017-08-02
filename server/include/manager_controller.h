#pragma once

#include "http_request.h"
#include "http_response.h"

namespace m2 {
namespace server {

class ManagerController
{
public:
	ManagerController();
	responsePtr doProcess(requestPtr request);

private:

};


}} // m2::server
