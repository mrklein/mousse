// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Euler
// Description
//   Euler-implicit integration
#ifndef _euler_hpp_
#define _euler_hpp_
#include "_integration_scheme.hpp"
namespace mousse
{
template<class Type>
class Euler
:
  public IntegrationScheme<Type>
{
public:
  //- Runtime type information
  TypeName("Euler");
  // Constructors
    //- Construct from components
    Euler(const word& phiName, const dictionary& dict);
    //- Copy constructor
    Euler(const Euler& is);
    //- Construct and return clone
    virtual autoPtr<IntegrationScheme<Type> > clone() const
    {
      return autoPtr<IntegrationScheme<Type> >(new Euler<Type>(*this));
    }
  //- Destructor
  virtual ~Euler();
  // Member Functions
    //- Perform the integration
    virtual typename IntegrationScheme<Type>::integrationResult integrate
    (
      const Type& phi,
      const scalar dt,
      const Type& alphaBeta,
      const scalar beta
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_euler.cpp"
#endif
#endif
