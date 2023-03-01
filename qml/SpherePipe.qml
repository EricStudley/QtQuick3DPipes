import QtQuick
import QtQuick3D

import Pipe 1.0

Model {
    visible: !straightPipe()
    source: "#Sphere"
    scale: startPipe || endPipe ? Qt.vector3d(1.5, 1.5, 1.5) : Qt.vector3d(1, 1, 1)
    materials: PrincipledMaterial {
        baseColor: parent.color
        metalness: 0.1
    }

    required property var directionList
    required property color color

    readonly property bool startPipe: directionList.length === 1
    readonly property bool endPipe: contains(Direction.None)

    function contains(direction) {
        return directionList.indexOf(direction) >= 0
    }

    function straightPipe() {
        return (contains(Direction.Up) && contains(Direction.Down))
                || (contains(Direction.Left) && contains(Direction.Right))
                || (contains(Direction.Forward) && contains(Direction.Back))
    }
}
