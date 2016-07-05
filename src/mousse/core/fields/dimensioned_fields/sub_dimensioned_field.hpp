#ifndef CORE_FIELDS_DIMENSIONED_FIELDS_SUB_DIMENSIONED_FIELD_HPP_
#define CORE_FIELDS_DIMENSIONED_FIELDS_SUB_DIMENSIONED_FIELD_HPP_

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

#include "field.hpp"
#include "sub_field.hpp"


namespace mousse {

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


// Constructors 
template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
  const SubField<Type>& slist
)
:
  SubField<Type>{slist}
{}


template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
  const UList<Type>& list,
  const label subSize
)
:
  SubField<Type>{list, subSize}
{}


template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
  const UList<Type>& list,
  const label subSize,
  const label startIndex
)
:
  SubField<Type>{list, subSize, startIndex}
{}


template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::SubDimensionedField
(
  const SubDimensionedField<Type, GeoMesh>& sfield
)
:
  refCount{},
  SubField<Type>{sfield}
{}


// Member Functions 
template<class Type, class GeoMesh>
inline const mousse::SubDimensionedField<Type, GeoMesh>&
mousse::SubDimensionedField<Type, GeoMesh>::null()
{
  return NullObjectRef<SubDimensionedField<Type, GeoMesh> >();
}


template<class Type, class GeoMesh>
inline
mousse::tmp
<
  mousse::Field<typename mousse::SubDimensionedField<Type, GeoMesh>::cmptType>
>
mousse::SubDimensionedField<Type, GeoMesh>::component
(
  const direction d
) const
{
  return
  (
    reinterpret_cast<const DimensionedField<Type, GeoMesh>&>(*this)
  ).component(d);
}


template<class Type, class GeoMesh>
inline mousse::tmp<mousse::DimensionedField<Type, GeoMesh> >
mousse::SubDimensionedField<Type, GeoMesh>::T() const
{
  return
  (
    reinterpret_cast<const DimensionedField<Type, GeoMesh>&>(*this)
  ).T();
}


// Member Operators 
template<class Type, class GeoMesh>
inline void mousse::SubDimensionedField<Type, GeoMesh>::operator=
(
  const SubDimensionedField<Type, GeoMesh>& rhs
)
{
  dimensions() = rhs.dimensions();
  SubField<Type>::operator=(rhs);
}


template<class Type, class GeoMesh>
inline mousse::SubDimensionedField<Type, GeoMesh>::operator
const mousse::DimensionedField<Type, GeoMesh>&() const
{
  return *(reinterpret_cast<const DimensionedField<Type, GeoMesh>*>(this));
}


#endif
