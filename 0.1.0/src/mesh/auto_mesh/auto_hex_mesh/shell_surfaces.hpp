#ifndef MESH_AUTO_MESH_AUTO_HEX_MESH_SHELL_SURFACES_SHELL_SURFACES_HPP_
#define MESH_AUTO_MESH_AUTO_HEX_MESH_SHELL_SURFACES_SHELL_SURFACES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::shellSurfaces
// Description
//   Encapsulates queries for volume refinement ('refine all cells within
//   shell').

#include "searchable_surface.hpp"
#include "tuple2.hpp"


namespace mousse {

class searchableSurfaces;


class shellSurfaces
{
public:
  // Public data types
    //- Volume refinement controls
    enum refineMode
    {
      INSIDE,         // Refine all inside shell
      OUTSIDE,        // ,,         outside
      DISTANCE        // Refine based on distance to shell
    };
private:
  // Private data
    //- Reference to all geometry.
    const searchableSurfaces& allGeometry_;
    //- Indices of surfaces that are shells
    labelList shells_;
    //- Per shell whether to refine inside or outside
    List<refineMode> modes_;
    //- Per shell the list of ranges
    List<scalarField> distances_;
    //- Per shell per distance the refinement level
    labelListList levels_;
  // Private data
    //- refineMode names
    static const NamedEnum<refineMode, 3> refineModeNames_;
  // Private Member Functions
    //- Helper function for initialisation.
    void setAndCheckLevels
    (
      const label shellI,
      const List<Tuple2<scalar, label>>&
    );
    void orient();
    void findHigherLevel
    (
      const pointField& pt,
      const label shellI,
      labelList& maxLevel
    ) const;
public:
  // Constructors
    //- Construct from geometry and dictionary
    shellSurfaces
    (
      const searchableSurfaces& allGeometry,
      const dictionary& shellsDict
    );
  // Member Functions
    // Access
      //const List<scalarField>& distances() const
      //{
      //    return distances_;
      //}
      //
      ////- Per shell per distance the refinement level
      //const labelListList& levels() const
      //{
      //    return levels_;
      //}
    // Query
      //- Highest shell level
      label maxLevel() const;
      //- Find shell level higher than ptLevel
      void findHigherLevel
      (
        const pointField& pt,
        const labelList& ptLevel,
        labelList& maxLevel
      ) const;
};

}  // namespace mousse

#endif

