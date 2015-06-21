#include "CopyDevice.h"
#include "CopyDir.h"
#include "CopyFile.h"
#include "CopySymlink.h"
#include "messmer/fspp/fuse/FuseErrnoException.h"

using cpputils::unique_ref;
using cpputils::make_unique_ref;
using boost::optional;

//TODO Get rid of this in favor of exception hierarchy
using fspp::fuse::CHECK_RETVAL;

namespace copyfs {

CopyDevice::CopyDevice(const bf::path &root_path): _root_path(root_path) {
}

CopyDevice::~CopyDevice() {
}

optional<unique_ref<fspp::Node>> CopyDevice::Load(const bf::path &path) {
  auto real_path = RootDir() / path;
  if(bf::is_symlink(real_path)) { //We have to check for symlink first, because bf::is_directory/bf::is_regular_file return true if the symlink is pointing to a respective entry
    return optional<unique_ref<fspp::Node>>(make_unique_ref<CopySymlink>(this, path));
  }else if(bf::is_directory(real_path)) {
    return optional<unique_ref<fspp::Node>>(make_unique_ref<CopyDir>(this, path));
  } else if(bf::is_regular_file(real_path)) {
    return optional<unique_ref<fspp::Node>>(make_unique_ref<CopyFile>(this, path));
  }

  throw fspp::fuse::FuseErrnoException(ENOENT);
}

void CopyDevice::statfs(const bf::path &path, struct statvfs *fsstat) {
  auto real_path = RootDir() / path;
  int retval = ::statvfs(real_path.c_str(), fsstat);
  CHECK_RETVAL(retval);
}

}
