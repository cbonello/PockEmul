
import QtQuick 2.0
//import QtQuick.Controls 2.4

Rectangle {
    id: delegate

    property string name
    property string labelString
    property string type
    property bool saveInput: true
    property alias buttonElementEnabled: buttonElement.enabled
    property alias inputText: inputElement.text
    property alias defaultText: inputElement.defaultText
    property alias echoMode: inputElement.echoMode

    property int margin : 10

    signal buttonClicked

     Component.onCompleted: {
//         console.log("settingsDelegate",name,delegate.ListView,delegate.ListView.view);
     }

    width: parent ? parent.width : 0; //delegate.ListView.view.width;
    height: cloud.getValueFor("hiResRatio","1") *
            (type == "input") ? inputElement.height + 40:
            (type == "action")? buttonElement.height +40 : labelElement.height +40

    border.color: "lightgrey"
    border.width: 1


    LineInput {
        id: inputElement
        objectName: name
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        x: (margin )
        label: labelString
        font.pointSize: 16
        onInputAccepted: if (saveInput) cloud.saveValueFor(key,value)
        visible: (type == "input")
    }
    ComboBox {

        visible: (type=="combo")
    }

    Rectangle {
        width: parent.width
        height: labelElement.height +40
        anchors.verticalCenter: parent.verticalCenter
        x:  0
        color: "black"
        opacity: 1
        visible: (type == "text")
    }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        x: margin
        y:2
        spacing: 10
        Text {
            id: labelElement
            text: labelString
            color: (delegate.ListView.isCurrentItem | (type == "text")) ? "white" : "black"
            font { family: "Helvetica"; pointSize: 16; bold: false }
            anchors.verticalCenter: parent.verticalCenter
            visible: ((type == "text") | (type == "checkbox"))
            renderType: Text.NativeRendering
        }

        TextButton {
            id: buttonElement
            text: labelString
            objectName: name
            font.pointSize: 16
            visible: (type=="action")
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                buttonClicked();
            }
        }
    }

       SwitchCustom {
           objectName: name
           anchors.right: parent.right
           anchors.rightMargin: 20
           anchors.verticalCenter: parent.verticalCenter

           bheight: buttonElement.height*.5
           bwidth: bheight * 2
           on: (labelString=="checked") ? true:false
           onToggleState: if (saveInput) cloud.saveValueFor(key,value)
           visible: (type == "checkbox")
       }

//    Switch {
//        objectName: name
//        anchors.right: parent.right
//        anchors.verticalCenter: parent.verticalCenter
//        checked: cloud.getValueFor(objectName, defaultText) === "on" //(labelString=="checked") ? true:false
//        onCheckedChanged: {
//            console.log("toggle check:"+name);
//            if (saveInput) cloud.saveValueFor(name,checked ? "on":"off");
//            buttonClicked();
//        }
//        visible: (type == "checkbox")

//    }

//    Rectangle {
//        width: delegate.width; height: 1; color: "#cccccc"
//        anchors.bottom: delegate.bottom
//        visible: delegate.ListView.isCurrentItem ? false : true
//    }
//    Rectangle {
//        width: delegate.width; height: 1; color: "white"
//        visible: delegate.ListView.isCurrentItem ? false : true
//    }

}
