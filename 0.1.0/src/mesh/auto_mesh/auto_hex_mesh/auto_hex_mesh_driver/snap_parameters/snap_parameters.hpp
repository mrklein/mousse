// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::snapParameters
// Description
//   Simple container to keep together snap specific information.
// SourceFiles
//   snap_parameters.cpp
#ifndef snap_parameters_hpp_
#define snap_parameters_hpp_
#include "dictionary.hpp"
#include "scalar.hpp"
#include "switch.hpp"
namespace mousse
{
// Class forward declarations
class snapParameters
{
  // Private data
    const label nSmoothPatch_;
    const scalar snapTol_;
    const label nSmoothDispl_;
    const label nSnap_;
    const label nFeatureSnap_;
    const Switch explicitFeatureSnap_;
    const Switch implicitFeatureSnap_;
    const Switch multiRegionFeatureSnap_;
    const Switch detectNearSurfacesSnap_;
public:
  // Constructors
    //- Construct from dictionary
    snapParameters(const dictionary& dict);
    //- Disallow default bitwise copy construct
    snapParameters(const snapParameters&) = delete;
    //- Disallow default bitwise assignment
    snapParameters& operator=(const snapParameters&) = delete;
  // Member Functions
    // Access
      //- Number of patch smoothing iterations before finding
      //  correspondence to surface
      label nSmoothPatch() const
      {
        return nSmoothPatch_;
      }
      //- Relative distance for points to be attracted by surface
      //  feature point
      //  or edge. True distance is this factor times local
      //  maximum edge length.
      scalar snapTol() const
      {
        return snapTol_;
      }
      //- Number of mesh displacement smoothing iterations.
      label nSmoothDispl() const
      {
        return nSmoothDispl_;
      }
      //- Maximum number of snapping relaxation iterations. Should stop
      //  before upon reaching a correct mesh.
      label nSnap() const
      {
        return nSnap_;
      }
      label nFeatureSnap() const
      {
        return nFeatureSnap_;
      }
      Switch explicitFeatureSnap() const
      {
        return explicitFeatureSnap_;
      }
      Switch implicitFeatureSnap() const
      {
        return implicitFeatureSnap_;
      }
      Switch multiRegionFeatureSnap() const
      {
        return multiRegionFeatureSnap_;
      }
      Switch detectNearSurfacesSnap() const
      {
        return detectNearSurfacesSnap_;
      }
};
}  // namespace mousse
#endif
