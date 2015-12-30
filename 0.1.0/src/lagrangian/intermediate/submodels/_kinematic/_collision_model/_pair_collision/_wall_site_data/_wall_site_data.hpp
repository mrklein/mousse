// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::WallSiteData
// Description
//   Stores the patch ID and templated data to represent a collision
//   with a wall to be passed to the wall model.
// SourceFiles
//   _wall_site_data_i.hpp
//   _wall_site_data.cpp
//   _wall_site_data_io.cpp
#ifndef _wall_site_data_hpp_
#define _wall_site_data_hpp_
#include "label.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Type>
class WallSiteData;
template<class Type>
Istream& operator>>(Istream&, WallSiteData<Type>&);
template<class Type>
Ostream& operator<<(Ostream&, const WallSiteData<Type>&);
template<class Type>
class WallSiteData
{
  // Private data
    //- Index of originating patch
    label patchI_;
    //- Wall data
    Type wallData_;
public:
  // Constructors
    //- Construct null
    WallSiteData();
    //- Construct from components
    WallSiteData
    (
      label patchI,
      const Type& wallData
    );
  //- Destructor
  ~WallSiteData();
  // Member Functions
      //- Return access to the patch index
      inline label patchIndex() const;
      //- Return non-const access to the patch index
      inline label& patchIndex();
      //- Return access to wall data
      inline const Type& wallData() const;
      //- Return non-const access to wall data
      inline Type& wallData();
  // Member Operators
    bool operator==(const WallSiteData<Type>&) const;
    bool operator!=(const WallSiteData<Type>&) const;
  // IOstream Operators
    friend Istream& operator>> <Type>
    (Istream&, WallSiteData<Type>&);
    friend Ostream& operator<< <Type>
    (Ostream&, const WallSiteData<Type>&);
};
}  // namespace mousse
#include "_wall_site_data_i.hpp"
#ifdef NoRepository
#   include "_wall_site_data.cpp"
#endif
#endif
