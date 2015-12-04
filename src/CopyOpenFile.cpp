#include "CopyDevice.h"
#include "CopyOpenFile.h"
#include <sys/types.h>
#include <fcntl.h>

#include "messmer/fspp/fuse/FuseErrnoException.h"
#include <messmer/cpp-utils/assert/assert.h>

namespace bf = boost::filesystem;

//TODO Get rid of this in favor of a exception hierarchy
using fspp::fuse::CHECK_RETVAL;

namespace copyfs {

CopyOpenFile::CopyOpenFile(const CopyDevice *device, const bf::path &path, int flags)
  :_descriptor(::open((device->RootDir() / path).c_str(), flags)) {
  CHECK_RETVAL(_descriptor);
}

CopyOpenFile::~CopyOpenFile() {
  int retval = ::close(_descriptor);
  CHECK_RETVAL(retval);
}

void CopyOpenFile::flush() {
}

void CopyOpenFile::stat(struct ::stat *result) const {
  int retval = ::fstat(_descriptor, result);
  CHECK_RETVAL(retval);
}

void CopyOpenFile::truncate(off_t size) const {
  int retval = ::ftruncate(_descriptor, size);
  CHECK_RETVAL(retval);
}

ssize_t CopyOpenFile::read(void *buf, size_t count, off_t offset) const {
  int retval = ::pread(_descriptor, buf, count, offset);
  CHECK_RETVAL(retval);
  ASSERT(static_cast<unsigned int>(retval) <= count, "Read wrong number of bytes");
  return retval;
}

void CopyOpenFile::write(const void *buf, size_t count, off_t offset) {
  int retval = ::pwrite(_descriptor, buf, count, offset);
  CHECK_RETVAL(retval);
  ASSERT(static_cast<unsigned int>(retval) == count, "Wrong number of bytes written");
}

void CopyOpenFile::fsync() {
  int retval = ::fsync(_descriptor);
  CHECK_RETVAL(retval);
}

void CopyOpenFile::fdatasync() {
#ifdef F_FULLFSYNC
  // This is MacOSX, which doesn't know fdatasync
  int retval = fcntl(_descriptor, F_FULLFSYNC);
#else
  int retval = ::fdatasync(_descriptor);
#endif
  CHECK_RETVAL(retval);
}

}
