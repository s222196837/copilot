import QtQuick 2.15
import QtQuick.Controls 2.15
import QtPositioning 5.15
import QtLocation 5.15

Page {
    height: 700
    width: 500
    visible: true
    title: "COPILOT"

    property variant torquay: QtPositioning.coordinate(-38.342647,144.319029)
    property variant winkipop: QtPositioning.coordinate(-38.365793,144.285479)
    property variant spionkop: QtPositioning.coordinate(-38.468168,144.069488)

    property variant viewPoint: torquay
    property variant viewPort: QtPositioning.rectangle(viewPoint, 0.008, 0.08)

    //property variant topLeft: QtPositioning.coordinate(-38.439028,144.029164)
    //property variant bottomRight: QtPositioning.coordinate(-38.496956,144.108478)
    //property variant viewPort: QtPositioning.rectangle(topLeft, bottomRight)
    //property variant topLeftEurope: QtPositioning.coordinate(60.5, 0.0)
    //property variant bottomRightEurope: QtPositioning.coordinate(51.0, 14.0)
    //property variant viewPort:
    //        QtPositioning.rectangle(topLeftEurope, bottomRightEurope)

    property variant berlin: QtPositioning.coordinate(52.5175, 13.384)
    property variant oslo: QtPositioning.coordinate(59.9154, 10.7425)
    property variant london: QtPositioning.coordinate(51.5, 0.1275)

    Map {
        id: mapOfEurope
        anchors.centerIn: parent;
        anchors.fill: parent
        plugin: Plugin {
            name: "mapboxgl" // "osm", "mapboxgl", "esri", ...
        }

        Plane {
            id: gpsPlane
            //pilotName: "GPS"
            coordinate: gps.position

            SequentialAnimation {
                id: gpsPlaneAnimation
                property real rotationDirection : 0;
                NumberAnimation {
                    target: gpsPlane; property: "bearing"; duration: 100
                    easing.type: Easing.InOutQuad
                    to: gpsPlaneAnimation.rotationDirection
                }
            }
	}

        Plane {
            id: qmlPlane
            pilotName: "QML"
            coordinate: oslo2Berlin.position

            SequentialAnimation {
                id: qmlPlaneAnimation
                property real rotationDirection : 0;
                NumberAnimation {
                    target: qmlPlane; property: "bearing"; duration: 1000
                    easing.type: Easing.InOutQuad
                    to: qmlPlaneAnimation.rotationDirection
                }
                //! [QmlPlane1]
                CoordinateAnimation {
                    id: coordinateAnimation; duration: 5000
                    target: oslo2Berlin; property: "position"
                    easing.type: Easing.InOutQuad
                }
                //! [QmlPlane1]

                onStopped: {
                    if (coordinateAnimation.to === berlin)
                        qmlPlane.showMessage(qsTr("Hello Berlin!"))
                    else if (coordinateAnimation.to === oslo)
                        qmlPlane.showMessage(qsTr("Hello Oslo!"))
                }
                onStarted: {
                    if (coordinateAnimation.from === oslo)
                        qmlPlane.showMessage(qsTr("See you Oslo!"))
                    else if (coordinateAnimation.from === berlin)
                        qmlPlane.showMessage(qsTr("See you Berlin!"))
                }
            }

            //! [QmlPlane2]
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (qmlPlaneAnimation.running) {
                        console.log("Plane still in the air.");
                        return;
                    }

                    if (oslo2Berlin.position === berlin) {
                        coordinateAnimation.from = berlin;
                        coordinateAnimation.to = oslo;
                    } else if (oslo2Berlin.position === oslo) {
                        coordinateAnimation.from = oslo;
                        coordinateAnimation.to = berlin;
                    }

                    qmlPlaneAnimation.rotationDirection = oslo2Berlin.position.azimuthTo(coordinateAnimation.to)
                    qmlPlaneAnimation.start()
                }
            }
            //! [QmlPlane2]
            Component.onCompleted: {
                oslo2Berlin.position = oslo;
            }
        }

        //! [CppPlane1]
        Plane {
            id: cppPlane
            pilotName: "C++"
            coordinate: berlin2London.position

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (cppPlaneAnimation.running || berlin2London.isFlying()) {
                        console.log("Plane still in the air.");
                        return;
                    }

                    berlin2London.swapDestinations();
                    cppPlaneAnimation.rotationDirection = berlin2London.position.azimuthTo(berlin2London.to)
                    cppPlaneAnimation.start();
                    cppPlane.departed();
                }
            }
        //! [CppPlane1]
            //! [CppPlane3]
            SequentialAnimation {
                id: cppPlaneAnimation
                property real rotationDirection : 0;
                NumberAnimation {
                    target: cppPlane; property: "bearing"; duration: 1000
                    easing.type: Easing.InOutQuad
                    to: cppPlaneAnimation.rotationDirection
                }
                ScriptAction { script: berlin2London.startFlight() }
            }
            //! [CppPlane3]

            Component.onCompleted: {
                berlin2London.position = berlin;
                berlin2London.to = london;
                berlin2London.from = berlin;
                berlin2London.arrived.connect(arrived)
            }

            function arrived(){
                if (berlin2London.to === berlin)
                    cppPlane.showMessage(qsTr("Hello Berlin!"))
                else if (berlin2London.to === london)
                    cppPlane.showMessage(qsTr("Hello London!"))
            }

            function departed(){
                if (berlin2London.from === berlin)
                    cppPlane.showMessage(qsTr("See you Berlin!"))
                else if (berlin2London.from === london)
                    cppPlane.showMessage(qsTr("See you London!"))
            }
        //! [CppPlane2]
        }
        //! [CppPlane2]

        visibleRegion: viewPort
    }
}
