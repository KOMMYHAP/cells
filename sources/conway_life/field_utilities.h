#pragma once

class Field;

struct FieldHasher {
    std::size_t operator()(const Field& field) const;
};

struct FieldComparator {
    bool operator()(const Field& lhs, const Field& rhs) const;
};
