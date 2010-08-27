import Qt 4.7

Item {
    id: wrapper

    signal loginClicked(string username, string password)

    Column {
        anchors.centerIn: parent
        spacing: 20
        Column {
            spacing: 4
            Text {
                text: "Username:"
                font.pixelSize: 16
                font.bold: true
                color: "white"
                style: Text.Raised
                styleColor:  "black"
                horizontalAlignment: Qt.AlignRight
            }
            Item {
                width: 220
                height: 28
                BorderImage {
                    anchors.fill: parent
                    source: "../images/test_lineedit.png"
                    border.left: 10; border.top: 10
                    border.right: 10; border.bottom: 10
                }
                TextInput {
                    id: nameIn
                    width: parent.width - 8
                    anchors.centerIn: parent
                    maximumLength: 21
                    font.pixelSize: 16
                    font.bold: true
                    color: "#151515"; selectionColor: "green"
                    KeyNavigation.tab: passIn
                    focus: true
                }
            }
        }
        Column {
             spacing: 4
             Text {
                 text: "Password:"
                 font.pixelSize: 16; font.bold: true; color: "white"; style: Text.Raised; styleColor: "black"
                 horizontalAlignment: Qt.AlignRight
             }
             Item {
                 width: 220
                 height: 28
                 BorderImage {
                     source: "../images/test_lineedit.png"
                     anchors.fill: parent
                     border.left: 10; border.top: 10
                     border.right: 10; border.bottom: 10
                 }
                 TextInput{
                     id: passIn
                     width: parent.width - 8
                     anchors.centerIn: parent
                     maximumLength:21
                     echoMode: TextInput.Password
                     font.pixelSize: 16;
                     font.bold: true
                     color: "#151515"; selectionColor: "green"
                     KeyNavigation.tab: login
                     KeyNavigation.backtab: nameIn
                     onAccepted: login.doLogin();
                 }
             }
        }
        Row {
             spacing: 10
             TestButton {
                 width: 100
                 height: 32
                 id: login
                 keyUsing: true

                 function doLogin(){
                    loginClicked(nameIn.text, passIn.text)
                 }

                 text: "Log in"
                 KeyNavigation.tab: nameIn
                 KeyNavigation.backtab: passIn
                 Keys.onReturnPressed: login.doLogin();
                 Keys.onEnterPressed: login.doLogin();
                 Keys.onSelectPressed: login.doLogin();
                 Keys.onSpacePressed: login.doLogin();
                 onClicked: login.doLogin();
             }
         }
    }
}
