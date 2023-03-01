import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick.Controls

import Pipe 1.0

Window {
    id: window
//    visibility: Window.FullScreen
    visible: true
    width: 1200
    height: 900
    color: "black"
    title: qsTr("Qt Quick 3D Pipes")

    Shortcut { sequence: "Shift+="; onActivated: pipeModel.pipeCount++ }
    Shortcut { sequence: "Shift+-"; onActivated: pipeModel.pipeCount-- }

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

        OrbitCameraController {
            anchors { fill: parent }
            origin: scene
            camera: perspectiveCamera
        }

        DirectionalLight {
            eulerRotation.x: -45
            ambientColor: Qt.darker("slategrey")
        }

        PerspectiveCamera {
            id: perspectiveCamera
            z: pipeModel.maxDistance * 0.9
        }

        Node {
            id: scene

            Repeater3D {
                model: pipeModel

                delegate: Node {
                    id: cell
                    position: Qt.vector3d(positionByCellIndex(cellIndex.x),
                                          positionByCellIndex(cellIndex.y),
                                          positionByCellIndex(cellIndex.z))

                    property vector3d cellIndex: role_cellIndex

                    property int cellSize: pipeModel.maxDistance / pipeModel.cellRowCount

                    function positionByCellIndex(cellIndex) {
                        return ((cellIndex * cellSize) + (cellSize / 2)) - (pipeModel.maxDistance / 2)
                    }

                    SpherePipe {
                        color: role_color
                        directionList: role_directionList
                    }

                    Repeater3D {
                        model: role_directionList

                        delegate: CylinderPipe {
                            cellSize: cell.cellSize
                            color: role_color
                            direction: modelData
                        }
                    }
                }
            }
        }
    }
}
