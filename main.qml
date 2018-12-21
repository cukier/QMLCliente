import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4

Window {
    visible: true
    width: 320
    height: 480
    title: qsTr("QML Cliente")

    ListView {
        id: sampleListView
        anchors.fill: parent
        model: listmodel
        cacheBuffer: 100
        spacing: 10

        delegate: Rectangle {
            width: parent.width
            radius: 5
            anchors.horizontalCenter: parent.horizontalCenter
            height: 40

            Text {
                id: nameTxt
                text: ItemName
            }
        }
    }

    Button {
        width: 60
        height: 40
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        text: "conectar"

        onClicked: {
            socket.receberModelo("localhost", 53593)
        }
    }
}
