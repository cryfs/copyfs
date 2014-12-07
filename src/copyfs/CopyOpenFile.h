#pragma once
#ifndef COPYFS_LIB_COPYOPENFILE_H_
#define COPYFS_LIB_COPYOPENFILE_H_

#include "fspp/fs_interface/OpenFile.h"
#include "fspp/utils/macros.h"

namespace copyfs {
class CopyDevice;

class CopyOpenFile: public fspp::OpenFile {
public:
  CopyOpenFile(const CopyDevice *device, const boost::filesystem::path &path, int flags);
  virtual ~CopyOpenFile();

  void stat(struct ::stat *result) const override;
  void truncate(off_t size) const override;
  int read(void *buf, size_t count, off_t offset) override;
  void write(const void *buf, size_t count, off_t offset) override;
  void flush() override;
  void fsync() override;
  void fdatasync() override;

private:
  int _descriptor;

  DISALLOW_COPY_AND_ASSIGN(CopyOpenFile);
};

}

#endif
