#include "CopySymlink.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "messmer/fspp/fuse/FuseErrnoException.h"
#include "CopyDevice.h"
#include <messmer/cpp-utils/assert/assert.h>

//TODO Get rid of this in favor of exception hierarchy
using fspp::fuse::CHECK_RETVAL;

namespace bf = boost::filesystem;

using std::string;
using std::vector;

namespace copyfs {

CopySymlink::CopySymlink(CopyDevice *device, const bf::path &path)
  :CopyNode(device, path) {
  ASSERT(bf::is_symlink(base_path()), "Path isn't valid symlink");
}

CopySymlink::~CopySymlink() {
}

bf::path CopySymlink::target() const {
  int readBytes;
  struct stat st;
  while(true) {
    CHECK_RETVAL(::lstat(base_path().c_str(), &st));
    char buffer[st.st_size+1];
    readBytes = ::readlink(base_path().c_str(), buffer, st.st_size+1);
    buffer[st.st_size] = '\0';
    if (readBytes == st.st_size) { //If readBytes > st.st_size, then the size of the symlink changed between the ::lstat and the ::readlink call
      return buffer;
    }
  }
}

void CopySymlink::remove() {
  int retval = ::unlink(base_path().c_str());
  CHECK_RETVAL(retval);
}

}

