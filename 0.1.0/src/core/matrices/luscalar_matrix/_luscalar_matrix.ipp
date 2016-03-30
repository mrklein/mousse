// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_luscalar_matrix.hpp"


// Member Functions 
template<class Type>
void mousse::LUscalarMatrix::solve(Field<Type>& sourceSol) const
{
  if (Pstream::parRun()) {
    Field<Type> completeSourceSol{n()};
    if (Pstream::master(comm_)) {
      typename Field<Type>::subField
      (
        completeSourceSol,
        sourceSol.size()
      ).assign(sourceSol);
      for
      (
        int slave=Pstream::firstSlave();
        slave<=Pstream::lastSlave(comm_);
        slave++
      ) {
        IPstream::read
        (
          Pstream::scheduled,
          slave,
          reinterpret_cast<char*>
          (
            &(completeSourceSol[procOffsets_[slave]])
          ),
          (procOffsets_[slave+1]-procOffsets_[slave])*sizeof(Type),
          Pstream::msgType(),
          comm_
        );
      }
    } else {
      OPstream::write
      (
        Pstream::scheduled,
        Pstream::masterNo(),
        reinterpret_cast<const char*>(sourceSol.begin()),
        sourceSol.byteSize(),
        Pstream::msgType(),
        comm_
      );
    }
    if (Pstream::master(comm_)) {
      LUBacksubstitute(*this, pivotIndices_, completeSourceSol);
      sourceSol = typename Field<Type>::subField
      (
        completeSourceSol,
        sourceSol.size()
      );
      for
      (
        int slave=Pstream::firstSlave();
        slave<=Pstream::lastSlave(comm_);
        slave++
      ) {
        OPstream::write
        (
          Pstream::scheduled,
          slave,
          reinterpret_cast<const char*>
          (
            &(completeSourceSol[procOffsets_[slave]])
          ),
          (procOffsets_[slave + 1]-procOffsets_[slave])*sizeof(Type),
          Pstream::msgType(),
          comm_
        );
      }
    } else {
      IPstream::read
      (
        Pstream::scheduled,
        Pstream::masterNo(),
        reinterpret_cast<char*>(sourceSol.begin()),
        sourceSol.byteSize(),
        Pstream::msgType(),
        comm_
      );
    }
  } else {
    LUBacksubstitute(*this, pivotIndices_, sourceSol);
  }
}
