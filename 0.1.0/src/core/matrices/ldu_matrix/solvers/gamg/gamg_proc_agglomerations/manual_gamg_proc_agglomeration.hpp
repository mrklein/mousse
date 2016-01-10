// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::manualGAMGProcAgglomeration
// Description
//   Manual processor agglomeration of GAMGAgglomerations.
//   In the GAMG control dictionary:
//     processorAgglomerator manual;
//     // List of level+procagglomeration where
//     // procagglomeration is a set of labelLists. Each labelList is
//     // a cluster of processor which gets combined onto the first element
//     // in the list.
//     processorAgglomeration
//     (
//       (
//         3           //at level 3
//         (
//           (0 1)   //coarse 0 from 0,1 (and moved onto 0)
//           (3 2)   //coarse 1 from 2,3 (and moved onto 3)
//         )
//       )
//       (
//         6           //at level6
//         (
//           (0 1)   //coarse 0 from 0,1 (and moved onto 0)
//         )
//       )
//     );
// SourceFiles
//   manual_gamg_proc_agglomeration.cpp

#ifndef manual_gamg_proc_agglomeration_hpp_
#define manual_gamg_proc_agglomeration_hpp_

#include "gamg_proc_agglomeration.hpp"
#include "dynamic_list.hpp"
#include "tuple2.hpp"

namespace mousse
{

class GAMGAgglomeration;

class manualGAMGProcAgglomeration
:
  public GAMGProcAgglomeration
{
  // Private data

    //- Per level the agglomeration map
    const List<Tuple2<label, List<labelList> > > procAgglomMaps_;

    //- Any allocated communicators
    DynamicList<label> comms_;

public:

  //- Runtime type information
  TYPE_NAME("manual");

  // Constructors

    //- Construct given agglomerator and controls
    manualGAMGProcAgglomeration
    (
      GAMGAgglomeration& agglom,
      const dictionary& controlDict
    );

    //- Disallow default bitwise copy construct
    manualGAMGProcAgglomeration
    (
      const manualGAMGProcAgglomeration&
    ) = delete;

    //- Disallow default bitwise assignment
    manualGAMGProcAgglomeration& operator=
    (
      const manualGAMGProcAgglomeration&
    ) = delete;

  //- Destructor
  virtual ~manualGAMGProcAgglomeration();

  // Member Functions

   //- Modify agglomeration. Return true if modified
    virtual bool agglomerate();

};

}  // namespace mousse
#endif
