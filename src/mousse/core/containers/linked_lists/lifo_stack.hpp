#ifndef CORE_CONTAINERS_LINKED_LISTS_LIFO_STACK_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_LIFO_STACK_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LIFOStack
// Description
//   A LIFO stack based on a singly-linked list.
//   Operations are push(), pop(), top(), bottom() and empty().

#include "sl_list.hpp"


namespace mousse {

template<class T>
class LIFOStack
:
  public SLList<T>
{
public:
  // Constructors
    //- Construct null
    LIFOStack()
    {}
    //- Construct given initial T
    LIFOStack(T a)
    :
      SLList<T>{a}
    {}
    //- Construct from Istream
    LIFOStack(Istream& is)
    :
      SLList<T>{is}
    {}
  // Member Functions
    // Access
      //- Return a copy of the top element
      T top() const
      {
        return this->first();
      }
      //- Return a copy of the bottom element
      T bottom() const
      {
        return this->last();
      }
    // Edit
      //- Push an element onto the stack
      void push(const T& a)
      {
        this->insert(a);
      }
      //- Pop the top element off the stack
      T pop()
      {
        return this->removeHead();
      }
};
}  // namespace mousse
#endif
