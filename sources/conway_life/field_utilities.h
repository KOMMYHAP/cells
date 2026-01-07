#pragma once

class Field;

struct FieldHistory {
    enum class Index : std::size_t {};

    std::vector<Field> _fields;
};

struct FieldHasher {
    std::size_t operator()(FieldHistory::Index field) const;
    std::size_t operator()(const Field& field) const;

    FieldHistory* history{nullptr};
};

struct FieldComparator {
    bool operator()(FieldHistory::Index lhsIndex, FieldHistory::Index rhsIndex) const;
    bool operator()(const Field& lhs, const Field& rhs) const;

    FieldHistory* history{nullptr};
};
