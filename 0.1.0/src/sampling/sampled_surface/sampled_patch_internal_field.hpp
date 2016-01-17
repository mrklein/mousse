// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sampledPatchInternalField
// Description
//   Variation of sampledPatch that samples the internalField (at a given
//   normal distance from the patch) instead of the patchField.
//   Note:
//   - interpolate=false : get cell value on faces
//   - interpolate=true  : interpolate inside cell and interpolate to points
//   There is no option to get interpolated value inside the cell on the faces.
// SourceFiles
//   sampled_patch_internal_field.cpp
#ifndef sampled_patch_internal_field_hpp_
#define sampled_patch_internal_field_hpp_
#include "sampled_patch.hpp"
#include "mapped_patch_base.hpp"
namespace mousse
{
class sampledPatchInternalField
:
  public sampledPatch
{
  // Private data
    //- Mapping engines
    PtrList<mappedPatchBase> mappers_;
  // Private Member Functions
    //- Sample field on faces
    template<class Type>
    tmp<Field<Type> > sampleField
    (
      const GeometricField<Type, fvPatchField, volMesh>& vField
    ) const;
    template<class Type>
    tmp<Field<Type> > interpolateField(const interpolation<Type>&) const;
public:
  //- Runtime type information
  TYPE_NAME("sampledPatchInternalField");
  // Constructors
    //- Construct from dictionary
    sampledPatchInternalField
    (
      const word& name,
      const polyMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~sampledPatchInternalField();
  // Member Functions
    // Sample
      //- Sample field on surface
      virtual tmp<scalarField> sample
      (
        const volScalarField&
      ) const;
      //- Sample field on surface
      virtual tmp<vectorField> sample
      (
        const volVectorField&
      ) const;
      //- Sample field on surface
      virtual tmp<sphericalTensorField> sample
      (
        const volSphericalTensorField&
      ) const;
      //- Sample field on surface
      virtual tmp<symmTensorField> sample
      (
        const volSymmTensorField&
      ) const;
      //- Sample field on surface
      virtual tmp<tensorField> sample
      (
        const volTensorField&
      ) const;
    // Interpolate
      //- Interpolate field on surface
      virtual tmp<scalarField> interpolate
      (
        const interpolation<scalar>&
      ) const;
      //- Interpolate field on surface
      virtual tmp<vectorField> interpolate
      (
        const interpolation<vector>&
      ) const;
      //- Interpolate field on surface
      virtual tmp<sphericalTensorField> interpolate
      (
        const interpolation<sphericalTensor>&
      ) const;
      //- Interpolate field on surface
      virtual tmp<symmTensorField> interpolate
      (
        const interpolation<symmTensor>&
      ) const;
      //- Interpolate field on surface
      virtual tmp<tensorField> interpolate
      (
        const interpolation<tensor>&
      ) const;
    //- Write
    virtual void print(Ostream&) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "sampled_patch_internal_field_templates.cpp"
#endif
#endif
