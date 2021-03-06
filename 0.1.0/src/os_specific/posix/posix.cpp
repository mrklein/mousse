// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifdef solarisGcc
# define _SYS_VNODE_H
#endif
#include "os_specific.hpp"
#include "posix.hpp"
#include "mousse_version.hpp"
#include "file_name.hpp"
#include "file_stat.hpp"
#include "timer.hpp"
#include "ifstream.hpp"
#include "dynamic_list.hpp"
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <dlfcn.h>
#if defined(darwin64)
#include <mach-o/dyld.h>
#else
#include <link.h>
#endif
#include <netinet/in.h>
#ifdef USE_RANDOM
#   include <climits>
#   if INT_MAX    != 2147483647
#       error "INT_MAX    != 2147483647"
#       error "The random number generator may not work!"
#   endif
#endif


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(POSIX, 0);

}


pid_t mousse::pid()
{
  return ::getpid();
}


pid_t mousse::ppid()
{
  return ::getppid();
}


pid_t mousse::pgid()
{
  return ::getpgrp();
}


bool mousse::env(const word& envName)
{
  return ::getenv(envName.c_str()) != NULL;
}


mousse::string mousse::getEnv(const word& envName)
{
  char* env = ::getenv(envName.c_str());
  if (env) {
    return string(env);
  } else {
    // Return null-constructed string rather than string::null
    // to avoid cyclic dependencies in the construction of globals
    return string();
  }
}


bool mousse::setEnv
(
  const word& envName,
  const std::string& value,
  const bool overwrite
)
{
  return setenv(envName.c_str(), value.c_str(), overwrite) == 0;
}


mousse::string mousse::hostName(bool full)
{
  char buf[128];
  ::gethostname(buf, sizeof(buf));
  // implementation as per hostname from net-tools
  if (full) {
    struct hostent *hp = ::gethostbyname(buf);
    if (hp) {
      return hp->h_name;
    }
  }
  return buf;
}


mousse::string mousse::domainName()
{
  char buf[128];
  ::gethostname(buf, sizeof(buf));
  // implementation as per hostname from net-tools
  struct hostent *hp = ::gethostbyname(buf);
  if (hp) {
    char *p = ::strchr(hp->h_name, '.');
    if (p) {
      ++p;
      return p;
    }
  }
  return string::null;
}


mousse::string mousse::userName()
{
  struct passwd* pw = ::getpwuid(::getuid());
  if (pw != NULL) {
    return pw->pw_name;
  } else {
    return string::null;
  }
}


bool mousse::isAdministrator()
{
  return (::geteuid() == 0);
}


// use $HOME environment variable or passwd info
mousse::fileName mousse::home()
{
  char* env = ::getenv("HOME");
  if (env != NULL) {
    return fileName(env);
  } else {
    struct passwd* pw = ::getpwuid(getuid());
    if (pw != NULL) {
      return pw->pw_dir;
    } else {
      return fileName::null;
    }
  }
}


mousse::fileName mousse::home(const string& userName)
{
  struct passwd* pw;
  if (userName.size()) {
    pw = ::getpwnam(userName.c_str());
  } else {
    char* env = ::getenv("HOME");
    if (env != NULL) {
      return fileName(env);
    }
    pw = ::getpwuid(::getuid());
  }
  if (pw != NULL) {
    return pw->pw_dir;
  } else {
    return fileName::null;
  }
}


mousse::fileName mousse::cwd()
{
  char buf[256];
  if (::getcwd(buf, sizeof(buf))) {
    return buf;
  } else {
    FATAL_ERROR_IN("mousse::cwd()")
      << "Couldn't get the current working directory"
      << exit(FatalError);
    return fileName::null;
  }
}


bool mousse::chDir(const fileName& dir)
{
  return ::chdir(dir.c_str()) == 0;
}


