// const String MAIN_page = R"==+==(

const char index_html[] PROGMEM = R"==+==(
<HTML>
	<HEAD>
		<TITLE>Antenna rotator by PE5E</TITLE>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
    <style>
      .compass
      {
        position:absolute;
        top: 50px;
        left: 40px;
        z-index: 1;
      }
      .arrow
      {
        position:absolute;
        top: 50px;
        left: 183px;
        z-index: 2;
      }
      .elevations
      {
        position:absolute;
        top: 100px;
        left: 420px;
        z-index: 1;
      }
      .arrow_left
      {
        position:absolute;
        top: 291px;
        left: 420px;
        z-index: 2;
      }
  </style>
  <meta http-equiv="refresh" content="10" >
	</HEAD>
<body style="background-color:MidnightBlue;">
  <p style="color:DarkKhaki">
  
  <b>     Antenna rotator by PE5E</b>
  <br><br><br>
  <br><br><br><br><br><br><br><br><br><br><br>
  <br><br><br><br><br><br><br><br><br><br><br>

  
Current azimuth: %CURRENT_AZIMUTH%, current elevation: %CURRENT_ELEVATION%
<br>

<img src="https://raw.githubusercontent.com/PE5E/ESP8266/master/antenna_rotator_PE5E/images/compass.jpg" height="300" width="300" class="compass">
<img src="https://raw.githubusercontent.com/PE5E/ESP8266/master/antenna_rotator_PE5E/images/red_arrow_up.png" height="300" width="14" class="arrow" data-rotate=%CURRENT_AZIMUTH% data-xaxis="center" data-yaxis="center">
<img src="https://raw.githubusercontent.com/PE5E/ESP8266/master/antenna_rotator_PE5E/images/0-90deg.jpg" height="200" width="200" class="elevations">
<img src="https://raw.githubusercontent.com/PE5E/ESP8266/master/antenna_rotator_PE5E/images/red_arrow_left.png" height="10" width="200" class="arrow_left" data-rotate=%CURRENT_ELEVATION% data-xaxis="right" data-yaxis="center">
<br><br> 

Move the antenna:
<form action = "/action" method="get" id="rotate_buttons">
  <input type="submit" name="direction" value="UP">
  <input type="submit" name="direction" value="DOWN">
  <input type="submit" name="direction" value="CCW->LEFT">
  <input type="submit" name="direction" value="CW->RIGHT">
  <input type="submit" name="direction" value="STOP">
</form>
<br>
Antenna moving status: %DIRECTION_STATUS%
<br>

</p>

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
</body>
</HTML>)==+==";



// )==+==";
