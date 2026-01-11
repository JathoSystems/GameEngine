#include "Animation/Converter/LerpConverter.hpp"

int LerpConverter::convert(int start, int end, float t) {
    return start + static_cast<int>((end - start) * t);
}
