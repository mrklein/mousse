#ifndef CORE_FIELDS_GEOMETRIC_FIELDS_SLICED_GEOMETRIC_FIELD_HPP_
#define CORE_FIELDS_GEOMETRIC_FIELDS_SLICED_GEOMETRIC_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SlicedGeometricField
// Description
//   Specialization of GeometricField which holds slices of given complete
//   fields in a form that they act as a GeometricField.
//   The destructor is wrapped to avoid deallocation of the storage of the
//   complete fields when this is destroyed.
//   SlicedGeometricField can only be instantiated with a valid form of
//   SlicedPatchField to handle the slicing and storage deallocation of the
//   boundary field.
// SourceFiles
//   sliced_geometric_field.cpp


#include "geometric_field.hpp"

namespace mousse
{

template
<
  class Type,
  template<class> class PatchField,
  template<class> class SlicedPatchField,
  class GeoMesh
>
class SlicedGeometricField
:
  public GeometricField<Type, PatchField, GeoMesh>
{
public:
  typedef typename GeoMesh::Mesh Mesh;
  typedef typename GeoMesh::BoundaryMesh BoundaryMesh;
  class DimensionedInternalField;

private:

  // Private Member Functions

    //- Slice the given field and a create a PtrList of SlicedPatchField
    //  from which the boundary field is built
    tmp<FieldField<PatchField, Type>>  slicedBoundaryField
    (
      const Mesh& mesh,
      const Field<Type>& completeField,
      const bool preserveCouples,
      const bool preserveProcessorOnly = false
    );

    //- Slice the given field and a create a PtrList of SlicedPatchField
    //  from which the boundary field is built
    tmp<FieldField<PatchField, Type> >  slicedBoundaryField
    (
      const Mesh& mesh,
      const FieldField<PatchField, Type>& bField,
      const bool preserveCouples
    );

public:

  // Constructors

    //- Construct from components and field to slice
    SlicedGeometricField
    (
      const IOobject&,
      const Mesh&,
      const dimensionSet&,
      const Field<Type>& completeField,
      const bool preserveCouples=true
    );

    //- Construct from components and separate fields to slice for the
    //  internal field and boundary field
    SlicedGeometricField
    (
      const IOobject&,
      const Mesh&,
      const dimensionSet&,
      const Field<Type>& completeIField,
      const Field<Type>& completeBField,
      const bool preserveCouples=true,
      const bool preserveProcessorOnly = false
    );

    //- Construct from GeometricField. Reuses full internal and
    //  patch fields except on couples (preserveCouples=true).
    SlicedGeometricField
    (
      const IOobject&,
      const GeometricField<Type, PatchField, GeoMesh>&,
      const bool preserveCouples=true
    );

    //- Construct as copy
    SlicedGeometricField
    (
      const SlicedGeometricField
      <
        Type,
        PatchField,
        SlicedPatchField,
        GeoMesh
      >&
    );

    //- Disallow default bitwise assignment
    SlicedGeometricField& operator=(const SlicedGeometricField&) = delete;

    //- Disallow standard assignment to GeometricField,
    //  == assignment is allowed.
    void operator=(const GeometricField<Type, PatchField, GeoMesh>&) = delete;

    //- Disallow standard assignment to tmp<GeometricField>,
    //  == assignment is allowed.
    void operator=(const tmp<GeometricField<Type, PatchField, GeoMesh>>&) = delete;

  //- Destructor
  ~SlicedGeometricField();

  // Member functions
    //- Correct boundary field
    void correctBoundaryConditions();
};

//- The internalField of a SlicedGeometricField
template
<
  class Type,
  template<class> class PatchField,
  template<class> class SlicedPatchField,
  class GeoMesh
>
class SlicedGeometricField<Type, PatchField, SlicedPatchField, GeoMesh>::
DimensionedInternalField
:
  public GeometricField<Type, PatchField, GeoMesh>::DimensionedInternalField
{
public:
  // Constructors
    //- Construct from components and field to slice
    DimensionedInternalField
    (
      const IOobject&,
      const Mesh&,
      const dimensionSet&,
      const Field<Type>& iField
    );
  //- Destructor
  ~DimensionedInternalField();
};

}  // namespace mousse

#ifdef NoRepository
#   include "sliced_geometric_field.cpp"
#endif
#endif
