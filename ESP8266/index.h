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
        top: 10px;
        left: 400px;
        z-index: 1;
      }
      .arrow
      {
        position:absolute;
        top: 10px;
        left: 543px;
        z-index: 2;
      }
  </style>
	</HEAD>
<body style="background-color:MidnightBlue;">
  <p style="color:DarkKhaki">
  <br><br>
  
Current heading: %CURRENT_HEADING%
<br>

<img src="https://raw.githubusercontent.com/FastjacktheV/ESP8266/master/images/compass.jpg" height="300" width="300" class="compass">
<img src="https://raw.githubusercontent.com/FastjacktheV/ESP8266/master/images/red_arrow_up.png" height="300" width="14" class="arrow" data-rotate=%CURRENT_HEADING%>
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
      var deg = $(this).data('rotate') || 0;
      var rotate = 'rotate(' + $(this).data('rotate') + 'deg)';
      $(this).css({ 
          '-webkit-transform': rotate,
          '-moz-transform': rotate,
          '-o-transform': rotate,
          '-ms-transform': rotate,
          'transform': rotate 
      });
  });
</script>
</body>
</HTML>)==+==";



// )==+==";