mousse::fileNameList mousse::findEtcFiles
(
  const fileName& name,
  bool mandatory,
  bool findFirst
)
{
  fileNameList results;
  // Search for user files in
  // * ~/.mousse/VERSION
  // * ~/.mousse
  //
  fileName searchDir = home()/".mousse";
  if (isDir(searchDir)) {
    fileName fullName = searchDir/mousse_version/name;
    if (isFile(fullName)) {
      results.append(fullName);
      if (findFirst) {
        return results;
      }
    }
    fullName = searchDir/name;
    if (isFile(fullName)) {
      results.append(fullName);
      if (findFirst) {
        return results;
      }
    }
  }
  // Search for group (site) files in
  // * $WM_PROJECT_SITE/VERSION
  // * $WM_PROJECT_SITE
  //
  searchDir = getEnv("WM_PROJECT_SITE");
  if (searchDir.size()) {
    if (isDir(searchDir)) {
      fileName fullName = searchDir/mousse_version/name;
      if (isFile(fullName)) {
        results.append(fullName);
        if (findFirst) {
          return results;
        }
      }
      fullName = searchDir/name;
      if (isFile(fullName)) {
        results.append(fullName);
        if (findFirst) {
          return results;
        }
      }
    }
  } else {
    // OR search for group (site) files in
    // * $WM_PROJECT_INST_DIR/site/VERSION
    // * $WM_PROJECT_INST_DIR/site
    //
    searchDir = getEnv("WM_PROJECT_INST_DIR");
    if (isDir(searchDir)) {
      fileName fullName = searchDir/"site"/mousse_version/name;
      if (isFile(fullName)) {
        results.append(fullName);
        if (findFirst) {
          return results;
        }
      }
      fullName = searchDir/"site"/name;
      if (isFile(fullName)) {
        results.append(fullName);
        if (findFirst) {
          return results;
        }
      }
    }
  }
  // Search for other (shipped) files in
  // * $WM_PROJECT_DIR/etc
  //
  searchDir = getEnv("WM_PROJECT_DIR");
  if (isDir(searchDir)) {
    fileName fullName = searchDir/"etc"/name;
    if (isFile(fullName)) {
      results.append(fullName);
      if (findFirst) {
        return results;
      }
    }
  }
  // Not found
  if (results.empty()) {
    // Abort if the file is mandatory, otherwise return null
    if (mandatory) {
      std::cerr
        << "--> FOAM FATAL ERROR in mousse::findEtcFiles() :"
           " could not find mandatory file\n    '"
        << name.c_str() << "'\n\n" << std::endl;
      ::exit(1);
    }
  }
  // Return list of matching paths or empty list if none found
  return results;
}


mousse::fileName mousse::findEtcFile(const fileName& name, bool mandatory)
{
  fileNameList results(findEtcFiles(name, mandatory, true));
  if (results.size()) {
    return results[0];
  } else {
    return fileName();
  }
}


bool mousse::mkDir(const fileName& pathName, mode_t mode)
{
  // empty names are meaningless
  if (pathName.empty()) {
    return false;
  }
  // Construct instance path directory if does not exist
  if (::mkdir(pathName.c_str(), mode) == 0) {
    // Directory made OK so return true
    return true;
  } else {
    switch (errno) {
      case EPERM:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "The filesystem containing " << pathName
          << " does not support the creation of directories."
          << exit(FatalError);
        return false;
      }
      case EEXIST:
      {
        // Directory already exists so simply return true
        return true;
      }
      case EFAULT:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "" << pathName
          << " points outside your accessible address space."
          << exit(FatalError);
        return false;
      }
      case EACCES:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "The parent directory does not allow write "
           "permission to the process,"<< nl
          << "or one of the directories in " << pathName
          << " did not allow search (execute) permission."
          << exit(FatalError);
        return false;
      }
      case ENAMETOOLONG:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "" << pathName << " is too long."
          << exit(FatalError);
        return false;
      }
      case ENOENT:
      {
        // Part of the path does not exist so try to create it
        if (pathName.path().size() && mkDir(pathName.path(), mode))
        {
          return mkDir(pathName, mode);
        }
        else
        {
          FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
            << "Couldn't create directory " << pathName
            << exit(FatalError);
          return false;
        }
      }
      case ENOTDIR:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "A component used as a directory in " << pathName
          << " is not, in fact, a directory."
          << exit(FatalError);
        return false;
      }
      case ENOMEM:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "Insufficient kernel memory was available to make "
           "directory " << pathName << '.'
          << exit(FatalError);
        return false;
      }
      case EROFS:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "" << pathName
          << " refers to a file on a read-only filesystem."
          << exit(FatalError);
        return false;
      }
      case ELOOP:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "Too many symbolic links were encountered in resolving "
          << pathName << '.'
          << exit(FatalError);
        return false;
      }
      case ENOSPC:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "The device containing " << pathName
          << " has no room for the new directory or "
          << "the user's disk quota is exhausted."
          << exit(FatalError);
        return false;
      }
      default:
      {
        FATAL_ERROR_IN("mousse::mkDir(const fileName&, mode_t)")
          << "Couldn't create directory " << pathName
          << exit(FatalError);
        return false;
      }
    }
  }
}


