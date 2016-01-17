// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CompactListList
// Description
//   A packed storage unstructured matrix of objects of type \<T\>
//   using an offset table for access.
//   The offset table is the size of the number of rows+1
//   whose elements are the
//   accumulated sizes of the rows, i.e.
//    - offset[i] gives the index of first element of row i
//    - offset[i+1] - offset[i] is the number of elements in row i
//   Storage is allocated on free-store during construction.
//   As a special case a null-constructed CompactListList has an empty
//   offsets_ (instead of size 1).
// SourceFiles
//   compact_list_list.cpp
//   compact_list_list_io.cpp

#ifndef compact_list_list_hpp_
#define compact_list_list_hpp_

#include "label_list.hpp"
#include "list_ops.hpp"
#include "sub_list.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
template<class T, class Container> class CompactListList;
template<class T, class Container> Istream& operator>>
(
  Istream&,
  CompactListList<T, Container>&
);
template<class T, class Container> Ostream& operator<<
(
  Ostream&,
  const CompactListList<T, Container>&
);

template<class T, class Container = List<T> >
class CompactListList
{
  // Private data
    label size_;
    //- Offset table
    List<label> offsets_;
    //- Packed matrix of data
    List<T> m_;
public:
  // Static Member Functions
    //- Return a null CompactListList
    inline static const CompactListList<T, Container>& null();
  // Constructors
    //- Null constructor.
    inline CompactListList();
    //- Construct by converting given List<List<T> >
    explicit CompactListList(const List<Container>&);
    //- Construct given size of offset table (number of rows)
    //  and number of data.
    inline CompactListList(const label nRows, const label nData);
    //- Construct given size of offset table (number of rows),
    //  the number of data and a value for all elements.
    inline CompactListList(const label nRows, const label nData, const T&);
    //- Construct given list of row-sizes.
    explicit CompactListList(const labelUList& rowSizes);
    //- Construct given list of row-sizes
    CompactListList(const labelUList& rowSizes, const T&);
    //- Construct by transferring the parameter contents
    explicit CompactListList(const Xfer<CompactListList<T, Container> >&);
    //- Construct as copy or re-use as specified.
    CompactListList(CompactListList<T, Container>&, bool reUse);
    //- Construct from Istream.
    CompactListList(Istream&);
    //- Clone
    inline autoPtr<CompactListList<T, Container> > clone() const;
  // Member Functions
    // Access
      //- Return the primary size, i.e. the number of rows
      inline label size() const;
      //- Return true if the number of rows is zero
      inline bool empty() const;
      //- Return the offset table (= size()+1)
      inline const List<label>& offsets() const;
      //- Return non-const access to the offset table
      inline List<label>& offsets();
      //- Return the packed matrix of data
      inline const List<T>& m() const;
      //- Return non-const access to the packed matrix of data
      inline List<T>& m();
    // Edit
      //- Reset size of CompactListList.
      //  This form only allows contraction of the CompactListList.
      void setSize(const label nRows);
      //- Reset size of CompactListList.
      void setSize(const label nRows, const label nData);
      //- Reset sizes of CompactListList and value for new elements.
      void setSize(const label nRows, const label nData, const T&);
      //- Reset size of CompactListList.
      void setSize(const labelUList& rowSizes);
      //- Reset size of CompactListList.
      //  This form only allows contraction of the CompactListList.
      inline void resize(const label nRows);
      //- Reset size of CompactListList.
      inline void resize(const label nRows, const label nData);
      //- Reset sizes of CompactListList and value for new elements.
      inline void resize(const label nRows, const label nData, const T&);
      //- Reset size of CompactListList.
      inline void resize(const labelUList& rowSizes);
      //- Clear the CompactListList, i.e. set sizes to zero.
      void clear();
      //- Return sizes (to be used e.g. for construction)
      labelList sizes() const;
      //- Transfer the contents of the argument CompactListList
      //  into this CompactListList and annul the argument list.
      void transfer(CompactListList<T, Container>&);
      //- Transfer the contents to the Xfer container
      inline Xfer<CompactListList<T, Container> > xfer();
    // Other
      //- Return index into m
      inline label index(const label row, const label col) const;
      //- Get row for index into m.
      inline label whichRow(const label index) const;
      //- Get column index (j) given above row
      inline label whichColumn(const label row, const label index) const;
  // Member operators
    //- Return subscript-checked row as UList.
    inline UList<T> operator[](const label i);
    //- Return const subscript-checked row as UList.
    inline const UList<T> operator[](const label i) const;
    //- Return subscript-checked element.
    inline T& operator()(const label i, const label j);
    //- Return const subscript-checked element.
    inline const T& operator()(const label i, const label j) const;
    //- Return as List<Container>
    List<Container> operator()() const;
    //- Assignment of all entries to the given value
    inline void operator=(const T&);
  // Istream operator
    //- Read CompactListList from Istream, discarding contents
    //  of existing CompactListList.
    friend Istream& operator>> <T, Container>
    (
      Istream&,
      CompactListList<T, Container>&
    );
    // Write CompactListList to Ostream.
    friend Ostream& operator<< <T, Container>
    (
      Ostream&,
      const CompactListList<T, Container>&
    );
};
}  // namespace mousse

