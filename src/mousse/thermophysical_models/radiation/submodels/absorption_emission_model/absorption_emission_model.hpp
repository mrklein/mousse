#ifndef THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_ABSORPTION_EMISSION_MODEL_ABSORPTION_EMISSION_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_RADIATION_SUBMODELS_ABSORPTION_EMISSION_MODEL_ABSORPTION_EMISSION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::absorptionEmissionModel
// Description
//   Model to supply absorption and emission coefficients for radiation
//   modelling

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "vol_fields.hpp"
#include "vector_2d.hpp"


namespace mousse {
namespace radiation {

class absorptionEmissionModel
{
protected:
  // Protected data
    //- Radiation model dictionary
    const dictionary dict_;
    //- Reference to the fvMesh
    const fvMesh& mesh_;
public:
  //- Runtime type information
  TYPE_NAME("absorptionEmissionModel");
  //- Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      absorptionEmissionModel,
      dictionary,
      (
        const dictionary& dict,
        const fvMesh& mesh
      ),
      (dict, mesh)
    );
  // Constructors
    //- Construct from components
    absorptionEmissionModel
    (
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Selector
  static autoPtr<absorptionEmissionModel> New
  (
    const dictionary& dict,
    const fvMesh& mesh
  );
  //- Destructor
  virtual ~absorptionEmissionModel();
  // Member Functions
    // Access
      //- Reference to the mesh
      inline const fvMesh& mesh() const { return mesh_; }
      //- Reference to the dictionary
      inline const dictionary& dict() const { return dict_; }
      // Absorption coefficient
        //- Absorption coefficient (net)
        virtual tmp<volScalarField> a(const label bandI = 0) const;
        //- Absorption coefficient for continuous phase
        virtual tmp<volScalarField> aCont(const label bandI = 0) const;
        //- Absorption coefficient for dispersed phase
        virtual tmp<volScalarField> aDisp(const label bandI = 0) const;
      // Emission coefficient
        //- Emission coefficient (net)
        virtual tmp<volScalarField> e(const label bandI = 0) const;
        //- Return emission coefficient for continuous phase
        virtual tmp<volScalarField> eCont(const label bandI = 0) const;
        //- Return emission coefficient for dispersed phase
        virtual tmp<volScalarField> eDisp(const label bandI = 0) const;
      // Emission contribution
        //- Emission contribution (net)
        virtual tmp<volScalarField> E(const label bandI = 0) const;
        //- Emission contribution for continuous phase
        virtual tmp<volScalarField> ECont(const label bandI = 0) const;
        //- Emission contribution for dispersed phase
        virtual tmp<volScalarField> EDisp(const label bandI = 0) const;
      //- Const access to the number of bands - defaults to 1 for grey
      //  absorption/emission
      virtual label nBands() const;
      //- Const access to the bands - defaults to Vector2D::one for grey
      //  absorption/emission
      virtual const Vector2D<scalar>& bands(const label n) const;
      //- Flag for whether the absorption/emission is for a grey gas
      virtual bool isGrey() const;
      //- Correct absorption coefficients
      virtual void correct
      (
        volScalarField& a,
        PtrList<volScalarField>& aj
      ) const;
};

}  // namespace radiation
}  // namespace mousse

#endif

