// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_mixture_properties.hpp"


// Constructors 
mousse::solidMixtureProperties::solidMixtureProperties(const dictionary& dict)
:
  components_{},
  properties_{}
{
  components_ = dict.toc();
  properties_.setSize(components_.size());
  FOR_ALL(components_, i) {
    properties_.set(i, solidProperties::New(dict.subDict(components_[i])));
  }
}


mousse::solidMixtureProperties::solidMixtureProperties
(
  const solidMixtureProperties& s
)
:
  components_{s.components_},
  properties_{s.properties_.size()}
{
  FOR_ALL(properties_, i) {
    properties_.set(i, s.properties_(i)->clone());
  }
}


// Selectors
mousse::autoPtr<mousse::solidMixtureProperties> mousse::solidMixtureProperties::New
(
  const dictionary& thermophysicalProperties
)
{
  return
    autoPtr<solidMixtureProperties>
    {
      new solidMixtureProperties{thermophysicalProperties}
    };
}


// Member Functions 
mousse::scalarField mousse::solidMixtureProperties::X(const scalarField& Y) const
{
  scalarField X{Y.size()};
  scalar rhoInv = 0.0;
  FOR_ALL(X, i) {
    rhoInv += Y[i]/properties_[i].rho();
    X[i] = Y[i]/properties_[i].rho();
  }
  tmp<scalarField> tfld{X/rhoInv};
  return tfld();
}


mousse::scalar mousse::solidMixtureProperties::rho(const scalarField& X) const
{
  scalar val = 0.0;
  FOR_ALL(properties_, i) {
    val += properties_[i].rho()*X[i];
  }
  return val;
}


mousse::scalar mousse::solidMixtureProperties::Cp(const scalarField& Y) const
{
  scalar val = 0.0;
  FOR_ALL(properties_, i) {
    val += properties_[i].Cp()*Y[i];
  }
  return val;
}

