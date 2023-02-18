#pragma once

#include <QObject>

class PipeEnums
{
    Q_GADGET
public:
    enum SideEnum {
        Up = 0,
        Front,
        Right,
        Back,
        Left,
        Down
    };
    Q_ENUM(SideEnum)

    static SideEnum oppositeSide(SideEnum side) {
        switch (side) {
        case Up: return Down;
        case Down: return Up;
        case Left: return Right;
        case Right: return Left;
        case Front: return Back;
        case Back: return Front;
        }
    }

    enum ColorEnum {
        White = 0,
        Green,
        Red,
        Blue,
        Orange,
        Yellow,
    };
    Q_ENUM(ColorEnum)

private:
    explicit PipeEnums() = delete;
};

typedef PipeEnums::SideEnum Side;
typedef PipeEnums::ColorEnum Color;
