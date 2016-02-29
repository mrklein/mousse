#ifndef CORE_MESHES_IDENTIFIERS_DYNAMIC_ID_HPP_
#define CORE_MESHES_IDENTIFIERS_DYNAMIC_ID_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::DynamicID
// Description
//   A class that holds the data needed to identify things (zones, patches)
//   in a dynamic mesh.
//   The thing is identified by name.
//   Its indices are updated if the mesh has changed.
#include "key_type.hpp"
#include "label_list.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class> class DynamicID;
template<class ObjectType>
Ostream& operator<<(Ostream&, const DynamicID<ObjectType>&);
template<class ObjectType>
class DynamicID
{
  // Private data
    //- Zone name
    keyType key_;
    //- Zone indices
    labelList indices_;
public:
  // Constructors
    //- Construct from name
    DynamicID(const keyType& key, const ObjectType& obj)
    :
      key_(key),
      indices_(obj.findIndices(key_))
    {}
    //- Construct from Istream
    DynamicID(Istream& is, const ObjectType& obj)
    :
      key_(is),
      indices_(obj.findIndices(key_))
    {}
  // Destructor - default
  // Member Functions
    // Access
      //- Return name
      const keyType& name() const
      {
        return key_;
      }
      //- Return indices of matching zones
      const labelList& indices() const
      {
        return indices_;
      }
      //- Return index of first matching zone
      label index() const
      {
        return indices_.empty() ? -1 : indices_[0];
      }
      //- Has the zone been found
      bool active() const
      {
        return !indices_.empty();
      }
    // Edit
      //- Update
      void update(const ObjectType& obj)
      {
        indices_ = obj.findIndices(key_);
      }
  // IOstream Operators
    friend Ostream& operator<< <ObjectType>
    (Ostream&, const DynamicID<ObjectType>&);
};
template<class ObjectType>
Ostream& operator<<(Ostream& os, const DynamicID<ObjectType>& dynId)
{
  os  << token::BEGIN_LIST
    << dynId.name() << token::SPACE << dynId.index()
    << token::END_LIST;
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const DynamicID<ObjectType>&)");
  return os;
}
}  // namespace mousse
#endif
