// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::globalIndex
// Description
//   Calculates a unique integer (label so might not have enough room - 2G max)
//   for processor + local index. E.g.
//   globalIndex globalFaces(mesh.nFaces());
//   label globalFaceI = globalFaces.toGlobal(faceI);
// SourceFiles
//   global_index_i.hpp
//   global_index.cpp
//   global_index_templates.cpp

#ifndef global_index_hpp_
#define global_index_hpp_

#include "pstream.hpp"
#include "list_ops.hpp"

namespace mousse
{

// Forward declaration of friend functions and operators
class globalIndex;
Istream& operator>>(Istream& is, globalIndex& gi);
Ostream& operator<<(Ostream& os, const globalIndex& gi);

class globalIndex
{
  // Private data

    //- Start of procI. Size is nProcs()+1. (so like CompactListList)
    labelList offsets_;

public:

  // Constructors

    //- Construct null
    inline globalIndex();

    //- Construct from local max size. Does communication with default
    //  communicator and message tag.
    globalIndex(const label localSize);

    //- Construct from local max size. Does communication with given
    //  communicator and message tag
    globalIndex
    (
      const label localSize,
      const int tag,
      const label comm,
      const bool parallel     // use parallel comms
    );

    //- Construct from components
    globalIndex(const labelList& offsets);

    //- Construct from components
    inline globalIndex(const Xfer<labelList>& offsets);

    //- Construct from Istream
    globalIndex(Istream& is);

  // Member Functions

    // Edit

      //- Change after construction
      inline labelList& offsets();

    // Queries relating to my processor (using world communicator)

      //- My local size
      inline label localSize() const;

      //- From local to global
      inline label toGlobal(const label i) const;

      //- Is on local processor
      inline bool isLocal(const label i) const;

      //- From global to local on current processor.
      //  FatalError if not on local processor.
      inline label toLocal(const label i) const;

    // Global queries

      //- Global sum of localSizes
      inline label size() const;

      //- Size of procI data
      inline label localSize(const label procI) const;

      //- From local to global on procI
      inline label toGlobal(const label procI, const label i) const;

      //- Is on processor procI
      inline bool isLocal(const label procI, const label i) const;

      //- From global to local on procI
      inline label toLocal(const label procI, const label i) const;

      //- Which processor does global come from? Binary search.
      inline label whichProcID(const label i) const;

      //- Start of procI data
      inline label offset(const label procI) const;

    // Other

      //- Collect data in processor order on master (== procIDs[0]).
      //  Needs offsets only on master.
      template<class Type>
      static void gather
      (
        const labelUList& offsets,
        const label comm,
        const labelList& procIDs,
        const UList<Type>& fld,
        List<Type>& allFld,
        const int tag = UPstream::msgType(),
        const Pstream::commsTypes commsType=Pstream::nonBlocking
      );

      //- Collect data in processor order on master (== procIDs[0]).
      //  Needs offsets only on master.
      template<class Type>
      void gather
      (
        const label comm,
        const labelList& procIDs,
        const UList<Type>& fld,
        List<Type>& allFld,
        const int tag = UPstream::msgType(),
        const Pstream::commsTypes commsType=Pstream::nonBlocking
      ) const
      {
        gather(offsets_, comm, procIDs, fld, allFld, tag, commsType);
      }

      //- Inplace collect data in processor order on master
      //  (== procIDs[0]). Needs offsets only on master.
      template<class Type>
      static void gather
      (
        const labelUList& offsets,
        const label comm,
        const labelList& procIDs,
        List<Type>& fld,
        const int tag = UPstream::msgType(),
        const Pstream::commsTypes commsType=Pstream::nonBlocking
      );

      //- Inplace collect data in processor order on master
      //  (== procIDs[0]). Needs offsets only on master.
      template<class Type>
      void gather
      (
        const label comm,
        const labelList& procIDs,
        List<Type>& fld,
        const int tag = UPstream::msgType(),
        const Pstream::commsTypes commsType=Pstream::nonBlocking
      ) const
      {
        gather(offsets_, comm, procIDs, fld, tag, commsType);
      }

      //- Distribute data in processor order. Requires fld to be sized!
      template<class Type>
      static void scatter
      (
        const labelUList& offsets,
        const label comm,
        const labelList& procIDs,
        const UList<Type>& allFld,
        UList<Type>& fld,
        const int tag = UPstream::msgType(),
        const Pstream::commsTypes commsType=Pstream::nonBlocking
      );

      //- Distribute data in processor order. Requires fld to be sized!
      template<class Type>
      void scatter
      (
        const label comm,
        const labelList& procIDs,
        const UList<Type>& allFld,
        UList<Type>& fld,
        const int tag = UPstream::msgType(),
        const Pstream::commsTypes commsType=Pstream::nonBlocking
      ) const
      {
        scatter(offsets_, comm, procIDs, allFld, fld, tag, commsType);
      }

  // IOstream Operators

    friend Istream& operator>>(Istream& is, globalIndex& gi);

    friend Ostream& operator<<(Ostream& os, const globalIndex& gi);
};

}  // namespace mousse

#ifdef NoRepository
#   include "global_index_templates.cpp"
#endif

// Constructors 
mousse::globalIndex::globalIndex()
{}


mousse::globalIndex::globalIndex(const Xfer<labelList>& offsets)
:
  offsets_{offsets}
{}


// Member Functions 
inline mousse::labelList& mousse::globalIndex::offsets()
{
  return offsets_;
}


inline mousse::label mousse::globalIndex::offset(const label procI) const
{
  return offsets_[procI];
}


inline mousse::label mousse::globalIndex::localSize(const label procI) const
{
  return offsets_[procI+1] - offsets_[procI];
}


inline mousse::label mousse::globalIndex::localSize() const
{
  return localSize(Pstream::myProcNo());
}


inline mousse::label mousse::globalIndex::size() const
{
  return offsets_.last();
}


inline mousse::label mousse::globalIndex::toGlobal
(
  const label procI,
  const label i
) const
{
  return i + offsets_[procI];
}


inline mousse::label mousse::globalIndex::toGlobal(const label i) const
{
  return toGlobal(Pstream::myProcNo(), i);
}


//- Is on local processor
inline bool mousse::globalIndex::isLocal(const label procI, const label i) const
{
  return i >= offsets_[procI] && i < offsets_[procI+1];
}


inline bool mousse::globalIndex::isLocal(const label i) const
{
  return isLocal(Pstream::myProcNo(), i);
}


inline mousse::label mousse::globalIndex::toLocal(const label procI, const label i)
const
{
  label localI = i - offsets_[procI];
  if (localI < 0 || i >= offsets_[procI+1])
  {
    FATAL_ERROR_IN("globalIndex::toLocal(const label, const label)")
      << "Global " << i << " does not belong on processor "
      << procI << endl << "Offsets:" << offsets_
      << abort(FatalError);
  }
  return localI;
}


inline mousse::label mousse::globalIndex::toLocal(const label i) const
{
  return toLocal(Pstream::myProcNo(), i);
}


inline mousse::label mousse::globalIndex::whichProcID(const label i) const
{
  if (i < 0 || i >= size())
  {
    FATAL_ERROR_IN("globalIndex::whichProcID(const label)")
      << "Global " << i << " does not belong on any processor."
      << " Offsets:" << offsets_
      << abort(FatalError);
  }
  return findLower(offsets_, i+1);
}

#endif
