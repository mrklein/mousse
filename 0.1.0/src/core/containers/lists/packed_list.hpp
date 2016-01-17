// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PackedList
// Description
//   A dynamically allocatable list of packed unsigned integers.
//   The list resizing is similar to DynamicList, thus the methods clear()
//   and setSize() behave like their DynamicList counterparts and the methods
//   reserve() and setCapacity() can be used to influence the allocation.
//   The number of bits per item is specified by the template parameter nBits.
// Note
//   In a const context, the '[]' operator simply returns the stored value,
//   with out-of-range elements returned as zero.
//   In a non-const context, the '[]' operator returns an iteratorBase, which
//   might not have a valid reference for out-of-range elements.
//   The iteratorBase class handles the assignment of new values.
//   Using the iteratorBase as a proxy allows assignment of values
//   between list elements. Thus the following bit of code works as expected:
//   \code
//     list[1] = list[5];      // value assignment, not iterator position
//     list[2] = list[5] = 4;  // propagates value
//     list[1] = list[5] = list[6];  // propagates value
//   \endcode
//   Using get() or the '[]' operator are similarly fast. Looping and reading
//   via an iterator is approx. 15% slower, but can be more flexible.
//   Using the set() operator (and the '[]' operator) are marginally slower
//   (approx. 5%) than using an iterator, but the set() method has the
//   advantage of also returning a bool if the value changed.  This can be
//   useful for branching on changed values.
//   \code
//     list[5] = 4;
//     changed = list.set(5, 8);
//     if (changed) ...
//   \endcode
//   The lazy evaluation used means that reading an out-of-range element
//   returns zero, but does not affect the list size.  Even in a non-const
//   context, only the assigment itself causes the element to be created.
//   For example,
//   \code
//     list.resize(4);
//     Info<< list[10] << "\n";  // print zero, but doesn't adjust list
//     list[8] = 1;
//   \endcode
//   Also note that all unused internal storage elements are guaranteed to
//   always be bit-wise zero. This property must not be violated by any
//   inheriting classes.
//   In addition to the normal output format, PackedList also supports a
//   compact ASCII format that may be convenient for user input in some
//   situations. The general format is a group of index/value pairs:
//   \verbatim
//     { (index1 value1) (index2 value2) (index3 value3) }
//   \endverbatim
//   The bool specialization just uses the indices corresponding to
//   non-zero entries instead of a index/value pair:
//   \verbatim
//     { index1 index2 index3 }
//   \endverbatim
//   In both cases, the supplied indices can be randomly ordered.
// SeeAlso
//   mousse::DynamicList
// SourceFiles
//   packed_list.cpp

#ifndef packed_list_hpp_
#define packed_list_hpp_

#include <climits>

#include "label_list.hpp"
#include "uindirect_list.hpp"
#include "static_assert.hpp"
#include "class_name.hpp"
#include "error.hpp"
#include "istream.hpp"
#include "token.hpp"


