#pragma once
#ifndef COPYFS_LIB_COPYDIR_H_
#define COPYFS_LIB_COPYDIR_H_

#include <copyfs/CopyNode.h>
#include <fspp/fs_interface/Dir.h>

namespace copyfs {

class CopyDir: public fspp::Dir, CopyNode {
public:
  CopyDir(CopyDevice *device, const bf::path &path);
  virtual ~CopyDir();

  //TODO return type variance to CopyFile/CopyDir?
  std::unique_ptr<fspp::File> createFile(const std::string &name, mode_t mode) override;
  std::unique_ptr<fspp::Dir> createDir(const std::string &name, mode_t mode) override;
  void rmdir() override;

  std::unique_ptr<std::vector<std::string>> children() const override;

private:
  DISALLOW_COPY_AND_ASSIGN(CopyDir);
};

}

#endif
