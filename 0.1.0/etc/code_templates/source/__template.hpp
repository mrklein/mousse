// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
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
class Istream;
class Ostream;
// Forward declaration of friend functions and operators
class CLASSNAME;
Istream& operator>>(Istream&, CLASSNAME&);
Ostream& operator<<(Ostream&, const CLASSNAME&);
class CLASSNAME
:
  public baseClassName
{
  // Private data
    //- Description of data_
    dataType data_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    CLASSNAME(const CLASSNAME&);
    //- Disallow default bitwise assignment
    void operator=(const CLASSNAME&);
public:
  // Static data members
    //- Static data staticData
    static const dataType staticData;
  // Constructors
    //- Construct null
    CLASSNAME();
    //- Construct from components
    CLASSNAME(const dataType& data);
    //- Construct from Istream
    CLASSNAME(Istream&);
    //- Construct as copy
    CLASSNAME(const CLASSNAME&);
  // Selectors
    //- Select null constructed
    static autoPtr<CLASSNAME> New();
  //- Destructor
  ~CLASSNAME();
  // Member Functions
    // Access
    // Check
    // Edit
    // Write
  // Member Operators
    void operator=(const CLASSNAME&);
  // Friend Functions
  // Friend Operators
  // IOstream Operators
    friend Istream& operator>>(Istream&, CLASSNAME&);
    friend Ostream& operator<<(Ostream&, const CLASSNAME&);
};
}  // namespace mousse
#include "classnamei.hpp"
#endif
