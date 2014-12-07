#include <copyfs/CopyDir.h>
#include <copyfs/CopyFile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "fspp/fuse/FuseErrnoException.h"
#include "CopyDevice.h"

//TODO Get rid of this in favor of exception hierarchy
using fspp::fuse::CHECK_RETVAL;

namespace bf = boost::filesystem;

using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;

namespace copyfs {

CopyDir::CopyDir(CopyDevice *device, const bf::path &path)
  :CopyNode(device, path) {
  assert(bf::is_directory(base_path()));
}

CopyDir::~CopyDir() {
}

unique_ptr<fspp::File> CopyDir::createFile(const string &name, mode_t mode) {
  auto file_path = base_path() / name;
  //Create file
  int fd = ::creat(file_path.c_str(), mode);
  CHECK_RETVAL(fd);
  ::close(fd);
  return make_unique<CopyFile>(device(), path() / name);
}

unique_ptr<fspp::Dir> CopyDir::createDir(const string &name, mode_t mode) {
  auto dir_path = base_path() / name;
  //Create dir
  int retval = ::mkdir(dir_path.c_str(), mode);
  CHECK_RETVAL(retval);
  return make_unique<CopyDir>(device(), path() / name);
}

void CopyDir::rmdir() {
  int retval = ::rmdir(base_path().c_str());
  CHECK_RETVAL(retval);
}

unique_ptr<vector<string>> CopyDir::children() const {
  DIR *dir = ::opendir(base_path().c_str());
  if (dir == nullptr) {
    throw fspp::fuse::FuseErrnoException(errno);
  }

  // Set errno=0 so we can detect whether it changed later
  errno = 0;

  auto result = make_unique<vector<string>>();

  struct dirent *entry = ::readdir(dir);
  while(entry != nullptr) {
    result->push_back(entry->d_name);
    entry = ::readdir(dir);
  }
  //On error, ::readdir returns nullptr and sets errno.
  if (errno != 0) {
    int readdir_errno = errno;
    ::closedir(dir);
    throw fspp::fuse::FuseErrnoException(readdir_errno);
  }
  int retval = ::closedir(dir);
  CHECK_RETVAL(retval);
  return result;
}

}
