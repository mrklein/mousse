// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ConstCirculator
// Description
//   Walks over a container as if it were circular. The container must have the
//   following members defined:
//     - value_type
//     - size_type
//     - difference_type
//     - const_iterator
//     - const_reference
//   Examples:
//   \code
//     face f(identity(5));
//     // Construct circulator from the face
//     ConstCirculator<face> circ(f);
//     // First check that the circulator has a size to iterate over.
//     // Then circulate around the list starting and finishing at the fulcrum.
//     if (circ.size()) do
//     {
//       Info<< "Iterate forwards over face : " << circ() << endl;
//     } while (circ.circulate(CirculatorBase::CLOCKWISE));
//   \endcode
//   \code
//     face f(identity(5));
//     ConstCirculator<face> circClockwise(f);
//     ConstCirculator<face> circAnticlockwise(f);
//     if (circClockwise.size() && circAnticlockwise.size()) do
//     {
//       Info<< "Iterate forward over face :" << circClockwise() << endl;
//       Info<< "Iterate backward over face:" << circAnticlockwise() << endl;
//     }
//     while
//     (
//       circClockwise.circulate(CirculatorBase::CLOCKWISE),
//       circAnticlockwise.circulate(CirculatorBase::ANTICLOCKWISE)
//     );
//   \endcode
#ifndef const_circulator_hpp_
#define const_circulator_hpp_
#include "circulator_base.hpp"
namespace mousse
{
template<class ContainerType>
class ConstCirculator
:
  public CirculatorBase
{
protected:
  // Protected data
    //- Iterator pointing to the beginning of the container
    typename ContainerType::const_iterator begin_;
    //- Iterator pointing to the end of the container
    typename ContainerType::const_iterator end_;
    //- Iterator
    typename ContainerType::const_iterator iter_;
    //- Iterator holding the location of the fulcrum (start and end) of
    //  the container. Used to decide when the iterator should stop
    //  circulating over the container
    typename ContainerType::const_iterator fulcrum_;
public:
  // STL type definitions
    //- Type of values ContainerType contains.
    typedef typename ContainerType::value_type      value_type;
    //- The type that can represent the size of ContainerType
    typedef typename ContainerType::size_type       size_type;
    //- The type that can represent the difference between any two
    //  iterator objects.
    typedef typename ContainerType::difference_type difference_type;
    //- Random access iterator for traversing ContainerType.
    typedef typename ContainerType::const_iterator  const_iterator;
    //- Type that can be used for storing into
    //  const ContainerType::value_type objects.
    typedef typename ContainerType::const_reference const_reference;
  // Constructors
    //- Construct null
    inline ConstCirculator();
    //- Construct from a container.
    inline explicit ConstCirculator(const ContainerType& container);
    //- Construct from two iterators
    inline ConstCirculator
    (
      const const_iterator& begin,
      const const_iterator& end
    );
    //- Construct as copy
    inline ConstCirculator(const ConstCirculator<ContainerType>&);
  //- Destructor
  ~ConstCirculator();
  // Member Functions
    //- Return the range of the iterator
    inline size_type size() const;
    //- Circulate around the list in the given direction
    inline bool circulate(const CirculatorBase::direction dir = NONE);
    //- Set the fulcrum to the current position of the iterator
    inline void setFulcrumToIterator();
    //- Set the iterator to the current position of the fulcrum
    inline void setIteratorToFulcrum();
    //- Return the distance between the iterator and the fulcrum. This is
    //  equivalent to the number of rotations of the circulator.
    inline difference_type nRotations() const;
    //- Dereference the next iterator and return
    inline const_reference next() const;
    //- Dereference the previous iterator and return
    inline const_reference prev() const;
  // Member Operators
    //- Assignment operator for circulators that operate on the same
    //  container type
    inline void operator=(const ConstCirculator<ContainerType>&);
    //- Prefix increment. Increments the iterator.
    //  Sets the iterator to the beginning of the container if it reaches
    //  the end
    inline ConstCirculator<ContainerType>& operator++();
    //- Postfix increment. Increments the iterator.
    //  Sets the iterator to the beginning of the container if it reaches
    //  the end
    inline ConstCirculator<ContainerType> operator++(int);
    //- Prefix decrement. Decrements the iterator.
    //  Sets the iterator to the end of the container if it reaches
    //  the beginning
    inline ConstCirculator<ContainerType>& operator--();
    //- Postfix decrement. Decrements the iterator.
    //  Sets the iterator to the end of the container if it reaches
    //  the beginning
    inline ConstCirculator<ContainerType> operator--(int);
    //- Check for equality of this iterator with another iterator that
    //  operate on the same container type
    inline bool operator==(const ConstCirculator<ContainerType>& c) const;
    //- Check for inequality of this iterator with another iterator that
    //  operate on the same container type
    inline bool operator!=(const ConstCirculator<ContainerType>& c) const;
    //- Dereference the iterator and return
    inline const_reference operator*() const;
    //- Dereference the iterator and return
    inline const_reference operator()() const;
    //- Return the difference between this iterator and another iterator
    //  that operate on the same container type
    inline difference_type operator-
    (
      const ConstCirculator<ContainerType>& c
    ) const;
};
}  // namespace mousse

