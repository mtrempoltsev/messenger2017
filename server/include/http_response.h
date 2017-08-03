#pragma once


namespace m2 {
namespace server {


class HttpResponse
{
public:

	enum class HeaderType {
		OK,
		NOT_FOUND,

	};

    HttpResponse();

    void setData(const std::string& data, HeaderType type = HeaderType::OK);
    std::string toString() const;

private:
	void changeHeader(HeaderType type);

private:
    std::string header_;
    std::string data_;
};

typedef std::shared_ptr<HttpResponse> responsePtr;

}}