// Set the file mode
bool mousse::chMod(const fileName& name, const mode_t m)
{
  return ::chmod(name.c_str(), m) == 0;
}


// Return the file mode
mode_t mousse::mode(const fileName& name)
{
  fileStat fileStatus(name);
  if (fileStatus.isValid()) {
    return fileStatus.status().st_mode;
  } else {
    return 0;
  }
}


// Return the file type: FILE or DIRECTORY
mousse::fileName::Type mousse::type(const fileName& name)
{
  mode_t m = mode(name);
  if (S_ISREG(m)) {
    return fileName::FILE;
  } else if (S_ISDIR(m)) {
    return fileName::DIRECTORY;
  } else {
    return fileName::UNDEFINED;
  }
}


// Does the name exist in the filing system?
bool mousse::exists(const fileName& name, const bool checkGzip)
{
  return mode(name) || isFile(name, checkGzip);
}


// Does the directory exist?
bool mousse::isDir(const fileName& name)
{
  return S_ISDIR(mode(name));
}


// Does the file exist?
bool mousse::isFile(const fileName& name, const bool checkGzip)
{
  return S_ISREG(mode(name)) || (checkGzip && S_ISREG(mode(name + ".gz")));
}


// Return size of file
off_t mousse::fileSize(const fileName& name)
{
  fileStat fileStatus(name);
  if (fileStatus.isValid()) {
    return fileStatus.status().st_size;
  } else {
    return -1;
  }
}


// Return time of last file modification
time_t mousse::lastModified(const fileName& name)
{
  fileStat fileStatus(name);
  if (fileStatus.isValid()) {
    return fileStatus.status().st_mtime;
  } else {
    return 0;
  }
}


// Read a directory and return the entries as a string list
mousse::fileNameList mousse::readDir
(
  const fileName& directory,
  const fileName::Type type,
  const bool filtergz
)
{
  // Initial filename list size
  // also used as increment if initial size found to be insufficient
  static const int maxNnames = 100;
  if (POSIX::debug) {
    Info << "readDir(const fileName&, const fileType, const bool filtergz)"
      << " : reading directory " << directory << endl;
  }
  // Setup empty string list MAXTVALUES long
  fileNameList dirEntries{maxNnames};
  // Pointers to the directory entries
  DIR *source;
  struct dirent *list;
  // Temporary variables and counters
  label nEntries = 0;
  // Attempt to open directory and set the structure pointer
  if ((source = ::opendir(directory.c_str())) == NULL) {
    dirEntries.setSize(0);
    if (POSIX::debug) {
      Info << "readDir(const fileName&, const fileType, "
         "const bool filtergz) : cannot open directory "
        << directory << endl;
    }
  } else {
    // Read and parse all the entries in the directory
    while ((list = ::readdir(source)) != NULL) {
      fileName fName{list->d_name};
      // ignore files begining with ., i.e. '.', '..' and '.*'
      if (fName.size() && fName[0] != '.') {
        word fExt = fName.ext();
        if ((type == fileName::DIRECTORY)
            || (type == fileName::FILE
                && fName[fName.size()-1] != '~'
                && fExt != "bak"
                && fExt != "BAK"
                && fExt != "old"
                && fExt != "save")) {
          if ((directory/fName).type() == type) {
            if (nEntries >= dirEntries.size()) {
              dirEntries.setSize(dirEntries.size() + maxNnames);
            }
            if (filtergz && fExt == "gz") {
              dirEntries[nEntries++] = fName.lessExt();
            } else {
              dirEntries[nEntries++] = fName;
            }
          }
        }
      }
    }
    // Reset the length of the entries list
    dirEntries.setSize(nEntries);
    ::closedir(source);
  }
  return dirEntries;
}


