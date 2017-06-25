//
// multiget1.cpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2009-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <urdl/read_stream.hpp>

class downloader
  : public std::enable_shared_from_this<downloader>
{
public:
  downloader(asio::io_service& io_service)
    : read_stream_(io_service)
  {
  }

  void start(const urdl::url& url, const std::string& file)
  {
    file_ = file;
    read_stream_.async_open(url, [self = shared_from_this()] (std::error_code const& ec) {
        self->handle_open(ec);
    });
  }

private:
  void handle_open(const std::error_code& ec)
  {
    if (!ec)
    {
      ofstream_.open(file_.c_str(), std::ios_base::out | std::ios_base::binary);
      read_stream_.async_read_some(
          asio::buffer(buffer_),
          [self = shared_from_this()] (std::error_code const& e, std::size_t bytes_transferred) {
              self->handle_read(e, bytes_transferred);
          }
      );
    }
  }

  void handle_read(const std::error_code& ec, std::size_t length)
  {
    if (!ec)
    {
      ofstream_.write(buffer_.data(), length);
      read_stream_.async_read_some(
          asio::buffer(buffer_),
          [self = shared_from_this()] (std::error_code const& e, std::size_t bytes_transferred) {
              self->handle_read(e, bytes_transferred);
          });
    }
  }

  urdl::read_stream read_stream_;
  std::string file_;
  std::ofstream ofstream_;
  std::array<char, 1024> buffer_;
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc < 3 || argc % 2 == 0)
    {
      std::cerr << "Usage: multiget1 <url> <outputfile> ";
      std::cerr << "[<url> <outputfile> ...]\n";
      return 1;
    }

    asio::io_service io_service;

    for (int i = 1; i < argc; i += 2)
    {
      std::shared_ptr<downloader> d(new downloader(io_service));
      d->start(argv[i], argv[i + 1]);
    }

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}
