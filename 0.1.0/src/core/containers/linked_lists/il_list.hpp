#ifndef CORE_CONTAINERS_LINKED_LISTS_IL_LIST_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_IL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ILList
// Description
//   Template class for intrusive linked lists.
// SourceFiles
//   il_list.cpp
//   il_list_io.cpp

#include "uil_list.hpp"


namespace mousse {

class Istream;
class Ostream;

// Forward declaration of friend functions and operators
template<class LListBase, class T> class ILList;
template<class LListBase, class T> Istream& operator>>
(
  Istream&,
  ILList<LListBase, T>&
);


template<class LListBase, class T>
class ILList
:
  public UILList<LListBase, T>
{
  // Private Member Functions
    //- Read from Istream using given Istream constructor class
    template<class INew>
    void read(Istream&, const INew&);
public:
  // Constructors
    //- Null construct
    ILList()
    {}
    //- Construct given initial T
    ILList(T* a)
    :
      UILList<LListBase, T>{a}
    {}
    //- Construct from Istream
    ILList(Istream&);
    //- Construct as copy
    ILList(const ILList<LListBase, T>&);
    //- Copy constructor with additional argument for clone
    template<class CloneArg>
    ILList(const ILList<LListBase, T>& lst, const CloneArg& cloneArg);
    //- Construct from Istream using given Istream constructor class
    template<class INew>
    ILList(Istream&, const INew&);
  //- Destructor
  ~ILList();
  // Member Functions
    // Edit
      //- Remove the head element specified from the list and delete it
      bool eraseHead();
      //- Remove the specified element from the list and delete it
      bool erase(T* p);
      //- Clear the contents of the list
      void clear();
      //- Transfer the contents of the argument into this List
      //  and annul the argument list.
      void transfer(ILList<LListBase, T>&);
  // Member operators
    void operator=(const ILList<LListBase, T>&);
  // Istream operator
    //- Read List from Istream, discarding contents of existing List.
    friend Istream& operator>> <LListBase, T>
    (
      Istream&,
      ILList<LListBase, T>&
    );
};
}  // namespace mousse

#include "il_list.ipp"

#endif