// Copy, recursively if necessary, the source to the destination
bool mousse::cp(const fileName& src, const fileName& dest)
{
  // Make sure source exists.
  if (!exists(src)) {
    return false;
  }
  fileName destFile{dest};
  // Check type of source file.
  if (src.type() == fileName::FILE) {
    // If dest is a directory, create the destination file name.
    if (destFile.type() == fileName::DIRECTORY) {
      destFile = destFile/src.name();
    }
    // Make sure the destination directory exists.
    if (!isDir(destFile.path()) && !mkDir(destFile.path())) {
      return false;
    }
    // Open and check streams.
    std::ifstream srcStream{src.c_str()};
    if (!srcStream) {
      return false;
    }
    std::ofstream destStream{destFile.c_str()};
    if (!destStream) {
      return false;
    }
    // Copy character data.
    char ch;
    while (srcStream.get(ch)) {
      destStream.put(ch);
    }
    // Final check.
    if (!srcStream.eof() || !destStream) {
      return false;
    }
  } else if (src.type() == fileName::DIRECTORY) {
    // If dest is a directory, create the destination file name.
    if (destFile.type() == fileName::DIRECTORY) {
      destFile = destFile/src.component(src.components().size() -1);
    }
    // Make sure the destination directory exists.
    if (!isDir(destFile) && !mkDir(destFile)) {
      return false;
    }
    // Copy files
    fileNameList contents = readDir(src, fileName::FILE, false);
    FOR_ALL(contents, i) {
      if (POSIX::debug) {
        Info
          << "Copying : " << src/contents[i]
          << " to " << destFile/contents[i] << endl;
      }
      // File to file.
      cp(src/contents[i], destFile/contents[i]);
    }
    // Copy sub directories.
    fileNameList subdirs = readDir(src, fileName::DIRECTORY);
    FOR_ALL(subdirs, i) {
      if (POSIX::debug) {
        Info << "Copying : " << src/subdirs[i]
          << " to " << destFile << endl;
      }
      // Dir to Dir.
      cp(src/subdirs[i], destFile);
    }
  }
  return true;
}


// Create a softlink. dst should not exist. Returns true if successful.
bool mousse::ln(const fileName& src, const fileName& dst)
{
  if (POSIX::debug) {
    Info << "Create softlink from : " << src << " to " << dst
      << endl;
  }
  if (exists(dst)) {
    WARNING_IN("ln(const fileName&, const fileName&)")
      << "destination " << dst << " already exists. Not linking."
      << endl;
    return false;
  }
  if (src.isAbsolute() && !exists(src)) {
    WARNING_IN("ln(const fileName&, const fileName&)")
      << "source " << src << " does not exist." << endl;
    return false;
  }
  if (::symlink(src.c_str(), dst.c_str()) == 0) {
    return true;
  } else {
    WARNING_IN("ln(const fileName&, const fileName&)")
      << "symlink from " << src << " to " << dst << " failed." << endl;
    return false;
  }
}


// Rename srcFile dstFile
bool mousse::mv(const fileName& src, const fileName& dst)
{
  if (POSIX::debug) {
    Info << "Move : " << src << " to " << dst << endl;
  }
  if (dst.type() == fileName::DIRECTORY && src.type() != fileName::DIRECTORY) {
    const fileName dstName(dst/src.name());
    return ::rename(src.c_str(), dstName.c_str()) == 0;
  } else {
    return ::rename(src.c_str(), dst.c_str()) == 0;
  }
}


//- Rename to a corresponding backup file
//  If the backup file already exists, attempt with "01" .. "99" index
bool mousse::mvBak(const fileName& src, const std::string& ext)
{
  if (POSIX::debug) {
    Info << "mvBak : " << src << " to extension " << ext << endl;
  }
  if (exists(src, false)) {
    const int maxIndex = 99;
    char index[3];
    for (int n = 0; n <= maxIndex; n++) {
      fileName dstName(src + "." + ext);
      if (n) {
        sprintf(index, "%02d", n);
        dstName += index;
      }
      // avoid overwriting existing files, except for the last
      // possible index where we have no choice
      if (!exists(dstName, false) || n == maxIndex) {
        return ::rename(src.c_str(), dstName.c_str()) == 0;
      }
    }
  }
  // fall-through: nothing to do
  return false;
}