namespace mousse
{

// Forward declaration of classes
class Istream;
class Ostream;

// Forward declaration of friend functions and operators
template<unsigned nBits> class PackedList;
template<unsigned nBits>
Istream& operator>>(Istream&, PackedList<nBits>&);
template<unsigned nBits>
Ostream& operator<<(Ostream&, const PackedList<nBits>&);

//- Template-invariant bits for PackedList
struct PackedListCore
{
  //- Construct null
  PackedListCore()
  {}
  //- Define template name and debug
  CLASS_NAME("PackedList");
};

template<unsigned nBits=1>
class PackedList
:
  public PackedListCore,
  private List<unsigned int>
{
protected:

  typedef unsigned int      StorageType;

  typedef List<StorageType> StorageList;

  // Protected Member Functions

    //- Calculate the list length when packed
    inline static label packedLength(const label);

    //- Read a list entry (allows for specialization)
    inline static unsigned int readValue(Istream&);

    //- Read an index/value pair and set accordingly.
    //  For bool specialization, read a single index value
    inline void setPair(Istream&);

private:

  //- nBits must be positive (non-zero) and fit within the storage.
  //  For efficiency, however, require packing at least 2 items otherwise
  //  it is more efficient to use a normal list.
  //  Thus max nBits is 1/2 of the base storage size.
  //  For simplicity, assume 8-bit bytes in the assert.
  STATIC_ASSERT(nBits && nBits <= (sizeof(StorageType) << 2));

  // Private data
    //- Number of nBits entries
    label size_;

public:

  // Public data

    //- The max. number of bits that can be templated.
    //  Might someday be useful for a template assert.
    inline static unsigned int max_bits();

    //- The max. value for an entry, which simultaneously the bit-mask
    //  eg, ((1 << 2) - 1) yields 0b0011
    inline static unsigned int max_value();

    //- The number of entries per packed storage element
    inline static unsigned int packing();

    //- Masking for all bits below the offset
    inline static unsigned int maskLower(unsigned offset);

  // Forward declaration of iterators
    class iteratorBase;
    class iterator;
    class const_iterator;

  // Constructors

    //- Null constructor
    inline PackedList();

    //- Construct with given size, initializes list to 0
    explicit inline PackedList(const label size);

    //- Construct with given size and value for all elements
    inline PackedList(const label size, const unsigned val);

    //- Construct from Istream
    inline PackedList(Istream&);

    //- Copy constructor
    inline PackedList(const PackedList<nBits>&);

    //- Construct by transferring the parameter contents
    inline PackedList(const Xfer<PackedList<nBits> >&);

    //- Construct from a list of labels
    explicit inline PackedList(const labelUList&);

    //- Construct from an indirect list of labels
    explicit inline PackedList(const UIndirectList<label>&);

    //- Clone
    inline autoPtr<PackedList<nBits>> clone() const;

  // Member Functions

    // Access

      //- The number of elements that can be stored before reallocating
      inline label capacity() const;

      //- Number of entries.
      inline label size() const;

      //- Return true if the list is empty (ie, size() is zero).
      inline bool empty() const;

      //- Get value at index I.
      //  Never auto-vivify entries.
      inline unsigned int get(const label) const;

      //- Set value at index I. Return true if value changed.
      //  Does auto-vivify for non-existent entries.
      //  Default value set is the max_value.
      inline bool set(const label, const unsigned int val = ~0u);

      //- Unset the entry at index I. Return true if value changed.
      //  Never auto-vivify entries.
      inline bool unset(const label);

      //- Return the underlying packed storage
      //  Manipulate with utmost caution
      inline List<unsigned int>& storage();

      //- Return the underlying packed storage
      inline const List<unsigned int>& storage() const;

      //- The list length when packed
      inline label packedLength() const;

      //- Return the binary size in number of characters
      //  used in the underlying storage
      inline std::streamsize byteSize() const;

      //- Count number of bits set, O(log(n))
      //  Uses the Hamming weight (population count) method
      //  http://en.wikipedia.org/wiki/Hamming_weight
      unsigned int count() const;

      //- Return the values as a list of labels
      Xfer<labelList> values() const;

      //- Print bit patterns, optionally output unused elements
      //
      // addressable bits:
      //   on: '1', off: '-'
      //
      // non-addressable bits:
      //   on: '!', off: '.'
      //
      Ostream& printBits(Ostream&, const bool fullOutput=false) const;

      //- Print information and bit patterns (with printBits)
      Ostream& printInfo(Ostream&, const bool fullOutput=false) const;

    // Edit

      //- Trim any trailing zero elements
      bool trim();

      //- Invert the bits in the addressable region
      void flip();

      //- Clear all bits
      inline void reset();

      //- Alter the size of the underlying storage.
      //  The addressed size will be truncated if needed to fit, but will
      //  remain otherwise untouched.
      inline void setCapacity(const label);

      //- Reset addressable list size, does not shrink the allocated size.
      //  Optionally specify a value for new elements.
      inline void resize(const label, const unsigned int& val = 0u);

      //- Alias for resize()
      inline void setSize(const label, const unsigned int& val = 0u);

      //- Reserve allocation space for at least this size.
      //  Never shrinks the allocated size.
      //  The list size is adjusted as per DynamicList with
      //  SizeInc=0, SizeMult=2, SizeDiv=1
      inline void reserve(const label);

      //- Clear the list, i.e. set addressable size to zero.
      //  Does not adjust the underlying storage
      inline void clear();

      //- Clear the list and delete storage.
      inline void clearStorage();

      //- Shrink the allocated space to what is actually used.
      inline void shrink();

      //- Transfer the contents of the argument list into this list
      //  and annul the argument list.
      inline void transfer(PackedList<nBits>&);

      //- Transfer contents to the Xfer container
      inline Xfer<PackedList<nBits> > xfer();

    // IO

      //- Clear list and read from stream
      Istream& read(Istream&);

      //- Write, optionally with indexedOutput
      //
      // The indexed output may be convenient in some situations.
      // The general format is a group of index/value pairs:
      //  \verbatim
      //      { (index1 value1) (index2 value2) (index3 value3) }
      // \endverbatim
      // The bool specialization just uses the indices corresponding to
      // non-zero entries instead of a index/value pair:
      // \verbatim
      //     { index1 index2 index3 }
      // \endverbatim
      //
      // Note the indexed output is only supported for ASCII streams.
      Ostream& write
      (
        Ostream&,
        const bool indexedOutput=false
      ) const;

      //- Write as a dictionary entry
      void writeEntry(Ostream&) const;

      //- Write as a dictionary entry with keyword
      void writeEntry(const word& keyword, Ostream&) const;

  // Member operators

      //- Append a value at the end of the list
      inline PackedList<nBits>& append(const unsigned int val);

      //- Remove and return the last element
      inline unsigned int remove();

      //- Get value at index I
      //  Never auto-vivify entries.
      inline unsigned int operator[](const label) const;

      //- Set value at index I.
      //  Returns iterator to perform the actual operation.
      //  Does not auto-vivify entries, but will when assigned to.
      inline iteratorBase operator[](const label);

      //- Assignment of all entries to the given value. Takes linear time.
      inline PackedList<nBits>& operator=(const unsigned int val);

      //- Assignment operator.
      PackedList<nBits>& operator=(const PackedList<nBits>&);

      //- Assignment operator.
      PackedList<nBits>& operator=(const labelUList&);

      //- Assignment operator.
      PackedList<nBits>& operator=(const UIndirectList<label>&);

  // Iterators and helpers
    //- The iterator base for PackedList
    //  Note: data and functions are protected, to allow reuse by iterator
    //  and prevent most external usage.
    class iteratorBase
    {
      friend class PackedList;

    protected:

      // Protected Data

        //- Pointer to original list
        //  This also lets us use the default bitwise copy/assignment
        PackedList* list_;

        //- Element index
        label index_;

      // Protected Member Functions

        //- Get value as unsigned, no range-checking
        inline unsigned int get() const;

        //- Set value, returning true if changed, no range-checking
        inline bool set(unsigned int);

      // Constructors

        //- Construct null
        inline iteratorBase();

        //- Construct from base list and position index
        inline iteratorBase(const PackedList*, const label);

    public:

      // Member Functions

        //- Return the element index corresponding to the iterator
        inline label key() const;

        //- Write index/value for a non-zero entry
        //  The bool specialization writes the index only
        inline bool writeIfSet(Ostream&) const;

      // Member Operators

        //- Compare values (not positions)
        inline bool operator==(const iteratorBase&) const;
        inline bool operator!=(const iteratorBase&) const;

        //- Assign value, not position.
        //  This allows packed[0] = packed[3] for assigning values
        inline unsigned int operator=(const iteratorBase&);

        //- Assign value.
        //  A non-existent entry will be auto-vivified.
        inline unsigned int operator=(const unsigned int val);

        //- Conversion operator
        //  Never auto-vivify entries.
        inline operator unsigned int () const;

      //- Print information and values
      Ostream& printInfo(Ostream&) const;

    };
    //- The iterator class used for PackedList
    class iterator
    :
      public iteratorBase
    {
    public:
      // Constructors
        //- Construct null
        inline iterator();
        //- Construct from iterator base, eg iter(packedlist[i])
        //  but also  "iterator iter = packedlist[i];"
        //  An out-of-range iterator is assigned end()
        inline iterator(const iteratorBase&);
        //- Construct from base list and position index
        inline iterator(const PackedList*, const label);

        //- Disallow copy constructor from const_iterator
        //  This would violate const-ness!
        iterator(const const_iterator&) = delete;

        //- Disallow assignment from const_iterator
        //  This would violate const-ness!
        const_iterator& operator=(const const_iterator&) = delete;

      // Member Operators

        //- Compare positions (not values)
        inline bool operator==(const iteratorBase&) const;
        inline bool operator!=(const iteratorBase&) const;

        //- Assign from iteratorBase, eg iter = packedlist[i]
        //  An out-of-range iterator is assigned end()
        inline iterator& operator=(const iteratorBase&);

        //- Return value
        inline unsigned int operator*() const;

        //- Return value
        inline unsigned int operator()() const;

        //- Return iteratorBase for assigning values
        inline iteratorBase& operator*();

        //- Return iteratorBase for assigning values
        inline iteratorBase& operator()();
        inline iterator& operator++();
        inline iterator operator++(int);
        inline iterator& operator--();
        inline iterator operator--(int);
    };

    //- Iterator set to the beginning of the PackedList
    inline iterator begin();

    //- Iterator set to beyond the end of the PackedList
    inline iterator end();

    //- The const_iterator for PackedList
    class const_iterator
    :
      public iteratorBase
    {
    public:

      // Constructors

        //- Construct null
        inline const_iterator();

        //- Construct from iterator base, eg iter(packedlist[i])
        //  but also  "const_iterator iter = packedlist[i];"
        //  An out-of-range iterator is assigned cend()
        inline const_iterator(const iteratorBase&);

        //- Construct from base list and position index
        inline const_iterator(const PackedList*, const label);

        //- Construct from iterator
        inline const_iterator(const iterator&);

      // Member operators

        //- Compare positions (not values)
        inline bool operator==(const iteratorBase&) const;
        inline bool operator!=(const iteratorBase&) const;

        //- Assign from iteratorBase or derived
        //  eg, iter = packedlist[i] or even iter = list.begin()
        inline const_iterator& operator=(const iteratorBase&);

        //- Return referenced value directly
        inline unsigned int operator*() const;

        //- Return referenced value directly
        inline unsigned int operator()() const;
        inline const_iterator& operator++();
        inline const_iterator operator++(int);
        inline const_iterator& operator--();
        inline const_iterator operator--(int);
    };

    //- const_iterator set to the beginning of the PackedList
    inline const_iterator cbegin() const;

    //- const_iterator set to beyond the end of the PackedList
    inline const_iterator cend() const;

    //- const_iterator set to the beginning of the PackedList
    inline const_iterator begin() const;

    //- const_iterator set to beyond the end of the PackedList
    inline const_iterator end() const;

  // IOstream Operators

    friend Istream& operator>> <nBits>
    (
      Istream&,
      PackedList<nBits>&
    );

    friend Ostream& operator<< <nBits>
    (
      Ostream&,
      const PackedList<nBits>&
    );
};

}  // namespace mousse

