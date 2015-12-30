// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SRF::SRFModel
// Description
//   Top level model for single rotating frame
//   - Steady state only - no time derivatives included
// SourceFiles
//   srf_model.cpp
#ifndef srf_model_hpp_
#define srf_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "vector_field.hpp"
namespace mousse
{
namespace SRF
{
class SRFModel
:
  public IOdictionary
{
protected:
  // Protected data
    //- Reference to the relative velocity field
    const volVectorField& Urel_;
    //- Reference to the mesh
    const fvMesh& mesh_;
    //- Origin of the axis
    dimensionedVector origin_;
    //- Axis of rotation, a direction vector which passes through the origin
    vector axis_;
    //- SRF model coeficients dictionary
    dictionary SRFModelCoeffs_;
    //- Angular velocity of the frame (rad/s)
    dimensionedVector omega_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    SRFModel(const SRFModel&);
    //- Disallow default bitwise assignment
    void operator=(const SRFModel&);
public:
  //- Runtime type information
  TypeName("SRFModel");
  // Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      SRFModel,
      dictionary,
      (
        const volVectorField& Urel
      ),
      (Urel)
    );
  // Constructors
    //- Construct from components
    SRFModel
    (
      const word& type,
      const volVectorField& Urel
    );
  // Selectors
    //- Return a reference to the selected SRF model
    static autoPtr<SRFModel> New
    (
      const volVectorField& Urel
    );
  //- Destructor
  virtual ~SRFModel();
  // Member Functions
    // Edit
      //- Read radiationProperties dictionary
      virtual bool read();
    // Access
      //- Return the origin of rotation
      const dimensionedVector& origin() const;
      //- Return the axis of rotation
      const vector& axis() const;
      //- Return the angular velocity field [rad/s]
      const dimensionedVector& omega() const;
      //- Return the coriolis force
      tmp<DimensionedField<vector, volMesh> > Fcoriolis() const;
      //- Return the centrifugal force
      tmp<DimensionedField<vector, volMesh> > Fcentrifugal() const;
      //- Source term component for momentum equation
      tmp<DimensionedField<vector, volMesh> > Su() const;
      //- Return velocity vector from positions
      vectorField velocity(const vectorField& positions) const;
      //- Return velocity of SRF for complete mesh
      tmp<volVectorField> U() const;
      //- Return absolute velocity for complete mesh
      tmp<volVectorField> Uabs() const;
};
}  // namespace SRF
}  // namespace mousse
#endif
