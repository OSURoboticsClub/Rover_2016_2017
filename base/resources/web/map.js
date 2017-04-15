// TODO: refactor this!!!




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
var map = L.map('map').setView([lat, long], 17);

var _wayPointNumber = 0;

function getWayPointNumber() {
    if (!gps.name)
        return _wayPointNumber;
    else
        return gps.name;
}
L.tileLayer.wms(
            "https://services.nationalmap.gov/arcgis/services/USGSImageOnlyLarge/MapServer/WMSServer",
            {
                layers: "0,1,2,3,4,5,6,7,8,9,10",
                useCache: true,
                crossOrigin: true,
                useOnlyCache: false
            }
            ).addTo(map);

L.tileLayer.wms('https://basemap.nationalmap.gov/arcgis/services/USGSImageryOnly/MapServer/WMSServer', {
                    layers: "0",
                    maxZoom: 15,
                    useCache: true,
                    crossOrigin: true,
                    useOnlyCache: false
                }).addTo(map);

var icon = L.icon({
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
    gps.name = '';
    gps.dist = '[]';
}

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
    gps.name = '';
    gps.dist = '[]'
};

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

function placeWayPoint(name, clickAble = false){
    var marker;
    var poly;
    var x;
    var y;
    if (gps._inputLatitude != null && gps._inputLongitude != null){
        x = gps._inputLatitude;
        y = gps._inputLongitude;
    }
    else {
        x = 0;
        y = 0;
    }
    //use clicked way point
    if (clickAble){
        marker = L.marker(wayPointLatLong, {

                          }).addTo(map);
        map.closePopup();

        var li = document.createElement("list");
        var p1 = document.createElement("p");
        p1.appendChild(document.createTextNode( "Waypoint: " + name));
        p1.appendChild(document.createElement("br"));
        p1.appendChild(document.createTextNode(wayPointLatLong.toString()));
        var button = document.createElement("button");
        button.innerHTML = "delete";
        button.onclick = function(){deleteClicked()};
        button.style.textAlign="center";
        li.appendChild(p1);
        li.appendChild(button);

        clickPopup.setContent(li);
        marker.bindPopup(clickPopup);
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
            gps.name = name;
            var dist = measure(wayPointLatLong.lat, wayPointLatLong.lng, roverLat, roverLong);
            gps.dist = " [" + dist + "] m";
        });
    }
    //use manual entry
    else {
        marker = L.marker([x,y], {

                          }).addTo(map);
        map.closePopup();
        var popUp = L.popup();
        var li = document.createElement("list");
        var p1 = document.createElement("p");
        p1.appendChild(document.createTextNode( "Waypoint: " + name));
        p1.appendChild(document.createElement("br"));
        p1.appendChild(document.createTextNode(L.latLng(x, y).toString()));
        var button = document.createElement("button");
        button.innerHTML = "delete";
        button.onclick = function(){deleteClicked()};
        li.appendChild(p1);
        li.appendChild(button);

        popUp.setContent(li);
        marker.bindPopup(popUp);
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
            gps.name = name;
            var dist = measure(L.latLng(x,y).lat, L.latLng(x,y).lng, roverLat, roverLong);
            gps.dist = " [" + dist + "] m";
        });
    }
}

function changeText(clickAble ) {
    _wayPointNumber++;
    var name = gps._inputName;
    if (!name)
        name = _wayPointNumber;
    placeWayPoint(name, clickAble);
};


function checkCoordWithClick(e) {

    var li = document.createElement("list");
    var p1 = document.createElement("p");
    p1.appendChild(document.createTextNode( "Clicked Waypoint:"));
    p1.appendChild(document.createElement("br"));
    p1.appendChild(document.createTextNode(e.latlng.toString()));
    var button = document.createElement("button");
    button.innerHTML = "add";
    button.onclick = function(){changeText(true)};
    li.appendChild(p1);
    li.appendChild(button);

    clickPopup.setLatLng(e.latlng);
    clickPopup.setContent(li);
    clickPopup.openOn(map);
    clickCoords = e.latlng;
    wayPointLatLong = e.latlng;
};
map.on('click', checkCoordWithClick);

var webChannel = new QWebChannel(qt.webChannelTransport, function(channel) {
    console.log("marker made");
    window.gps = channel.objects.gps;
    gps._pushRoverCoords.connect(function(){
        
        var marker = L.marker([gps.coords[0], gps.coords[1]], {
                                  rotationAngle: gps.coords[2],
                                  icon: snazzyCircle
                              });
        
        rov_marker.clearLayers();
        
        rov_marker.bindPopup("Rover:</br>" + "LatLng(" + roverLat + ", " + roverLong + ")");
        
        roverLat = gps.coords[0];
        roverLong = gps.coords[1];
        roverRotation = gps.coords[2];

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
    gps._submitButton.connect(function(){
        changeText(false);
    });
    gps._addFromClicked.connect(function(){
        changeText(true);
    });
    gps._deleteClicked.connect(function(){
        deleteClicked();
    });
    gps._deleteAll.connect(function() {
        deleteMarkers();
    });
});


