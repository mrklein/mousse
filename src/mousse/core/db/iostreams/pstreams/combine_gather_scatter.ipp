// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "opstream.hpp"
#include "ipstream.hpp"
#include "iostreams.hpp"
#include "contiguous.hpp"


namespace mousse {

// Member Functions 
template<class T, class CombineOp>
void Pstream::combineGather
(
  const List<UPstream::commsStruct>& comms,
  T& Value,
  const CombineOp& cop,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1) {
    // Get my communication order
    const commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Receive from my downstairs neighbours
    FOR_ALL(myComm.below(), belowI) {
      label belowID = myComm.below()[belowI];
      if (contiguous<T>()) {
        T value;
        UIPstream::read
        (
          UPstream::scheduled,
          belowID,
          reinterpret_cast<char*>(&value),
          sizeof(T),
          tag,
          comm
        );
        if (debug & 2) {
          Pout << " received from "
            << belowID << " data:" << value << endl;
        }
        cop(Value, value);
      } else {
        IPstream fromBelow
        {
          UPstream::scheduled,
          static_cast<int>(belowID),
          0,
          tag,
          comm
        };
        T value{fromBelow};
        if (debug & 2) {
          Pout << " received from "
            << belowID << " data:" << value << endl;
        }
        cop(Value, value);
      }
    }
    // Send up Value
    if (myComm.above() != -1) {
      if (debug & 2) {
        Pout << " sending to " << myComm.above()
          << " data:" << Value << endl;
      }
      if (contiguous<T>()) {
        UOPstream::write
        (
          UPstream::scheduled,
          static_cast<int>(myComm.above()),
          reinterpret_cast<const char*>(&Value),
          sizeof(T),
          tag,
          comm
        );
      } else {
        OPstream toAbove
        {
          UPstream::scheduled,
          static_cast<int>(myComm.above()),
          0,
          tag,
          comm
        };
        toAbove << Value;
      }
    }
  }
}


template <class T, class CombineOp>
void Pstream::combineGather
(
  T& Value,
  const CombineOp& cop,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum) {
    combineGather
    (
      UPstream::linearCommunication(comm),
      Value,
      cop,
      tag,
      comm
    );
  } else {
    combineGather
    (
      UPstream::treeCommunication(comm),
      Value,
      cop,
      tag,
      comm
    );
  }
}


template<class T>
void Pstream::combineScatter
(
  const List<UPstream::commsStruct>& comms,
  T& Value,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1) {
    // Get my communication order
    const UPstream::commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Reveive from up
    if (myComm.above() != -1) {
      if (contiguous<T>()) {
        UIPstream::read
        (
          UPstream::scheduled,
          myComm.above(),
          reinterpret_cast<char*>(&Value),
          sizeof(T),
          tag,
          comm
        );
      } else {
        IPstream fromAbove
        {
          UPstream::scheduled,
          static_cast<int>(myComm.above()),
          0,
          tag,
          comm
        };
        Value = T{fromAbove};
      }
      if (debug & 2) {
        Pout << " received from " << myComm.above() << " data:" << Value
          << endl;
      }
    }
    // Send to my downstairs neighbours
    FOR_ALL(myComm.below(), belowI) {
      label belowID = myComm.below()[belowI];
      if (debug & 2) {
        Pout << " sending to " << belowID << " data:" << Value << endl;
      }
      if (contiguous<T>()) {
        UOPstream::write
        (
          UPstream::scheduled,
          belowID,
          reinterpret_cast<const char*>(&Value),
          sizeof(T),
          tag,
          comm
        );
      } else {
        OPstream toBelow
        {
          UPstream::scheduled,
          static_cast<int>(belowID),
          0,
          tag,
          comm
        };
        toBelow << Value;
      }
    }
  }
}


template <class T>
void Pstream::combineScatter
(
  T& Value,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum) {
    combineScatter(UPstream::linearCommunication(comm), Value, tag, comm);
  } else {
    combineScatter(UPstream::treeCommunication(comm), Value, tag, comm);
  }
}


// Same thing but for whole list at a time
template<class T, class CombineOp>
void Pstream::listCombineGather
(
  const List<UPstream::commsStruct>& comms,
  List<T>& Values,
  const CombineOp& cop,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1) {
    // Get my communication order
    const commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Receive from my downstairs neighbours
    FOR_ALL(myComm.below(), belowI) {
      label belowID = myComm.below()[belowI];
      if (contiguous<T>()) {
        List<T> receivedValues(Values.size());
        UIPstream::read
        (
          UPstream::scheduled,
          belowID,
          reinterpret_cast<char*>(receivedValues.begin()),
          receivedValues.byteSize(),
          tag,
          comm
        );
        if (debug & 2) {
          Pout << " received from "
            << belowID << " data:" << receivedValues << endl;
        }
        FOR_ALL(Values, i) {
          cop(Values[i], receivedValues[i]);
        }
      } else {
        IPstream fromBelow
        {
          UPstream::scheduled,
          static_cast<int>(belowID),
          0,
          tag,
          comm
        };
        List<T> receivedValues{fromBelow};
        if (debug & 2) {
          Pout << " received from "
            << belowID << " data:" << receivedValues << endl;
        }
        FOR_ALL(Values, i) {
          cop(Values[i], receivedValues[i]);
        }
      }
    }
    // Send up Value
    if (myComm.above() != -1) {
      if (debug & 2) {
        Pout << " sending to " << myComm.above()
          << " data:" << Values << endl;
      }
      if (contiguous<T>()) {
        UOPstream::write
        (
          UPstream::scheduled,
          myComm.above(),
          reinterpret_cast<const char*>(Values.begin()),
          Values.byteSize(),
          tag,
          comm
        );
      } else {
        OPstream toAbove
        {
          UPstream::scheduled,
          static_cast<int>(myComm.above()),
          0,
          tag,
          comm
        };
        toAbove << Values;
      }
    }
  }
}


