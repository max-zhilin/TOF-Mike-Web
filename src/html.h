/*
  ESP32 HTML WebServer Page Code
  http:://www.electronicwings.com
*/

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <style>
    body {font-family: sans-serif;}
    h1 {text-align: center; font-size: 30px;}
    p {text-align: center; color: #120a8f; font-size: 60px;}
  </style>

<body>
  <h1>VL53L1X Laser Distance Measurement. BuzNel Lab (c)</h1><br>
  <p>Pallet Height : <span id="_palHeight">0</span> cm</p>
  <p>Mounted Sensor Height : <span id="_sensHeight">0</span> cm</p>
  <p>Real Measured Distance : <span id="_realDist">0</span> cm</p>
 

<script>
  setInterval(function() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        const text = this.responseText;
        const myArr = JSON.parse(text);
        document.getElementById("_palHeight").innerHTML = myArr[0];
        document.getElementById("_sensHeight").innerHTML = myArr[1];
	      document.getElementById("_realDist").innerHTML = myArr[2];
      }
    };
    xhttp.open("GET", "readHightData", true);
    xhttp.send();
  },50);
</script>
</body>
</html>
)rawliteral";