// Constructors 
template<class T, class Container>
inline mousse::CompactListList<T, Container>::CompactListList()
:
  size_{0}
{}

template<class T, class Container>
inline mousse::CompactListList<T, Container>::CompactListList
(
  const label nRows,
  const label nData
)
:
  size_{nRows},
  offsets_{nRows+1, 0},
  m_{nData}
{}

template<class T, class Container>
inline mousse::CompactListList<T, Container>::CompactListList
(
  const label nRows,
  const label nData,
  const T& t
)
:
  size_{nRows},
  offsets_{nRows+1, 0},
  m_{nData, t}
{}

template<class T, class Container>
inline mousse::autoPtr<mousse::CompactListList<T, Container> >
mousse::CompactListList<T, Container>::clone() const
{
  return autoPtr<CompactListList<T, Container>>
  {
    new CompactListList<T, Container>{*this}
  };
}

// Member Functions 
template<class T, class Container>
inline const mousse::CompactListList<T, Container>&
mousse::CompactListList<T, Container>::null()
{
  return NullObjectRef<CompactListList<T, Container> >();
}

template<class T, class Container>
inline mousse::label mousse::CompactListList<T, Container>::size() const
{
  return size_;
}

template<class T, class Container>
inline bool mousse::CompactListList<T, Container>::empty() const
{
  return !size_;
}

template<class T, class Container>
inline const mousse::List<mousse::label>&
mousse::CompactListList<T, Container>::offsets() const
{
  return offsets_;
}

template<class T, class Container>
inline mousse::List<mousse::label>& mousse::CompactListList<T, Container>::offsets()
{
  return offsets_;
}

template<class T, class Container>
inline const mousse::List<T>& mousse::CompactListList<T, Container>::m()
const
{
  return m_;
}

template<class T, class Container>
inline mousse::List<T>& mousse::CompactListList<T, Container>::m()
{
  return m_;
}

template<class T, class Container>
inline mousse::label mousse::CompactListList<T, Container>::index
(
  const label i,
  const label j
) const
{
  return offsets_[i] + j;
}

template<class T, class Container>
inline mousse::label mousse::CompactListList<T, Container>::whichRow(const label i)
const
{
  if (i < 0 || i >= m_.size())
  {
    FATAL_ERROR_IN
    (
      "CompactListList<T, Container>::whichRow(const label) const"
    )   << "Index " << i << " outside 0.." << m_.size()
      << abort(FatalError);
  }
  return findLower(offsets_, i+1);
}

template<class T, class Container>
inline mousse::label mousse::CompactListList<T, Container>::whichColumn
(
  const label row,
  const label i
) const
{
  return i - index(row, 0);
}

template<class T, class Container>
inline mousse::Xfer<mousse::CompactListList<T, Container> >
mousse::CompactListList<T, Container>::xfer()
{
  return xferMove(*this);
}

template<class T, class Container>
inline void mousse::CompactListList<T, Container>::resize(const label nRows)
{
  this->setSize(nRows);
}

template<class T, class Container>
inline void mousse::CompactListList<T, Container>::resize
(
  const label nRows,
  const label nData
)
{
  this->setSize(nRows, nData);
}

template<class T, class Container>
inline void mousse::CompactListList<T, Container>::resize
(
  const label nRows,
  const label nData,
  const T& t
)
{
  this->setSize(nRows, nData, t);
}

template<class T, class Container>
inline void mousse::CompactListList<T, Container>::resize
(
  const labelUList& rowSizes
)
{
  this->setSize(rowSizes);
}

// Member Operators 
template<class T, class Container>
inline mousse::UList<T> mousse::CompactListList<T, Container>::operator[]
(
  const label i
)
{
  label start = offsets_[i];
  return UList<T>(m_.begin() + start, offsets_[i+1] - start);
}

template<class T, class Container>
inline const mousse::UList<T>
mousse::CompactListList<T, Container>::operator[]
(
  const label i
) const
{
  label start = offsets_[i];
  return UList<T>
  (
    const_cast<T*>(m_.begin() + start),
    offsets_[i+1] - start
  );
}

template<class T, class Container>
inline T& mousse::CompactListList<T, Container>::operator()
(
  const label i,
  const label j
)
{
  return m_[index(i, j)];
}

template<class T, class Container>
inline const T& mousse::CompactListList<T, Container>::operator()
(
  const label i,
  const label j
) const
{
  return m_[index(i, j)];
}

template<class T, class Container>
inline void mousse::CompactListList<T, Container>::operator=(const T& t)
{
  m_ = t;
}

#ifdef NoRepository
#   include "compact_list_list.cpp"
#endif
#endif
