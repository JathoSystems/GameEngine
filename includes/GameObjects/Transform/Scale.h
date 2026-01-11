#ifndef GAMEENGINE_SCALE_H
#define GAMEENGINE_SCALE_H

class Scale {
private:
    int _scale;

public:
    Scale(int scale);

    int getScale() const;

    void setScale(int scale);
};

#endif //GAMEENGINE_SCALE_H
