#ifndef CORE_CONTAINERS_LINKED_LISTS_SL_LIST_BASE_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_SL_LIST_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SLListBase
// Description
//   Base singly-linked list.

#include "bool.hpp"
#include "label.hpp"
#include "ulabel.hpp"
#include "error.hpp"


namespace mousse {

class SLListBase
{
public:

  //- Link structure
  struct link
  {

    //- Pointer to next entry in list
    link* next_;

    //- Null construct
    inline link();

    //- Construct given pointer to another link
    inline link(link* p);

  };

private:

  // Private data

   //- last_ points to last element
   //  last_->next_ points to first element, i.e. circular storage
   link* last_;

   //- Number of elements in in list
   label nElmts_;

public:

  // Forward declaration of STL iterators
    class iterator;
    friend class iterator;
    class const_iterator;
    friend class const_iterator;

  // Constructors

    //- Null construct
    inline SLListBase();

    //- Construct given initial entry
    inline SLListBase(link*);

    //- Disallow default bitwise copy construct
    SLListBase(const SLListBase&) = delete;

    //- Disallow default bitwise assignment
    SLListBase& operator=(const SLListBase&) = delete;

  //- Destructor
  ~SLListBase();

  // Member Functions

    // Access

      //- Return number of elements in list
      inline label size() const;

      //- Return true if the list is empty
      inline bool empty() const;

      //- Return first entry
      inline link* first();

      //- Return const access to first entry
      inline const link* first() const;

      //- Return last entry
      inline link* last();

      //- Return const access to last entry
      inline const link* last() const;

    // Edit

      //- Add at head of list
      void insert(link*);

      //- Add at tail of list
      void append(link*);

      //- Remove and return head
      link* removeHead();

      // Remove and return element
      link* remove(link*);

      // Remove and return element specified by iterator
      inline link* remove(iterator&);

      //- Clear the list
      inline void clear();

      //- Transfer the contents of the argument into this List
      //  and annul the argument list.
      inline void transfer(SLListBase&);

  // STL iterator

    //- An STL-conforming iterator
    class iterator
    {
      friend class SLListBase;
      friend class const_iterator;

      // Private data

        //- Reference to the list this is an iterator for
        SLListBase& curList_;

        //- Current element
        link* curElmt_;

        //- Copy of the link
        link curLink_;

      // Private Member Functions
      //- Construct for a given SLListBase with NULL element and link.
      //  Only used to create endIter
      inline iterator(SLListBase&);

    public:

      //- Construct for a given SLListBase and link
      inline iterator(SLListBase&, link*);

      // Member operators
        inline void operator=(const iterator&);
        inline bool operator==(const iterator&) const;
        inline bool operator!=(const iterator&) const;
        inline link& operator*();
        inline iterator& operator++();
        inline iterator operator++(int);

    };

    inline iterator begin();
    inline const iterator& end();

  // STL const_iterator

    //- An STL-conforming const_iterator
    class const_iterator
    {

      // Private data

        //- Reference to the list this is an iterator for
        const SLListBase& curList_;

        //- Current element
        const link* curElmt_;

    public:

      //- Construct for a given SLListBase and link
      inline const_iterator(const SLListBase&, const link*);

      //- Construct from a non-const iterator
      inline const_iterator(const iterator&);

      // Member operators
        inline void operator=(const const_iterator&);
        inline bool operator==(const const_iterator&) const;
        inline bool operator!=(const const_iterator&) const;
        inline const link& operator*();
        inline const_iterator& operator++();
        inline const_iterator operator++(int);
    };

    inline const_iterator cbegin() const;
    inline const const_iterator& cend() const;
    inline const_iterator begin() const;
    inline const const_iterator& end() const;

private:
    //- Iterator returned by end()
    static iterator endIter_;
    //- const_iterator returned by end()
    static const_iterator endConstIter_;
};

}  // namespace mousse

// Constructors
inline mousse::SLListBase::link::link()
:
  next_{0}
{}

inline mousse::SLListBase::link::link(link* p)
:
  next_{p}
{}

inline mousse::SLListBase::SLListBase()
:
  last_{0},
  nElmts_{0}
{}

inline mousse::SLListBase::SLListBase(link* a)
:
  last_{a->next_ = a},
  nElmts_{1}
{}

// Destructor
inline mousse::SLListBase::~SLListBase()
{}

// Member Functions
inline mousse::label mousse::SLListBase::size() const
{
  return nElmts_;
}

inline bool mousse::SLListBase::empty() const
{
  return !nElmts_;
}

inline mousse::SLListBase::link*
mousse::SLListBase::first()
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("SLListBase::first()")
      << "list is empty"
      << abort(FatalError);
  }
  return last_->next_;
}

