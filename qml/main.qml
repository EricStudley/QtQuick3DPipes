import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick.Controls

import Pipe 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    color: "black"
    title: qsTr("Windows 98 3D Pipes")

    Shortcut { sequence: "Space"; onActivated: showBars = !showBars }

    property bool showBars: true

    View3D {
        id: view3D
        anchors.fill: parent

        environment: SceneEnvironment {
            id: sceneEnvironment
            clearColor: "black"
            backgroundMode: SceneEnvironment.Color
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.VeryHigh
            temporalAAEnabled: true
            temporalAAStrength: 2.0
        }

        OrbitCameraController {
            anchors.fill: parent
            origin: scene
            camera: perspectiveCamera
        }

        PerspectiveCamera {
            id: perspectiveCamera
            z: 5000
        }

        DirectionalLight {
            eulerRotation.x: -30
        }

        Node {
            id: scene

            //!EDGE
            Repeater3D {
                model: ListModel {
                    ListElement { role_position: "-5000, -5000, -5000" }
                    ListElement { role_position: "5000, -5000, -5000" }
                    ListElement { role_position: "5000, 5000, -5000" }
                    ListElement { role_position: "-5000, 5000, -5000" }
                    ListElement { role_position: "-5000, 5000, 5000" }
                    ListElement { role_position: "-5000, -5000, 5000" }
                    ListElement { role_position: "5000, -5000, 5000" }
                    ListElement { role_position: "5000, 5000, 5000" }
                }

                delegate: Model {
                    source: "#Sphere"
                    position: role_position
                    scale: Qt.vector3d(1, 1, 1)
                    materials: DefaultMaterial {
                        diffuseColor: "red"
                    }
                }
            }

            Repeater3D {
                model: ListModel {
                    ListElement { role_position: "-5000, 0, -5000"; role_rotationAxis: "0, 0, 0" }
                    ListElement { role_position: "5000, 0, -5000"; role_rotationAxis: "0, 0, 0" }
                    ListElement { role_position: "0, -5000, -5000"; role_rotationAxis: "0, 0, 1" }
                    ListElement { role_position: "0, 5000, -5000"; role_rotationAxis: "0, 0, 1" }

                    ListElement { role_position: "-5000, 0, 5000"; role_rotationAxis: "0, 0, 0" }
                    ListElement { role_position: "5000, 0, 5000"; role_rotationAxis: "0, 0, 0" }
                    ListElement { role_position: "0, -5000, 5000"; role_rotationAxis: "0, 0, 1" }
                    ListElement { role_position: "0, 5000, 5000"; role_rotationAxis: "0, 0, 1" }

                    ListElement { role_position: "-5000, -5000, 0"; role_rotationAxis: "1, 0, 0" }
                    ListElement { role_position: "-5000, 5000, 0"; role_rotationAxis: "1, 0, 0" }
                    ListElement { role_position: "5000, -5000, 0"; role_rotationAxis: "1, 0, 0" }
                    ListElement { role_position: "5000, 5000, 0"; role_rotationAxis: "1, 0, 0" }
                }

                delegate: Model {
                    source: "#Cylinder"
                    position: role_position
                    scale: Qt.vector3d(1, 100, 1)
                    rotation: Quaternion.fromAxisAndAngle(rotationAxis, 90)
                    materials: DefaultMaterial {
                        diffuseColor: "red"
                    }

                    property vector3d rotationAxis: role_rotationAxis
                }
            }
            //!EDGE

            Repeater3D {
                model: pipeModel

                delegate: Node {
                    position: Qt.vector3d(positionByIndex(index.x), positionByIndex(index.y), positionByIndex(index.z))

                    property vector3d index: role_index

                    function positionByIndex(index) {
                        return ((index * 500) + 250) - 5000
                    }

                    Model {
                        source: "#Sphere"
                        scale: role_sideList.length > 1 ? Qt.vector3d(1, 1, 1)
                                                        : Qt.vector3d(1.5, 1.5, 1.5)
                        materials: DefaultMaterial {
                            diffuseColor: role_color
                        }
                    }

                    Repeater3D {
                        model: role_sideList

                        delegate: Model {
                            visible: showBars
                            source: "#Cylinder"
                            scale: Qt.vector3d(1, 2.5, 1)
                            position: switch (modelData) {
                                      case Side.Up: return Qt.vector3d(0, pipeOffset, 0)
                                      case Side.Front: return Qt.vector3d(0, 0, pipeOffset)
                                      case Side.Right: return Qt.vector3d(pipeOffset, 0, 0)
                                      case Side.Back: return Qt.vector3d(0, 0, -pipeOffset)
                                      case Side.Left: return Qt.vector3d(-pipeOffset, 0, 0)
                                      case Side.Down: return Qt.vector3d(0, -pipeOffset, 0)
                                      }
                            rotation: switch (modelData) {
                                      case Side.Up: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 0), 90)
                                      case Side.Front: return Quaternion.fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
                                      case Side.Right: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 1), 90)
                                      case Side.Back: return Quaternion.fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
                                      case Side.Left: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 1), 90)
                                      case Side.Down: return Quaternion.fromAxisAndAngle(Qt.vector3d(0, 0, 0), 90)
                                      }
                            materials: DefaultMaterial {
                                diffuseColor: role_color
                            }

                            readonly property real pipeOffset: (scale.y * 100) / 2
                        }
                    }
                }
            }
        }
    }
}
