#ifndef CORE_DB_ERROR_ERROR_HPP_
#define CORE_DB_ERROR_ERROR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::error
// Description
//   Class to handle errors and exceptions in a simple, consistent stream-based
//   manner.
//   The error class is globally instantiated with a title string. Errors,
//   messages and other data are piped to the messageStream class in the
//   standard manner.  Manipulators are supplied for exit and abort which may
//   terminate the program or throw an exception depending on whether the
//   exception handling has been switched on (off by default).
// Usage
//   \code
//     error << "message1" << "message2" << FoamDataType << exit(errNo);
//     error << "message1" << "message2" << FoamDataType << abort();
//   \endcode
// SourceFiles
//   error.cpp

#include "message_stream.hpp"


namespace mousse {

// Forward declaration of friend functions and operators
class error;

Ostream& operator<<(Ostream&, const error&);

class error
:
  public std::exception,
  public messageStream
{
protected:
  // Protected data
    string functionName_;
    string sourceFileName_;
    label sourceFileLineNumber_;
    bool abort_;
    bool throwExceptions_;
    OStringStream* messageStreamPtr_;

public:
  // Constructors
    //- Construct from title string
    error(const string& title);
    //- Construct from dictionary
    error(const dictionary&);
    //- Construct as copy
    error(const error&);
  //- Destructor
  virtual ~error() throw();
  // Member functions
    string message() const;
    const string& functionName() const
    {
      return functionName_;
    }
    const string& sourceFileName() const
    {
      return sourceFileName_;
    }
    label sourceFileLineNumber() const
    {
      return sourceFileLineNumber_;
    }
    void throwExceptions()
    {
      throwExceptions_ = true;
    }
    void dontThrowExceptions()
    {
      throwExceptions_ = false;
    }
    //- Convert to OSstream
    //  Prints basic message and returns OSstream for further info.
    OSstream& operator()
    (
      const char* functionName,
      const char* sourceFileName,
      const int sourceFileLineNumber = 0
    );
    //- Convert to OSstream
    //  Prints basic message and returns OSstream for further info.
    OSstream& operator()
    (
      const string& functionName,
      const char* sourceFileName,
      const int sourceFileLineNumber = 0
    );
    //- Convert to OSstream
    //  Prints basic message and returns OSstream for further info.
    operator OSstream&();
    //- Explicitly convert to OSstream for << operations
    OSstream& operator()()
    {
      return operator OSstream&();
    }
    //- Create and return a dictionary
    operator dictionary() const;
    //- Helper function to print a stack (if OpenFOAM IO not yet
    //  initialised)
    static void safePrintStack(std::ostream&);
    //- Helper function to print a stack
    static void printStack(Ostream&);
    //- Exit : can be called for any error to exit program.
    //  Prints stack before exiting.
    void exit(const int errNo = 1);
    //- Abort : used to stop code for fatal errors.
    //  Prints stack before exiting.
    void abort();
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const error&);
};
// Forward declaration of friend functions and operators
class IOerror;
Ostream& operator<<(Ostream&, const IOerror&);
//- Report an I/O error
class IOerror
:
  public error
{
  // Private data
    string ioFileName_;
    label ioStartLineNumber_;
    label ioEndLineNumber_;
public:
  // Constructors
    //- Construct from title string
    IOerror(const string& title);
    //- Construct from dictionary
    IOerror(const dictionary&);
  //- Destructor
  virtual ~IOerror() throw();
  // Member functions
    const string& ioFileName() const
    {
      return ioFileName_;
    }
    label ioStartLineNumber() const
    {
      return ioStartLineNumber_;
    }
    label ioEndLineNumber() const
    {
      return ioEndLineNumber_;
    }
    //- Convert to OSstream
    //  Prints basic message and returns OSstream for further info.
    OSstream& operator()
    (
      const char* functionName,
      const char* sourceFileName,
      const int sourceFileLineNumber,
      const string& ioFileName,
      const label ioStartLineNumber = -1,
      const label ioEndLineNumber = -1
    );
    //- Convert to OSstream
    //  Prints basic message and returns OSstream for further info.
    OSstream& operator()
    (
      const char* functionName,
      const char* sourceFileName,
      const int sourceFileLineNumber,
      const IOstream&
    );
    //- Convert to OSstream
    //  Prints basic message and returns OSstream for further info.
    OSstream& operator()
    (
      const char* functionName,
      const char* sourceFileName,
      const int sourceFileLineNumber,
      const dictionary&
    );
    //- Print basic message and exit. Uses cerr if streams not constructed
    //  yet (at startup). Use in startup parsing instead of FatalError.
    static void SafeFatalIOError
    (
      const char* functionName,
      const char* sourceFileName,
      const int sourceFileLineNumber,
      const IOstream&,
      const string& msg
    );
    //- Create and return a dictionary
    operator dictionary() const;
    //- Exit : can be called for any error to exit program
    void exit(const int errNo = 1);
    //- Abort : used to stop code for fatal errors
    void abort();
  // Ostream operator
    friend Ostream& operator<<(Ostream&, const IOerror&);
};

// Global error declarations: defined in error.C
extern error FatalError;
extern IOerror FatalIOError;
}  // namespace mousse

// Convenience macros to add the file name and line number to the function name
//- Report an error message using mousse::FatalError
//  for functionName in file __FILE__ at line __LINE__
#define FATAL_ERROR_IN(functionName)                                          \
  ::mousse::FatalError((functionName), __FILE__, __LINE__)

//- Report an error message using mousse::FatalError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define FATAL_ERROR_IN_FUNCTION FATAL_ERROR_IN(FUNCTION_NAME)

//- Report an error message using mousse::FatalIOError
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define FATAL_IO_ERROR_IN(functionName, ios)                                  \
  ::mousse::FatalIOError((functionName), __FILE__, __LINE__, (ios))

//- Report an error message using mousse::FatalIOError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define FATAL_IO_ERROR_IN_FUNCTION(ios) FATAL_IO_ERROR_IN(FUNCTION_NAME, ios)

//- Report an error message using mousse::FatalIOError
//  (or cerr if FatalIOError not yet constructed)
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define SAFE_FATAL_IO_ERROR_IN(functionName, ios, msg)                        \
  ::mousse::IOerror::SafeFatalIOError                                         \
  ((functionName), __FILE__, __LINE__, (ios), (msg))

//- Report an error message using mousse::FatalIOError
//  (or cerr if FatalIOError not yet constructed)
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define SAFE_FATAL_IO_ERROR_IN_FUNCTION(ios, msg)                             \
  SAFE_FATAL_IO_ERROR_IN(FUNCTION_NAME, ios, msg)

//- Issue a FATAL_ERROR_IN for a function not currently implemented.
//  The functionName is printed and then abort is called.
//
//  This macro can be particularly useful when methods must be defined to
//  complete the interface of a derived class even if they should never be
//  called for this derived class.
#define NOT_IMPLEMENTED(functionName)                                         \
  FATAL_ERROR_IN(functionName)                                                \
    << "Not implemented" << ::mousse::abort(FatalError);

//- Issue a FATAL_ERROR_IN for a function not currently implemented.
//  The FUNCTION_NAME is printed and then abort is called.
//
//  This macro can be particularly useful when methods must be defined to
//  complete the interface of a derived class even if they should never be
//  called for this derived class.
#define NOTIMPLEMENTED NOT_IMPLEMENTED(FUNCTION_NAME)

#include "error_manip.hpp"

#endif
