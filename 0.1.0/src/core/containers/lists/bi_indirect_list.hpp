// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BiIndirectList
// Description
//   Indexes into negList (negative index) or posList (zero or positive index).
// SourceFiles
//   bi_indirect_list_i.hpp
#ifndef bi_indirect_list_hpp_
#define bi_indirect_list_hpp_
#include "list.hpp"
namespace mousse
{
template<class T>
class BiIndirectList
{
  // Private data
    UList<T>& posList_;
    UList<T>& negList_;
    List<label> addressing_;
public:
  // Constructors
    //- Construct given the complete lists and the addressing array
    inline BiIndirectList
    (
      const UList<T>& posList,
      const UList<T>& negList,
      const labelUList&
    );
    //- Construct given the complete list and by transferring addressing
    inline BiIndirectList
    (
      const UList<T>& posList,
      const UList<T>& negList,
      const Xfer<List<label> >&
    );
  // Member Functions
    // Access
      //- Return the number of elements in the list
      inline label size() const;
      //- Return true if the list is empty (ie, size() is zero).
      inline bool empty() const;
      inline const UList<T>& posList() const;
      inline const UList<T>& negList() const;
      //- Return the list addressing
      inline const List<label>& addressing() const;
      //- Calculate index given whether index is into posList or negList
      inline static label posIndex(const label);
      inline static label negIndex(const label);
    // Edit
      //- Reset addressing
      inline void resetAddressing(const labelUList&);
      inline void resetAddressing(const Xfer<List<label> >&);
    // Member Operators
      //- Return the addressed elements as a List
      inline List<T> operator()() const;
      //- Return non-const access to an element
      inline T& operator[](const label);
      //- Return const access to an element
      inline const T& operator[](const label) const;
      //- Assignment from UList of addressed elements
      inline void operator=(const UList<T>&);
      //- Assignment of all entries to the given value
      inline void operator=(const T&);
};
}  // namespace mousse
#include "bi_indirect_list_i.hpp"
#endif
