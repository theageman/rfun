import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Extras 1.4

Window {
	 id:mainWnd
    width: 800
	 height: 1200
	 color: "#000000"
    title: "Fun With Resistors"
    visible: true
    Rectangle {
        id:menuBar
		  width:mainWnd.width
        height:160
		  //anchors.horizontalCenter: mainWnd
        color:"steelblue"
        scale: 1

        Text {
            id: lblResistor
            color: "#ffffff"
            text: qsTr("Value")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: rectangle1.left
            anchors.leftMargin: 0
            font.bold: true
            font.pixelSize: 24
        }

        Rectangle {
            id: rectangle1
            x: 8
            width: 200
            height: 32
				color: "#474a6f"
            anchors.top: lblResistor.bottom
            anchors.topMargin: 10
            scale: 1

            TextInput {
					id: edtVal
					color: "#ffffff"
                text: qsTr("200")
                font.strikeout: false
                scale: 1
                cursorVisible: false
                anchors.fill: parent
                font.bold: true
                font.pixelSize: 24
                inputMethodHints: Qt.ImhDigitsOnly
                selectByMouse: true
            }
        }

        Text {
            id: lblESeries
            color: "#ffffff"
				text: qsTr("E")
				anchors.top: parent.top
				anchors.topMargin: 10
            anchors.left: cmbESeries.left
            anchors.leftMargin: 0
            font.bold: true
            font.pixelSize: 24
        }

        ComboBox {
            id: cmbESeries
            anchors.top: lblESeries.bottom
            anchors.topMargin: 10
            anchors.left: rectangle1.right
            anchors.leftMargin: 11
            model: ["E3", "E6", "E12", "E24", "E48", "E96", "E192"]
            currentIndex:2
        }

        Button {
            id: btnGo
				checkable: true
				checked: false
            width: 120
            text: qsTr("Go")
            anchors.top: rectangle1.bottom
            anchors.topMargin: 10
				anchors.left: rectangle1.left
				anchors.leftMargin: 0
				onClicked: {
					prg.value;
                CPP.newTask(edtVal.text.valueOf(), cmbESeries.currentIndex, edtNumR.text.valueOf());
                lblStatus.text = "calculating...";
					statusBar.color = "red";
					btnStop.enabled = true;
					btnGo.enabled = false;
					btnGo.checked = true;
            }
        }

        Button {
            id: btnStop
				enabled: false
            width: 120
            text: qsTr("Stop")
            anchors.left: btnGo.right
            anchors.leftMargin: 10
            anchors.top: rectangle1.bottom
            anchors.topMargin: 10
            onClicked: {
                CPP.stopTask();
					 lblStatus.text = "canceled";
					statusBar.color = "steelBlue";
					btnGo.enabled = true;
					btnStop.enabled = false;
					btnGo.checked = false;
            }
        }

        Text {
            id: lblNumResistors
            color: "#ffffff"
            text: qsTr("Number of resistors")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: rectangle2.left
            anchors.leftMargin: 0
            font.bold: true
            font.pixelSize: 24
        }

        Rectangle {
            id: rectangle2
            width: 100
            height: 32
				color: "#474a6f"
            anchors.top: lblNumResistors.bottom
            anchors.topMargin: 10
				anchors.left: cmbESeries.right
				anchors.leftMargin: 16
				TextInput {
					id: edtNumR
					color: "#ffffff"
					text: qsTr("2")
					anchors.fill: parent
					font.bold: true
					font.pixelSize: 24
					inputMethodHints: Qt.ImhDigitsOnly
				}
		  }

		  Text {
				id: lblFormula
				x: 8
				y: 9
				color: "#ffffff"
				text: qsTr("R = ")
				anchors.topMargin: 123
				anchors.left: rectangle1.left
				anchors.leftMargin: 0
				font.pixelSize: 24
				font.bold: true
				anchors.top: parent.top
		  }

    }
    Rectangle {
        id:statusBar
        height:64
		  y: mainWnd.height-height
		  width:mainWnd.width
		  //anchors.horizontalCenter: mainWnd.width/2
        color:"steelblue"
        scale: 1

        ProgressBar {
            id: prg
            width: 200
            maximumValue: 1000
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: lblStatus
            y: 5
            color: "#ffffff"
            text: qsTr("ready")
				clip: true
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: prg.left
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
            font.pixelSize: 24
        }
    }


    Image {
        id: img
        fillMode: Image.Pad;
        cache:false
        x: 0
        y: menuBar.height
		  width:mainWnd.width
		  height:mainWnd.height-menuBar.height-statusBar.height
        scale: 1.0
        source:"image://rpainter/image.png"
    }
    Timer {
        id:timer
        interval : 200
        running: true
        repeat: true
        onTriggered: prg.value = CPP.getProgress();
    }

    Connections {
        target: CPP
        onNewImage: {
            img.source = "image://rpainter/image.png"+Math.random(100000);
            lblStatus.text = sMsg;
			  lblFormula.text = CPP.getFormular();
			  statusBar.color = bFinal ? "steelBlue" : "red";
			  btnGo.enabled = bFinal ? true : false;
			  btnStop.enabled = bFinal ? false : true;
			  btnGo.checked = bFinal ? false : true;
        }
    }
}
