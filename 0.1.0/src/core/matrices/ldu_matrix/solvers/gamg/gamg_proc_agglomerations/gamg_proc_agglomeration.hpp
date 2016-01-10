// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GAMGProcAgglomeration
// Description
//   Processor agglomeration of GAMGAgglomerations.
// SourceFiles
//   gamg_proc_agglomeration.cpp

#ifndef gamg_proc_agglomeration_hpp_
#define gamg_proc_agglomeration_hpp_

#include "run_time_selection_tables.hpp"
#include "label_list.hpp"

namespace mousse
{

class GAMGAgglomeration;
class lduMesh;

class GAMGProcAgglomeration
{

protected:

  // Protected data

    //- Reference to agglomeration
    GAMGAgglomeration& agglom_;

  // Protected Member Functions

    //- Debug: write agglomeration info
    void printStats(Ostream& os, GAMGAgglomeration& agglom) const;

    //- Agglomerate a level. Return true if anything has changed
    bool agglomerate
    (
      const label fineLevelIndex,
      const labelList& procAgglomMap,
      const labelList& masterProcs,
      const List<label>& agglomProcIDs,
      const label procAgglomComm
    );

    //- Debug: calculate global cell-cells
    static labelListList globalCellCells(const lduMesh&);

public:

  //- Runtime type information
  TYPE_NAME("GAMGProcAgglomeration");

  // Declare run-time constructor selection tables

    //- Runtime selection table for pure geometric agglomerators
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      GAMGProcAgglomeration,
      GAMGAgglomeration,
      (
        GAMGAgglomeration& agglom,
        const dictionary& controlDict
      ),
      (
        agglom,
        controlDict
      )
    );

  // Constructors

    //- Construct given agglomerator and controls
    GAMGProcAgglomeration
    (
      GAMGAgglomeration& agglom,
      const dictionary& controlDict
    );

    //- Disallow default bitwise copy construct
    GAMGProcAgglomeration(const GAMGProcAgglomeration&) = delete;

    //- Disallow default bitwise assignment
    GAMGProcAgglomeration& operator=(const GAMGProcAgglomeration&) = delete;

  // Selectors

    //- Return the selected agglomerator
    static autoPtr<GAMGProcAgglomeration> New
    (
      const word& type,
      GAMGAgglomeration& agglom,
      const dictionary& controlDict
    );

  //- Destructor
  virtual ~GAMGProcAgglomeration();

  // Member Functions

    //- Modify agglomeration. Return true if modified
    virtual bool agglomerate() = 0;

};

}  // namespace mousse
#endif
