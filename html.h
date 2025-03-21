#ifndef __HTML_H__
#define __HTML_H__

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <style>
      html {
        font-family: Arial;
        display:inline-block
        margin: 0px auto;
        text-align: center;
      }
    </style>
    <title>Keg Scale Config</title>
    <script>
      function onUpdateClick() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("message").innerHTML = this.responseText;
          }
        };
        var wifi_ssid = document.getElementById("wifi_ssid").value;
        var wifi_password = document.getElementById("wifi_password").value;
        var mqtt_broker = document.getElementById("mqtt_broker").value;
        var mqtt_topic = document.getElementById("mqtt_topic").value;
        var mqtt_client_id = document.getElementById("mqtt_client_id").value;
        var mqtt_port = document.getElementById("mqtt_port").value;
        xhttp.open("POST", "/updateconfig", true);
        xhttp.send(JSON.stringify({
          wifi_ssid,
          wifi_password,
          mqtt_broker,
          mqtt_topic,
          mqtt_client_id,
          mqtt_port
        }));
      }
  
      function onRebootClick() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("message").innerHTML = this.responseText;
          }
        };
        xhttp.open("POST", "/reboot", true);
        xhttp.send();
      }

      function onTareClick() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("message").innerHTML = this.responseText;
          }
        };
        xhttp.open("POST", "/tare", true);
        xhttp.send();
      }

      function onCalibrateClick() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("message").innerHTML = this.responseText;
          }
        };
        var calibration = document.getElementById("calibration").value;
        xhttp.open("POST", "/calibrate", true);
        xhttp.send(JSON.stringify({
          calibration
        }));
      }
    </script>
  </head>
  <body>
    <h1>
      Configuration
    </h1>
    <table>
      <tr>
        <td>
          WIFI SSID
        </td>
        <td>
          <input id="wifi_ssid" name="wifi_ssid" value="%WIFI_SSID%" />
        </td>
      </tr>
      <tr>
        <td>
          WIFI Password
        </td>
        <td>
          <input id="wifi_password" name="wifi_password" type="password" value="%WIFI_PASSWORD%" />
        </td>
      </tr>
      <tr>
        <td>
          MQTT Broker IP Address
        </td>
        <td>
          <input id="mqtt_broker" name="mqtt_broker" value="%MQTT_BROKER%" />
        </td>
      </tr>
      <tr>
        <td>
          MQTT Topic
        </td>
        <td>
          <input id="mqtt_topic" name="mqtt_topic" value="%MQTT_TOPIC%" />
        </td>
      </tr>
      <tr>
        <td>
          MQTT Client ID
        </td>
        <td>
          <input id="mqtt_client_id" name="mqtt_client_id" value="%MQTT_CLIENT_ID%" />
        </td>
      </tr>
      <tr>
        <td>
          MQTT Broker Port
        </td>
        <td>
          <input id="mqtt_port" name="mqtt_port" value="%MQTT_PORT%" />
        </td>
      </tr>
      <tr>
        <td>
          Weight Calibration
        </td>
        <td>
          <input id="calibration" name="calibration" />
        </td>
      </tr>
    </table>
    <input type="button" id="update" value="Update" onclick="onUpdateClick()" />
    <input type="button" id="reboot" value="Reboot" onclick="onRebootClick()" />
    <input type="button" id="tare" value="Tare" onclick="onTareClick()" />
    <input type="button" id="calibrate" value="Calibrate" onclick="onCalibrateClick()" />
    <br />
    <span id="message"></span>
  </body>
</html>
)rawliteral";

#endif /* __HTML_H__ */