template<class T, class CombineOp>
void Pstream::listCombineGather
(
  List<T>& Values,
  const CombineOp& cop,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum) {
    listCombineGather
    (
      UPstream::linearCommunication(comm),
      Values,
      cop,
      tag,
      comm
    );
  } else {
    listCombineGather
    (
      UPstream::treeCommunication(comm),
      Values,
      cop,
      tag,
      comm
    );
  }
}


template<class T>
void Pstream::listCombineScatter
(
  const List<UPstream::commsStruct>& comms,
  List<T>& Values,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1) {
    // Get my communication order
    const UPstream::commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Reveive from up
    if (myComm.above() != -1) {
      if (contiguous<T>()) {
        UIPstream::read
        (
          UPstream::scheduled,
          myComm.above(),
          reinterpret_cast<char*>(Values.begin()),
          Values.byteSize(),
          tag,
          comm
        );
      } else {
        IPstream fromAbove
        {
          UPstream::scheduled,
          static_cast<int>(myComm.above()),
          0,
          tag,
          comm
        };
        fromAbove >> Values;
      }
      if (debug & 2) {
        Pout << " received from "
          << myComm.above() << " data:" << Values << endl;
      }
    }
    // Send to my downstairs neighbours
    FOR_ALL(myComm.below(), belowI) {
      label belowID = myComm.below()[belowI];
      if (debug & 2) {
        Pout << " sending to " << belowID << " data:" << Values << endl;
      }
      if (contiguous<T>()) {
        UOPstream::write
        (
          UPstream::scheduled,
          belowID,
          reinterpret_cast<const char*>(Values.begin()),
          Values.byteSize(),
          tag,
          comm
        );
      } else {
        OPstream toBelow
        {
          UPstream::scheduled,
          static_cast<int>(belowID),
          0,
          tag,
          comm
        };
        toBelow << Values;
      }
    }
  }
}


template <class T>
void Pstream::listCombineScatter
(
  List<T>& Values,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum) {
    listCombineScatter
    (
      UPstream::linearCommunication(comm),
      Values,
      tag,
      comm
    );
  } else {
    listCombineScatter
    (
      UPstream::treeCommunication(comm),
      Values,
      tag,
      comm
    );
  }
}


// Same thing but for sparse list (map)
template<class Container, class CombineOp>
void Pstream::mapCombineGather
(
  const List<UPstream::commsStruct>& comms,
  Container& Values,
  const CombineOp& cop,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1) {
    // Get my communication order
    const commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Receive from my downstairs neighbours
    FOR_ALL(myComm.below(), belowI) {
      label belowID = myComm.below()[belowI];
      IPstream fromBelow
      {
        UPstream::scheduled,
        static_cast<int>(belowID),
        0,
        tag,
        comm
      };
      Container receivedValues{fromBelow};
      if (debug & 2) {
        Pout << " received from "
          << belowID << " data:" << receivedValues << endl;
      }
      for
      (
        typename Container::const_iterator slaveIter =
          receivedValues.begin();
        slaveIter != receivedValues.end();
        ++slaveIter
      ) {
        typename Container::iterator
          masterIter = Values.find(slaveIter.key());
        if (masterIter != Values.end()) {
          cop(masterIter(), slaveIter());
        } else {
          Values.insert(slaveIter.key(), slaveIter());
        }
      }
    }
    // Send up Value
    if (myComm.above() != -1) {
      if (debug & 2) {
        Pout << " sending to " << myComm.above()
          << " data:" << Values << endl;
      }
      OPstream toAbove
      {
        UPstream::scheduled,
        static_cast<int>(myComm.above()),
        0,
        tag,
        comm
      };
      toAbove << Values;
    }
  }
}


template<class Container, class CombineOp>
void Pstream::mapCombineGather
(
  Container& Values,
  const CombineOp& cop,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum) {
    mapCombineGather
    (
      UPstream::linearCommunication(comm),
      Values,
      cop,
      tag,
      comm
    );
  } else {
    mapCombineGather
    (
      UPstream::treeCommunication(comm),
      Values,
      cop,
      tag,
      comm
    );
  }
}


template<class Container>
void Pstream::mapCombineScatter
(
  const List<UPstream::commsStruct>& comms,
  Container& Values,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) > 1) {
    // Get my communication order
    const UPstream::commsStruct& myComm = comms[UPstream::myProcNo(comm)];
    // Reveive from up
    if (myComm.above() != -1) {
      IPstream fromAbove
      {
        UPstream::scheduled,
        static_cast<int>(myComm.above()),
        0,
        tag,
        comm
      };
      fromAbove >> Values;
      if (debug & 2) {
        Pout << " received from "
          << myComm.above() << " data:" << Values << endl;
      }
    }
    // Send to my downstairs neighbours
    FOR_ALL(myComm.below(), belowI) {
      label belowID = myComm.below()[belowI];
      if (debug & 2) {
        Pout << " sending to " << belowID << " data:" << Values << endl;
      }
      OPstream toBelow
      {
        UPstream::scheduled,
        static_cast<int>(belowID),
        0,
        tag,
        comm
      };
      toBelow << Values;
    }
  }
}


template <class Container>
void Pstream::mapCombineScatter
(
  Container& Values,
  const int tag,
  const label comm
)
{
  if (UPstream::nProcs(comm) < UPstream::nProcsSimpleSum) {
    mapCombineScatter
    (
      UPstream::linearCommunication(comm),
      Values,
      tag,
      comm
    );
  } else {
    mapCombineScatter
    (
      UPstream::treeCommunication(comm),
      Values,
      tag,
      comm
    );
  }
}

}  // namespace mousse
