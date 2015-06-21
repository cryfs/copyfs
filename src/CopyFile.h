#pragma once
#ifndef COPYFS_LIB_COPYFILE_H_
#define COPYFS_LIB_COPYFILE_H_

#include "CopyNode.h"
#include <messmer/fspp/fs_interface/File.h>

namespace copyfs {

class CopyFile: public fspp::File, CopyNode {
public:
  CopyFile(CopyDevice *device, const boost::filesystem::path &path);
  virtual ~CopyFile();

  cpputils::unique_ref<fspp::OpenFile> open(int flags) const override;
  void truncate(off_t size) const override;
  void remove() override;

private:
  DISALLOW_COPY_AND_ASSIGN(CopyFile);
};

}

#endif
