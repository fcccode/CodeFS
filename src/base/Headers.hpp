#ifndef __ETERNAL_TCP_HEADERS__
#define __ETERNAL_TCP_HEADERS__

#if __FreeBSD__
#define _WITH_GETLINE
#endif

#include <sys/xattr.h>

#if __APPLE__
#include <sys/attr.h>
#include <sys/ucred.h>
#include <sys/vnode.h>
#include <util.h>
#elif __FreeBSD__
#include <libutil.h>
#elif __NetBSD__  // do not need pty.h on NetBSD
#else
#include <pty.h>
#endif

#if __NetBSD__
#include <util.h>
#endif

#ifndef __APPLE__
extern "C" {
#include <ulockmgr.h>
}
#endif

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <pwd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/xattr.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <array>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <streambuf>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define FUSE_USE_VERSION 26
#include "fuse.h"

#include <gflags/gflags.h>

#include "easylogging++.h"

#include "base64.hpp"
#include "json.hpp"
#include "msgpack.hpp"
#include "sole.hpp"
#include "zmq.hpp"
#include "zmq_addon.hpp"

#ifndef _LIBCPP_OPTIONAL
#include "optional.hpp"
using namespace std::experimental;
#endif

#include "CodeFS.pb.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#define HAVE_CXX_MUTEX (1)
#include "libfswatch/c++/monitor.hpp"
#include "libfswatch/c++/monitor_factory.hpp"

using namespace std;

namespace google {}
namespace gflags {}
using namespace google;
using namespace gflags;

using json = nlohmann::json;

// The protocol version supported by this binary
static const int PROTOCOL_VERSION = 1;

#define FATAL_IF_FALSE(X) \
  if (((X) == false))     \
    LOG(FATAL) << "Error: (" << errno << "): " << strerror(errno);

#define FATAL_IF_FALSE_NOT_EAGAIN(X)                                 \
  if (((X) == false)) {                                              \
    if (errno == EAGAIN) {                                           \
      VLOG(10) << "Could not complete: (" << errno                   \
               << "): " << strerror(errno);                          \
    } else {                                                         \
      LOG(FATAL) << "Error: (" << errno << "): " << strerror(errno); \
    }                                                                \
  }

#define FATAL_FAIL(X) \
  if (((X) == -1))    \
    LOG(FATAL) << "Error: (" << errno << "): " << strerror(errno);

#define DRAW_FROM_UNORDERED(ITERATOR, COLLECTION) \
  auto ITERATOR = COLLECTION.begin();             \
  std::advance(ITERATOR, rand() % COLLECTION.size());

template <typename Out>
inline void split(const std::string& s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

inline std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

inline std::string SystemToStr(const char* cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }
  return result;
}

inline std::string fileToStr(const string& path) {
  std::ifstream t(path);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

inline bool replace(std::string& str, const std::string& from,
                    const std::string& to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos) return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

inline int replaceAll(std::string& str, const std::string& from,
                      const std::string& to) {
  if (from.empty()) return 0;
  int retval = 0;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    retval++;
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();  // In case 'to' contains 'from', like replacing
                               // 'x' with 'yx'
  }
  return retval;
}

#endif
