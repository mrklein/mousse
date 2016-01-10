// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DynamicField
// Description
//   Dynamically sized Field.
// SourceFiles
//   dynamic_field_i.hpp
//   dynamic_field.cpp
#ifndef dynamic_field_hpp_
#define dynamic_field_hpp_
#include "field.hpp"
#include "static_assert.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
class DynamicField;
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Ostream& operator<<
(
  Ostream&,
  const DynamicField<T, SizeInc, SizeMult, SizeDiv>&
);
template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
Istream& operator>>
(
  Istream&,
  DynamicField<T, SizeInc, SizeMult, SizeDiv>&
);
template<class T, unsigned SizeInc=0, unsigned SizeMult=2, unsigned SizeDiv=1>
class DynamicField
:
  public Field<T>
{
  //- Avoid invalid sizing parameters
  STATIC_ASSERT((SizeInc || SizeMult) && SizeDiv);
  // Private data
    //- The capacity (allocated size) of the underlying field.
    label capacity_;
public:
  // Static Member Functions
    //- Return a null field
    inline static const DynamicField<T, SizeInc, SizeMult, SizeDiv>& null()
    {
      return *reinterpret_cast
      <
        DynamicField<T, SizeInc, SizeMult, SizeDiv>*
      >(0);
    }
  // Constructors
    //- Construct null
    inline DynamicField();
    //- Construct given size.
    explicit inline DynamicField(const label);
    //- Construct from UList. Size set to UList size.
    //  Also constructs from DynamicField with different sizing parameters.
    explicit inline DynamicField(const UList<T>&);
    //- Construct by transferring the parameter contents
    explicit inline DynamicField(const Xfer<List<T> >&);
    //- Construct by 1 to 1 mapping from the given field
    inline DynamicField
    (
      const UList<T>& mapF,
      const labelList& mapAddressing
    );
    //- Construct by interpolative mapping from the given field
    inline DynamicField
    (
      const UList<T>& mapF,
      const labelListList& mapAddressing,
      const scalarListList& weights
    );
    //- Construct by mapping from the given field
    inline DynamicField
    (
      const UList<T>& mapF,
      const FieldMapper& map
    );
    //- Construct copy
    inline DynamicField(const DynamicField<T, SizeInc, SizeMult, SizeDiv>&);
    //- Construct by transferring the Field contents
    inline DynamicField
    (
      const Xfer<DynamicField<T, SizeInc, SizeMult, SizeDiv> >&
    );
    //- Construct from Istream. Size set to size of list read.
    explicit DynamicField(Istream&);
    //- Clone
    tmp<DynamicField<T, SizeInc, SizeMult, SizeDiv> > clone() const;
  // Member Functions
    // Access
      //- Size of the underlying storage.
      inline label capacity() const;
    // Edit
      //- Alter the size of the underlying storage.
      //  The addressed size will be truncated if needed to fit, but will
      //  remain otherwise untouched.
      //  Use this or reserve() in combination with append().
      inline void setCapacity(const label);
      //- Alter the addressed list size.
      //  New space will be allocated if required.
      //  Use this to resize the list prior to using the operator[] for
      //  setting values (as per List usage).
      inline void setSize(const label);
      //- Alter the addressed list size and fill new space with a
      //  constant.
      inline void setSize(const label, const T&);
      //- Alter the addressed list size.
      //  New space will be allocated if required.
      //  Use this to resize the list prior to using the operator[] for
      //  setting values (as per List usage).
      inline void resize(const label);
      //- Alter the addressed list size and fill new space with a
      //  constant.
      inline void resize(const label, const T&);
      //- Reserve allocation space for at least this size.
      //  Never shrinks the allocated size, use setCapacity() for that.
      inline void reserve(const label);
      //- Clear the addressed list, i.e. set the size to zero.
      //  Allocated size does not change
      inline void clear();
      //- Clear the list and delete storage.
      inline void clearStorage();
      //- Shrink the allocated space to the number of elements used.
      //  Returns a reference to the DynamicField.
      inline DynamicField<T, SizeInc, SizeMult, SizeDiv>& shrink();
      //- Transfer contents to the Xfer container as a plain List
      inline Xfer<List<T> > xfer();
    // Member Operators
      //- Append an element at the end of the list
      inline DynamicField<T, SizeInc, SizeMult, SizeDiv>& append
      (
        const T&
      );
      //- Append a List at the end of this list
      inline DynamicField<T, SizeInc, SizeMult, SizeDiv>& append
      (
        const UList<T>&
      );
      //- Remove and return the top element
      inline T remove();
      //- Return non-const access to an element, resizing list if
      //  necessary
      inline T& operator()(const label);
      //- Assignment of all addressed entries to the given value
      inline void operator=(const T&);
      //- Assignment from DynamicField
      inline void operator=
      (
        const DynamicField<T, SizeInc, SizeMult, SizeDiv>&
      );
      //- Assignment from UList
      inline void operator=(const UList<T>&);
};
}  // namespace mousse
#include "dynamic_field_i.hpp"
#ifdef NoRepository
#   include "dynamic_field.cpp"
#endif
#endif
