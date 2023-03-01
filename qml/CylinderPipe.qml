import QtQuick
import QtQuick3D

import Pipe 1.0

Model {
    source: "#Cylinder"
    scale: Qt.vector3d(1, cellSize / 100, 1)
    position: switch (direction) {
              case Direction.Up: return Qt.vector3d(0, offset, 0)
              case Direction.Forward: return Qt.vector3d(0, 0, offset)
              case Direction.Right: return Qt.vector3d(offset, 0, 0)
              case Direction.Back: return Qt.vector3d(0, 0, -offset)
              case Direction.Left: return Qt.vector3d(-offset, 0, 0)
              case Direction.Down: return Qt.vector3d(0, -offset, 0)
              }
    rotation: switch (direction) {
              case Direction.Up: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 0), 90)
              case Direction.Forward: return Quaternion.fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
              case Direction.Right: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 1), 90)
              case Direction.Back: return Quaternion.fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
              case Direction.Left: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 1), 90)
              case Direction.Down: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 0), 90)
              }
    materials: PrincipledMaterial {
        baseColor: parent.color
        metalness: 0.1
    }

    readonly property real offset: cellSize / 2

    property real cellSize
    property color color
    property var direction
}
