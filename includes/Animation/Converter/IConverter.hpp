//
// Created by jusra on 14-11-2025.
//

#ifndef GAMEENGINE_ICONVERTER_HPP
#define GAMEENGINE_ICONVERTER_HPP

class IConverter {
public:
    virtual ~IConverter() = default;

    virtual int convert(int start, int end, float t) = 0;
};

#endif //GAMEENGINE_ICONVERTER_HPP