template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::max_bits()
{
  return sizeof(StorageType)*CHAR_BIT - 1;
}


template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::max_value()
{
  return (1u << nBits) - 1;
}


template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::packing()
{
  return sizeof(StorageType)*CHAR_BIT / nBits;
}


template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::maskLower(unsigned offset)
{
  // return (1u << (nBits * offset)) - 1;
  // The next one works more reliably with overflows
  // eg, when compiled without optimization
  return (~0u >> ( sizeof(StorageType)*CHAR_BIT - nBits * offset));
}


template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::packedLength(const label nElem)
{
  return (nElem + packing() - 1) / packing();
}


namespace mousse
{
// Template specialization for bool entries
template<>
inline unsigned int mousse::PackedList<1>::readValue(Istream& is)
{
  return readBool(is);
}

// Template specialization for bool entries
template<>
inline void mousse::PackedList<1>::setPair(Istream& is)
{
  set(readLabel(is), true);
}

// Template specialization for bool entries
template<>
inline bool mousse::PackedList<1>::iteratorBase::writeIfSet(Ostream& os) const
{
  if (this->get())
  {
    os  << index_;
    return true;
  }
  else
  {
    return false;
  }
}

}


template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::readValue(Istream& is)
{
  const unsigned int val = readLabel(is);
  if (val > max_value())
  {
    FATAL_IO_ERROR_IN
    (
      "PackedList<nBits>::readValue(Istream&)",
      is
    )
    << "Out-of-range value " << val << " for PackedList<" << nBits
    << ">. Maximum permitted value is " << max_value() << "."
    << exit(FatalIOError);
  }
  return val;
}


