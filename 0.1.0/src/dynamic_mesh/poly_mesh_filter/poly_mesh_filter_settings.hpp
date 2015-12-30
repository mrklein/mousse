// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyMeshFilterSettings
// Description
//   Class to store the settings for the polyMeshFilter class.
// SourceFiles
//   poly_mesh_filter_settings.cpp
//   poly_mesh_filter_settings_i.hpp
#ifndef poly_mesh_filter_settings_hpp_
#define poly_mesh_filter_settings_hpp_
#include "dictionary.hpp"
#include "switch.hpp"
#include "scalar.hpp"
#include "label.hpp"
namespace mousse
{
class polyMeshFilterSettings
{
  // Private data
    //- Dictionary containing the coefficient sub-dictionaries
    const dictionary dict_;
    //- After collapsing, check the mesh quality and redo the collapsing
    //  iteration if there are too many bad faces in the mesh
    Switch controlMeshQuality_;
    //- Coefficients for collapsing edges
    const dictionary& collapseEdgesCoeffDict_;
    //- Coefficients for collapsing faces
    dictionary collapseFacesCoeffDict_;
    //- Coefficients for controlling the mesh quality
    dictionary meshQualityCoeffDict_;
    //- Remove edges shorter than this length
    const scalar minLen_;
    //- Merge points that are only attached to two edges and have an angle
    //  between the edge greater than this value
    const scalar maxCos_;
    //- The amount that the local minimum edge length will be reduced by if
    //  the edge is part of a collapse string that generates poor quality
    //  faces
    const scalar edgeReductionFactor_;
    //- Maximum number of outer iterations
    const label maxIterations_;
    //- Maximum number of smoothing iterations of minEdgeLen_ and
    //  faceFilterFactor_
    const label maxSmoothIters_;
    //- Initialisation value of faceFilterFactor_
    const scalar initialFaceLengthFactor_;
    //- The amount that the local face size factor will be reduced by if
    //  the face is part of a collapse string that generates poor quality
    //  faces
    const scalar faceReductionFactor_;
    //- Maximum number of times a deleted point can be associated with the
    //  creation of a bad face it is forced to be kept.
    const label maxPointErrorCount_;
public:
  // Constructors
    //- Construct from dictionary
    explicit polyMeshFilterSettings(const dictionary& dict);
  //- Destructor
  ~polyMeshFilterSettings(){};
  // Member Functions
    // Access
      inline const dictionary& collapseEdgesCoeffDict() const;
      inline const dictionary& collapseFacesCoeffDict() const;
      inline const dictionary& meshQualityCoeffDict() const;
      inline const Switch& controlMeshQuality() const;
      inline const scalar& minLen() const;
      inline const scalar& maxCos() const;
      inline const scalar& edgeReductionFactor() const;
      inline const label& maxIterations() const;
      inline const label& maxSmoothIters() const;
      inline const scalar& initialFaceLengthFactor() const;
      inline const scalar& faceReductionFactor() const;
      inline const label& maxPointErrorCount() const;
    // Write
      //- Write the settings to a stream
      void writeSettings(Ostream& os) const;
};
}  // namespace mousse
#include "poly_mesh_filter_settings_i.hpp"
#endif
