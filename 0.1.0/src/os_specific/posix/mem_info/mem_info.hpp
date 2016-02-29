#ifndef OS_SPECIFIC_POSIX_MEM_INFO_MEM_INFO_HPP_
#define OS_SPECIFIC_POSIX_MEM_INFO_MEM_INFO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::memInfo
// Description
//   Memory usage information for the process running this object.
// Note
//   Uses the information from /proc/\<pid\>/status
// SourceFiles
//   mem_info.cpp
#include "os_specific.hpp"
#include "posix.hpp"
#include "ifstream.hpp"
namespace mousse
{
class memInfo
{
  // Private data
    //- Peak memory used by the process (VmPeak in /proc/\<pid\>/status)
    int peak_;
    //- Memory used by the process (VmSize in /proc/\<pid\>/status)
    int size_;
    //- Resident set size of the process (VmRSS in /proc/\<pid\>/status)
    int rss_;
public:
  // Constructors
    //- Construct null
    memInfo();
  //- Destructor
  ~memInfo();
  // Member Functions
    //- Parse /proc/\<pid\>/status
    const memInfo& update();
    // Access
      //- Access the stored peak memory (VmPeak in /proc/\<pid\>/status)
      //  The value is stored from the previous update()
      int peak() const
      {
        return peak_;
      }
      //- Access the stored memory size (VmSize in /proc/\<pid\>/status)
      //  The value is stored from the previous update()
      int size() const
      {
        return size_;
      }
      //- Access the stored rss value (VmRSS in /proc/\<pid\>/status)
      //  The value is stored from the previous update()
      int rss() const
      {
        return rss_;
      }
      //- True if the memory information appears valid
      bool valid() const;
  // IOstream Operators
    //- Read peak/size/rss from stream
    friend Istream& operator>>(Istream&, memInfo&);
    //- Write peak/size/rss to stream
    friend Ostream& operator<<(Ostream&, const memInfo&);
};
}  // namespace mousse
#endif
