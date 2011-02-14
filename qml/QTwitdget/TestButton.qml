import Qt 4.7

Item {
    id: container

    signal clicked

    property string text
    property bool keyUsing: false

    BorderImage {
        id: buttonImage
        width: container.width
        height: container.height
        border.left: 15
        border.top: 4
        border.bottom: 4
        border.right: 15
        source: "images/test_toolbutton.png"
    }

    BorderImage {
        id: pressed
        width: container.width
        height: container.height
        border.left: 15
        border.top: 4
        border.bottom: 4
        border.right: 15
        opacity: 0
        source: "images/test_toolbutton.png"
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: buttonImage
        onClicked: {
            container.clicked();
        }
    }

    Text {
         id: btnText
         color: if(container.keyUsing){"#DDDDDD";} else {"#FFFFFF";}
         anchors.centerIn: buttonImage; font.bold: true
         text: container.text; style: Text.Raised; styleColor: "black"
         font.pixelSize: 12
    }

    states: [
        State {
            name: "Pressed"
            when: mouseRegion.pressed == true
            PropertyChanges { target: pressed; opacity: 1 }
        }
    ]

    transitions: Transition {
        ColorAnimation { target: btnText; }
    }
}
