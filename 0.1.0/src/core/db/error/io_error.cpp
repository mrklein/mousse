// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "ostring_stream.hpp"
#include "file_name.hpp"
#include "dictionary.hpp"
#include "job_info.hpp"
#include "pstream.hpp"
#include "job_info.hpp"


mousse::IOerror::IOerror(const string& title)
:
  error{title},
  ioFileName_{"unknown"},
  ioStartLineNumber_{-1},
  ioEndLineNumber_{-1}
{}


mousse::IOerror::IOerror(const dictionary& errDict)
:
  error{errDict},
  ioFileName_{errDict.lookup("ioFileName")},
  ioStartLineNumber_{readLabel(errDict.lookup("ioStartLineNumber"))},
  ioEndLineNumber_{readLabel(errDict.lookup("ioEndLineNumber"))}
{}


mousse::IOerror::~IOerror() throw()
{}


mousse::OSstream& mousse::IOerror::operator()
(
  const char* functionName,
  const char* sourceFileName,
  const int sourceFileLineNumber,
  const string& ioFileName,
  const label ioStartLineNumber,
  const label ioEndLineNumber
)
{
  error::operator()(functionName, sourceFileName, sourceFileLineNumber);
  ioFileName_ = ioFileName;
  ioStartLineNumber_ = ioStartLineNumber;
  ioEndLineNumber_ = ioEndLineNumber;
  return operator OSstream&();
}


mousse::OSstream& mousse::IOerror::operator()
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


mousse::OSstream& mousse::IOerror::operator()
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


void mousse::IOerror::SafeFatalIOError
(
  const char* functionName,
  const char* sourceFileName,
  const int sourceFileLineNumber,
  const IOstream& ioStream,
  const string& msg
)
{
  if (JobInfo::constructed) {
    FATAL_IO_ERROR_IN
    (
      "primitiveEntry::readEntry(const dictionary&, Istream&)",
      ioStream
    )   << msg << mousse::exit(FatalIOError);
  } else {
    std::cerr
      << std::endl
      << "--> mousse FATAL IO ERROR:" << std::endl
      << msg
      << std::endl
      << "file: " << ioStream.name()
      << " at line " << ioStream.lineNumber() << '.'
      << std::endl << std::endl
      << "    From function " << functionName
      << std::endl
      << "    in file " << sourceFileName
      << " at line " << sourceFileLineNumber << '.'
      << std::endl;
    ::exit(1);
  }
}


mousse::IOerror::operator mousse::dictionary() const
{
  dictionary errDict(error::operator dictionary());
  errDict.remove("type");
  errDict.add("type", word("mousse::IOerror"));
  errDict.add("ioFileName", ioFileName());
  errDict.add("ioStartLineNumber", ioStartLineNumber());
  errDict.add("ioEndLineNumber", ioEndLineNumber());
  return errDict;
}
void mousse::IOerror::exit(const int)
{
  if (!throwExceptions_ && JobInfo::constructed) {
    jobInfo.add("FatalIOError", operator dictionary());
    jobInfo.exit();
  }
  if (abort_) {
    abort();
  }
  if (Pstream::parRun()) {
    Perr << endl << *this << endl
      << "\nmousse parallel run exiting\n" << endl;
    Pstream::exit(1);
  } else {
    if (throwExceptions_) {
      // Make a copy of the error to throw
      IOerror errorException(*this);
      // Rewind the message buffer for the next error message
      messageStreamPtr_->rewind();
      throw errorException;
    } else {
      Perr << endl << *this << endl
        << "\nmousse exiting\n" << endl;
      ::exit(1);
    }
  }
}


void mousse::IOerror::abort()
{
  if (!throwExceptions_ && JobInfo::constructed) {
    jobInfo.add("FatalIOError", operator dictionary());
    jobInfo.abort();
  }
  if (abort_) {
    Perr << endl << *this << endl
      << "\nmousse aborting (MOUSSE_ABORT set)\n" << endl;
    printStack(Perr);
    ::abort();
  }
  if (Pstream::parRun()) {
    Perr << endl << *this << endl
      << "\nmousse parallel run aborting\n" << endl;
    printStack(Perr);
    Pstream::abort();
  } else {
    if (throwExceptions_) {
      // Make a copy of the error to throw
      IOerror errorException{*this};
      // Rewind the message buffer for the next error message
      messageStreamPtr_->rewind();
      throw errorException;
    } else {
      Perr << endl << *this << endl << "\nmousse aborting\n" << endl;
      printStack(Perr);
      ::abort();
    }
  }
}


mousse::Ostream& mousse::operator<<(Ostream& os, const IOerror& ioErr)
{
  if (!os.bad()) {
    os << endl
      << ioErr.title().c_str() << endl
      << ioErr.message().c_str() << endl << endl;
    os << "file: " << ioErr.ioFileName().c_str();
    if (ioErr.ioStartLineNumber() >= 0 && ioErr.ioEndLineNumber() >= 0) {
      os << " from line " << ioErr.ioStartLineNumber()
        << " to line " << ioErr.ioEndLineNumber() << '.';
    } else if (ioErr.ioStartLineNumber() >= 0) {
      os << " at line " << ioErr.ioStartLineNumber() << '.';
    }
    if (IOerror::level >= 2 && ioErr.sourceFileLineNumber()) {
      os << endl << endl
        << "    From function " << ioErr.functionName().c_str() << endl
        << "    in file " << ioErr.sourceFileName().c_str()
        << " at line " << ioErr.sourceFileLineNumber() << '.';
    }
  }
  return os;
}


// Global error definitions
mousse::IOerror mousse::FatalIOError(">>> mousse FATAL IO ERROR: ");
