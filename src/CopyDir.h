#pragma once
#ifndef COPYFS_LIB_COPYDIR_H_
#define COPYFS_LIB_COPYDIR_H_

#include "CopyNode.h"
#include <messmer/fspp/fs_interface/Dir.h>
#include <messmer/fspp/fs_interface/File.h>

namespace copyfs {

class CopyDir: public fspp::Dir, CopyNode {
public:
  CopyDir(CopyDevice *device, const bf::path &path);
  virtual ~CopyDir();

  //TODO return type variance to CopyFile/CopyDir?
  std::unique_ptr<fspp::OpenFile> createAndOpenFile(const std::string &name, mode_t mode) override;
  void createDir(const std::string &name, mode_t mode) override;
  void remove() override;

  std::unique_ptr<std::vector<Entry>> children() const override;

private:
  DISALLOW_COPY_AND_ASSIGN(CopyDir);
};

}

#endif