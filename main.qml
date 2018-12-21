import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Window {
    visible: true
    width: 320
    height: 480
    title: qsTr("QML Cliente")

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        ListView {
            id: listViewItemName
            x: 0
            y: 0
            width: 110
            height: 160
            Layout.fillWidth: true
            Layout.fillHeight: true
            cacheBuffer: 100
            spacing: 10
            model: listmodel

            delegate: Rectangle {
                width: parent.width
                radius: 5
                anchors.horizontalCenter: parent.horizontalCenter
                height: 40

                RowLayout {
                    id: colLayout
                    anchors.fill: parent

                    Text {
                        id: nameTxtItemName
                        text: ItemName
                        wrapMode: Text.WordWrap
                    }

                    Text {
                        id: nameTxtItemCode
                        text: ItemCode
                    }

                    Text {
                        id: nameTxtStateCode
                        text: StateCode
                    }
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
}


/*##^## Designer {
    D{i:10;anchors_height:100;anchors_width:100}
}
 ##^##*/
