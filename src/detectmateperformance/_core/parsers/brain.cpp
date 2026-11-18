#include "brain.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include <arrow/api.h>
#include <arrow/compute/cast.h>

namespace {

arrow::Status appendSentences(
    const arrow::Array& length_column,
    const arrow::Array& template_column,
    arrow::LargeStringBuilder& builder
) {
    static const auto template_type = arrow::large_list(arrow::struct_({
        arrow::field("position", arrow::int64()),
        arrow::field("word", arrow::large_utf8()),
    }));
    ARROW_ASSIGN_OR_RAISE(auto length_array, arrow::compute::Cast(length_column, arrow::int64()));
    ARROW_ASSIGN_OR_RAISE(auto template_array, arrow::compute::Cast(template_column, template_type));
    const auto& lengths = static_cast<const arrow::Int64Array&>(*length_array);
    const auto& templates = static_cast<const arrow::LargeListArray&>(*template_array);
    const auto& entries = static_cast<const arrow::StructArray&>(*templates.values());
    const auto& positions = static_cast<const arrow::Int64Array&>(*entries.field(0));
    const auto& words = static_cast<const arrow::LargeStringArray&>(*entries.field(1));

    std::vector<std::string_view> tokens;
    std::string sentence;
    std::int64_t row;
    std::int64_t index;
    std::int64_t length;
    std::int64_t position;
    for (row = 0; row < lengths.length(); ++row) {
        if (lengths.IsNull(row) || lengths.Value(row) < 0) {
            return arrow::Status::Invalid("Lenght must be a nonnegative integer");
        }
        length = lengths.Value(row);
        tokens.assign(length, "<*>");
        if (templates.IsValid(row)) {
            for (index = templates.value_offset(row); index < templates.value_offset(row + 1); ++index) {
                if (entries.IsNull(index) || positions.IsNull(index) || words.IsNull(index)) {
                    continue;
                }
                position = positions.Value(index);
                if (position < 0 || position >= length) {
                    return arrow::Status::Invalid("Template position is outside Lenght");
                }
                tokens[position] = words.GetView(index);
            }
        }

        sentence.clear();
        for (index = 0; index < length; ++index) {
            if (index > 0) {
                sentence += ' ';
            }
            sentence += tokens[index];
        }
        ARROW_RETURN_NOT_OK(builder.Append(sentence));
    }
    return arrow::Status::OK();
}

}  // namespace

arrow::Result<std::shared_ptr<arrow::Table>> brain(const arrow::Table& table) {
    ARROW_RETURN_NOT_OK(table.Validate());
    const int length_index = table.schema()->GetFieldIndex("Lenght");
    const int template_index = table.schema()->GetFieldIndex("Template");
    if (length_index < 0 || template_index < 0) {
        return arrow::Status::Invalid("Brain requires Lenght and Template columns");
    }

    arrow::LargeStringBuilder builder;
    ARROW_RETURN_NOT_OK(builder.Reserve(table.num_rows()));
    arrow::TableBatchReader reader(table);
    std::shared_ptr<arrow::RecordBatch> batch;
    while (true) {
        ARROW_RETURN_NOT_OK(reader.ReadNext(&batch));
        if (!batch) {
            break;
        }
        ARROW_RETURN_NOT_OK(appendSentences(
            *batch->column(length_index), *batch->column(template_index), builder
        ));
    }
    ARROW_ASSIGN_OR_RAISE(auto sentences, builder.Finish());
    return arrow::Table::Make(
        arrow::schema({arrow::field("Templates", arrow::large_utf8())}), {sentences}
    );
}
