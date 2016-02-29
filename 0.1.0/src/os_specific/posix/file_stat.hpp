#ifndef OS_SPECIFIC_POSIX_FILE_STAT_HPP_
#define OS_SPECIFIC_POSIX_FILE_STAT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileStat
// Description
//   Wrapper for stat() system call.
// Warning
//   on Linux (an maybe on others) a stat() of an nfs mounted (remote)
//   file does never timeout and cannot be interrupted!
//   So e.g. mousse::ping first and hope nfs is running.
// SourceFiles
//   file_stat.cpp
#include <sys/stat.h>
#include <sys/types.h>
#include "label.hpp"
#include "file_name.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class fileStat;
Istream& operator>>(Istream&, fileStat&);
Ostream& operator<<(Ostream&, const fileStat&);
class fileStat
{
  // Private data
    struct stat status_;
    bool isValid_;
public:
  // Constructors
    //- Empty constructor
    fileStat();
    //- Construct from components
    fileStat(const fileName& fName, const unsigned int maxTime=0);
    //- Construct from Istream
    fileStat(Istream&);
  // Member Functions
    // Access
      //- Raw status
      const struct stat& status() const
      {
        return status_;
      }
      //- Did constructor fail
      bool isValid() const
      {
        return isValid_;
      }
    // Check
      //- Compare two fileStats for same device
      bool sameDevice(const fileStat& stat2) const;
      //- Compare two fileStats for same Inode
      bool sameINode(const fileStat& stat2) const;
      //- Compare state against inode
      bool sameINode(const label iNode) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, fileStat&);
    friend Ostream& operator<<(Ostream&, const fileStat&);
};
}  // namespace mousse
#endif
