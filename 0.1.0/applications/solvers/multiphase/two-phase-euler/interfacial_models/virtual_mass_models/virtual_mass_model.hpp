// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::virtualMassModel
// Description
// SourceFiles
//   virtual_mass_model.cpp
//   new_virtual_mass_model.cpp
#ifndef VIRTUAL_MASS_MODEL_HPP_
#define VIRTUAL_MASS_MODEL_HPP_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class virtualMassModel
:
  public regIOobject
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
public:
  //- Runtime type information
  TYPE_NAME("virtualMassModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      virtualMassModel,
      dictionary,
      (
        const dictionary& dict,
        const phasePair& pair,
        const bool registerObject
      ),
      (dict, pair, registerObject)
    );
  // Static data members
    //- Coefficient dimensions
    static const dimensionSet dimK;
  // Constructors
    //- Construct from a dictionary and a phase pair
    virtualMassModel
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~virtualMassModel();
  // Selectors
    static autoPtr<virtualMassModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- Return the virtual mass coefficient
    virtual tmp<volScalarField> Cvm() const = 0;
    //- Return the phase-intensive virtual mass coefficient Ki
    //  used in the momentum equation
    //    ddt(alpha1*rho1*U1) + ... = ... alphad*K*(DU1_Dt - DU2_Dt)
    //    ddt(alpha2*rho2*U2) + ... = ... alphad*K*(DU1_Dt - DU2_Dt)
    virtual tmp<volScalarField> Ki() const;
    //- Return the virtual mass coefficient K
    //  used in the momentum equation
    //    ddt(alpha1*rho1*U1) + ... = ... K*(DU1_Dt - DU2_Dt)
    //    ddt(alpha2*rho2*U2) + ... = ... K*(DU1_Dt - DU2_Dt)
    virtual tmp<volScalarField> K() const;
    //- Return the virtual mass coefficient Kf
    //  used in the face-momentum equations
    virtual tmp<surfaceScalarField> Kf() const;
    // Dummy write for regIOobject
    bool writeData(Ostream& os) const;
};
}  // namespace mousse
#endif
