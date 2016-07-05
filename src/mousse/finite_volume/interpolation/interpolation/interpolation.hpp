#ifndef FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_HPP_
#define FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolation
// Description
//   Abstract base class for interpolation

#include "face_list.hpp"
#include "vol_fields_fwd.hpp"
#include "point_fields.hpp"
#include "type_info.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "tet_indices.hpp"


namespace mousse {

class polyMesh;


template<class Type>
class interpolation
{
protected:
  // Protected data
    const GeometricField<Type, fvPatchField, volMesh>& psi_;
    const polyMesh& pMesh_;
    const vectorField& pMeshPoints_;
    const faceList& pMeshFaces_;
    const vectorField& pMeshFaceCentres_;
    const vectorField& pMeshFaceAreas_;
public:
  //- Runtime type information
  virtual const word& type() const = 0;
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      interpolation,
      dictionary,
      (
        const GeometricField<Type, fvPatchField, volMesh>& psi
      ),
      (psi)
    );
  // Selectors
    //- Return a reference to the specified interpolation scheme
    static autoPtr<interpolation<Type> > New
    (
      const word& interpolationType,
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
    //- Return a reference to the selected interpolation scheme
    static autoPtr<interpolation<Type> > New
    (
      const dictionary& interpolationSchemes,
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
  // Constructors
    //- Construct from components
    interpolation
    (
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
  //- Destructor
  virtual ~interpolation()
  {}
  // Member Functions
    //- Return the field to be interpolated
    const GeometricField<Type, fvPatchField, volMesh>& psi() const
    {
      return psi_;
    }
    //- Interpolate field to the given point in the given cell
    virtual Type interpolate
    (
      const vector& position,
      const label cellI,
      const label faceI = -1
    ) const = 0;
    //- Interpolate field to the given point in the tetrahedron
    //  defined by the given indices.  Calls interpolate function
    //  above here execpt where overridden by derived
    //  interpolation types.
    virtual Type interpolate
    (
      const vector& position,
      const tetIndices& tetIs,
      const label faceI = -1
    ) const
    {
      return interpolate(position, tetIs.cell(), faceI);
    }
};
}  // namespace mousse


#define MAKE_INTERPOLATION_TYPE(SS, Type)                                     \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS<Type>, 0);                       \
                                                                              \
interpolation<Type>::adddictionaryConstructorToTable<SS<Type> >               \
  add##SS##Type##ConstructorToTable_;


#define MAKE_INTERPOLATION(SS)                                                \
                                                                              \
MAKE_INTERPOLATION_TYPE(SS, scalar)                                           \
MAKE_INTERPOLATION_TYPE(SS, vector)                                           \
MAKE_INTERPOLATION_TYPE(SS, sphericalTensor)                                  \
MAKE_INTERPOLATION_TYPE(SS, symmTensor)                                       \
MAKE_INTERPOLATION_TYPE(SS, tensor)


#include "interpolation.ipp"

#endif
