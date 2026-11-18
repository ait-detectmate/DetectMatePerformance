#include "brain.h"

#include <cstdint>
#include <limits>
#include <utility>

#include <arrow/array/builder_primitive.h>
#include <arrow/table.h>

namespace {

template <typename ArrowType>
arrow::Result<std::shared_ptr<arrow::Table>> negateColumn(
    const arrow::Table& table, int column_index
) {
    using ArrayType = arrow::NumericArray<ArrowType>;
    using ValueType = typename ArrowType::c_type;

    const auto& column = *table.column(column_index);
    arrow::NumericBuilder<ArrowType> builder;
    arrow::ArrayVector chunks;
    chunks.reserve(column.num_chunks());

    const ArrayType* values;
    std::shared_ptr<ArrayType> negated_chunk;
    ValueType value;
    int chunk_index;
    std::int64_t row;
    for (chunk_index = 0; chunk_index < column.num_chunks(); ++chunk_index) {
        values = static_cast<const ArrayType*>(column.chunk(chunk_index).get());
        ARROW_RETURN_NOT_OK(builder.Reserve(values->length()));
        for (row = 0; row < values->length(); ++row) {
            if (values->IsNull(row)) {
                builder.UnsafeAppendNull();
                continue;
            }
            value = values->Value(row);
            if (value == std::numeric_limits<ValueType>::min()) {
                return arrow::Status::Invalid("Negating column A would overflow its integer type");
            }
            builder.UnsafeAppend(static_cast<ValueType>(-value));
        }
        ARROW_RETURN_NOT_OK(builder.Finish(&negated_chunk));
        chunks.push_back(std::move(negated_chunk));
    }

    auto negated = std::make_shared<arrow::ChunkedArray>(std::move(chunks), column.type());
    return table.SetColumn(column_index, table.field(column_index), std::move(negated));
}

}  // namespace

arrow::Result<std::shared_ptr<arrow::Table>> brain(const arrow::Table& table) {
    ARROW_RETURN_NOT_OK(table.Validate());
    const int column_index = table.schema()->GetFieldIndex("A");
    if (column_index >= 0) {
        switch (table.column(column_index)->type()->id()) {
            case arrow::Type::INT8:
                return negateColumn<arrow::Int8Type>(table, column_index);
            case arrow::Type::INT16:
                return negateColumn<arrow::Int16Type>(table, column_index);
            case arrow::Type::INT32:
                return negateColumn<arrow::Int32Type>(table, column_index);
            case arrow::Type::INT64:
                return negateColumn<arrow::Int64Type>(table, column_index);
            default:
                break;
        }
    }
    return arrow::Table::Make(table.schema(), table.columns(), table.num_rows());
}
