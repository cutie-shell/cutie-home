import QtQuick
import Qt5Compat.GraphicalEffects
import QtQuick.Controls
import QtMultimedia
import Cutie
import Cutie.Feedback
import Cutie.Wlc

Item {
    id: root
    state: "appSwitcher" 
    visible: true
    width: Screen.width
    height: Screen.height

    states: [
        State{
            name: "appSwitcher"
            PropertyChanges { target: appSwitcher; opacity: 1 }
            PropertyChanges { target: notificationScreen; opacity: 0 }
        },
        State {
            name: "notificationScreen"
            PropertyChanges { target: appSwitcher; opacity: 0 }
            PropertyChanges { target: notificationScreen; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            to: "*"
            NumberAnimation { target: notificationScreen; properties: "opacity"; duration: 300; easing.type: Easing.InOutQuad; }
            NumberAnimation { target: appSwitcher; properties: "opacity"; duration: 300; easing.type: Easing.InOutQuad; }
        }
    ]
    
    Connections {
        target: notifications
        function onNotificationAdded(id, summary, body, feedbackHints) {
            // Check if sound should be suppressed
            if (feedbackHints["suppress-sound"] === true) {
                return; // Don't trigger any feedback
            }
            
            CutieFeedback.trigger(Application.name, "message-new-instant", feedbackHints, -1);
        }
    }
    
    ForeignToplevelManagerV1 {
        id: toplevelManager
    }

    CutieWlc {
        id: cutieWlc
    }

    Image {
        id: wallpaper
        width: Screen.width
        height: Screen.height
        source: "file:/" + Atmosphere.path + "/wallpaper.jpg"
        fillMode: Image.PreserveAspectCrop
        visible: true
    }

    FastBlur {
        id: wallpaperBlur
        anchors.fill: wallpaper
        source: wallpaper
        radius: 70
        visible: true
        opacity: Math.max(cutieWlc.blur, notificationScreen.opacity)
    }

    AppSwitcher { id: appSwitcher }
    NotificationScreen { id: notificationScreen }
    ScreenSwipe { id: screenSwipe }
}
