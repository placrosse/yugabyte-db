//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under the BSD-style license found in the
//  LICENSE file in the root directory of this source tree. An additional grant
//  of patent rights can be found in the PATENTS file in the same directory.
#ifndef ROCKSDB_DB_COMPACTED_DB_IMPL_H
#define ROCKSDB_DB_COMPACTED_DB_IMPL_H

#pragma once
#ifndef ROCKSDB_LITE
#include <vector>
#include <string>

#include "db/db_impl.h"

namespace rocksdb {

class CompactedDBImpl : public DBImpl {
 public:
  CompactedDBImpl(const DBOptions& options, const std::string& dbname);
  virtual ~CompactedDBImpl();

  static Status Open(const Options& options, const std::string& dbname,
                     DB** dbptr);

  // Implementations of the DB interface
  using DB::Get;
  virtual Status Get(const ReadOptions& options,
                     ColumnFamilyHandle* column_family, const Slice& key,
                     std::string* value) override;
  using DB::MultiGet;
  virtual std::vector<Status> MultiGet(
      const ReadOptions& options,
      const std::vector<ColumnFamilyHandle*>&,
      const std::vector<Slice>& keys, std::vector<std::string>* values)
    override;

  using DBImpl::Put;
  virtual Status Put(const WriteOptions& options,
                     ColumnFamilyHandle* column_family, const Slice& key,
                     const Slice& value) override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }
  using DBImpl::Merge;
  virtual Status Merge(const WriteOptions& options,
                       ColumnFamilyHandle* column_family, const Slice& key,
                       const Slice& value) override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }
  using DBImpl::Delete;
  virtual Status Delete(const WriteOptions& options,
                        ColumnFamilyHandle* column_family,
                        const Slice& key) override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }
  virtual Status Write(const WriteOptions& options,
                       WriteBatch* updates) override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }
  using DBImpl::CompactRange;
  virtual Status CompactRange(const CompactRangeOptions& options,
                              ColumnFamilyHandle* column_family,
                              const Slice* begin, const Slice* end) override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }

  virtual Status DisableFileDeletions() override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }
  virtual Status EnableFileDeletions(bool force) override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }
  virtual Status GetLiveFiles(std::vector<std::string>&,
                              uint64_t* manifest_file_size,
                              bool flush_memtable = true) override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }
  using DBImpl::Flush;
  virtual Status Flush(const FlushOptions& options,
                       ColumnFamilyHandle* column_family) override {
    return STATUS(NotSupported, "Not supported in compacted db mode.");
  }

 private:
  friend class DB;
  inline size_t FindFile(const Slice& key);
  Status Init(const Options& options);

  ColumnFamilyData* cfd_;
  Version* version_;
  const Comparator* user_comparator_;
  LevelFilesBrief files_;

  // No copying allowed
  CompactedDBImpl(const CompactedDBImpl&);
  void operator=(const CompactedDBImpl&);
};
} // namespace rocksdb
#endif  // ROCKSDB_LITE

#endif // ROCKSDB_DB_COMPACTED_DB_IMPL_H
