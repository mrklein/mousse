#ifndef CORE_DB_ERROR_MESSAGE_STREAM_HPP_
#define CORE_DB_ERROR_MESSAGE_STREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::messageStream
// Description
//   Class to handle messaging in a simple, consistent stream-based
//   manner.
//   The messageStream class is globaly instantiated with a title string a
//   given severity, which controls the program termination, and a number of
//   errors before termination.  Errors, messages and other data are piped to
//   the messageStream class in the standard manner.
// Usage
//   \code
//     messageStream
//       << "message1" << "message2" << FoamDataType << endl;
//   \endcode
// SourceFiles
//   message_stream.cpp


#include "label.hpp"
#include "string.hpp"

namespace mousse
{
// Forward declaration of classes
class IOstream;
class Ostream;
class OSstream;
class OStringStream;
class dictionary;
class messageStream
{
public:
  //- Severity flags
  enum errorSeverity
  {
    INFO,       // Debugging information in event of error
    WARNING,    // Warning of possible problem
    SERIOUS,    // A serious problem (data corruption?)
    FATAL       // Oh bugger!
  };
protected:
  // Private data
    string title_;
    errorSeverity severity_;
    int maxErrors_;
    int errorCount_;
public:
  // Debug switches
    static int level;
  // Constructors
    //- Construct from components
    messageStream
    (
      const string& title,
      errorSeverity,
      const int maxErrors = 0
    );
    //- Construct from dictionary
    messageStream(const dictionary&);
  // Member functions
    //- Return the title of this error type
    const string& title() const
    {
      return title_;
    }
    //- Return the maximum number of errors before program termination
    int maxErrors() const
    {
      return maxErrors_;
    }
    //- Return non-const access to the maximum number of errors before
    //  program termination to enable user to reset it
    int& maxErrors()
    {
      return maxErrors_;
    }
    //- Convert to OSstream
    //  Prints to Pout for the master stream
    OSstream& masterStream(const label communicator);
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
    //- Convert to OSstream for << operations
    operator OSstream&();
    //- Explicitly convert to OSstream for << operations
    OSstream& operator()()
    {
      return operator OSstream&();
    }
};
// Global error declarations: defined in messageStream.C
extern messageStream SeriousError;
extern messageStream Warning;
extern messageStream Info;
}  // namespace mousse
#include "osstream.hpp"

// Convenience macros to add the file name and line number to the function name
// Compiler provided function name string:
//     for gcc-compatible compilers use __PRETTY_FUNCTION__
//     otherwise use the standard __func__
#ifdef __GNUC__
  #define FUNCTION_NAME __PRETTY_FUNCTION__
#else
  #define FUNCTION_NAME __func__
#endif

//- Report an error message using mousse::SeriousError
//  for functionName in file __FILE__ at line __LINE__
#define SERIOUS_ERROR_IN(functionName)                                        \
  ::mousse::SeriousError((functionName), __FILE__, __LINE__)

//- Report an error message using mousse::SeriousError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define SERIOUS_ERROR_IN_FUNCTION SERIOUS_ERROR_IN(FUNCTION_NAME)

//- Report an IO error message using mousse::SeriousError
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define SERIOUS_IO_ERROR_IN(functionName, ios)                                \
  ::mousse::SeriousError((functionName), __FILE__, __LINE__, ios)

//- Report an IO error message using mousse::SeriousError
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define SERIOUS_IO_ERROR_IN_FUNCTION(ios)                                     \
  SERIOUS_IO_ERROR_IN(FUNCTION_NAME, ios)

//- Report a warning using mousse::Warning
//  for functionName in file __FILE__ at line __LINE__
#define WARNING_IN(functionName)                                              \
  ::mousse::Warning((functionName), __FILE__, __LINE__)

//- Report a warning using mousse::Warning
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define WARNING_IN_FUNCTION WARNING_IN(FUNCTION_NAME)

//- Report an IO warning using mousse::Warning
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define IO_WARNING_IN(functionName, ios)                                      \
  ::mousse::Warning((functionName), __FILE__, __LINE__, (ios))

//- Report an IO warning using mousse::Warning
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define IO_WARNING_IN_FUNCTION(ios) IO_WARNING_IN(FUNCTION_NAME, ios)

//- Report a information message using mousse::Info
//  for functionName in file __FILE__ at line __LINE__
#define INFO_IN(functionName)                                                 \
  ::mousse::Info((functionName), __FILE__, __LINE__)

//- Report a information message using mousse::Info
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
#define INFO_IN_FUNCTION INFO_IN(FUNCTION_NAME)

//- Report an IO information message using mousse::Info
//  for functionName in file __FILE__ at line __LINE__
//  for a particular IOstream
#define IO_INFO_IN(functionName, ios)                                         \
  ::mousse::Info((functionName), __FILE__, __LINE__, (ios))

//- Report an IO information message using mousse::Info
//  for FUNCTION_NAME in file __FILE__ at line __LINE__
//  for a particular IOstream
#define IO_INFO_IN_FUNCTION(ios) IO_INFO_IN(FUNCTION_NAME, ios)

//- Report a variable name and value
//  using mousse::Pout in file __FILE__ at line __LINE__
#define DEBUG(var)                                                             \
  ::mousse::Pout<< "["<< __FILE__ << ":" << __LINE__ << "] "                   \
  << #var " " << var << ::mousse::endl

#endif
