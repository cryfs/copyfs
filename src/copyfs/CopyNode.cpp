#include <copyfs/CopyNode.h>
#include <sys/time.h>

#include "CopyDevice.h"
#include "fspp/fuse/FuseErrnoException.h"

namespace bf = boost::filesystem;

//TODO Get rid of this in favor of an exception hierarchy
using fspp::fuse::CHECK_RETVAL;

namespace copyfs {

CopyNode::CopyNode(CopyDevice *device, const bf::path &path)
  :_device(device), _path(path) {
}

CopyNode::~CopyNode() {
}

void CopyNode::stat(struct ::stat *result) const {
  int retval = ::lstat(base_path().c_str(), result);
  CHECK_RETVAL(retval);
}

void CopyNode::access(int mask) const {
  int retval = ::access(base_path().c_str(), mask);
  CHECK_RETVAL(retval);
}

void CopyNode::rename(const bf::path &to) {
  auto new_base_path = device()->RootDir() / to;
  int retval = ::rename(base_path().c_str(), new_base_path.c_str());
  CHECK_RETVAL(retval);
  _path = to;
}

void CopyNode::utimens(const timespec times[2]) {
  struct timeval timevals[2];
  TIMESPEC_TO_TIMEVAL(&timevals[0], &times[0]);
  TIMESPEC_TO_TIMEVAL(&timevals[1], &times[1]);
  ::lutimes(base_path().c_str(), timevals);
}

}
