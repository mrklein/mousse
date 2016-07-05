// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "dictionary.hpp"
#include "pstream.hpp"


// Static Data Members
int mousse::messageStream::level(mousse::debug::debugSwitch("level", 2));


mousse::messageStream::messageStream
(
  const string& title,
  errorSeverity sev,
  const int maxErrors
)
:
  title_{title},
  severity_{sev},
  maxErrors_{maxErrors},
  errorCount_{0}
{}


mousse::messageStream::messageStream(const dictionary& dict)
:
  title_{dict.lookup("title")},
  severity_{FATAL},
  maxErrors_{0},
  errorCount_{0}
{}


mousse::OSstream& mousse::messageStream::masterStream(const label communicator)
{
  if (UPstream::warnComm != -1 && communicator != UPstream::warnComm) {
    Pout << "** messageStream with comm:" << communicator
      << endl;
    error::printStack(Pout);
  }
  if (communicator == UPstream::worldComm || UPstream::master(communicator)) {
    return operator()();
  } else {
    return Snull;
  }
}


mousse::OSstream& mousse::messageStream::operator()
(
  const char* functionName,
  const char* sourceFileName,
  const int sourceFileLineNumber
)
{
  OSstream& os = operator OSstream&();
  os << endl
    << "    From function " << functionName << endl
    << "    in file " << sourceFileName
    << " at line " << sourceFileLineNumber << endl
    << "    ";
  return os;
}


mousse::OSstream& mousse::messageStream::operator()
(
  const string& functionName,
  const char* sourceFileName,
  const int sourceFileLineNumber
)
{
  return operator()
  (
    functionName.c_str(),
    sourceFileName,
    sourceFileLineNumber
  );
}


mousse::OSstream& mousse::messageStream::operator()
(
  const char* functionName,
  const char* sourceFileName,
  const int sourceFileLineNumber,
  const string& ioFileName,
  const label ioStartLineNumber,
  const label ioEndLineNumber
)
{
  OSstream& os = operator OSstream&();
  os << endl
    << "    From function " << functionName << endl
    << "    in file " << sourceFileName
    << " at line " << sourceFileLineNumber << endl
    << "    Reading " << ioFileName;
  if (ioStartLineNumber >= 0 && ioEndLineNumber >= 0) {
    os << " from line " << ioStartLineNumber
      << " to line " << ioEndLineNumber;
  } else if (ioStartLineNumber >= 0) {
    os << " at line " << ioStartLineNumber;
  }
  os << endl  << "    ";
  return os;
}


mousse::OSstream& mousse::messageStream::operator()
(
  const char* functionName,
  const char* sourceFileName,
  const int sourceFileLineNumber,
  const IOstream& ioStream
)
{
  return operator()
  (
    functionName,
    sourceFileName,
    sourceFileLineNumber,
    ioStream.name(),
    ioStream.lineNumber(),
    -1
  );
}


mousse::OSstream& mousse::messageStream::operator()
(
  const char* functionName,
  const char* sourceFileName,
  const int sourceFileLineNumber,
  const dictionary& dict
)
{
  return operator()
  (
    functionName,
    sourceFileName,
    sourceFileLineNumber,
    dict.name(),
    dict.startLineNumber(),
    dict.endLineNumber()
  );
}


mousse::messageStream::operator mousse::OSstream&()
{
  if (level) {
    bool collect = (severity_ == INFO || severity_ == WARNING);
    // Report the error
    if (!Pstream::master() && collect) {
      return Snull;
    } else {
      if (title().size()) {
        if (Pstream::parRun() && !collect) {
          Pout << title().c_str();
        } else {
          Sout << title().c_str();
        }
      }
      if (maxErrors_) {
        errorCount_++;
        if (errorCount_ >= maxErrors_) {
          FATAL_ERROR_IN("messageStream::operator OSstream&()")
            << "Too many errors"
            << abort(FatalError);
        }
      }
      if (Pstream::parRun() && !collect) {
        return Pout;
      } else {
        return Sout;
      }
    }
  }
  return Snull;
}


// Global messageStream definitions
mousse::messageStream mousse::SeriousError
(
  ">>> mousse serious error : ",
  messageStream::SERIOUS,
  100
);


mousse::messageStream mousse::Warning
(
  ">> mousse warning : ",
  messageStream::WARNING
);


mousse::messageStream mousse::Info("", messageStream::INFO);
