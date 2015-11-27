#pragma once
#ifndef MESSMER_COPYFS_SRC_COPYOPENFILE_H_
#define MESSMER_COPYFS_SRC_COPYOPENFILE_H_

#include "messmer/fspp/fs_interface/OpenFile.h"
#include "messmer/cpp-utils/macros.h"

namespace copyfs {
class CopyDevice;

class CopyOpenFile final: public fspp::OpenFile {
public:
  CopyOpenFile(const CopyDevice *device, const boost::filesystem::path &path, int flags);
  ~CopyOpenFile();

  void stat(struct ::stat *result) const override;
  void truncate(off_t size) const override;
  ssize_t read(void *buf, size_t count, off_t offset) const override;
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
