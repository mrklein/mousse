// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CLASSNAME
// Description
// SourceFiles
//   classnamei.hpp
//   classname.cpp
//   classnameio.cpp
#ifndef CLASSNAME_H
#define CLASSNAME_H
#include ".h"
namespace mousse
{
// Forward declaration of classes
class someClass;
// Forward declaration of friend functions and operators
template<TemplateClassArgument>
class CLASSNAME;
template<TemplateClassArgument>
Istream& operator>>(Istream&, CLASSNAME<TemplateArgument>&);
template<TemplateClassArgument>
Ostream& operator<<(Ostream&, const CLASSNAME<TemplateArgument>&);
template<TemplateClassArgument>
class CLASSNAME
:
  public baseClassName
{
  // Private data
    dataType data_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    CLASSNAME(const CLASSNAME<TemplateArgument>&);
    //- Disallow default bitwise assignment
    void operator=(const CLASSNAME<TemplateArgument>&);
public:
  // Static data members
    //- Static data someStaticData
    static const dataType staticData;
  // Constructors
    //- Construct null
    CLASSNAME();
    //- Construct from components
    CLASSNAME(const dataType& data);
    //- Construct from Istream
    CLASSNAME(Istream&);
    //- Construct as copy
    CLASSNAME(const CLASSNAME<TemplateArgument>&);
  // Selectors
    //- Select null constructed
    static autoPtr<CLASSNAME<TemplateArgument> > New();
  //- Destructor
  ~CLASSNAME();
  // Member Functions
  // Member Operators
    void operator=(const CLASSNAME<TemplateArgument>&);
  // Friend Functions
  // Friend Operators
  // IOstream Operators
    friend Istream& operator>> <TemplateArgument>
    (Istream&, CLASSNAME<TemplateArgument>&);
    friend Ostream& operator<< <TemplateArgument>
    (Ostream&, const CLASSNAME<TemplateArgument>&);
};
}  // namespace mousse
#include "classnamei.hpp"
#ifdef NoRepository
#   include "classname.cpp"
#endif
#endif