template<unsigned nBits>
inline void mousse::PackedList<nBits>::setPair(Istream& is)
{
  is.readBegin("Tuple2<label, unsigned int>");
  const label ind = readLabel(is);
  const unsigned int val = readLabel(is);
  is.readEnd("Tuple2<label, unsigned int>");
  if (val > max_value())
  {
    FATAL_IO_ERROR_IN
    (
      "PackedList<nBits>::setPair(Istream&)",
      is
    )
    << "Out-of-range value " << val << " for PackedList<" << nBits
    << "> at index " << ind
    << ". Maximum permitted value is " << max_value() << "."
    << exit(FatalIOError);
  }
  set(ind, val);
  // Check state of Istream
  is.check("PackedList<nBits>::setPair(Istream&)");
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iteratorBase::writeIfSet(Ostream& os) const
{
  const label val = this->get();
  if (val)
  {
    os<< token::BEGIN_LIST
      << index_ << token::SPACE << val
      << token::END_LIST;
    return true;
  }
  else
  {
    return false;
  }
}


// Constructors 
template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList()
:
  PackedListCore{},
  StorageList{},
  size_{0}
{}


template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const label size)
:
  PackedListCore{},
  StorageList{packedLength(size), 0u},
  size_{size}
{}


template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList
(
  const label size,
  const unsigned int val
)
:
  PackedListCore{},
  StorageList{packedLength(size), 0u},
  size_{size}
{
  if (val)
  {
    operator=(val);
  }
}


