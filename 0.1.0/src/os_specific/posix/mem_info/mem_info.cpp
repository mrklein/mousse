// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mem_info.hpp"


// Constructors 
mousse::memInfo::memInfo()
:
  peak_{-1},
  size_{-1},
  rss_{-1}
{
  update();
}


// Destructor 
mousse::memInfo::~memInfo()
{}


// Member Functions 
const mousse::memInfo& mousse::memInfo::update()
{
  // reset to invalid values first
  peak_ = size_ = rss_ = -1;
  IFstream is{"/proc/" + name(pid()) + "/status"};
  while (is.good()) {
    string line;
    is.getLine(line);
    char tag[32];
    int value;
    if (sscanf(line.c_str(), "%30s %d", tag, &value) == 2) {
      if (!strcmp(tag, "VmPeak:")) {
        peak_ = value;
      } else if (!strcmp(tag, "VmSize:")) {
        size_ = value;
      } else if (!strcmp(tag, "VmRSS:")) {
        rss_ = value;
      }
    }
  }
  return *this;
}


bool mousse::memInfo::valid() const
{
  return peak_ != -1;
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, memInfo& m)
{
  is.readBegin("memInfo");
  is >> m.peak_ >> m.size_ >> m.rss_;
  is.readEnd("memInfo");
  // Check state of Istream
  is.check
  (
    "mousse::Istream& mousse::operator>>(mousse::Istream&, mousse::memInfo&)"
  );
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const memInfo& m)
{
  os << token::BEGIN_LIST
    << m.peak_ << token::SPACE << m.size_ << token::SPACE << m.rss_
    << token::END_LIST;
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::memInfo&)"
  );
  return os;
}

