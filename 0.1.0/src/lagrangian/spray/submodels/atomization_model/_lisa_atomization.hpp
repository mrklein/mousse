#ifndef LAGRANGIAN_SPRAY_SUBMODELS_ATOMIZATION_MODEL_TLISA_ATOMIZATION_HPP_
#define LAGRANGIAN_SPRAY_SUBMODELS_ATOMIZATION_MODEL_TLISA_ATOMIZATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LISAAtomization
// Description
//   Primary Breakup Model for pressure swirl atomizers.
//   Accurate description in
//   @verbatim
//   P.K. Senecal, D.P. Schmidt, I. Nouar, C.J. Rutland, R.D. Reitz, M. Corradini
//   "Modeling high-speed viscous liquid sheet atomization"
//   International Journal of Multiphase Flow 25 (1999) pags. 1073-1097
//   @endverbatim
//   and
//   @verbatim
//   D.P. Schmidt, I. Nouar, P.K. Senecal, C.J. Rutland, J.K. Martin, R.D. Reitz
//   "Pressure-Swirl Atomization in the Near Field"
//   SAE Techical Paper Series 1999-01-0496
//   @endverbatim
//

#include "_atomization_model.hpp"

namespace mousse
{
template<class CloudType>
class LISAAtomization
:
  public AtomizationModel<CloudType>
{
public:
  //- Enumeration for SMD calculations
  enum SMDMethods
  {
    method1,
    method2
  };
private:
  // Private data
    scalar Cl_;
    scalar cTau_;
    scalar Q_;
    scalar lisaExp_;
    vector injectorDirection_;
    word SMDCalcMethod_;
    SMDMethods SMDMethod_;
public:
  //- Runtime type information
  TYPE_NAME("LISA");
  // Constructors
    //- Construct from dictionary
    LISAAtomization(const dictionary&, CloudType&);
    //- Construct copy
    LISAAtomization(const LISAAtomization<CloudType>& am);
    //- Construct and return a clone
    virtual autoPtr<AtomizationModel<CloudType> > clone() const
    {
      return autoPtr<AtomizationModel<CloudType> >
      (
        new LISAAtomization<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~LISAAtomization();
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
#   include "_lisa_atomization.cpp"
#endif
#endif
