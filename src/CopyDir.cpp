#include "CopyDir.h"
#include "CopyFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "messmer/fspp/fuse/FuseErrnoException.h"
#include "CopyDevice.h"
#include "CopyOpenFile.h"

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

unique_ptr<fspp::OpenFile> CopyDir::createAndOpenFile(const string &name, mode_t mode) {
  auto file_path = base_path() / name;
  //Create file
  int fd = ::creat(file_path.c_str(), mode);
  CHECK_RETVAL(fd);
  //TODO Don't close and reopen, that's inperformant
  ::close(fd);
  return make_unique<CopyOpenFile>(device(), path() / name, O_RDWR | O_TRUNC);
}

void CopyDir::createDir(const string &name, mode_t mode) {
  auto dir_path = base_path() / name;
  //Create dir
  int retval = ::mkdir(dir_path.c_str(), mode);
  CHECK_RETVAL(retval);
}

void CopyDir::rmdir() {
  int retval = ::rmdir(base_path().c_str());
  CHECK_RETVAL(retval);
}

unique_ptr<vector<CopyDir::Entry>> CopyDir::children() const {
  DIR *dir = ::opendir(base_path().c_str());
  if (dir == nullptr) {
    throw fspp::fuse::FuseErrnoException(errno);
  }

  // Set errno=0 so we can detect whether it changed later
  errno = 0;

  auto result = make_unique<vector<Entry>>();

  struct dirent *entry = ::readdir(dir);
  while(entry != nullptr) {
  EntryType type;
#ifdef _DIRENT_HAVE_D_TYPE
  if(entry->d_type == DT_DIR) {
    result->push_back(Entry(EntryType::DIR, entry->d_name));
  } else if(entry->d_type == DT_REG) {
    result->push_back(Entry(EntryType::FILE, entry->d_name));
  } // else: Ignore files we can't handle (e.g. block device, pipe, ...)
#else
  if(bf::is_regular_file(entry->d_name)) {
    result->push_back(Entry(EntryType::FILE, entry->d_name));
  } else if(bf::is_directory(entry->d_name)) {
    result->push_back(Entry(EntryType::DIR, entry->d_name));
  } // else: Ignore files we can't handle (e.g. block device, pipe, ...)
#endif
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
