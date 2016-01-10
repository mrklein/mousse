// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Constructors 
template<class ContainerType>
mousse::Circulator<ContainerType>::Circulator()
:
  CirculatorBase{},
  begin_{0},
  end_{0},
  iter_{0},
  fulcrum_{0}
{}


template<class ContainerType>
mousse::Circulator<ContainerType>::Circulator(ContainerType& container)
:
  CirculatorBase{},
  begin_{container.begin()},
  end_{container.end()},
  iter_{begin_},
  fulcrum_{begin_}
{}


template<class ContainerType>
mousse::Circulator<ContainerType>::Circulator
(
  const iterator& begin,
  const iterator& end
)
:
  CirculatorBase{},
  begin_{begin},
  end_{end},
  iter_{begin},
  fulcrum_{begin}
{}


template<class ContainerType>
mousse::Circulator<ContainerType>::Circulator
(
  const Circulator<ContainerType>& rhs
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
mousse::Circulator<ContainerType>::~Circulator()
{}


// Member Functions 
template<class ContainerType>
typename mousse::Circulator<ContainerType>::size_type
mousse::Circulator<ContainerType>::size() const
{
  return end_ - begin_;
}


template<class ContainerType>
bool mousse::Circulator<ContainerType>::circulate
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
void mousse::Circulator<ContainerType>::setFulcrumToIterator()
{
  fulcrum_ = iter_;
}


template<class ContainerType>
void mousse::Circulator<ContainerType>::setIteratorToFulcrum()
{
  iter_ = fulcrum_;
}


template<class ContainerType>
typename mousse::Circulator<ContainerType>::difference_type
mousse::Circulator<ContainerType>::nRotations() const
{
  return (iter_ - fulcrum_);
}


template<class ContainerType>
typename mousse::Circulator<ContainerType>::reference
mousse::Circulator<ContainerType>::next() const
{
  if (iter_ == end_ - 1)
  {
    return *begin_;
  }
  return *(iter_ + 1);
}


template<class ContainerType>
typename mousse::Circulator<ContainerType>::reference
mousse::Circulator<ContainerType>::prev() const
{
  if (iter_ == begin_)
  {
    return *(end_ - 1);
  }
  return *(iter_ - 1);
}


// Member Operators 
template<class ContainerType>
void mousse::Circulator<ContainerType>::operator=
(
  const Circulator<ContainerType>& rhs
)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FATAL_ERROR_IN
    (
      "mousse::Circulator<ContainerType>::operator="
      "(const mousse::Circulator<ContainerType>&)"
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
mousse::Circulator<ContainerType>&
mousse::Circulator<ContainerType>::operator++()
{
  ++iter_;
  if (iter_ == end_)
  {
    iter_ = begin_;
  }
  return *this;
}


template<class ContainerType>
mousse::Circulator<ContainerType>
mousse::Circulator<ContainerType>::operator++(int)
{
  Circulator<ContainerType> tmp = *this;
  ++(*this);
  return tmp;
}


template<class ContainerType>
mousse::Circulator<ContainerType>&
mousse::Circulator<ContainerType>::operator--()
{
  if (iter_ == begin_)
  {
    iter_ = end_;
  }
  --iter_;
  return *this;
}


template<class ContainerType>
mousse::Circulator<ContainerType>
mousse::Circulator<ContainerType>::operator--(int)
{
  Circulator<ContainerType> tmp = *this;
  --(*this);
  return tmp;
}


template<class ContainerType>
bool mousse::Circulator<ContainerType>::operator==
(
  const Circulator<ContainerType>& c
) const
{
  return
  (
    begin_ == c.begin_
  && end_ == c.end_
  && iter_ == c.iter_
  && fulcrum_ == c.fulcrum_
  );
}


template<class ContainerType>
bool mousse::Circulator<ContainerType>::operator!=
(
  const Circulator<ContainerType>& c
) const
{
  return !(*this == c);
}


template<class ContainerType>
typename mousse::Circulator<ContainerType>::reference
mousse::Circulator<ContainerType>::operator*() const
{
  return *iter_;
}


template<class ContainerType>
typename mousse::Circulator<ContainerType>::reference
mousse::Circulator<ContainerType>::operator()() const
{
  return operator*();
}


template<class ContainerType>
typename mousse::Circulator<ContainerType>::difference_type
mousse::Circulator<ContainerType>::operator-
(
  const Circulator<ContainerType>& c
) const
{
  return iter_ - c.iter_;
}
