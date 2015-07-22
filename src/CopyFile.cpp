#include "CopyFile.h"
#include "CopyOpenFile.h"
#include "CopyDevice.h"
#include "messmer/fspp/fuse/FuseErrnoException.h"
#include <messmer/cpp-utils/assert/assert.h>

namespace bf = boost::filesystem;

//TODO Get rid of this in favor of exception hierarchy
using fspp::fuse::CHECK_RETVAL;

using cpputils::unique_ref;
using cpputils::make_unique_ref;

namespace copyfs {

CopyFile::CopyFile(CopyDevice *device, const bf::path &path)
  :CopyNode(device, path) {
  ASSERT(bf::is_regular_file(base_path()), "Path isn't a valid file");
}

CopyFile::~CopyFile() {
}

unique_ref<fspp::OpenFile> CopyFile::open(int flags) const {
  return make_unique_ref<CopyOpenFile>(device(), path(), flags);
}

void CopyFile::truncate(off_t size) const {
  int retval = ::truncate(base_path().c_str(), size);
  CHECK_RETVAL(retval);
}

void CopyFile::remove() {
  int retval = ::unlink(base_path().c_str());
  CHECK_RETVAL(retval);
}

}
