// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SubList
// Description
//   A List obtained as a section of another List.
//   Since the SubList is itself unallocated, no storage is allocated or
//   de-allocated during its use.  To achieve this behaviour, SubList is
//   derived from UList rather than List.
// SourceFiles
//   sub_list_i.hpp
#ifndef sub_list_hpp_
#define sub_list_hpp_
#include "list.hpp"
namespace mousse
{
template<class T>
class SubList
:
  public UList<T>
{
public:
  // Static Member Functions
    //- Return a null SubList
    inline static const SubList<T>& null();
  // Constructors
    //- Construct from UList and sub-list size
    inline SubList
    (
      const UList<T>& list,
      const label subSize
    );
    //- Construct from UList, sub-list size and start index
    inline SubList
    (
      const UList<T>& list,
      const label subSize,
      const label startIndex
    );
  // Member operators
    //- Allow cast to a const List<T>&
    inline operator const mousse::List<T>&() const;
    //- Assignment of all entries to the given value
    inline void operator=(const T&);
};
}  // namespace mousse
#include "sub_list_i.hpp"
#endif
