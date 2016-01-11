// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::scatterModel
// Description
//   Base class for radiation scattering
#ifndef scatter_model_hpp_
#define scatter_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "vol_fields.hpp"
namespace mousse
{
namespace radiation
{
class scatterModel
{
protected:
  // Protected data
    //- Reference to the fvMesh
    const fvMesh& mesh_;
public:
  //- Runtime type information
  TYPE_NAME("scatterModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      scatterModel,
      dictionary,
      (
        const dictionary& dict,
        const fvMesh& mesh
      ),
      (dict, mesh)
    );
  // Constructors
    //- Construct from components
    scatterModel(const dictionary& dict, const fvMesh& mesh);
  // Selector
    static autoPtr<scatterModel> New
    (
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~scatterModel();
  // Member Functions
    //- Return scatter coefficient
    virtual tmp<volScalarField> sigmaEff() const = 0;
};
}  // namespace radiation
}  // namespace mousse
#endif
