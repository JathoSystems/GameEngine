//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_SIZE_H
#define GAMEENGINE_SIZE_H

class Size {
private:
    int _width;
    int _height;

public:
    Size(int width, int height);

    int getWidth() const;
    void setWidth(int width);

    int getHeight() const;
    void setHeight(int height);
};

#endif //GAMEENGINE_SIZE_H