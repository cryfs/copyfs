#pragma once
#ifndef FSPP_OPENFILE_H_
#define FSPP_OPENFILE_H_

#include <boost/filesystem.hpp>
#include <sys/stat.h>

namespace fspp {
class Device;

class OpenFile {
public:
  virtual ~OpenFile() {}

  virtual void stat(struct ::stat *result) const = 0;
  virtual void truncate(off_t size) const = 0;
  virtual int read(void *buf, size_t count, off_t offset) = 0;
  virtual void write(const void *buf, size_t count, off_t offset) = 0;
  virtual void fsync() = 0;
  virtual void fdatasync() = 0;
};

}

#endif /* FSPP_OPENFILE_H_ */