inline const mousse::SLListBase::link*
mousse::SLListBase::first() const
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("SLListBase::first() const")
      << "list is empty"
      << abort(FatalError);
  }
  return last_->next_;
}

inline mousse::SLListBase::link*
mousse::SLListBase::last()
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("SLListBase::last()")
      << "list is empty"
      << abort(FatalError);
  }
  return last_;
}

inline const mousse::SLListBase::link*
mousse::SLListBase::last() const
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("SLListBase::last() const")
      << "list is empty"
      << abort(FatalError);
  }
  return last_;
}

inline void mousse::SLListBase::clear()
{
  last_ = 0;
  nElmts_ = 0;
}

inline void mousse::SLListBase::transfer(SLListBase& lst)
{
  last_   = lst.last_;
  nElmts_ = lst.nElmts_;
  lst.clear();
}

inline mousse::SLListBase::link* mousse::SLListBase::remove
(
  SLListBase::iterator& it
)
{
  return remove(it.curElmt_);
}

// STL iterator
inline mousse::SLListBase::iterator::iterator(SLListBase& s, link* elmt)
:
  curList_{s},
  curElmt_{elmt},
  curLink_{*curElmt_}
{}

inline mousse::SLListBase::iterator::iterator(SLListBase& s)
:
  curList_{s},
  curElmt_{nullptr},
  curLink_{}
{}

inline void mousse::SLListBase::iterator::operator=(const iterator& iter)
{
  curElmt_ = iter.curElmt_;
  curLink_ = iter.curLink_;
}

inline bool mousse::SLListBase::iterator::operator==(const iterator& iter) const
{
  return curElmt_ == iter.curElmt_;
}

inline bool mousse::SLListBase::iterator::operator!=(const iterator& iter) const
{
  return curElmt_ != iter.curElmt_;
}

inline mousse::SLListBase::link& mousse::SLListBase::iterator::operator*()
{
  return *curElmt_;
}

inline mousse::SLListBase::iterator& mousse::SLListBase::iterator::operator++()
{
  if (curElmt_ == curList_.last_ || curList_.last_ == 0)
  {
    curElmt_ = 0;
  }
  else
  {
    curElmt_ = curLink_.next_;
    curLink_ = *curElmt_;
  }
  return *this;
}

inline mousse::SLListBase::iterator
mousse::SLListBase::iterator::operator++(int)
{
  iterator tmp = *this;
  ++*this;
  return tmp;
}

inline mousse::SLListBase::iterator
mousse::SLListBase::begin()
{
  if (size()) {
    return iterator(*this, first());
  } else {
    return endIter_;
  }
}

inline const mousse::SLListBase::iterator&
mousse::SLListBase::end()
{
  return endIter_;
}

// STL const_iterator
inline mousse::SLListBase::const_iterator::const_iterator
(
  const SLListBase& s,
  const link* elmt
)
:
  curList_{s},
  curElmt_{elmt}
{}

inline mousse::SLListBase::const_iterator::const_iterator(const iterator& iter)
:
  curList_{iter.curList_},
  curElmt_{iter.curElmt_}
{}

inline void mousse::SLListBase::const_iterator::operator=
(
  const const_iterator& iter
)
{
  curElmt_ = iter.curElmt_;
}

inline bool mousse::SLListBase::const_iterator::operator==
(
  const const_iterator& iter
) const
{
  return curElmt_ == iter.curElmt_;
}

inline bool mousse::SLListBase::const_iterator::operator!=
(
  const const_iterator& iter
) const
{
  return curElmt_ != iter.curElmt_;
}

inline const mousse::SLListBase::link&
mousse::SLListBase::const_iterator::operator*()
{
  return *curElmt_;
}

inline mousse::SLListBase::const_iterator&
mousse::SLListBase::const_iterator::operator++()
{
  if (curElmt_ == curList_.last_) {
    curElmt_ = 0;
  } else {
    curElmt_ = curElmt_->next_;
  }
  return *this;
}

inline mousse::SLListBase::const_iterator
mousse::SLListBase::const_iterator::operator++(int)
{
  const_iterator tmp = *this;
  ++*this;
  return tmp;
}

inline mousse::SLListBase::const_iterator
mousse::SLListBase::cbegin() const
{
  if (size()) {
    return const_iterator(*this, first());
  } else {
    return endConstIter_;
  }
}

inline const mousse::SLListBase::const_iterator&
mousse::SLListBase::cend() const
{
  return endConstIter_;
}

inline mousse::SLListBase::const_iterator
mousse::SLListBase::begin() const
{
  return this->cbegin();
}

inline const mousse::SLListBase::const_iterator&
mousse::SLListBase::end() const
{
  return endConstIter_;
}

#endif
