// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Constant
// Description
//   Templated basic entry that holds a constant value.
//   Usage - for entry \<entryName\> having the value <value>:
//   \verbatim
//     <entryName>    constant  <value>
//   \endverbatim
// SourceFiles
//   constant.cpp

#ifndef constant_hpp_
#define constant_hpp_

#include "data_entry.hpp"
#include "dimension_set.hpp"

namespace mousse
{

template<class Type>
class Constant;
template<class Type>
Ostream& operator<<(Ostream&, const Constant<Type>&);

template<class Type>
class Constant
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
  TYPE_NAME("constant");
  // Constructors
    //- Construct from entry name and Istream
    Constant(const word& entryName, const dictionary& dict);
    //- Copy constructor
    Constant(const Constant<Type>& cnst);
    //- Construct and return a clone
    virtual tmp<DataEntry<Type> > clone() const
    {
      return tmp<DataEntry<Type> >(new Constant<Type>(*this));
    }
    //- Disallow default bitwise assignment
    Constant<Type>& operator=(const Constant<Type>&) = delete;
  //- Destructor
  virtual ~Constant();
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
      const Constant<Type>& cnst
    );
    //- Write in dictionary format
    virtual void writeData(Ostream& os) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "constant.cpp"
#endif
#endif
