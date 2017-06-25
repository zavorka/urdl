//
// get2.cpp
// ~~~~~~~~
//
// Copyright (c) 2009-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <fstream>
#include <urdl/read_stream.hpp>
#include <boost/progress.hpp>

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: get2 <url> <outputfile>\n";
      return 1;
    }

    asio::io_service io_service;

    urdl::read_stream stream(io_service);
    stream.open(argv[1]);

    std::ofstream os(argv[2], std::ios_base::out | std::ios_base::binary);
    for (;;)
    {
      char data[1024];
      std::error_code ec;
      std::size_t length = stream.read_some(asio::buffer(data), ec);
      if (ec == asio::error::eof)
        break;
      if (ec)
        throw std::system_error(ec);
      os.write(data, length);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}
