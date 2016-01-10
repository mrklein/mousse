// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SubDimensionedField
// Description
//   SubDimensionedField is a DimensionedField obtained as a section of another
//   DimensionedField.
//   Thus it is itself unallocated so that no storage is allocated or
//   deallocated during its use.  To achieve this behaviour,
//   SubDimensionedField is derived from SubField rather than Field.
// SourceFiles
//   sub_dimensioned_field_i.hpp
#ifndef sub_dimensioned_field_hpp_
#define sub_dimensioned_field_hpp_
#include "field.hpp"
#include "sub_field.hpp"
namespace mousse
{
template<class Type, class GeoMesh>
class SubDimensionedField
:
  public regIOobject,
  public SubField<Type>
{
public:
  // Public typedefs
    typedef typename GeoMesh::Mesh Mesh;
    typedef typename Field<Type>::cmptType cmptType;
  // Constructors
    //- Construct from a SubField
    inline SubDimensionedField
    (
      const SubField<Type>& slist
    );
    //- Construct from a UList and size
    inline SubDimensionedField
    (
      const UList<Type>& list,
      const label subSize
    );
    //- Construct from a UList start and end indices
    inline SubDimensionedField
    (
      const UList<Type>& list,
      const label subSize,
      const label startIndex
    );
    //- Construct as copy
    inline SubDimensionedField
    (
      const SubDimensionedField<cmptType, GeoMesh>& sfield
    );
  // Member functions
    //- Return a null SubDimensionedField
    static inline const SubDimensionedField<Type, GeoMesh>& null();
    //- Return a component field of the field
    inline tmp<DimensionedField<cmptType, GeoMesh> > component
    (
      const direction
    ) const;
    //- Return the field transpose (only defined for second rank tensors)
    tmp<DimensionedField<Type, GeoMesh> > T() const;
  // Member operators
    //- Assignment
    inline void operator=(const SubDimensionedField<Type, GeoMesh>&);
    //- Allow cast to a const DimensionedField<Type, GeoMesh>&
    inline operator const DimensionedField<Type, GeoMesh>&() const;
};
}  // namespace mousse
#include "sub_dimensioned_field_i.hpp"
#endif
