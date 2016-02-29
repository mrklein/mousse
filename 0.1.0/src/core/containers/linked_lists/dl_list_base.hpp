#ifndef CORE_CONTAINERS_LINKED_LISTS_DL_LIST_BASE_HPP_
#define CORE_CONTAINERS_LINKED_LISTS_DL_LIST_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DLListBase
// Description
//   Base doubly-linked list.
// SourceFiles
//   dl_list_base.cpp


#include "bool.hpp"
#include "label.hpp"
#include "ulabel.hpp"
#include "error.hpp"

namespace mousse
{

class DLListBase
{
public:

  //- Link structure
  struct link
  {
    //- Pointer to next entry in list
    link *prev_, *next_;

    //- Null construct
    inline link();

    //- Check if the link is registered with the DLListBase
    inline bool registered() const;

    //- Deregister the link after removal
    inline void deregister();

  };

private:

  // Private data

   //- first_ points to first element and last_ points to last element.
   link *first_, *last_;

   //- Number of elements in in list
   label nElmts_;

public:

  // Forward declaration of STL iterators

    class iterator;
    friend class iterator;
    class const_iterator;
    friend class const_iterator;
    class const_reverse_iterator;
    friend class const_reverse_iterator;

  // Constructors

    //- Null construct
    inline DLListBase();

    //- Construct given initial entry
    inline DLListBase(link*);

    //- Disallow default bitwise copy construct
    DLListBase(const DLListBase&) = delete;

    //- Disallow default bitwise assignment
    DLListBase& operator=(const DLListBase&) = delete;

  //- Destructor
  ~DLListBase();

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

      //- Swap this element with the one above unless it is at the top
      bool swapUp(link*);

      //- Swap this element with the one below unless it is at the bottom
      bool swapDown(link*);

      //- Remove and return head
      link* removeHead();

      //- Remove and return element
      link* remove(link*);

      // Remove and return element specified by iterator
      inline link* remove(iterator&);

      //- Replace oldLink with newLink and return element
      link* replace(link* oldLink, link* newLink);

      //- Replace oldIter with newLink and return element
      inline link* replace(iterator& oldIter, link* newLink);

      //- Clear the list
      inline void clear();

      //- Transfer the contents of the argument into this List
      //  and annul the argument list.
      inline void transfer(DLListBase&);

  // STL iterator

    //- An STL-conforming iterator
    class iterator
    {

      friend class DLListBase;
      friend class const_iterator;

      // Private data

        //- Reference to the list this is an iterator for
        DLListBase& curList_;

        //- Current element
        link* curElmt_;

        //- Copy of the link
        link curLink_;

      // Private Member Functions
      //- Construct for a given SLListBase with NULL element and link.
      //  Only used to create endIter
      inline iterator(DLListBase&);

    public:

      //- Construct for a given DLListBase and link
      inline iterator(DLListBase&, link*);

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
        const DLListBase& curList_;

        //- Current element
        const link* curElmt_;

    public:

      //- Construct for a given DLListBase and link
      inline const_iterator(const DLListBase&, const link*);

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

  // STL const_reverse_iterator

    //- An STL-conforming const_reverse_iterator
    class const_reverse_iterator
    {
      // Private data

        //- Reference to the list this is an reverse_iterator for
        const DLListBase& curList_;

        //- Current element
        const link* curElmt_;

    public:

      //- Construct for a given DLListBase and link
      inline const_reverse_iterator(const DLListBase&, const link*);

      // Member operators
        inline void operator=(const const_reverse_iterator&);
        inline bool operator==(const const_reverse_iterator&) const;
        inline bool operator!=(const const_reverse_iterator&) const;
        inline const link& operator*();
        inline const_reverse_iterator& operator++();
        inline const_reverse_iterator operator++(int);
    };

    inline const_reverse_iterator crbegin() const;
    inline const const_reverse_iterator& crend() const;
    inline const_reverse_iterator rbegin() const;
    inline const const_reverse_iterator& rend() const;

private:

    //- Iterator returned by end()
    static iterator endIter_;

    //- const_iterator returned by end()
    static const_iterator endConstIter_;

