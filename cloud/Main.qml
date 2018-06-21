import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import "TabbedQuickApp"
import "content"
import "."
//import QtSensors 5.3 as Sensors


Rectangle {
    width: 360
    height: 640

    id: rootCloud
    signal sendWarning(string test)
    signal close

    property alias search_text: newprivateSearchItem.text
    property alias tabbedui: tabbedui

    property string serverURL: cloud.getValueFor("serverURL","http://pockemul.dscloud.me/elgg/")
    property string currentUserid: "pock emul"
    property string currentApiKey: cloud.getValueFor("apikey","0")
    property bool   isPortrait: false
//    property alias bigposter: bigposter
//    property alias publicCloudTabName: publicCloudTab.name

    property color backGroundColor: "white"
    property color textColor : "black"
    property color inputBorderColor: "black"

    property string auth_token: ""


    Component.onCompleted: {
        console.log("Main.qml: Completed",new Date());

//        showroomPocket.launched.connect(cloudHide);
//        showroomExt.launched.connect(cloudHide);
    }

    function currentObjectChanged(json) {
//        console.log("change:",JSON.stringify(json));
    }

    function loginChanged() {
        console.log(parse.userName," logged ?",parse.loggedIn);
        // Refresh pml List
        parse.pmlList();
        // Refresh messages
        // Refresh news

    }

    onWidthChanged: {
        isPortrait = cloud.isPortraitOrientation();
        //        console.log(isPortrait)
    }

    VisualItemModel {
        id: tabsModel
        Tab {
            name: "Cloud"
            icon: "pics/public-cloud-white.png"

            color: "yellow"

            SearchBox {
                id: newprivateSearchItem
                width: parent.width
                height: 50
                objectName: "searchFld"
                onTextChanged: {
                    myCloud.populate(text)
                }
            }
            PmlView2   {
                id: myCloud
                anchors.top: newprivateSearchItem.bottom
                anchors.bottom: parent.bottom
                width: parent.width
                ispublicCloud: false
                searchText: newprivateSearchItem.text
                cacheFileName: "newprivateCloud.xml"
//                xml: cloud.loadCache(cacheFileName)
            }

        }

        Tab {name: "Action"
            icon: "pics/action-white.png"
            Actions {}
        }

        Tab {name: "Settings"
            icon: "pics/settings-white.png"

            color: "red"

            Settings {
                anchors.fill: parent
            }
        }

        Tab {name: ""
            icon: "pics/back-white.png"
            MouseArea {
                anchors.fill: parent
                onClicked: close();
            }
        }
//        Tab {
//            name: "Test"
//            icon: "pics/white-about-256.png"
//            Camera {
//                anchors.fill: parent
//            }
//        }
//        Tab {name: "Portal"
//            Portal {
//                id: myPortal
////                z: 9999
//                visible: true
//                anchors.fill: parent
//            }
//        }

    }

    TabbedUI {
        id: tabbedui
        tabsHeight: 72 * cloud.getValueFor("hiResRatio","1")
        tabIndex: 0
        tabsModel: tabsModel
        quitIndex: 3
        onClose: rootCloud.close();
    }

    function save_pml(title,description) {
        var serverURL = '';
        var url = '';
        var xml = '';

        if (apiSelected==='parse') {
            xml = cloud.save();
            parse.saveCurrentSession(title,description,xml);
        }
    }

    function set_access(pmlid,access,on_success,on_failure) {

        var url = cloud.getValueFor("serverURL","")+'/parse/classes/Pml/'+pmlid;
        console.log('url:'+url);
        renderArea.showWorkingScreen();

        var data;
        var  owner = parse.userId;
        if (access == 0) {
            data = '{
                "ACL": {"'+
                    owner+'": {
                        "read": true,
                        "write": true
                    }
                }
            }';
        }
        if (access == 2) {
            data = '{
                "ACL": {"'+
                    owner+'": {
                        "read": true,
                        "write": true
                    },
                    "*": {
                        "read": true
                    }
                }
            }';
        }

        console.log(data);
        requestPut(url, data , function (o) {
            renderArea.hideWorkingScreen();

            console.log(o.readyState,o.status);
            if (o.readyState == 4 ) {
                if (o.status == 200) {
                    var obj = JSON.parse(o.responseText);
                    message.showMessage("Access rights changed",2000);
                    on_success();
                }
                else {
                    var obj = JSON.parse(o.responseText);
                    console.log(obj);
                    message.showErrorMessage(obj.error,5000);
                    on_failure();
                }
            }
        });
    }

