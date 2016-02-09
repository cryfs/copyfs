#include "CopyNode.h"
#include <sys/time.h>

#include "CopyDevice.h"
#include "messmer/fspp/fuse/FuseErrnoException.h"

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

void CopyNode::chmod(mode_t mode) {
  int retval = ::chmod(base_path().c_str(), mode);
  CHECK_RETVAL(retval);
}

void CopyNode::chown(uid_t uid, gid_t gid) {
  int retval = ::chown(base_path().c_str(), uid, gid);
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

void CopyNode::utimens(timespec lastAccessTime, timespec lastModificationTime) {
  struct timeval timevals[2];
  TIMESPEC_TO_TIMEVAL(&timevals[0], &lastAccessTime);
  TIMESPEC_TO_TIMEVAL(&timevals[1], &lastModificationTime);
  ::lutimes(base_path().c_str(), timevals);
}

}
