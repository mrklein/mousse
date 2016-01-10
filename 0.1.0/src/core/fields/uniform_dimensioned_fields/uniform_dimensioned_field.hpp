// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UniformDimensionedField
// Description
//   Dimensioned<Type> registered with the database as a registered IOobject
//   which has the functionality of a uniform field and allows values from the
//   top-level code to be passed to boundary conditions etc.
// SourceFiles
//   uniform_dimensioned_field.cpp
//
#ifndef uniform_dimensioned_field_hpp_
#define uniform_dimensioned_field_hpp_

#include "reg_ioobject.hpp"
#include "dimensioned_type.hpp"

namespace mousse
{
template<class Type>
class UniformDimensionedField
:
  public regIOobject,
  public dimensioned<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("UniformDimensionedField");
  // Constructors
    //- Construct from components. Either reads or uses supplied value.
    UniformDimensionedField(const IOobject&, const dimensioned<Type>&);
    //- Construct as copy
    UniformDimensionedField(const UniformDimensionedField<Type>&);
    //- Construct from Istream
    UniformDimensionedField(const IOobject&);
  //- Destructor
  virtual ~UniformDimensionedField();
  // Member Functions
    //- Name function provided to resolve the ambiguity between the
    //  name functions in regIOobject and dimensioned<Type>
    virtual const word& name() const
    {
      return dimensioned<Type>::name();
    }
    bool writeData(Ostream&) const;
  // Member Operators
    void operator=(const UniformDimensionedField<Type>&);
    void operator=(const dimensioned<Type>&);
    Type operator[](const label) const
    {
      return this->value();
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "uniform_dimensioned_field.cpp"
#endif
#endif
