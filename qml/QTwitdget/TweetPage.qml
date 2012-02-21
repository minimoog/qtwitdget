import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: container

    property string text
    property string tweetid
    property string screenname

    signal replyClicked(string tweetid, string screenname, string tweettext)
    signal retweetClicked(string tweetid)
    signal conversationClicked(string tweetid)

    function handleEntity(entity) {
        if (entity.slice(0, 1) == '@')
            entityMentionClicked(entity.slice(1));
        else if (entity.slice(0, 1) == '#')
            entityHashtagClicked(entity);
        else if (entity.slice(0, 7) == 'http://')
            Qt.openUrlExternally(entity);
    }

    Rectangle {
        id: background

        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0; color: '#d9d9d9' }
            GradientStop { position: 1; color: '#ffffff' }
        }

        TextArea {
            id: tweetTextArea

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top

            readOnly: true
            text: container.text
            wrapMode: TextEdit.WordWrap
        }
    }

    tools: ToolBarLayout {
        ToolIcon { iconId: "toolbar-back"; onClicked: { pageStack.pop(); } }
        ToolIcon { iconId: "toolbar-reply"; onClicked: replyClicked(container.tweetid, container.screenname, container.text) }
        ToolIcon { iconId: "toolbar-refresh"; onClicked: { } }
        ToolIcon { iconId: "toolbar-favorite-mark"; onClicked: { } }
        ToolIcon { iconId: "toolbar-new-chat"; onClicked: { } }
    }
}
