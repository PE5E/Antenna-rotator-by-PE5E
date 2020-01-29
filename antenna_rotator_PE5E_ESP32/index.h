const char index_html[] PROGMEM = R"==+==(
<HTML>
	<HEAD>
		<TITLE>Antenna rotator by PE5E</TITLE>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
    <script>
      window.onload = function() {
       var buttons = document.getElementById("rotate-div");
       var manual = document.getElementById("button_manual");
       var automatic = document.getElementById("button_auto");
       var action_arrow = document.getElementById("action_arrow");
       if("%MANUAL_CONTROL_TEXT%" === "manually") {
        manual.style.opacity = "0.6";
        automatic.style.opacity = "1";
        buttons.style.display = "block";
       }
       else if("%MANUAL_CONTROL_TEXT%" === "automatically") {
        automatic.style.opacity = "0.6";
        manual.style.opacity = "1";
        buttons.style.display = "none";
       }
        if("%DIRECTION_STATUS%" === "standing still") {
        action_arrow.style.display = "none";
       }
      }
    </script>
    <style>
      * {
        font-family: Garamond;
        color: #ffc380;
      }
      h2 {
        padding: 0px;
        margin: 0px;
      }
      h3 {
        padding: 0px;
        margin: 0px;
      }
      .compass
      {
        position: absolute;
        top: 150px;
        left: 40px;
        z-index: 1;
      }
      .arrow
      {
        position: absolute;
        top: 150px;
        left: 183px;
        z-index: 2;
      }
      .elevations
      {
        position: absolute;
        top: 150px;
        left: 370px;
        z-index: 1;
      }
      .arrow_left
      {
        position: absolute;
        top: 438px;
        left: 370px;
        z-index: 2;
      }
      .arrow_action
      {
        position: absolute;
        top: 300px;
        left: 730px;
        z-index: 1;
      }
      .cur_azi
      {
        position: absolute;
        top: 455px;
        left: 90px;
      }
      .cur_ele
      {
        position: absolute;
        top: 455px;
        left: 400px;
      }
      .cur_action
      {
        position: absolute;
        top: 455px;
        left: 725px;        
      }
      .speed_bar_front
      {
        height: 20px;
        width: %ROTATION_SPEED%px;
        background-color: #990011;
        position: absolute;
        top: 150px;
        left: 725px;
        z-index: 2
      }
      .speed_bar_back
      {
        height:20px;
        width: 255px;
        background-color: #008811;
        position: absolute;
        top: 150px;
        left: 725px;
        z-index: 1
      }
      .speed_text
      {
        position: absolute;
        top: 180px;
        left: 725px;
      }
  </style>
  <style>
      input[type=button], input[type=submit] {
      width: 130px;
      padding: 10px 10px;
      margin: 3px;
      border-radius: 4px;
      box-sizing: border-box;
      border: none;
      color: black;
      background-color: #cc7000;
      }
  </style>
  <meta http-equiv="refresh" content="60; URL=/">
</HEAD>
<BODY style="padding:20px;background-color:#004880">
  
<H1 style="margin-left:30px">Antenna rotator by PE5E</H1>

<div style="padding:10px;border:1px solid;border-radius:5px">
<H3>Status</H3>
<br><br><br><br><br><br><br><br><br><br>
<br><br><br><br><br><br><br><br><br>
  
<div class="cur_azi">Current azimuth:    %CURRENT_AZIMUTH%
<br>Requested azimuth: %REQUESTED_AZIMUTH%</div>
<div class="cur_ele">Current elevation:    %CURRENT_ELEVATION%
<br>Requested elevation: %REQUESTED_ELEVATION%</div>
<div class="speed_bar_front"></div>
<div class="speed_bar_back"></div>
<div class="speed_text">Speed: %ROTATION_SPEED%</div>
<div class="cur_action">Rotator status:
<br>%DIRECTION_STATUS%</div>
<br>
</div>
<br>

%ERROR_CODE_HTML%
<br>

<div style="padding:10px;border:1px solid;border-radius:5px">
<H3>Control source</H3>
<br>
The rotator is now controlled %MANUAL_CONTROL_TEXT%
<form action = "/action" method="get" id="control_mode">
  <input type="submit" name="controlmode" value="MANUAL" id="button_manual">
  <input type="submit" name="controlmode" value="AUTOMATIC" id="button_auto">
</form>
</div>
<br>

<div id="rotate-div" style="padding:10px;border:1px solid;border-radius:5px">
<H3>Actions</H3>
<br>
<form action = "/action" method="get" id="rotate_buttons">
  <input type="submit" name="direction" value="CW->RIGHT">
  <input type="submit" name="direction" value="CCW->LEFT">
  <input type="submit" name="direction" value="UP">
  <input type="submit" name="direction" value="DOWN">
  <input type="submit" name="direction" value="STOP">
</form>
<br>

<form action = "/action" method="get" id="request_azi_ele">
  Request azimuth: <input type="text" value="%CURRENT_AZIMUTH%" name="azi" style="background:black;margin:3"><br>
  Request elevation: <input type="text" value="%CURRENT_ELEVATION%" name="ele" style="background:black;margin:3"><br>
  <input type="submit" value="Request">
</form>
<br>

<form action = "/action" method="get" id="request_speed">
  Set new maximum speed percentage:  <input type="number" min="1" max="255" step="1" value="%ROTATION_SPEED%" name="set_speed" style="background:black;margin:3"><br>
  <input type="submit" value="Set speed">
</form>
<br>

</div>
<br><br>

<img src="https://raw.githubusercontent.com/PE5E/Antenna-rotator-by-PE5E/master/images/compass.jpg" height="300" width="300" class="compass">
<img src="https://raw.githubusercontent.com/PE5E/Antenna-rotator-by-PE5E/master/images/red_arrow_up.png" height="300" width="14" class="arrow" data-rotate=%CURRENT_AZIMUTH% data-xaxis="center" data-yaxis="center">
<img src="https://raw.githubusercontent.com/PE5E/Antenna-rotator-by-PE5E/master/images/0-90deg.jpg" height="300" width="300" class="elevations">
<img src="https://raw.githubusercontent.com/PE5E/Antenna-rotator-by-PE5E/master/images/red_arrow_left.png" height="14" width="300" class="arrow_left" data-rotate=%CURRENT_ELEVATION% data-xaxis="right" data-yaxis="center">
<img src="https://raw.githubusercontent.com/PE5E/Antenna-rotator-by-PE5E/master/images/red_arrow_up.png" height="100" width="100" class="arrow_action" data-rotate=%CURRENT_ACTION_DEG% data-xaxis="center" data-yaxis="center" id="action_arrow">

<script>
  $('img').each(function() {
      var xaxis = $(this).data('xaxis') || "center";
      var yaxis = $(this).data('yaxis') || "center";
      var origin = $(this).data('xaxis') + ' ' + $(this).data('yaxis');
      
      var deg = $(this).data('rotate') || 0;
      var rotate = 'rotate(' + $(this).data('rotate') + 'deg)';
      $(this).css({ 
          '-webkit-transform': rotate,
          '-webkit-transform-origin': origin,
          '-moz-transform': rotate,
          '-moz-transform-origin': origin,
          '-o-transform': rotate,
          '-o-transform-origin': origin,
          '-ms-transform': rotate,
          '-ms-transform-origin': origin,
          'transform': rotate,
          'transform-origin': origin
      });
  });
  // example: http://jsfiddle.net/verashn/6rRnd/5/
</script>
</BODY>
</HTML>)==+==";



// )==+==";
