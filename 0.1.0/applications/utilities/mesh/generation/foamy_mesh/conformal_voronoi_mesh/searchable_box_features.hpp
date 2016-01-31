// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableBoxFeatures
// Description
// SourceFiles
//   searchable_box_features.cpp
#ifndef searchable_box_features_hpp_
#define searchable_box_features_hpp_
#include "searchable_surface_features.hpp"
namespace mousse
{
class searchableBoxFeatures
:
  public searchableSurfaceFeatures
{
private:
  // Private Member Data
    //- Which side of the box to mesh
    const extendedFeatureEdgeMesh::sideVolumeType mode_;
public:
  //- Runtime type information
  TYPE_NAME("searchableBoxFeatures");
  // Constructors
    //- Construct from searchable surface and dictionary
    searchableBoxFeatures
    (
      const searchableSurface& surface,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    searchableBoxFeatures(const searchableBoxFeatures&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const searchableBoxFeatures&) = delete;
  //- Destructor
  virtual ~searchableBoxFeatures();
  // Member Functions
    //- Return true for a searchable box having features
    virtual bool hasFeatures() const
    {
      return true;
    }
    //- Return an extendedFeatureEdgeMesh containing the features
    virtual autoPtr<extendedFeatureEdgeMesh> features() const;
};
}  // namespace mousse
#endif
