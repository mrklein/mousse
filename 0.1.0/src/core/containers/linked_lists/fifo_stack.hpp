// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FIFOStack
// Description
//   A FIFO stack based on a singly-linked list.
//   Operations are push(), pop(), top(), bottom() and empty().
// SourceFiles
//   fifo_stack.cpp
#ifndef fifo_stack_hpp_
#define fifo_stack_hpp_
#include "sl_list.hpp"
namespace mousse
{
template<class T>
class FIFOStack
:
  public SLList<T>
{
public:
  // Constructors
    //- Construct null
    FIFOStack()
    {}
    //- Construct given initial T
    FIFOStack(T a)
    :
      SLList<T>(a)
    {}
    //- Construct from Istream
    FIFOStack(Istream& is)
    :
      SLList<T>(is)
    {}
  // Member Functions
    // Access
      //- Return a copy of the top element
      T top() const
      {
        return this->last();
      }
      //- Return a copy of the bottom element
      T bottom() const
      {
        return this->first();
      }
    // Edit
      //- Push an element onto the stack
      void push(const T& a)
      {
        this->append(a);
      }
      //- Pop the bottom element off the stack
      T pop()
      {
        return this->removeHead();
      }
};
}  // namespace mousse
#endif