    //- const_reverse_iterator returned by end()
    static const_reverse_iterator endConstRevIter_;
};

}  // namespace mousse

// Constructors 
inline mousse::DLListBase::link::link()
:
  prev_{0},
  next_{0}
{}
inline mousse::DLListBase::DLListBase()
:
  first_{0},
  last_{0},
  nElmts_{0}
{}

inline mousse::DLListBase::DLListBase(link* a)
:
  first_{a},
  last_{a},
  nElmts_{1}
{
  a->prev_ = a;
  a->next_ = a;
}

// Destructor 
inline mousse::DLListBase::~DLListBase()
{}

// Member Functions 
inline bool mousse::DLListBase::link::registered() const
{
  return prev_ != 0 && next_ != 0;
}

inline void mousse::DLListBase::link::deregister()
{
  prev_ = 0;
  next_ = 0;
}

inline mousse::label mousse::DLListBase::size() const
{
  return nElmts_;
}

inline bool mousse::DLListBase::empty() const
{
  return !nElmts_;
}

inline mousse::DLListBase::link*
mousse::DLListBase::first()
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("DLListBase::first()")
      << "list is empty"
      << abort(FatalError);
  }
  return first_;
}

inline const mousse::DLListBase::link*
mousse::DLListBase::first() const
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("DLListBase::first() const")
      << "list is empty"
      << abort(FatalError);
  }
  return first_;
}

inline mousse::DLListBase::link*
mousse::DLListBase::last()
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("DLListBase::last()")
      << "list is empty"
      << abort(FatalError);
  }
  return last_;
}

inline const mousse::DLListBase::link*
mousse::DLListBase::last() const
{
  if (!nElmts_)
  {
    FATAL_ERROR_IN("DLListBase::last() const")
      << "list is empty"
      << abort(FatalError);
  }
  return last_;
}

inline void mousse::DLListBase::clear()
{
  first_ = 0;
  last_  = 0;
  nElmts_ = 0;
}

inline void mousse::DLListBase::transfer(DLListBase& lst)
{
  first_  = lst.first_;
  last_   = lst.last_;
  nElmts_ = lst.nElmts_;
  lst.clear();
}

inline mousse::DLListBase::link*
mousse::DLListBase::remove
(
  DLListBase::iterator& it
)
{
  return remove(it.curElmt_);
}

inline mousse::DLListBase::link*
mousse::DLListBase::replace
(
  DLListBase::iterator& oldIter,
  DLListBase::link* newLink
)
{
  return replace(oldIter.curElmt_, newLink);
}

// STL iterator 
inline mousse::DLListBase::iterator::iterator(DLListBase& s, link* elmt)
:
  curList_{s},
  curElmt_{elmt},
  curLink_{*curElmt_}
{}

inline mousse::DLListBase::iterator::iterator(DLListBase& s)
:
  curList_{s},
  curElmt_{NULL},
  curLink_{}
{}

inline void mousse::DLListBase::iterator::operator=(const iterator& iter)
{
  curElmt_ = iter.curElmt_;
  curLink_ = iter.curLink_;
}

inline bool mousse::DLListBase::iterator::operator==(const iterator& iter) const
{
  return curElmt_ == iter.curElmt_;
}

inline bool mousse::DLListBase::iterator::operator!=(const iterator& iter) const
{
  return curElmt_ != iter.curElmt_;
}

inline mousse::DLListBase::link&
mousse::DLListBase::iterator::operator*()
{
  return *curElmt_;
}

