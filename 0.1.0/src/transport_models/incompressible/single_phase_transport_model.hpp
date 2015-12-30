// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::singlePhaseTransportModel
// Description
//   A simple single-phase transport model based on viscosityModel.
//   Used by the incompressible single-phase solvers like mousse-simple,
//   mousse-turb etc.
// SourceFiles
//   single_phase_transport_model.cpp
#ifndef single_phase_transport_model_hpp_
#define single_phase_transport_model_hpp_
#include "incompressible/transport_model.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
namespace mousse
{
class viscosityModel;
class singlePhaseTransportModel
:
  public IOdictionary,
  public transportModel
{
  // Private Data
    autoPtr<viscosityModel> viscosityModelPtr_;
  // Private Member Functions
    //- Disallow copy construct
    singlePhaseTransportModel(const singlePhaseTransportModel&);
    //- Disallow default bitwise assignment
    void operator=(const singlePhaseTransportModel&);
public:
  // Constructors
    //- Construct from components
    singlePhaseTransportModel
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~singlePhaseTransportModel();
  // Member Functions
    //- Return the laminar viscosity
    virtual tmp<volScalarField> nu() const;
    //- Return the laminar viscosity for patch
    virtual tmp<scalarField> nu(const label patchi) const;
    //- Correct the laminar viscosity
    virtual void correct();
    //- Read transportProperties dictionary
    virtual bool read();
};
}  // namespace mousse
#endif
