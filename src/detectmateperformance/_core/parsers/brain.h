#ifndef BRAIN_H_
#define BRAIN_H_

#include <memory>

#include <arrow/result.h>
#include <arrow/type_fwd.h>

arrow::Result<std::shared_ptr<arrow::Table>> brain(const arrow::Table& table);

#endif  // BRAIN_H_
