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
// SourceFiles
//   sub_field_i.hpp
#ifndef sub_field_hpp_
#define sub_field_hpp_
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
    inline tmp<Field<cmptType> > component(const direction) const;
    //- Return the field transpose (only defined for second rank tensors)
    tmp<Field<Type> > T() const;
  // Member operators
    //- Assignment via UList operator. Takes linear time.
    inline void operator=(const SubField<Type>&);
    //- Allow cast to a const Field\<Type\>&
    inline operator const Field<Type>&() const;
};
}  // namespace mousse
#include "sub_field_i.hpp"
#endif
