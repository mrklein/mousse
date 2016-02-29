#ifndef CORE_CONTAINERS_LISTS_INDIRECT_LIST_HPP_
#define CORE_CONTAINERS_LISTS_INDIRECT_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IndirectList
// Description
//   A List with indirect addressing.
// SeeAlso
//   mousse::UIndirectList for a version without any allocation for the
//   addressing.


#include "uindirect_list.hpp"

namespace mousse
{

//- A helper class for storing addresses.
class IndirectListAddressing
{
  // Private data

    //- Storage for the list addressing
    List<label> addressing_;

protected:

  // Constructors

    //- Construct by copying the addressing array
    explicit inline IndirectListAddressing(const labelUList& addr);

    //- Construct by transferring addressing array
    explicit inline IndirectListAddressing(const Xfer<List<label> >& addr);

  // Member Functions

    // Access

      //- Return the list addressing
      inline const List<label>& addressing() const;

    // Edit

      //- Reset addressing
      inline void resetAddressing(const labelUList&);
      inline void resetAddressing(const Xfer<List<label> >&);

public:

    //- Disallow default bitwise copy construct
    IndirectListAddressing(const IndirectListAddressing&) = delete;

    //- Disallow default bitwise assignment
    IndirectListAddressing& operator=(const IndirectListAddressing&) = delete;
};

template<class T>
class IndirectList
:
  private IndirectListAddressing,
  public  UIndirectList<T>
{
public:
  // Constructors

    //- Construct given the complete list and the addressing array
    inline IndirectList(const UList<T>&, const labelUList&);

    //- Construct given the complete list and by transferring addressing
    inline IndirectList(const UList<T>&, const Xfer<List<label> >&);

    //- Copy constructor
    inline IndirectList(const IndirectList<T>&);

    //- Construct from UIndirectList
    explicit inline IndirectList(const UIndirectList<T>&);

    //- Disable default assignment operator
    IndirectList<T>& operator=(const IndirectList<T>&) = delete;

    //- Disable assignment from UIndirectList
    UIndirectList<T>& operator=(const UIndirectList<T>&) = delete;

  // Member Functions

    // Access

      //- Return the list addressing
      using UIndirectList<T>::addressing;

    // Edit

      //- Reset addressing
      using IndirectListAddressing::resetAddressing;

    // Member Operators

      //- Assignment operator
      using UIndirectList<T>::operator=;
};

}  // namespace mousse

// Constructors 
inline mousse::IndirectListAddressing::IndirectListAddressing
(
  const labelUList& addr
)
:
  addressing_{addr}
{}

inline mousse::IndirectListAddressing::IndirectListAddressing
(
  const Xfer<List<label> >& addr
)
:
  addressing_(addr)
{}

template<class T>
inline mousse::IndirectList<T>::IndirectList
(
  const UList<T>& completeList,
  const labelUList& addr
)
:
  IndirectListAddressing{addr},
  UIndirectList<T>
  {
    completeList,
    IndirectListAddressing::addressing()
  }
{}

template<class T>
inline mousse::IndirectList<T>::IndirectList
(
  const UList<T>& completeList,
  const Xfer<List<label> >& addr
)
:
  IndirectListAddressing{addr},
  UIndirectList<T>
  {
    completeList,
    IndirectListAddressing::addressing()
  }
{}

template<class T>
inline mousse::IndirectList<T>::IndirectList
(
  const IndirectList<T>& lst
)
:
  IndirectListAddressing{lst.addressing()},
  UIndirectList<T>
  {
    lst.completeList(),
    IndirectListAddressing::addressing()
  }
{}

template<class T>
inline mousse::IndirectList<T>::IndirectList
(
  const UIndirectList<T>& lst
)
:
  IndirectListAddressing{lst.addressing()},
  UIndirectList<T>
  {
    lst.completeList(),
    IndirectListAddressing::addressing()
  }
{}

// Member Functions 
inline const mousse::List<mousse::label>&
mousse::IndirectListAddressing::addressing() const
{
  return addressing_;
}

inline void mousse::IndirectListAddressing::resetAddressing
(
  const labelUList& addr
)
{
  addressing_ = addr;
}

inline void mousse::IndirectListAddressing::resetAddressing
(
  const Xfer<List<label> >& addr
)
{
  addressing_.transfer(addr());
}

#endif
