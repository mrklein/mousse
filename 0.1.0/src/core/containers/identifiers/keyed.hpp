// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Keyed
// Description
//   A container with an integer key attached to any item.
//   The key can useful for sorting.
// SourceFiles
//   keyed_i.hpp
#ifndef keyed_hpp_
#define keyed_hpp_
#include "list.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class T> class Keyed;
template<class T> Istream& operator>>(Istream&, Keyed<T>&);
template<class T> Ostream& operator<<(Ostream&, const Keyed<T>&);
template<class T>
class Keyed
:
  public T
{
  // Private data
    label key_;
public:
  // Static Members
    //- Add labels to a list of values
    inline static List<Keyed<T> > createList
    (
      const List<T>&,
      const label key=0
    );
    //- Add labels to a list of values
    inline static List<Keyed<T> > createList
    (
      const List<T>&,
      const labelUList& keys
    );
  // Constructors
    //- Construct null
    inline Keyed();
    //- Construct as a copy of item, with a key
    inline Keyed(const T& item, const label key=0);
    //- Construct by transferring the item, with a key
    inline Keyed(const Xfer<T>& item, const label key=0);
    //- Construct from Istream
    inline Keyed(Istream&);
  // Member Functions
    // Access
      //- Return const access to the integer key
      inline label key() const;
      //- Return non-const access to the integer key
      inline label& key();
  // IOstream Operators
    friend Istream& operator>> <T>(Istream&, Keyed<T>&);
    friend Ostream& operator<< <T>(Ostream&, const Keyed<T>&);
};
}  // namespace mousse
#include "keyed_i.hpp"
#endif
