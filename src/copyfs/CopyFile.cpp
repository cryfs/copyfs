#include <copyfs/CopyFile.h>
#include <copyfs/CopyOpenFile.h>
#include "CopyDevice.h"
#include "fspp/fuse/FuseErrnoException.h"

namespace bf = boost::filesystem;

//TODO Get rid of this in favor of exception hierarchy
using fspp::fuse::CHECK_RETVAL;

using std::unique_ptr;
using std::make_unique;

namespace copyfs {

CopyFile::CopyFile(CopyDevice *device, const bf::path &path)
  :CopyNode(device, path) {
  assert(bf::is_regular_file(base_path()));
}

CopyFile::~CopyFile() {
}

unique_ptr<fspp::OpenFile> CopyFile::open(int flags) const {
  return make_unique<CopyOpenFile>(device(), path(), flags);
}

void CopyFile::truncate(off_t size) const {
  int retval = ::truncate(base_path().c_str(), size);
  CHECK_RETVAL(retval);
}

void CopyFile::unlink() {
  int retval = ::unlink(base_path().c_str());
  CHECK_RETVAL(retval);
}

}
