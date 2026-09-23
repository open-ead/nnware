#pragma once

#include <cstddef>
#include <cstdint>

namespace nn::g3d {

struct DDLDeclarations {
    uint64_t _0;
    uint32_t _8;
    uint32_t DDLDeclarations_xC;
    uint64_t _10;
    uint64_t _18;
};

class BindFuncTable {
    struct StringLength {
        size_t length;
        const char* content;
    };
    struct EntryPointer {
        void* something_0;
        nn::g3d::BindFuncTable::StringLength* string;
    };

private:
    int lengths[4];
    nn::g3d::BindFuncTable::EntryPointer strings[4];
};

} // namespace nn::g3d