// Migrate to PARSE
    function clone_pml(pmlid,on_success,on_failure) {
        var serverURL = cloud.getValueFor("serverURL","")+'services/api/rest/json/';
        var url = serverURL+ '?method=file.clone_pml'+
                '&file_guid='+pmlid+
                '&api_key=7118206e08fed2c5ec8c0f2db61bbbdc09ab2dfa'+
                '&auth_token='+auth_token;
        console.log('url:'+url);
        renderArea.showWorkingScreen();

        requestPost(url, "" , function (o) {
            renderArea.hideWorkingScreen();

            if (o.readyState == 4 ) {
                if (o.status==200) {
                    var obj = JSON.parse(o.responseText);
                    console.log(o.responseText);
                    if (obj.status == 0) {
                        message.showMessage("Session cloned",2000);
                        on_success();
                    }
                    else {
                        message.showErrorMessage(obj.message,5000);
                        on_failure();
                    }
                }
            }
        });
    }

    function delete_pml(pmlid,on_success,on_failure) {

        var url = cloud.getValueFor("serverURL","")+'/parse/classes/Pml/'+pmlid;

//        console.log('url:'+url);

        requestDelete(url, "" , function (o) {
            renderArea.hideWorkingScreen();
            console.log(o.readyState,o.status);
            if (o.readyState === 4 ) {
                if (o.status===200) {
                    var obj = JSON.parse(o.responseText);
                    console.log(o.responseText);
                    message.showMessage("File deleted",2000);
                    on_success();
                }
                else {
                    var obj = JSON.parse(o.responseText);
                    if (typeof obj.error != "undefined") {
                        message.showErrorMessage(obj.error,5000);
                        on_failure();
                    }
                }
            }
        });
    }

    function update_pml(title,description) {
        var serverURL = cloud.getValueFor("serverURL","")+'services/api/rest/json/';
        var url = serverURL+ '?method=file.update&'+
                '&title='+encodeURIComponent(title)+
                '&description='+encodeURIComponent(description)+
                '&api_key=7118206e08fed2c5ec8c0f2db61bbbdc09ab2dfa'+
                '&auth_token='+auth_token;
        console.log('url:'+url);
        renderArea.showWorkingScreen();

        requestPost(url, "" , function (o) {
            renderArea.hideWorkingScreen();

            if (o.readyState == 4 ) {
                if (o.status==200) {
                    var obj = JSON.parse(o.responseText);
                    console.log(o.responseText);
                    if (obj.status == 0) {
                        message.showMessage("Session updated",5000);
                    }
                    else {
                        message.showErrorMessage(obj.message,5000);
                    }
                }
            }
        });
    }


    function addRefPmlModel(_pmlid,
                            _username,
                            _keywords,
                            _ispublic,
                            _isdeleted,
                            _title,
                            _description,
                            _type) {
//        console.log("count before:"+refpmlModel.count());
        refpmlModel.append({rowid: refpmlModel.count(),
                               pmlid: _pmlid,
                               username: _username,
                               keywords: _keywords,
                               ispublic: _ispublic,
                               isdeleted: _isdeleted,
                               title: _title,
                               description: _description,
                               type: _type});
//        console.log("count after:"+refpmlModel.count());

    }

    function encodeXml(s) {
        return s.replace(/([\&"<>])/g, function(str, item) {
            var xml_special_to_escaped_one_map = {
                '&': '&amp;',
                '"': '&quot;',
                '<': '&lt;',
                '>': '&gt;'
            };
            return xml_special_to_escaped_one_map[item];
        });
    }

    function decodeXml(s) {
        return s.replace(/(&quot;|&lt;|&gt;|&amp;)/g,
            function(str, item) {
                var escaped_one_to_xml_special_map = {
                    '&amp;': '&',
                    '&quot;': '"',
                    '&lt;': '<',
                    '&gt;': '>'
                };
                return escaped_one_to_xml_special_map[item];
        });
    }

    Message {
        id:message
        anchors.top: parent.top
        anchors.left: parent.left
        width : parent.width
        height: 30
    }
}
