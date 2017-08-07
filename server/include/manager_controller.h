#pragma once

#include "http_request.h"
#include "http_response.h"
#include "Data/Database.h"

namespace m2 {
namespace server {

class ManagerController
{
public:
	ManagerController(Database *database);
	responsePtr doProcess(requestPtr request);

private:
	Database *db;


};


}} // m2::server
