#ifndef CORE_DB_ERROR_COCO_PARSER_ERRORS_HPP_
#define CORE_DB_ERROR_COCO_PARSER_ERRORS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CocoParserErrors
// Description
//   Templated class to shadow the error handling for Coco/R parsers


#include "error.hpp"
#include "wchar.hpp"

namespace mousse
{

template<class BaseClass, class StringClass=std::wstring>
class CocoParserErrors
:
  public BaseClass
{
  // Private data
    //- The name issued in warnings and errors
    word   name_;
public:
  // Constructors
    //- Construct with given name
    CocoParserErrors(const word& name)
    :
      BaseClass{},
      name_{name}
    {}

  //- Destructor
  virtual ~CocoParserErrors()
  {}

  // Member functions
    //- Return the name issued for warnings
    virtual const word& name() const
    {
      return name_;
    }

    //- Return the name issued for warnings
    virtual word& name()
    {
      return name_;
    }

  // Error Handling
    //- Handle a general warning 'msg'
    virtual void Warning(const StringClass& msg)
    {
      WARNING_IN(name_)
        << msg << endl;
    }

    //- Handle a general warning 'msg'
    virtual void Warning(int line, int col, const StringClass& msg)
    {
      WARNING_IN(name_)
        <<"line " << line << " col " << col << ": "
        << msg << endl;
    }

    //- Handle general error 'msg' (eg, a semantic error)
    virtual void Error(int line, int col, const StringClass& msg)
    {
      FATAL_ERROR_IN(name_)
        << "line " << line << " col " << col <<": " << msg << endl
        << exit(FatalError);
    }

    //- Handle general error 'msg' (eg, a semantic error)
    virtual void Error(const StringClass& msg)
    {
      FATAL_ERROR_IN(name_)
        << msg << endl
        << exit(FatalError);
    }

    //- Handle a general exception 'msg'
    virtual void Exception(const StringClass& msg)
    {
      this->Error(msg);
    }
};

}  // namespace mousse
#endif
