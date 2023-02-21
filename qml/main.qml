import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick.Controls

import Pipe 1.0

Window {
    id: window
//    visibility: Window.FullScreen
    flags: Qt.FramelessWindowHint
    width: 1200
    height: 900
    visible: true
    color: "black"
    title: qsTr("Windows 98 3D Pipes")

    Shortcut {
        sequence: "Space"

        onActivated: {
            pipeModel.restartPipes()
            perspectiveCamera.randomlyMoveCamera()
        }
    }

    View3D {
        id: view3D
        anchors { fill: parent }

        environment: SceneEnvironment {
            id: sceneEnvironment
            clearColor: "black"
            backgroundMode: SceneEnvironment.Color
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.VeryHigh
            temporalAAEnabled: true
            temporalAAStrength: 2.0
        }

        PerspectiveCamera {
            id: perspectiveCamera
            z: 4000

            x: 1000
            Component.onCompleted: lookAt(scene)


//            Component.onCompleted: randomlyMoveCamera()

            function randomlyMoveCamera() {
                x = Math.random() * pipeModel.maxDistance
                lookAt(scene)
            }
        }

        DirectionalLight {
            eulerRotation.x: -45
            ambientColor: Qt.darker("slategrey")
        }

        Node {
            id: scene

            Repeater3D {
                model: pipeModel

                delegate: Node {
                    position: Qt.vector3d(positionByIndex(index.x), positionByIndex(index.y), positionByIndex(index.z))

                    property vector3d index: role_index

                    property int indexSize: pipeModel.maxDistance / pipeModel.maxIndex

                    function positionByIndex(index) {
                        return ((index * indexSize) + (indexSize / 2)) - (pipeModel.maxDistance / 2)
                    }

                    Model {
                        visible: !straightPipe()
                        source: "#Sphere"
                        scale: role_directionList.length > 1 ? Qt.vector3d(1, 1, 1)
                                                             : Qt.vector3d(1.5, 1.5, 1.5)
                        materials: PrincipledMaterial {
                            baseColor: role_color
                            metalness: 0.1
                        }

                        function contains(direction) {
                            return role_directionList.indexOf(direction) >= 0
                        }

                        function straightPipe() {
                            return (contains(Direction.Up) && contains(Direction.Down))
                                    || (contains(Direction.Left) && contains(Direction.Right))
                                    || (contains(Direction.Forward) && contains(Direction.Back))
                        }
                    }

                    Repeater3D {
                        model: role_directionList

                        delegate: Model {
                            source: "#Cylinder"
                            scale: Qt.vector3d(1, 2.5, 1)
                            position: switch (modelData) {
                                      case Direction.Up: return Qt.vector3d(0, pipeOffset, 0)
                                      case Direction.Forward: return Qt.vector3d(0, 0, pipeOffset)
                                      case Direction.Right: return Qt.vector3d(pipeOffset, 0, 0)
                                      case Direction.Back: return Qt.vector3d(0, 0, -pipeOffset)
                                      case Direction.Left: return Qt.vector3d(-pipeOffset, 0, 0)
                                      case Direction.Down: return Qt.vector3d(0, -pipeOffset, 0)
                                      }
                            rotation: switch (modelData) {
                                      case Direction.Up: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 0), 90)
                                      case Direction.Forward: return Quaternion.fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
                                      case Direction.Right: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 1), 90)
                                      case Direction.Back: return Quaternion.fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
                                      case Direction.Left: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 1), 90)
                                      case Direction.Down: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 0), 90)
                                      }
                            materials: PrincipledMaterial {
                                baseColor: role_color
                                metalness: 0.1
                            }

                            readonly property real pipeOffset: (scale.y * 100) / 2
                        }
                    }
                }
            }
        }
    }

    MouseArea {
        anchors { fill: parent }

//        onClicked: Qt.quit()
    }
}
