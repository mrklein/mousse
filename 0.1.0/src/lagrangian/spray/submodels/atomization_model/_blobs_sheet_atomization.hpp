// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BlobsSheetAtomization
// Description
//   Primary Breakup Model for pressure swirl atomizers.
//   Accurate description in
//   @verbatim
//   Z. Han, S. Parrish, P.V. Farrell, R.D. Reitz
//   "Modeling Atomization Processes Of Pressure Swirl Hollow-Cone Fuel Sprays"
//   Atomization and Sprays, vol. 7, pp. 663-684, 1997
//   and
//   L. Allocca, G. Bella, A. De Vita, L. Di Angelo
//   "Experimental Validation of a GDI Spray Model"
//   SAE Technical Paper Series, 2002-01-1137
//   @endverbatim

#ifndef _blobs_sheet_atomization_hpp_
#define _blobs_sheet_atomization_hpp_

#include "_atomization_model.hpp"

namespace mousse
{
template<class CloudType>
class BlobsSheetAtomization
:
  public AtomizationModel<CloudType>
{
private:
  scalar B_;
  scalar angle_;
public:
  //- Runtime type information
  TypeName("blobsSheetAtomization");
  // Constructors
    //- Construct from dictionary
    BlobsSheetAtomization(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    BlobsSheetAtomization(const BlobsSheetAtomization<CloudType>& am);
    //- Construct and return a clone
    virtual autoPtr<AtomizationModel<CloudType> > clone() const
    {
      return autoPtr<AtomizationModel<CloudType> >
      (
        new BlobsSheetAtomization<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~BlobsSheetAtomization();
  // Member Functions
    //- Initial value of liquidCore
    virtual scalar initLiquidCore() const;
    //- Flag to indicate if chi needs to be calculated
    virtual bool calcChi() const;
    virtual void update
    (
      const scalar dt,
      scalar& d,
      scalar& liquidCore,
      scalar& tc,
      const scalar rho,
      const scalar mu,
      const scalar sigma,
      const scalar volFlowRate,
      const scalar rhoAv,
      const scalar Urel,
      const vector& pos,
      const vector& injectionPos,
      const scalar pAmbient,
      const scalar chi,
      cachedRandom& rndGen
    ) const;
};
}  // namespace mousse

#ifdef NoRepository
#   include "_blobs_sheet_atomization.cpp"
#endif
#endif
