
var lat = 38.4063;
var long = -110.792286;


var roverLat = 0;
var roverLong = 0;
var roverRotation = 0;

var wayPoints = [];
var rov_marker = new L.FeatureGroup();

var wayPointLatLong;
var clickCoords;
var wayPointName;
var clickPopup = L.popup();
var map = L.map('mapid').setView([lat, long], 17);
var _wayPointNumber = 0;

function getWayPointNumber() {
    if (!document.getElementById("_wayPointName").value)
        return _wayPointNumber;
    else
        return document.getElementById("_wayPointName").value;
}

L.tileLayer.wms(
    "https://services.nationalmap.gov/arcgis/services/USGSImageOnlyLarge/MapServer/WMSServer?",
    {
        layers: "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32",
        useCache: false,
        crossOrigin: true,
        useOnlyCache: false
    }
).addTo(map);
L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

var snazzyCircle = L.icon({
    iconUrl: 'qrc:/img/marker.png',
    iconSize: [25,25],
    iconAnchor: [12.5,12.5],
    popupAnchor: [0,-13]

});

L.control.scale().addTo(map);

function deleteMarkers(lines = true, markers = true) {
    for (var i = 0;i < wayPoints.length;i++){
        if (lines && wayPoints[i])
            map.removeLayer(wayPoints[i][1]);
        if (markers && wayPoints[i])	
            map.removeLayer(wayPoints[i][2]);
    }
    wayPoints = [];
}
//shamelessly copied from Ravindranath Akila on stack overflow. This is the haversine formula
function measure(lat1, lon1, lat2, lon2){  // generally used geo measurement function
    var R = 6378.137; // Radius of earth in KM
    var dLat = lat2 * Math.PI / 180 - lat1 * Math.PI / 180;
    var dLon = lon2 * Math.PI / 180 - lon1 * Math.PI / 180;
    var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
    Math.cos(lat1 * Math.PI / 180) * Math.cos(lat2 * Math.PI / 180) *
    Math.sin(dLon/2) * Math.sin(dLon/2);
    var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
    var d = R * c;
    return d * 1000; // meters
}
function placeWayPoint(name, clickAble = false, lat = 0, long = 0){
    var marker;
    var poly;
    var x;
    var y;
    if (!lat && !long){
        x = document.getElementById("_lat").value;
        y = document.getElementById("_long").value;
    }
    else {
        x = lat;
        y = long;
    }
    //use clicked way point
    if (clickAble){
        marker = L.marker(wayPointLatLong, {

        }).addTo(map);
        marker.bindPopup("Waypoint: " + name + "</br>" + wayPointLatLong.toString());
        poly = L.polygon([
            wayPointLatLong,
            [roverLat, roverLong]
        ]).addTo(map);
        var newMarker = [
            name,
            poly,
            marker,
            wayPointLatLong
        ];
        wayPoints.push(newMarker);
        marker.on("click", function () {
            wayPointName = name;
            document.getElementById("clickedName").innerHTML = "Name of waypoint: " + name;
            var dist = measure(wayPointLatLong.lat, wayPointLatLong.lng, roverLat, roverLong);
            document.getElementById("clickedDistInfo").innerHTML = "Distance from Rover: [" + dist + "] m";
        });
    }
    //use manual entry
    else {
        marker = L.marker([x,y], {

        }).addTo(map);
        marker.bindPopup("Waypoint: " + name + "</br>" + L.latLng(x, y).toString());
        poly = L.polygon([
            L.latLng(x,y),
            [roverLat, roverLong]
        ]).addTo(map);                   
        var newMarker = [
            name,
            poly,
            marker,
            L.latLng(x,y)
        ];
        wayPoints.push(newMarker);
        marker.on("click", function () {
            wayPointName = name;
            document.getElementById("clickedName").innerHTML = "Name of waypoint: " + name;
            var dist = measure(L.latLng(x,y).lat, L.latLng(x,y).lng, roverLat, roverLong);
            document.getElementById("clickedDistInfo").innerHTML = "Distance from Rover: [" + dist + "] m";
        });
    }
}

function changeText(clickAble ) {
    _wayPointNumber++;
    var name = document.getElementById("_wayPointName").value
    if (!name)
        name = _wayPointNumber;
    placeWayPoint(name, clickAble);
};


function checkCoordWithClick(e) {
    clickPopup.setLatLng(e.latlng);
    clickPopup.setContent("Clicked Waypoint:</br>" + e.latlng.toString());
    clickPopup.openOn(map);
    clickCoords = e.latlng;
    wayPointLatLong = e.latlng;
};
map.on('click', checkCoordWithClick);

var webChannel = new QWebChannel(qt.webChannelTransport, function(channel) {
    console.log("marker made");
    channel.objects.gps.pushRoverCoords.connect(function(){
        
        var marker = L.marker([channel.objects.gps.coords[0], channel.objects.gps.coords[1]], {
            rotationAngle: channel.objects.gps.coords[2],
            icon: snazzyCircle
        });
        
        rov_marker.clearLayers();
        
        rov_marker.bindPopup("Rover:</br>" + "LatLng(" + roverLat + ", " + roverLong + ")");
        
        roverLat = channel.objects.gps.coords[0];
        roverLong = channel.objects.gps.coords[1];
        roverRotation = channel.objects.gps.coords[2];

        for (var i = 0;i < wayPoints.length;i++){
            if (wayPoints[i]) {
                if (wayPoints[i][1])
                    map.removeLayer(wayPoints[i][1]);
                var poly = L.polygon([
                    wayPoints[i][3],
                    [roverLat, roverLong]
                ]).addTo(map);
                wayPoints[i][1] = poly;
            }  
        }
        rov_marker.addLayer(marker);
        map.addLayer(rov_marker);
     });
});

function deleteClicked(){
    var idx;
    for (var i = 0;i < wayPoints.length;i++){
        if (wayPoints[i][0] == wayPointName) {
            map.removeLayer(wayPoints[i][1]);
            map.removeLayer(wayPoints[i][2]);
            wayPoints[i] = 0;
            idx = i;
            break;
        }
    }
};

