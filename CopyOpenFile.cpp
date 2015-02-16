#include "CopyDevice.h"
#include "CopyOpenFile.h"
#include <sys/types.h>
#include <fcntl.h>

#include "messmer/fspp/fuse/FuseErrnoException.h"

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

int CopyOpenFile::read(void *buf, size_t count, off_t offset) {
  //printf("Reading from real descriptor %d (%d, %d)\n", _descriptor, offset, count);
  //fflush(stdout);
  int retval = ::pread(_descriptor, buf, count, offset);
  CHECK_RETVAL(retval);
  //printf("retval: %d, count: %d\n", retval, count);
  //fflush(stdout);
  assert(static_cast<unsigned int>(retval) <= count);
  return retval;
}

void CopyOpenFile::write(const void *buf, size_t count, off_t offset) {
  int retval = ::pwrite(_descriptor, buf, count, offset);
  CHECK_RETVAL(retval);
  assert(static_cast<unsigned int>(retval) == count);
}

void CopyOpenFile::fsync() {
  int retval = ::fsync(_descriptor);
  CHECK_RETVAL(retval);
}

void CopyOpenFile::fdatasync() {
  int retval = ::fdatasync(_descriptor);
  CHECK_RETVAL(retval);
}

}
