#ifndef CORE_FIELDS_FIELDS_SUB_FIELD_HPP_
#define CORE_FIELDS_FIELDS_SUB_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SubField
// Description
//   SubField is a Field obtained as a section of another Field.
//   Thus it is itself unallocated so that no storage is allocated or
//   deallocated during its use.  To achieve this behaviour, SubField is
//   derived from a SubList rather than a List.
#include "sub_list.hpp"
#include "field.hpp"
namespace mousse
{
//- Pre-declare SubField and related Field type
template<class Type> class Field;
template<class Type> class SubField;
template<class Type>
class SubField
:
  public refCount,
  public SubList<Type>
{
public:
  //- Component type
  typedef typename pTraits<Type>::cmptType cmptType;
  // Constructors
    //- Construct from a SubList
    inline SubField(const SubList<Type>&);
    //- Construct from a UList\<Type\>, using the entire size
    explicit inline SubField(const UList<Type>&);
    //- Construct from a UList\<Type\> with a given size
    inline SubField
    (
      const UList<Type>& list,
      const label subSize
    );
    //- Construct from a UList\<Type\> with a given size and start index
    inline SubField
    (
      const UList<Type>& list,
      const label subSize,
      const label startIndex
    );
    //- Construct as copy
    inline SubField(const SubField<Type>&);
  // Member functions
    //- Return a null SubField
    static inline const SubField<Type>& null();
    //- Return a component field of the field
    inline tmp<Field<cmptType>> component(const direction) const;
    //- Return the field transpose (only defined for second rank tensors)
    tmp<Field<Type>> T() const;
  // Member operators
    //- Assignment via UList operator. Takes linear time.
    inline void operator=(const SubField<Type>&);
    //- Allow cast to a const Field\<Type\>&
    inline operator const Field<Type>&() const;
};
}  // namespace mousse

// Constructors 
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const SubList<Type>& list
)
:
  SubList<Type>{list}
{}
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const UList<Type>& list
)
:
  SubList<Type>{list, list.size()}
{}
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const UList<Type>& list,
  const label subSize
)
:
  SubList<Type>{list, subSize}
{}
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const UList<Type>& list,
  const label subSize,
  const label startIndex
)
:
  SubList<Type>{list, subSize, startIndex}
{}
template<class Type>
inline mousse::SubField<Type>::SubField
(
  const SubField<Type>& sfield
)
:
  refCount{},
  SubList<Type>{sfield}
{}
// Member Functions 
template<class Type>
inline const mousse::SubField<Type>& mousse::SubField<Type>::null()
{
  return NullObjectRef<SubField<Type>>();
}
template<class Type>
inline mousse::tmp<mousse::Field<typename mousse::SubField<Type>::cmptType>>
mousse::SubField<Type>::component
(
  const direction d
) const
{
  return (reinterpret_cast<const Field<Type>&>(*this)).component(d);
}
template<class Type>
inline mousse::tmp<mousse::Field<Type>> mousse::SubField<Type>::T() const
{
  return (reinterpret_cast<const Field<Type>&>(*this)).T();
}
// Member Operators 
template<class Type>
inline void mousse::SubField<Type>::operator=(const SubField<Type>& rhs)
{
  UList<Type>::operator=(rhs);
}
template<class Type>
inline mousse::SubField<Type>::operator const mousse::Field<Type>&() const
{
  return *reinterpret_cast< const Field<Type>* >(this);
}
#endif