// Remove a file, returning true if successful otherwise false
bool mousse::rm(const fileName& file)
{
  if (POSIX::debug) {
    Info<< "Removing : " << file << endl;
  }
  // Try returning plain file name; if not there, try with .gz
  if (remove(file.c_str()) == 0) {
    return true;
  } else {
    return ::remove(string(file + ".gz").c_str()) == 0;
  }
}


// Remove a dirctory and its contents
bool mousse::rmDir(const fileName& directory)
{
  if (POSIX::debug) {
    Info << "rmDir(const fileName&) : "
      << "removing directory " << directory << endl;
  }
  // Pointers to the directory entries
  DIR *source;
  struct dirent *list;
  // Attempt to open directory and set the structure pointer
  if ((source = ::opendir(directory.c_str())) == NULL) {
    WARNING_IN("rmDir(const fileName&)")
      << "cannot open directory " << directory << endl;
    return false;
  } else {
    // Read and parse all the entries in the directory
    while ((list = ::readdir(source)) != NULL) {
      fileName fName{list->d_name};
      if (fName != "." && fName != "..") {
        fileName path = directory/fName;
        if (path.type() == fileName::DIRECTORY) {
          if (!rmDir(path)) {
            WARNING_IN("rmDir(const fileName&)")
              << "failed to remove directory " << fName
              << " while removing directory " << directory
              << endl;
            ::closedir(source);
            return false;
          }
        } else {
          if (!rm(path)) {
            WARNING_IN("rmDir(const fileName&)")
              << "failed to remove file " << fName
              << " while removing directory " << directory
              << endl;
            ::closedir(source);
            return false;
          }
        }
      }
    }
    if (!rm(directory)) {
      WARNING_IN("rmDir(const fileName&)")
        << "failed to remove directory " << directory << endl;
      ::closedir(source);
      return false;
    }
    ::closedir(source);
    return true;
  }
}


unsigned int mousse::sleep(const unsigned int s)
{
  return ::sleep(s);
}


void mousse::fdClose(const int fd)
{
  if (close(fd) != 0) {
    FATAL_ERROR_IN
    (
      "fdClose(const int fd)"
    )
    << "close error on " << fd << endl
    << abort(FatalError);
  }
}


bool mousse::ping
(
  const string& destName,
  const label destPort,
  const label timeOut
)
{
  struct hostent *hostPtr;
  volatile int sockfd;
  struct sockaddr_in destAddr;      // will hold the destination addr
  u_int addr;
  if ((hostPtr = ::gethostbyname(destName.c_str())) == NULL) {
    FATAL_ERROR_IN
    (
      "mousse::ping(const string&, ...)"
    )
    << "gethostbyname error " << h_errno << " for host " << destName
    << abort(FatalError);
  }
  // Get first of the SLL of addresses
  addr = (reinterpret_cast<struct in_addr*>(*(hostPtr->h_addr_list)))->s_addr;
  // Allocate socket
  sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    FATAL_ERROR_IN
    (
      "mousse::ping(const string&, const label)"
    )
    << "socket error"
    << abort(FatalError);
  }
  // Fill sockaddr_in structure with dest address and port
  memset(reinterpret_cast<char *>(&destAddr), '\0', sizeof(destAddr));
  destAddr.sin_family = AF_INET;
  destAddr.sin_port = htons(ushort(destPort));
  destAddr.sin_addr.s_addr = addr;
  timer myTimer(timeOut);
  if (timedOut(myTimer)) {
    // Setjmp from timer jumps back to here
    fdClose(sockfd);
    return false;
  }
  if
  (
    ::connect
    (
      sockfd,
      reinterpret_cast<struct sockaddr*>(&destAddr),
      sizeof(struct sockaddr)
    ) != 0
  ) {
    // Connection refused. Check if network was actually used or not.
    int connectErr = errno;
    fdClose(sockfd);
    if (connectErr == ECONNREFUSED) {
      return true;
    }
    //perror("connect");
    return false;
  }
  fdClose(sockfd);
  return true;
}


bool mousse::ping(const string& hostname, const label timeOut)
{
  return ping(hostname, 222, timeOut) || ping(hostname, 22, timeOut);
}


