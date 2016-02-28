// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dragModel
// Description
// SourceFiles
//   drag_model.cpp
//   new_drag_model.cpp
#ifndef drag_model_hpp_
#define drag_model_hpp_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class swarmCorrection;
class dragModel
:
  public regIOobject
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
    //- Swarm correction
    autoPtr<swarmCorrection> swarmCorrection_;
public:
  //- Runtime type information
  TYPE_NAME("dragModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      dragModel,
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
    // Construct without residual constants
    dragModel
    (
      const phasePair& pair,
      const bool registerObject
    );
    // Construct with residual constants
    dragModel
    (
      const dictionary& dict,
      const phasePair& pair,
      const bool registerObject
    );
  //- Destructor
  virtual ~dragModel();
  // Selectors
    static autoPtr<dragModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- Drag coefficient
    virtual tmp<volScalarField> CdRe() const = 0;
    //- Return the phase-intensive drag coefficient Ki
    //  used in the momentum equations
    //    ddt(alpha1*rho1*U1) + ... = ... alphad*K*(U1-U2)
    //    ddt(alpha2*rho2*U2) + ... = ... alphad*K*(U2-U1)
    virtual tmp<volScalarField> Ki() const;
    //- Return the drag coefficient K
    //  used in the momentum equations
    //    ddt(alpha1*rho1*U1) + ... = ... K*(U1-U2)
    //    ddt(alpha2*rho2*U2) + ... = ... K*(U2-U1)
    virtual tmp<volScalarField> K() const;
    //- Return the drag coefficient Kf
    //  used in the face-momentum equations
    virtual tmp<surfaceScalarField> Kf() const;
    //- Dummy write for regIOobject
    bool writeData(Ostream& os) const;
};
}  // namespace mousse
#endif
