#ifndef LAGRANGIAN_INTERMEDIATE_TINTEGRATION_SCHEME_TANALYTICAL_HPP_
#define LAGRANGIAN_INTERMEDIATE_TINTEGRATION_SCHEME_TANALYTICAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Analytical
// Description
//   Analytical integration

#include "_integration_scheme.hpp"


namespace mousse {

template<class Type>
class Analytical
:
  public IntegrationScheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("analytical");
  // Constructors
    //- Construct from components
    Analytical(const word& phiName, const dictionary& dict);
    //- Copy constructor
    Analytical(const Analytical& is);
    //- Construct and return clone
    virtual autoPtr<IntegrationScheme<Type>> clone() const
    {
      return
        autoPtr<IntegrationScheme<Type>>
        {
          new Analytical<Type>{*this}
        };
    }
  //- Destructor
  virtual ~Analytical();
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

#include "_analytical.ipp"

#endif
