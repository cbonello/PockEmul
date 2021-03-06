import QtQuick 2.0

Item {
    id: root

    property string name
    property bool isBrand: (model.idpocket.substring(0, 1)==='#')
    property bool installed: model.installed || isBrand
    property bool isSelected: listView.currentIndex === index

    width: parent.width //parent ? parent.width : imageItem.width
    height: parent.width /3 //Math.max(imageItem.height,500) //parent.height/2)
    z: isSelected ? 1000 : -index
    rotation: isSelected ? 0 : -15
    scale: isSelected ? 1 : 0.25 //mainView.height/540 : mainView.height/1080
    opacity: 1.0 - Math.abs((listView.currentIndex - index) * 0.25)

    Behavior on rotation {
        NumberAnimation { duration: 500; easing.type: Easing.OutBack }
    }
    Behavior on scale {
        NumberAnimation { duration: 1500; easing.type: Easing.OutElastic }
    }
    Behavior on opacity {
        NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
    }

    MouseArea {
        id: entryMouseArea
        anchors.fill: parent
        onClicked: select();
    }

    function select(){
        //            console.log("*****",model.brand,model.idpocket);

        var _b = model.brand;
//        if (model.idpocket.substring(0, 1)==='#') {
        if (isBrand) {
            // drill down into brand
            var _brand = model.idpocket;
            var _res = _brand.split('#');
            //                console.log(_brand,_res[0],_res[1],_res[2]);

            pobjectsmodel.brandsearch.push(_res[2]);
            pobjectsmodel.reload();
        }
        else {
            if (model.installed)
            {
                main.loadPocket(model.idpocket);
                showRoom.visible=false;
                showRoom.connectorsearch = '';
                launched();
            }
            else {
                if (cloud.askDialog(
                            "This model is not included in the default PockEmul package.\n"+
                            "Do you want to download it from the PockEmul Cloud ?",2) == 1) {
                    console.log("DOWNLOAD");

                    // Show Cloud
                    cloudShow();

                    // Select Cloud tab
                    thecloud.tabbedui.tabClicked(1);

                    // Fill search field
                    thecloud.search_text = "standard_"+model.res+"_package";
                    showroomNew.mainview.select();

                }
            }

        }
            //            if (isSelected) {
            //                detailsView.image = model.image
            //                detailsView.name =  model.name
            //                detailsView.year = model.year
            //                detailsView.director = model.director
            //                detailsView.cast = model.cast
            ////                detailsView.rating = model.rating
            //                detailsView.overview = model.overview
            //                detailsView.show();
            //            } else {
            //                listView.currentIndex = index;
            //                if (settings.showShootingStarParticles) shootingStarBurst.burst(50);
            //            }

    }
    Image {
        id: imageItem
        width: parent.width/2
        height: width *3/4 //parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
        source: main.getRes(model.image)
        visible: !settings.showLighting
    }

    ShaderEffectSource {
        id: s1
        sourceItem: imageItem
        hideSource: settings.showLighting
        visible: settings.showLighting
    }

    ShaderEffect {
        anchors.fill: imageItem
        property variant src: s1
        property variant srcNmap: coverNmapSource
        property real widthPortition: mainView.width/imageItem.width
        property real heightPortition: mainView.height/imageItem.height
        property real widthNorm: widthPortition * 0.5 - 0.5
        property real heightNorm: root.y/imageItem.height - listView.contentY / imageItem.height
        property real lightPosX: listView.globalLightPosX * widthPortition - widthNorm
        property real lightPosY: listView.globalLightPosY * heightPortition - heightNorm
        visible: settings.showLighting

        fragmentShader: "
            uniform sampler2D src;
            uniform sampler2D srcNmap;
            uniform lowp float qt_Opacity;
            varying highp vec2 qt_TexCoord0;
            uniform highp float lightPosX;
            uniform highp float lightPosY;
            void main() {
                highp vec4 pix = texture2D(src, qt_TexCoord0.st);
                highp vec4 pix2 = texture2D(srcNmap, qt_TexCoord0.st);
                highp vec3 normal = normalize(pix2.rgb * 2.0 - 1.0);
                highp vec3 light_pos = normalize(vec3(qt_TexCoord0.x - lightPosX, qt_TexCoord0.y - lightPosY, 0.8 ));
                highp float diffuse = max(dot(normal, light_pos), 0.2);

                // boost a bit
                diffuse *= 2.5;

                highp vec3 color = diffuse * pix.rgb;
                gl_FragColor = vec4(color, pix.a) * qt_Opacity;
            }
        "
    }
}
