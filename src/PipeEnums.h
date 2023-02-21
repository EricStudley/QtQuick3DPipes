#pragma once

#include <QObject>
#include <QRandomGenerator>

static auto constexpr DIRECTION_COUNT = 6;

class PipeEnums
{
    Q_GADGET
public:
    enum DirectionEnum {
        Up = 0,
        Forward,
        Right,
        Back,
        Left,
        Down,
        None
    };
    Q_ENUM(DirectionEnum)

    static DirectionEnum oppositeDirection(DirectionEnum direction) {
        switch (direction) {
        case Up: return Down;
        case Down: return Up;
        case Left: return Right;
        case Right: return Left;
        case Forward: return Back;
        case Back: return Forward;
        default: return None;
        }
    }

    static DirectionEnum randomDirection() {
        return static_cast<DirectionEnum>(QRandomGenerator::global()->bounded(DIRECTION_COUNT));
    }

private:
    explicit PipeEnums() = delete;
};

typedef PipeEnums::DirectionEnum Direction;