// Constructors 
template<class ContainerType>
mousse::ConstCirculator<ContainerType>::ConstCirculator()
:
  CirculatorBase{},
  begin_{0},
  end_{0},
  iter_{0},
  fulcrum_{0}
{}
template<class ContainerType>
mousse::ConstCirculator<ContainerType>::ConstCirculator
(
  const ContainerType& container
)
:
  CirculatorBase{},
  begin_{container.begin()},
  end_{container.end()},
  iter_{begin_},
  fulcrum_{begin_}
{}
template<class ContainerType>
mousse::ConstCirculator<ContainerType>::ConstCirculator
(
  const const_iterator& begin,
  const const_iterator& end
)
:
  CirculatorBase{},
  begin_{begin},
  end_{end},
  iter_{begin},
  fulcrum_{begin}
{}
template<class ContainerType>
mousse::ConstCirculator<ContainerType>::ConstCirculator
(
  const ConstCirculator<ContainerType>& rhs
)
:
  CirculatorBase{},
  begin_{rhs.begin_},
  end_{rhs.end_},
  iter_{rhs.iter_},
  fulcrum_{rhs.fulcrum_}
{}
// Destructor 
template<class ContainerType>
mousse::ConstCirculator<ContainerType>::~ConstCirculator()
{}
// Member Functions 
template<class ContainerType>
typename mousse::ConstCirculator<ContainerType>::size_type
mousse::ConstCirculator<ContainerType>::size() const
{
  return end_ - begin_;
}
template<class ContainerType>
bool mousse::ConstCirculator<ContainerType>::circulate
(
  const CirculatorBase::direction dir
)
{
  if (dir == CirculatorBase::CLOCKWISE)
  {
    operator++();
  }
  else if (dir == CirculatorBase::ANTICLOCKWISE)
  {
    operator--();
  }
  return !(iter_ == fulcrum_);
}
template<class ContainerType>
void mousse::ConstCirculator<ContainerType>::setFulcrumToIterator()
{
  fulcrum_ = iter_;
}
template<class ContainerType>
void mousse::ConstCirculator<ContainerType>::setIteratorToFulcrum()
{
  iter_ = fulcrum_;
}
template<class ContainerType>
typename mousse::ConstCirculator<ContainerType>::difference_type
mousse::ConstCirculator<ContainerType>::nRotations() const
{
  return (iter_ - fulcrum_);
}
template<class ContainerType>
typename mousse::ConstCirculator<ContainerType>::const_reference
mousse::ConstCirculator<ContainerType>::next() const
{
  if (iter_ == end_ - 1)
  {
    return *begin_;
  }
  return *(iter_ + 1);
}
template<class ContainerType>
typename mousse::ConstCirculator<ContainerType>::const_reference
mousse::ConstCirculator<ContainerType>::prev() const
{
  if (iter_ == begin_)
  {
    return *(end_ - 1);
  }
  return *(iter_ - 1);
}
// Member Operators 
template<class ContainerType>
void mousse::ConstCirculator<ContainerType>::operator=
(
  const ConstCirculator<ContainerType>& rhs
)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FATAL_ERROR_IN
    (
      "mousse::ConstCirculator<ContainerType>::operator="
      "(const mousse::ConstCirculator<ContainerType>&)"
    )
    << "Attempted assignment to self"
    << abort(FatalError);
  }
  begin_ = rhs.begin_;
  end_ = rhs.end_;
  iter_ = rhs.iter_;
  fulcrum_ = rhs.fulcrum_;
}
template<class ContainerType>
mousse::ConstCirculator<ContainerType>&
mousse::ConstCirculator<ContainerType>::operator++()
{
  ++iter_;
  if (iter_ == end_)
  {
    iter_ = begin_;
  }
  return *this;
}
template<class ContainerType>
mousse::ConstCirculator<ContainerType>
mousse::ConstCirculator<ContainerType>::operator++(int)
{
  ConstCirculator<ContainerType> tmp = *this;
  ++(*this);
  return tmp;
}
template<class ContainerType>
mousse::ConstCirculator<ContainerType>&
mousse::ConstCirculator<ContainerType>::operator--()
{
  if (iter_ == begin_)
  {
    iter_ = end_;
  }
  --iter_;
  return *this;
}
template<class ContainerType>
mousse::ConstCirculator<ContainerType>
mousse::ConstCirculator<ContainerType>::operator--(int)
{
  ConstCirculator<ContainerType> tmp = *this;
  --(*this);
  return tmp;
}
template<class ContainerType>
bool mousse::ConstCirculator<ContainerType>::operator==
(
  const ConstCirculator<ContainerType>& c
) const
{
  return begin_ == c.begin_ && end_ == c.end_ && iter_ == c.iter_
    && fulcrum_ == c.fulcrum_;
}
template<class ContainerType>
bool mousse::ConstCirculator<ContainerType>::operator!=
(
  const ConstCirculator<ContainerType>& c
) const
{
  return !(*this == c);
}
template<class ContainerType>
typename mousse::ConstCirculator<ContainerType>::const_reference
mousse::ConstCirculator<ContainerType>::operator*() const
{
  return *iter_;
}
template<class ContainerType>
typename mousse::ConstCirculator<ContainerType>::const_reference
mousse::ConstCirculator<ContainerType>::operator()() const
{
  return operator*();
}
template<class ContainerType>
typename mousse::ConstCirculator<ContainerType>::difference_type
mousse::ConstCirculator<ContainerType>::operator-
(
  const ConstCirculator<ContainerType>& c
) const
{
  return iter_ - c.iter_;
}
#endif