inline mousse::DLListBase::iterator&
mousse::DLListBase::iterator::operator++()
{
  // Check if the curElmt_ is the last element (if it points to itself)
  // or if the list is empty because the last element may have been removed
  if (curLink_.next_ == curElmt_ || curList_.last_ == 0)
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

inline mousse::DLListBase::iterator
mousse::DLListBase::iterator::operator++(int)
{
  iterator tmp = *this;
  ++*this;
  return tmp;
}

inline mousse::DLListBase::iterator
mousse::DLListBase::begin()
{
  if (size())
  {
    return iterator(*this, first());
  }
  else
  {
    return endIter_;
  }
}

inline const mousse::DLListBase::iterator& mousse::DLListBase::end()
{
  return endIter_;
}

// STL const_iterator 
inline mousse::DLListBase::const_iterator::const_iterator
(
  const DLListBase& s,
  const link* elmt
)
:
  curList_{s},
  curElmt_{elmt}
{}

inline mousse::DLListBase::const_iterator::const_iterator(const iterator& iter)
:
  curList_{iter.curList_},
  curElmt_{iter.curElmt_}
{}

inline void mousse::DLListBase::const_iterator::operator=
(
  const const_iterator& iter
)
{
  curElmt_ = iter.curElmt_;
}

inline bool mousse::DLListBase::const_iterator::operator==
(
  const const_iterator& iter
) const
{
  return curElmt_ == iter.curElmt_;
}

inline bool mousse::DLListBase::const_iterator::operator!=
(
  const const_iterator& iter
) const
{
  return curElmt_ != iter.curElmt_;
}

inline const mousse::DLListBase::link&
mousse::DLListBase::const_iterator::operator*()
{
  return *curElmt_;
}

inline mousse::DLListBase::const_iterator&
mousse::DLListBase::const_iterator::operator++()
{
  if (curElmt_ == curList_.last_)
  {
    curElmt_ = 0;
  }
  else
  {
    curElmt_ = curElmt_->next_;
  }
  return *this;
}

inline mousse::DLListBase::const_iterator
mousse::DLListBase::const_iterator::operator++(int)
{
  const_iterator tmp = *this;
  ++*this;
  return tmp;
}

inline mousse::DLListBase::const_iterator
mousse::DLListBase::cbegin() const
{
  if (size())
  {
    return const_iterator(*this, first());
  }
  else
  {
    return endConstIter_;
  }
}

inline const mousse::DLListBase::const_iterator&
mousse::DLListBase::cend() const
{
  return endConstIter_;
}

inline mousse::DLListBase::const_iterator
mousse::DLListBase::begin() const
{
  return this->cbegin();
}

inline const mousse::DLListBase::const_iterator&
mousse::DLListBase::end() const
{
  return endConstIter_;
}

// STL const_reverse_iterator 

inline mousse::DLListBase::const_reverse_iterator::const_reverse_iterator
(
  const DLListBase& s,
  const link* elmt
)
:
  curList_{s},
  curElmt_{elmt}
{}

inline void mousse::DLListBase::const_reverse_iterator::operator=
(
  const const_reverse_iterator& iter
)
{
  curElmt_ = iter.curElmt_;
}

inline bool mousse::DLListBase::const_reverse_iterator::operator==
(
  const const_reverse_iterator& iter
) const
{
  return curElmt_ == iter.curElmt_;
}

inline bool mousse::DLListBase::const_reverse_iterator::operator!=
(
  const const_reverse_iterator& iter
) const
{
  return curElmt_ != iter.curElmt_;
}

inline const mousse::DLListBase::link&
mousse::DLListBase::const_reverse_iterator::operator*()
{
  return *curElmt_;
}

inline mousse::DLListBase::const_reverse_iterator&
mousse::DLListBase::const_reverse_iterator::operator++()
{
  if (curElmt_ == curList_.first_)
  {
    curElmt_ = 0;
  }
  else
  {
    curElmt_ = curElmt_->prev_;
  }
  return *this;
}

inline mousse::DLListBase::const_reverse_iterator
mousse::DLListBase::const_reverse_iterator::operator++(int)
{
  const_reverse_iterator tmp = *this;
  ++*this;
  return tmp;
}

inline mousse::DLListBase::const_reverse_iterator
mousse::DLListBase::crbegin() const
{
  if (size())
  {
    return const_reverse_iterator(*this, last());
  }
  else
  {
    return endConstRevIter_;
  }
}

inline const mousse::DLListBase::const_reverse_iterator&
mousse::DLListBase::crend() const
{
  return endConstRevIter_;
}

inline mousse::DLListBase::const_reverse_iterator
mousse::DLListBase::rbegin() const
{
  return this->crbegin();
}

inline const mousse::DLListBase::const_reverse_iterator&
mousse::DLListBase::rend() const
{
  return endConstRevIter_;
}

#endif
