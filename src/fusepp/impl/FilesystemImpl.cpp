#include "FilesystemImpl.h"

#include <memory>
#include <fcntl.h>
#include <fusepp/fs_interface/Device.h>
#include <fusepp/fs_interface/Dir.h>

#include "FuseErrnoException.h"
#include "fusepp/fs_interface/File.h"


#include "fusepp/utils/pointer.h"

using namespace fspp;

using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::string;

namespace bf = boost::filesystem;

FilesystemImpl::FilesystemImpl(Device *device)
  :_device(device), _open_files() {
}

FilesystemImpl::~FilesystemImpl() {
}

unique_ptr<File> FilesystemImpl::LoadFile(const bf::path &path) {
  auto node = _device->Load(path);
  auto file = dynamic_pointer_move<File>(node);
  if (!file) {
	  throw FuseErrnoException(EISDIR);
  }
  return file;
}

unique_ptr<Dir> FilesystemImpl::LoadDir(const bf::path &path) {
  auto node = _device->Load(path);
  auto dir = dynamic_pointer_move<Dir>(node);
  if (!dir) {
    throw FuseErrnoException(ENOTDIR);
  }
  return dir;
}

int FilesystemImpl::openFile(const bf::path &path, int flags) {
  auto file = LoadFile(path);
  return openFile(*file, flags);
}

int FilesystemImpl::openFile(const File &file, int flags) {
  return _open_files.open(file, flags);
}

void FilesystemImpl::closeFile(int descriptor) {
  _open_files.close(descriptor);
}

void FilesystemImpl::lstat(const bf::path &path, struct ::stat *stbuf) {
  _device->Load(path)->stat(stbuf);
}

void FilesystemImpl::fstat(int descriptor, struct ::stat *stbuf) {
  _open_files.get(descriptor)->stat(stbuf);
}

void FilesystemImpl::truncate(const bf::path &path, off_t size) {
  LoadFile(path)->truncate(size);
}

void FilesystemImpl::ftruncate(int descriptor, off_t size) {
  _open_files.get(descriptor)->truncate(size);
}

int FilesystemImpl::read(int descriptor, void *buf, size_t count, off_t offset) {
  return _open_files.get(descriptor)->read(buf, count, offset);
}

void FilesystemImpl::write(int descriptor, const void *buf, size_t count, off_t offset) {
  _open_files.get(descriptor)->write(buf, count, offset);
}

void FilesystemImpl::fsync(int descriptor) {
  _open_files.get(descriptor)->fsync();
}

void FilesystemImpl::fdatasync(int descriptor) {
  _open_files.get(descriptor)->fdatasync();
}

void FilesystemImpl::access(const bf::path &path, int mask) {
  _device->Load(path)->access(mask);
}

int FilesystemImpl::createAndOpenFile(const bf::path &path, mode_t mode) {
  //TODO Creating the file opens and closes it. We then reopen it afterwards.
  //     This is slow. Improve!
  auto dir = LoadDir(path.parent_path());
  auto file = dir->createFile(path.filename().native(), mode);
  return openFile(*file, O_WRONLY | O_TRUNC);
}

void FilesystemImpl::mkdir(const bf::path &path, mode_t mode) {
  auto dir = LoadDir(path.parent_path());
  dir->createDir(path.filename().native(), mode);
}

void FilesystemImpl::rmdir(const bf::path &path) {
  auto dir = LoadDir(path);
  dir->rmdir();
}

void FilesystemImpl::unlink(const bf::path &path) {
  auto file = LoadFile(path);
  file->unlink();
}

void FilesystemImpl::rename(const bf::path &from, const bf::path &to) {
  auto node = _device->Load(from);
  node->rename(to);
}

unique_ptr<vector<string>> FilesystemImpl::readDir(const bf::path &path) {
  auto dir = LoadDir(path);
  return dir->children();
}

void FilesystemImpl::utimens(const bf::path &path, const timespec times[2]) {
  auto node = _device->Load(path);
  node->utimens(times);
}

void FilesystemImpl::statfs(const bf::path &path, struct statvfs *fsstat) {
  _device->statfs(path, fsstat);
}