template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(Istream& is)
:
  PackedListCore{},
  StorageList{},
  size_{0}
{
  read(is);
}


template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const PackedList<nBits>& lst)
:
  PackedListCore{},
  StorageList{lst},
  size_{lst.size_}
{}


template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const Xfer<PackedList<nBits> >& lst)
{
  transfer(lst());
}


template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const labelUList& lst)
:
  PackedListCore{},
  StorageList{packedLength(lst.size()), 0u},
  size_{lst.size()}
{
  FOR_ALL(lst, i)
  {
    set(i, lst[i]);
  }
}


template<unsigned nBits>
inline mousse::PackedList<nBits>::PackedList(const UIndirectList<label>& lst)
:
  PackedListCore{},
  StorageList{packedLength(lst.size()), 0u},
  size_{lst.size()}
{
  FOR_ALL(lst, i)
  {
    set(i, lst[i]);
  }
}


template<unsigned nBits>
inline mousse::autoPtr<mousse::PackedList<nBits> >
mousse::PackedList<nBits>::clone() const
{
  return {new PackedList<nBits>{*this}};
}


// Iterators
// iteratorBase
template<unsigned nBits>
inline mousse::PackedList<nBits>::iteratorBase::iteratorBase()
:
  list_{0},
  index_{0}
{}


template<unsigned nBits>
inline mousse::PackedList<nBits>::iteratorBase::iteratorBase
(
  const PackedList<nBits>* lst,
  const label i
)
:
  list_{const_cast<PackedList<nBits>*>(lst)},
  index_{i}
{}


template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::iteratorBase::get() const
{
  const unsigned int seg = index_ / packing();
  const unsigned int off = index_ % packing();
  const unsigned int& stored = list_->StorageList::operator[](seg);
  return (stored >> (nBits * off)) & max_value();
}


template<unsigned nBits>
inline bool
mousse::PackedList<nBits>::iteratorBase::set(const unsigned int val)
{
  const unsigned int seg = index_ / packing();
  const unsigned int off = index_ % packing();
  const unsigned int startBit = nBits * off;
  const unsigned int mask = max_value() << startBit;
  unsigned int& stored = list_->StorageList::operator[](seg);
  const unsigned int prev = stored;
  if (val >= max_value())
  {
    // overflow is max_value, fill everything
    stored |= mask;
  }
  else
  {
    stored &= ~mask;
    stored |= mask & (val << startBit);
  }
  return prev != stored;
}


