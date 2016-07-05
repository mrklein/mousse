#ifndef SOLVERS_COMBUSTION_PDR_PDR_MODELS_DRAG_MODELS_PDR_DRAG_MODEL_HPP_
#define SOLVERS_COMBUSTION_PDR_PDR_MODELS_DRAG_MODELS_PDR_DRAG_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PDRDragModel
// Description
//   Base-class for sub-grid obstacle drag models. The available drag model is at
//   \link basic.hpp \endlink.

#include "iodictionary.hpp"
#include "psiu_reaction_thermo.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "multivariate_surface_interpolation_scheme.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

class PDRDragModel
:
  public regIOobject
{
protected:
  // Protected data
    dictionary PDRDragModelCoeffs_;
    const compressible::RASModel& turbulence_;
    const volScalarField& rho_;
    const volVectorField& U_;
    const surfaceScalarField& phi_;
    Switch on_;
public:
  //- Runtime type information
  TYPE_NAME("PDRDragModel");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      PDRDragModel,
      dictionary,
      (
        const dictionary& PDRProperties,
        const compressible::RASModel& turbulence,
        const volScalarField& rho,
        const volVectorField& U,
        const surfaceScalarField& phi
      ),
      (
        PDRProperties,
        turbulence,
        rho,
        U,
        phi
      )
    );
  // Selectors
    //- Return a reference to the selected Xi model
    static autoPtr<PDRDragModel> New
    (
      const dictionary& PDRProperties,
      const compressible::RASModel& turbulence,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  // Constructors
    //- Construct from components
    PDRDragModel
    (
      const dictionary& PDRProperties,
      const compressible::RASModel& turbulence,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
    //- Disallow copy construct
    PDRDragModel(const PDRDragModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const PDRDragModel&) = delete;
  //- Destructor
  virtual ~PDRDragModel();
  // Member Functions
    //- Return true if the drag model is switched on
    bool on() const
    {
      return on_;
    }
    //- Return the momentum drag coefficient
    virtual tmp<volSymmTensorField> Dcu() const = 0;
    //- Return the momentum drag turbulence generation rate
    virtual tmp<volScalarField> Gk() const = 0;
    //- Inherit read from regIOobject
    using regIOobject::read;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& PDRProperties) = 0;
    virtual bool writeData(Ostream&) const { return true; }
    virtual void writeFields() const
    {
      NOT_IMPLEMENTED("PDRDragModel::write()");
    }
};

}  // namespace mousse

#endif

