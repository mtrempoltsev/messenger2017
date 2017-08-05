#include "http_response.h"

namespace m2 {
namespace server {

HttpResponse::HttpResponse() {
}

void HttpResponse::setData(const std::string &data, Code code) {
  data_ = data;
  changeHeader(code);
}

std::string HttpResponse::toString() const {
  return header_ + data_;
}

void HttpResponse::changeHeader(Code code) {
  if (code == Code::OK) {
    header_ =
        "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(data_.size()) + "\r\n"
            "Connection: keep-alive\r\n"
            "\r\n";
  } else if (code == NOT_FOUND) {
    header_ =
        "HTTP/1.0 404 Not Found\r\n"
            "Content-Length: " + std::to_string(data_.size()) + "\r\n"
            "\r\n";
  } else if (code == Code::FORBIDEN) {
    header_ =
        "HTTP/1.0 403 403 Forbidden\r\n"
            "Content-Length: " + std::to_string(data_.size()) + "\r\n"
            "\r\n";
  }
}
}
}
