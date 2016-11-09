import QtQuick 2.0
import QtPositioning 5.5
import QtLocation 5.5
import QtQuick.Extras 1.4

Map {
    id: map
    plugin: Plugin {
        name: "mapbox"
        PluginParameter {
            name: "mapbox.access_token";
            value: "pk.eyJ1IjoibmlraXRhcm96IiwiYSI6ImNpcXZvbDJldjAwMXpmbmtxcmFqbjVpd2kifQ.DNyLQnVkmb2WFALuOkIBow"
        }
        PluginParameter {
            name: "mapbox.map_id";
            value: "mapbox.satellite"
        }
    }

    center {
        latitude: -55.7512
        longitude: 37.6175
    }

    Component.onCompleted: {
            console.log(map.center);
    }

    zoomLevel: 16;
    gesture.enabled: true
}

