import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
    width: 800; height: 480

    Component.onCompleted: {
        console.log("Settings.qml: Completed",new Date());

        if (cloud.getValueFor("username","") !== "") {
            user_login(cloud.getValueFor("username"),cloud.getValueFor("password"));
            console.log("logged:");
        }

        parse.loginChanged.connect(rootCloud.loginChanged);
        parse.currentObjectChanged.connect(rootCloud.currentObjectChanged);
    }

    VisualItemModel {
        id: visualSettingsModel

//        SettingsDelegate { id: testcombo; name: "combobox"; labelString: "Combobox"; type: "combo"; saveInput: false }
        SettingsDelegate { id: labelCloud;  name: "label1";     labelString: "Cloud settings"; type: "text"; saveInput: false }
        SettingsDelegate { id: api;         name: "api";        labelString: "API type"; type: "input"; defaultText: "parse"}
        SettingsDelegate { id: servername;  name: "serverURL";  labelString: "Cloud Server"; type: "input"; defaultText: "http://pockemul.ddns.net:6969"}
        SettingsDelegate { id: nameFld;     name: "name";       labelString: "Name"; type: "input"; }
        SettingsDelegate { id: emailFld;    name: "email";      labelString: "Email"; type: "input"; }
        SettingsDelegate { id: usernameFld; name: "username";   labelString: "User Name"; type: "input"; }
        SettingsDelegate { id: passwordFld; name: "password";   labelString: "Password"; type: "input"; echoMode: TextInput.Password; }
        SettingsDelegate { id: passwordFld2;name: "password2";  labelString: "Confirm Password"; type: "input"; echoMode: TextInput.Password; saveInput:false;}
        SettingsDelegate { name: "registercloud"; labelString: "Register PockEmul Cloud"; type: "action"; saveInput:false;
            onButtonClicked: {
                if (passwordFld.inputText != passwordFld2.inputText) {
                    api.inputText = "Passwords mismatch !.";
                    return;
                }

                user_register(nameFld.inputText,
                                   emailFld.inputText,
                                   usernameFld.inputText,
                                   passwordFld.inputText);
            }
        }
        SettingsDelegate { name: "apikey"; labelString: "Login"; type: "action"; saveInput:false;
            onButtonClicked: {
                user_login(usernameFld.inputText,passwordFld.inputText);
            }
        }
        SettingsDelegate { name: "reset"; labelString: "Reset Password"; type: "action"; saveInput:false;
            onButtonClicked: {
                parse.passwordReset(emailFld.inputText);
            }
        }
//        SettingsDelegate { id: syncEnabled; name: "syncEnabled"; labelString: "Enable file synchronization"; type: "checkbox"; defaultText: "on"}

        SettingsDelegate { id: labelAppSettings;name: "labelAppSettings";   labelString: "Application Settings"; type: "text"; saveInput: false }
        SettingsDelegate { id: soundEnabled;    name: "soundEnabled";       labelString: "Enable sound"; type: "checkbox"; defaultText: "on"}
        SettingsDelegate { id: kbSoundEnabled;  name: "kbSoundEnabled";     labelString: "Keyboard audio feedback"; type: "checkbox"; defaultText: "on"}
        SettingsDelegate { id: hiRes;           name: "hiRes";              labelString: "Enable High Resolution"; type: "checkbox"; defaultText: "on"}
        SettingsDelegate { id: hiResRatio;      name: "hiResRatio";         labelString: "Scale icons"; type: "input"; defaultText: "0"}
        SettingsDelegate { id: vibDelay;        name: "vibDelay";           labelString: "Vibrator delay(ms)"; type: "input"; defaultText: "75"}
        SettingsDelegate { id: flipOnEdge;      name: "flipOnEdge";         labelString: "Flip on Edge"; type: "checkbox"; defaultText: "on"}
        SettingsDelegate { id: trackerEnabled;  name: "trackerEnabled";     labelString: "Enable tracking activity"; type: "checkbox"; defaultText: "false"}
        SettingsDelegate { id: magnifyTouch;    name: "magnifyTouch";       labelString: "Magnify key touch"; type: "checkbox"; defaultText: "on"}

        SettingsDelegate { id: labelServer;     name: "labelServer";        labelString: "Telnet Server settings"; type: "text"; saveInput: false }
        SettingsDelegate { id: telnetPort;      name: "telnetPort";         labelString: "Telnet Server port"; type: "input"; defaultText: "4000"}
        SettingsDelegate { id: telnetEnabled;   name: "telnetEnabled";      labelString: "Enable server"; type: "checkbox"; defaultText: "on"}
        SettingsDelegate { id: telnetLocalHost; name: "telnetLocalHost";    labelString: "Local Host only"; type: "checkbox"; defaultText: "on"}

    }

    ListView {
        focus: visible
        id: categories
        anchors.fill: parent
        interactive: true;
        model: visualSettingsModel
        //            footer: quitButtonDelegate
        //        highlight: Rectangle { color: "steelblue" }
        highlightMoveVelocity: 9999999
    }
    ScrollBar {
        scrollArea: categories; height: categories.height; width: 8
        anchors.right: categories.right
    }

    function user_login(username,password) {
        parse.login(username,password);
    }

    function user_register(name,email,username,password) {
        parse.signup(username,password,email);
    }


}


