// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DispersionRASModel
// Description
//   Base class for particle dispersion models based on RAS turbulence.

#ifndef _dispersion_ras_model_hpp_
#define _dispersion_ras_model_hpp_
#include "_dispersion_model.hpp"

namespace mousse
{
template<class CloudType>
class DispersionRASModel
:
  public DispersionModel<CloudType>
{
protected:
  // Protected data
    // Locally cached turbulence fields
      //- Turbulence k
      const volScalarField* kPtr_;
      //- Take ownership of the k field
      mutable bool ownK_;
      //- Turbulence epsilon
      const volScalarField* epsilonPtr_;
      //- Take ownership of the epsilon field
      mutable bool ownEpsilon_;
  // Protected Functions
    //- Return the k field from the turbulence model
    tmp<volScalarField> kModel() const;
    //- Return the epsilon field from the turbulence model
    tmp<volScalarField> epsilonModel() const;
public:
  //- Runtime type information
  TYPE_NAME("dispersionRASModel");
  // Constructors
    //- Construct from components
    DispersionRASModel(const dictionary& dict, CloudType& owner);
    //- Construct copy
    DispersionRASModel(const DispersionRASModel<CloudType>& dm);
    //- Construct and return a clone
    virtual autoPtr<DispersionModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~DispersionRASModel();
  // Member Functions
    //- Update (disperse particles)
    virtual vector update
    (
      const scalar dt,
      const label cellI,
      const vector& U,
      const vector& Uc,
      vector& UTurb,
      scalar& tTurb
    ) = 0;
    //- Cache carrier fields
    virtual void cacheFields(const bool store);
    // I-O
      //- Write
      virtual void write(Ostream& os) const;
};
}  // namespace mousse

#ifdef NoRepository
#   include "_dispersion_ras_model.cpp"
#endif
#endif