template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::iteratorBase::key() const
{
  return index_;
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iteratorBase::operator==
(
  const iteratorBase& iter
) const
{
  return this->get() == iter.get();
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iteratorBase::operator!=
(
  const iteratorBase& iter
) const
{
  return this->get() != iter.get();
}


template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::iteratorBase::operator=(const iteratorBase& iter)
{
  const unsigned int val = iter.get();
  this->set(val);
  return val;
}


template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::iteratorBase::operator=(const unsigned int val)
{
  // lazy evaluation - increase size on assigment
  if (index_ >= list_->size_)
  {
    list_->resize(index_ + 1);
  }
  this->set(val);
  return val;
}


template<unsigned nBits>
inline mousse::PackedList<nBits>::iteratorBase::operator
unsigned int () const
{
  // lazy evaluation - return 0 for out-of-range
  if (index_ >= list_->size_)
  {
    return 0;
  }
  return this->get();
}


// const_iterator, iterator
template<unsigned nBits>
inline mousse::PackedList<nBits>::iterator::iterator()
:
  iteratorBase{}
{}


template<unsigned nBits>
inline mousse::PackedList<nBits>::const_iterator::const_iterator()
:
  iteratorBase{}
{}


template<unsigned nBits>
inline mousse::PackedList<nBits>::iterator::iterator
(
  const iteratorBase& iter
)
:
  iteratorBase{iter}
{
  // avoid going past end()
  // eg, iter = iterator(list, Inf)
  if (this->index_ > this->list_->size_)
  {
    this->index_ = this->list_->size_;
  }
}


template<unsigned nBits>
inline mousse::PackedList<nBits>::const_iterator::const_iterator
(
  const iteratorBase& iter
)
:
  iteratorBase{iter}
{
  // avoid going past end()
  // eg, iter = iterator(list, Inf)
  if (this->index_ > this->list_->size_)
  {
    this->index_ = this->list_->size_;
  }
}


template<unsigned nBits>
inline mousse::PackedList<nBits>::iterator::iterator
(
  const PackedList<nBits>* lst,
  const label i
)
:
  iteratorBase{lst, i}
{}


template<unsigned nBits>
inline mousse::PackedList<nBits>::const_iterator::const_iterator
(
  const PackedList<nBits>* lst,
  const label i
)
:
  iteratorBase{lst, i}
{}


template<unsigned nBits>
inline mousse::PackedList<nBits>::const_iterator::const_iterator
(
  const iterator& iter
)
:
  iteratorBase{static_cast<const iteratorBase&>(iter)}
{}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iterator::operator==
(
  const iteratorBase& iter
) const
{
  return this->index_ == iter.index_;
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::iterator::operator!=
(
  const iteratorBase& iter
) const
{
  return this->index_ != iter.index_;
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::const_iterator::operator==
(
  const iteratorBase& iter
) const
{
  return this->index_ == iter.index_;
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::const_iterator::operator!=
(
  const iteratorBase& iter
) const
{
  return this->index_ != iter.index_;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator&
mousse::PackedList<nBits>::iterator::operator=(const iteratorBase& iter)
{
  this->list_  = iter.list_;
  this->index_ = iter.index_;
  // avoid going past end()
  // eg, iter = iterator(list, Inf)
  if (this->index_ > this->list_->size_)
  {
    this->index_ = this->list_->size_;
  }
  return *this;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator&
mousse::PackedList<nBits>::const_iterator::operator=(const iteratorBase& iter)
{
  this->list_  = iter.list_;
  this->index_ = iter.index_;
  // avoid going past end()
  // eg, iter = iterator(list, Inf)
  if (this->index_ > this->list_->size_)
  {
    this->index_ = this->list_->size_;
  }
  return *this;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator&
mousse::PackedList<nBits>::iterator::operator++()
{
  ++this->index_;
  return *this;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator&
mousse::PackedList<nBits>::const_iterator::operator++()
{
  ++this->index_;
  return *this;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator
mousse::PackedList<nBits>::iterator::operator++(int)
{
  iterator old = *this;
  ++this->index_;
  return old;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::const_iterator::operator++(int)
{
  const_iterator old = *this;
  ++this->index_;
  return old;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator&
mousse::PackedList<nBits>::iterator::operator--()
{
  --this->index_;
  return *this;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator&
mousse::PackedList<nBits>::const_iterator::operator--()
{
  --this->index_;
  return *this;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator
mousse::PackedList<nBits>::iterator::operator--(int)
{
  iterator old = *this;
  --this->index_;
  return old;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::const_iterator::operator--(int)
{
  const_iterator old = *this;
  --this->index_;
  return old;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iteratorBase&
mousse::PackedList<nBits>::iterator::operator*()
{
  return static_cast<iteratorBase&>(*this);
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iteratorBase&
mousse::PackedList<nBits>::iterator::operator()()
{
  return static_cast<iteratorBase&>(*this);
}


template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::const_iterator::operator*() const
{
  return this->get();
}


template<unsigned nBits>
inline unsigned int
mousse::PackedList<nBits>::const_iterator::operator()() const
{
  return this->get();
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator
mousse::PackedList<nBits>::begin()
{
  return iterator(this, 0);
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::begin() const
{
  return const_iterator(this, 0);
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::cbegin() const
{
  return const_iterator(this, 0);
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iterator
mousse::PackedList<nBits>::end()
{
  return iterator(this, size_);
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::end() const
{
  return const_iterator(this, size_);
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::const_iterator
mousse::PackedList<nBits>::cend() const
{
  return const_iterator(this, size_);
}


// Member Functions 
template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::size() const
{
  return size_;
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::empty() const
{
  return !size_;
}


template<unsigned nBits>
inline void mousse::PackedList<nBits>::resize
(
  const label newSize,
  const unsigned int& val
)
{
  reserve(newSize);
  const label oldSize = size_;
  size_ = newSize;
  if (size_ > oldSize)
  {
    // fill new elements or newly exposed elements
    if (val)
    {
      // fill value for complete segments
      unsigned int fill = val;
      if (val >= max_value())
      {
        // fill everything
        fill = maskLower(packing());
      }
      else
      {
        for (unsigned int i = 1; i < packing(); ++i)
        {
          fill |= (fill << nBits);
        }
      }
      // fill in complete segments
      const label oldLen = packedLength(oldSize);
      const label newLen = packedLength(size_);
      for (label i=oldLen; i < newLen; ++i)
      {
        StorageList::operator[](i) = fill;
      }
      // finish previous partial segment, preserve existing value
      {
        const unsigned int off = oldSize % packing();
        if (off)
        {
          const unsigned int seg = oldSize / packing();
          const unsigned int mask = maskLower(off);
          StorageList::operator[](seg) &= mask;
          StorageList::operator[](seg) |= ~mask & fill;
        }
      }
      // mask off the (new) final partial segment
      {
        const unsigned int off = size_ % packing();
        if (off)
        {
          const unsigned int seg = size_ / packing();
          StorageList::operator[](seg) &= maskLower(off);
        }
      }
    }
  }
  else if (size_ < oldSize)
  {
    // resize shrinking
    // - clear newly exposed elements
    // fill in complete segments
    const label oldLen = packedLength(oldSize);
    const label newLen = packedLength(size_);
    for (label i=newLen; i < oldLen; ++i)
    {
      StorageList::operator[](i) = 0u;
    }
    // mask off the final partial segment
    {
      const unsigned int off = size_ % packing();
      if (off)
      {
        const unsigned int seg = size_ / packing();
        StorageList::operator[](seg) &= maskLower(off);
      }
    }
  }
}


template<unsigned nBits>
inline void mousse::PackedList<nBits>::setSize
(
  const label newSize,
  const unsigned int& val
)
{
  resize(newSize, val);
}


template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::capacity() const
{
  return packing() * StorageList::size();
}


template<unsigned nBits>
inline void mousse::PackedList<nBits>::setCapacity(const label nElem)
{
  StorageList::setSize(packedLength(nElem), 0u);
  // truncate addressed size too
  if (size_ > nElem)
  {
    size_ = nElem;
    // mask off the final partial segment
    const unsigned int off = size_ % packing();
    if (off)
    {
      const unsigned int seg = size_ / packing();
      StorageList::operator[](seg) &= maskLower(off);
    }
  }
}


template<unsigned nBits>
inline void mousse::PackedList<nBits>::reserve(const label nElem)
{
  const label len = packedLength(nElem);
  // need more capacity?
  if (len > StorageList::size())
  {
    // Like DynamicList with SizeInc=0, SizeMult=2, SizeDiv=1
    StorageList::setSize
    (
      max
      (
        len,
        StorageList::size()*2
      ),
      0u
    );
  }
}


template<unsigned nBits>
inline void mousse::PackedList<nBits>::reset()
{
  StorageList::operator=(0u);
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::clear()
{
  reset();
  size_ = 0;
}


template<unsigned nBits>
inline void mousse::PackedList<nBits>::clearStorage()
{
  StorageList::clear();
  size_ = 0;
}
template<unsigned nBits>
inline void mousse::PackedList<nBits>::shrink()
{
  // any uneed space allocated?
  const label len = packedLength();
  if (len < StorageList::size())
  {
    StorageList::setSize(len);
  }
}
template<unsigned nBits>
inline mousse::List<unsigned int>& mousse::PackedList<nBits>::storage()
{
  return static_cast<StorageList&>(*this);
}


template<unsigned nBits>
inline const mousse::List<unsigned int>& mousse::PackedList<nBits>::storage() const
{
  return static_cast<const StorageList&>(*this);
}


template<unsigned nBits>
inline mousse::label mousse::PackedList<nBits>::packedLength() const
{
  return packedLength(size_);
}


template<unsigned nBits>
inline std::streamsize mousse::PackedList<nBits>::byteSize() const
{
  return packedLength() * sizeof(StorageType);
}


template<unsigned nBits>
inline void mousse::PackedList<nBits>::transfer(PackedList<nBits>& lst)
{
  size_ = lst.size_;
  lst.size_ = 0;
  StorageList::transfer(lst);
}



template<unsigned nBits>
inline mousse::Xfer<mousse::PackedList<nBits> > mousse::PackedList<nBits>::xfer()
{
  return xferMove(*this);
}


template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::get(const label i) const
{
  // lazy evaluation - return 0 for out-of-range
  if (i < 0 || i >= size_)
  {
    return 0;
  }
  else
  {
    return iteratorBase(this, i).get();
  }
}


template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::operator[](const label i) const
{
  // lazy evaluation - return 0 for out-of-range
  if (i < 0 || i >= size_)
  {
    return 0;
  }
  else
  {
    return iteratorBase(this, i).get();
  }
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::set
(
  const label i,
  const unsigned int val
)
{
  if (i < 0)
  {
    // lazy evaluation - ignore out-of-bounds
    return false;
  }
  else if (i >= size_)
  {
    // lazy evaluation - increase size on assigment
    resize(i + 1);
  }
  return iteratorBase(this, i).set(val);
}


template<unsigned nBits>
inline bool mousse::PackedList<nBits>::unset(const label i)
{
  // lazy evaluation - ignore out-of-bounds
  if (i < 0 || i >= size_)
  {
    return false;
  }
  else
  {
    return iteratorBase(this, i).set(0u);
  }
}


template<unsigned nBits>
inline mousse::PackedList<nBits>&
mousse::PackedList<nBits>::append(const unsigned int val)
{
  const label elemI = size_;
  reserve(elemI + 1);
  size_++;
  iteratorBase(this, elemI).set(val);
  return *this;
}


template<unsigned nBits>
inline unsigned int mousse::PackedList<nBits>::remove()
{
  if (!size_)
  {
    FATAL_ERROR_IN
    (
      "mousse::PackedList<nBits>::remove()"
    )   << "List is empty" << abort(FatalError);
  }
  label elemI = size_ - 1;
  const unsigned int val = iteratorBase(this, elemI).get();
  resize(elemI);
  return val;
}


template<unsigned nBits>
inline typename mousse::PackedList<nBits>::iteratorBase
mousse::PackedList<nBits>::operator[](const label i)
{
  return iteratorBase(this, i);
}


template<unsigned nBits>
inline mousse::PackedList<nBits>&
mousse::PackedList<nBits>::operator=(const unsigned int val)
{
  const label packLen = packedLength();
  if (val && size_)
  {
    unsigned int fill = val;
    if (val >= max_value())
    {
      // fill everything
      fill = maskLower(packing());
    }
    else
    {
      for (unsigned int i = 1; i < packing(); ++i)
      {
        fill |= (fill << nBits);
      }
    }
    for (label i=0; i < packLen; ++i)
    {
      StorageList::operator[](i) = fill;
    }
    // mask off the final partial segment
    {
      const unsigned int off = size_ % packing();
      if (off)
      {
        const unsigned int seg = size_ / packing();
        StorageList::operator[](seg) &= maskLower(off);
      }
    }
  }
  else
  {
    for (label i=0; i < packLen; ++i)
    {
      StorageList::operator[](i) = 0u;
    }
  }
  return *this;
}

#ifdef NoRepository
#   include "packed_list.cpp"
#endif
#endif