int mousse::system(const std::string& command)
{
  return ::system(command.c_str());
}


void* mousse::dlOpen(const fileName& lib, const bool check)
{
  if (POSIX::debug) {
    std::cout<< "dlOpen(const fileName&)"
      << " : dlopen of " << lib << std::endl;
  }
  void* handle = ::dlopen(lib.c_str(), RTLD_LAZY|RTLD_GLOBAL);
#if defined(darwin64)
  if (!handle && lib.ext() == "so") {
    fileName fallback_lib = lib.lessExt() + ".dylib";
    if (check) {
      WARNING_IN("dlOpen(const fileName&, const bool)")
        << "dlOpen: using fallback library name "
        << fallback_lib
        << endl;
    }
    handle = ::dlopen(fallback_lib.c_str(), RTLD_LAZY|RTLD_GLOBAL);
  }
#endif
  if (!handle && check) {
    WARNING_IN("dlOpen(const fileName&, const bool)")
      << "dlopen error : " << ::dlerror()
      << endl;
  }
  if (POSIX::debug) {
    std::cout
      << "dlOpen(const fileName&)"
      << " : dlopen of " << lib
      << " handle " << handle << std::endl;
  }
  return handle;
}


bool mousse::dlClose(void* handle)
{
  if (POSIX::debug) {
    std::cout
      << "dlClose(void*)"
      << " : dlclose of handle " << handle << std::endl;
  }
  return ::dlclose(handle) == 0;
}


void* mousse::dlSym(void* handle, const std::string& symbol)
{
  if (POSIX::debug) {
    std::cout
      << "dlSym(void*, const std::string&)"
      << " : dlsym of " << symbol << std::endl;
  }
  // clear any old errors - see manpage dlopen
  (void) ::dlerror();
  // get address of symbol
  void* fun = ::dlsym(handle, symbol.c_str());
  // find error (if any)
  char *error = ::dlerror();
  if (error) {
    WARNING_IN("dlSym(void*, const std::string&)")
      << "Cannot lookup symbol " << symbol << " : " << error
      << endl;
  }
  return fun;
}


bool mousse::dlSymFound(void* handle, const std::string& symbol)
{
  if (handle && !symbol.empty()) {
    if (POSIX::debug) {
      std::cout
        << "dlSymFound(void*, const std::string&)"
        << " : dlsym of " << symbol << std::endl;
    }
    // clear any old errors - see manpage dlopen
    (void) ::dlerror();
    // get address of symbol
    (void) ::dlsym(handle, symbol.c_str());
    // symbol can be found if there was no error
    return !::dlerror();
  } else {
    return false;
  }
}


#if defined(darwin64)
mousse::fileNameList mousse::dlLoaded()
{
  DynamicList<fileName> libs;
  for(uint32_t cnt = 0; cnt < _dyld_image_count(); ++cnt) {
    libs.append(_dyld_get_image_name(cnt));
  }
  if (POSIX::debug) {
    std::cout
      << "dlLoaded()"
      << " : determined loaded libraries :" << libs.size() << std::endl;
  }
  return libs;
}
#else
static int collectLibsCallback
(
  struct dl_phdr_info *info,
  size_t size,
  void *data
)
{
  mousse::DynamicList<mousse::fileName>* ptr =
    reinterpret_cast<mousse::DynamicList<mousse::fileName>*>(data);
  ptr->append(info->dlpi_name);
  return 0;
}
mousse::fileNameList mousse::dlLoaded()
{
  DynamicList<fileName> libs;
  dl_iterate_phdr(collectLibsCallback, &libs);
  if (POSIX::debug) {
    std::cout
      << "dlLoaded()"
      << " : determined loaded libraries :" << libs.size() << std::endl;
  }
  return libs;
}
#endif


void mousse::osRandomSeed(const label seed)
{
#ifdef USE_RANDOM
  srandom((unsigned int)seed);
#else
  srand48(seed);
#endif
}


mousse::label mousse::osRandomInteger()
{
#ifdef USE_RANDOM
  return random();
#else
  return lrand48();
#endif
}


mousse::scalar mousse::osRandomDouble()
{
#ifdef USE_RANDOM
  return (scalar)random()/INT_MAX;
#else
  return drand48();
#endif
}

