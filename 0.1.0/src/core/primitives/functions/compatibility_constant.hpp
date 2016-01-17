// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CompatibilityConstant
// Description
//   Templated basic entry that holds a constant value for backwards
//   compatibility (when DataEntry type is not present)
//   Usage - for entry \<entryName\> having the value <value>:
//   \verbatim
//     <entryName>    <value>
//   \endverbatim
// SourceFiles
//   compatibility_constant.cpp

#ifndef compatibility_constant_hpp_
#define compatibility_constant_hpp_

#include "data_entry.hpp"
#include "dimension_set.hpp"

namespace mousse
{

template<class Type>
class CompatibilityConstant;

template<class Type>
Ostream& operator<<(Ostream&, const CompatibilityConstant<Type>&);

template<class Type>
class CompatibilityConstant
:
  public DataEntry<Type>
{
  // Private data

    //- Constant value
    Type value_;

    //- The dimension set
    dimensionSet dimensions_;

public:

  // Runtime type information
  TYPE_NAME("CompatibilityConstant");

  // Constructors

    //- Construct from entry name and Istream
    CompatibilityConstant(const word& entryName, const dictionary& dict);

    //- Copy constructor
    CompatibilityConstant(const CompatibilityConstant<Type>& cnst);

    //- Disallow default bitwise assignment
    CompatibilityConstant<Type>& operator=
    (
      const CompatibilityConstant<Type>&
    ) = delete;

    //- Construct and return a clone
    virtual tmp<DataEntry<Type>> clone() const
    {
      return tmp<DataEntry<Type>>
      {
        new CompatibilityConstant<Type>(*this)
      };
    }

  //- Destructor
  virtual ~CompatibilityConstant();

  // Member Functions

    //- Return constant value
    Type value(const scalar) const;

    //- Integrate between two values
    Type integrate(const scalar x1, const scalar x2) const;

    //- Return dimensioned constant value
    dimensioned<Type> dimValue(const scalar) const;

    //- Integrate between two values and return dimensioned type
    dimensioned<Type> dimIntegrate
    (
      const scalar x1,
      const scalar x2
    ) const;

  // I/O

    //- Ostream Operator
    friend Ostream& operator<< <Type>
    (
      Ostream& os,
      const CompatibilityConstant<Type>& cnst
    );

    //- Write in dictionary format
    virtual void writeData(Ostream& os) const;
};

}  // namespace mousse

#ifdef NoRepository
#   include "compatibility_constant.cpp"
#endif
#endif
