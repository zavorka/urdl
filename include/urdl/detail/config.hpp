//
// config.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2009-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// No include guard.

#if !defined(URDL_DECL)
# define URDL_DECL inline
#endif // !defined(URDL_DECL)

#if !defined(URDL_ERROR_CATEGORY_NOEXCEPT)
# define URDL_ERROR_CATEGORY_NOEXCEPT noexcept
#endif // !defined(URDL_ERROR_CATEGORY_NOEXCEPT)

#if !defined(URDL_INITFN_RESULT_TYPE)
# define URDL_INITFN_RESULT_TYPE(h, sig) ASIO_INITFN_RESULT_TYPE(h, sig)
#endif // !defined(URDL_INITFN_RESULT_TYPE)
