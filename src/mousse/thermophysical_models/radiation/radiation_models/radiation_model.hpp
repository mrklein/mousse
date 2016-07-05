#ifndef THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_RADIATION_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_RADIATION_MODELS_RADIATION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::radiationModel
// Description
//   Top level model for radiation modelling

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "vol_fields_fwd.hpp"
#include "dimensioned_field.hpp"
#include "fv_matrices_fwd.hpp"
#include "switch.hpp"


namespace mousse {

class fluidThermo;
class fvMesh;

namespace radiation {

// Forward declaration of classes
class absorptionEmissionModel;
class scatterModel;
class sootModel;


class radiationModel
:
  public IOdictionary
{
protected:
  // Protected data
    //- Reference to the mesh database
    const fvMesh& mesh_;
    //- Reference to the time database
    const Time& time_;
    //- Reference to the temperature field
    const volScalarField& T_;
    //- Radiation model on/off flag
    Switch radiation_;
    //- Radiation model dictionary
    dictionary coeffs_;
    //- Radiation solver frequency - number flow solver iterations per
    //  radiation solver iteration
    label solverFreq_;
    //- Flag to enable radiation model to be evaluated on first iteration
    bool firstIter_;
    // References to the radiation sub-models
      //- Absorption/emission model
      autoPtr<absorptionEmissionModel> absorptionEmission_;
      //- Scatter model
      autoPtr<scatterModel> scatter_;
      //- Soot model
      autoPtr<sootModel> soot_;
private:
  // Private Member Functions
    //- Create IO object if dictionary is present
    IOobject createIOobject(const fvMesh& mesh) const;
    //- Initialise
    void initialise();
public:
  //- Runtime type information
  TYPE_NAME("radiationModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      radiationModel,
      T,
      (
        const volScalarField& T
      ),
      (T)
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      radiationModel,
      dictionary,
      (
        const dictionary& dict,
        const volScalarField& T
      ),
      (dict, T)
    );
  // Constructors
    //- Null constructor
    radiationModel(const volScalarField& T);
    //- Construct from components
    radiationModel(const word& type, const volScalarField& T);
    //- Construct from components
    radiationModel
    (
      const word& type,
      const dictionary& dict,
      const volScalarField& T
    );
    //- Disallow default bitwise copy construct
    radiationModel(const radiationModel&) = delete;
    //- Disallow default bitwise assignment
    radiationModel& operator=(const radiationModel&) = delete;
  // Selectors
    //- Return a reference to the selected radiation model
    static autoPtr<radiationModel> New(const volScalarField& T);
    //- Return a reference to the selected radiation model
    static autoPtr<radiationModel> New
    (
      const dictionary& dict,
      const volScalarField& T
    );
  //- Destructor
  virtual ~radiationModel();
  // Member Functions
    // Edit
      //- Main update/correction routine
      virtual void correct();
      //- Solve radiation equation(s)
      virtual void calculate() = 0;
      //- Read radiationProperties dictionary
      virtual bool read() = 0;
    // Access
      //- Radiation model on/off flag
      const Switch radiation() const { return radiation_; }
      //- Source term component (for power of T^4)
      virtual tmp<volScalarField> Rp() const = 0;
      //- Source term component (constant)
      virtual tmp<DimensionedField<scalar, volMesh> > Ru() const = 0;
      //- Energy source term
      virtual tmp<fvScalarMatrix> Sh(fluidThermo& thermo) const;
      //- Temperature source term
      virtual tmp<fvScalarMatrix> ST
      (
        const dimensionedScalar& rhoCp,
        volScalarField& T
      ) const;
      //- Access to absorptionEmission model
      const absorptionEmissionModel& absorptionEmission() const;
      //- Access to soot Model
      const sootModel& soot() const;
};


#define ADD_TO_RADIATION_RUN_TIME_SELECTION_TABLES(model)                     \
                                                                              \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    radiationModel,                                                           \
    model,                                                                    \
    dictionary                                                                \
  );                                                                          \
                                                                              \
  ADD_TO_RUN_TIME_SELECTION_TABLE                                             \
  (                                                                           \
    radiationModel,                                                           \
    model,                                                                    \
    T                                                                         \
  );

}  // namespace radiation
}  // namespace mousse

#endif

