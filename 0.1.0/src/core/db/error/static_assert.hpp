#ifndef CORE_DB_ERROR_STATIC_ASSERT_HPP_
#define CORE_DB_ERROR_STATIC_ASSERT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::StaticAssertionFailed
// Description
//   Macros and classes to provide static (compile-time) assertions.
//   Ideas from various sources
//   (http://www.ddj.com/cpp/184401547, http://www.boost.org)

namespace mousse {

//- Forward declaration of StaticAssertionFailed.
//  Leave as an incomplete class so that sizeof(..) fails
template<bool Truth> class StaticAssertionFailed;
//- Specialization for successful assertions
template<>
class StaticAssertionFailed<true>
{};
//- Helper class for handling static assertions
template<unsigned Test>
class StaticAssertionTest {};
}  // namespace mousse

// internal use:
// ~~~~~~~~~~~~~
// paste together strings, even if an argument is itself a macro
#define STATIC_ASSERT_MACRO(X,Y)  STATIC_ASSERT_MACRO1(X,Y)
#define STATIC_ASSERT_MACRO1(X,Y) STATIC_ASSERT_MACRO2(X,Y)
#define STATIC_ASSERT_MACRO2(X,Y) X##Y

// external use:
// ~~~~~~~~~~~~~
/**
* \def StaticAssert(Test)
* Assert that some test is true at compile-time
*/
#define STATIC_ASSERT(Test)                                                   \
  typedef ::mousse::StaticAssertionTest                                       \
  <                                                                           \
    sizeof( ::mousse::StaticAssertionFailed< ((Test) ? true : false) > )      \
  > STATIC_ASSERT_MACRO(StaticAssertionTest, __LINE__)
#endif
