import QtQuick
import Qt5Compat.GraphicalEffects
import QtQuick.Controls
import Cutie
import Cutie.Wlc

Item {
    id: appSwitcher
    anchors.fill: parent
    opacity: 0
    enabled: root.state == "appSwitcher"

    GridView {
        id: tabListView
        anchors.fill: parent
        anchors.topMargin: 40
        model: toplevelManager.toplevels
        cellWidth: appSwitcher.width / 2
        cellHeight: appSwitcher.height / 2 + 20
        opacity: 1.0 - cutieWlc.blur

        delegate: Item {
            id: appThumb
            width: tabListView.cellWidth
            height: tabListView.cellHeight

            Item {
                id: appBg
                width: appThumb.width - 20
                height: appThumb.height - 20
                x: 10

                Item {
                    id: tileBlurMask
                    x: -appThumb.x-appBg.x-tabListView.x+tabListView.contentX
                    y: -appThumb.y-appBg.y-tabListView.y+tabListView.contentY
                    width: Screen.width
                    height: Screen.height
                    clip: true
                    visible: false
                    Rectangle {
                        width: appBg.width
                        height: appBg.height
                        x: appThumb.x+appBg.x+tabListView.x-tabListView.contentX
                        y: appThumb.y+appBg.y+tabListView.y-tabListView.contentY
                        color: "black"
                        radius: 10
                    }
                }

                OpacityMask {
                    anchors.fill: tileBlurMask
                    source: wallpaperBlur
                    maskSource: tileBlurMask
                }

                Rectangle {
                    color: Atmosphere.secondaryAlphaColor
                    anchors.fill: appBg
                    opacity: 1/3
                    radius: 10
                }

                CutieAppThumbnail {
                    id: thumbImage
                    anchors.fill: appBg
                    anchors.bottomMargin: 25
                    wlc: cutieWlc
                    toplevel: modelData
                }

                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: 25
                    clip: true

                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        height: 50
                        radius: 10
                        color: Atmosphere.primaryColor
                    }

                    CutieLabel {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 7
                        text: modelData.title
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    drag.target: appBg; drag.axis: Drag.XAxis; drag.minimumX: -parent.width; drag.maximumX: parent.width
                    onClicked: {
                        modelData.state = [ForeignToplevelHandleV1.Activated];
                    }

                    onReleased: {
                        if (Math.abs(appBg.x - 10) > parent.width / 3) {
                            appThumb.opacity = 0;
                            modelData.close();
                        } else { 
                            appThumb.opacity = 1;
                        }
                        appBg.x = 10;
                    }

                    onPositionChanged: {
                        if (drag.active) {
                            appThumb.opacity = 1 - Math.abs(appBg.x - 10) / parent.width 
                        }
                    }
                }
            }
        }
    } 
}
