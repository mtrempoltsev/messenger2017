#pragma once
#include <string>
#include <memory>


namespace m2 {
namespace server {


class HttpResponse
{
public:

	enum Code {
		OK = 200,
		FORBIDEN = 403,
		NOT_FOUND = 404
	};

    HttpResponse();

    void setData(const std::string& data, Code code);
    std::string toString() const;

private:
	void changeHeader(Code code);

private:
    std::string header_;
    std::string data_;
};

typedef std::shared_ptr<HttpResponse> responsePtr;

}}
