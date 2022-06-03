const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
  
  <style>
  body{
  margin:0;
  padding: 0;
  color:#1c1c1c;
  font-family: roboto;
  position: relative;
  text-align:center;
}

p{margin:0;padding:0;}

.clr{
  clear:both;
}

@font-face {
  font-family: quartz;
  src: url(fonts/quartz.ttf);
}

#bg_mob{
  background-color: #f4f4f4;
  max-width: 540px;
  margin:0 auto;
  height:100vh;
}
.mob_inner{
  padding: 15px;
}

#espRange {
  -webkit-appearance: none;
  width: 100%;
  height: 15px;
  border-radius: 5px;
  background: #0e3d79;
  outline: none;
}

.slideresp:hover {
  opacity: 1;
}

.slideresp::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 28px;
  height: 28px;
  border-radius: 50%;
  background: #00acec;
  cursor: pointer;
}

.slideresp::-moz-range-thumb {
  width: 28px;
  height: 28px;
  border-radius: 50%;
  background: #00acec;
  cursor: pointer;
}

.value{
    width: 100px;
    background: #ff7700;
    margin: 0 auto;
    padding: 5px;
    margin-top: 10px;
    color: white;
}

#temperature_sec{margin: 30px 0;}

.form_max-min input{
    padding: 6px 4px;
    border: 1px solid #c8c8c8;
}
#esp-submit{
    background-color: #ff7700;
    color: white;
    border: none;
    padding: 5px 10px;
    font-size: 16px;
}
.form_max-min{margin:35px 0;}

input[type=number]::-webkit-inner-spin-button,
input[type=number]::-webkit-outer-spin-button { 
  -webkit-appearance: none;
}
  </style>
</head>
<body>
  
  <div id="bg_mob">
    <div class="mob_inner">
      <div class="slidecontainer">
        <h2>ESP Fan Speed Controller</h2>  
        <input type="range" onchange="updateSliderPWM(this)" id="espRange" min="0" max="100" value="%SLIDERVALUE%" step="1" class="slideresp" style="height:18px;width:290px; border-radius:5px;background:#0e3d79;outline:none;">
         <p style="width: 60px;background: #ff7700;margin: 0 auto;padding: 6px 12px;color: white;"><span id="textSliderValue">%SLIDERVALUE%</span></p>
      </div>
      <hr style="margin-top: 35px;border: 1px solid #e6e5e5;">
      <div id="temperature_sec">
        <div>
          <h2><i class="fa fa-thermometer-half" style="color: #ec5a00;margin-right: 10px;"></i>Room Temperature <span id="temperature" style="background:#dddddd;padding: 3px 12px;">%TEMPERATURE%<sup class="units">&deg;C</sup></span></h2> 
        </div>
      </div>
      <hr style="margin-top: 35px;border: 1px solid #e6e5e5;">
      <form action="/get" class="form_max-min">

       <label><b>Min:&nbsp;&nbsp;&nbsp;</b> </label>
       <input type="number" id="quantity" min="10" placeholder="Min 10" name="tempMin"><br><br>

        <label><b>Max:&nbsp;&nbsp;&nbsp;</b> </label>
        <input type="number" id="quantity"  max="60" placeholder="Max 60" name="tempMax"><br><br>

        <input type="submit" id="esp-submit" value="Enable Auto Mode" onclick="submitMessage()">
      </form>
      <p style="text-align: left;background: #e8e8e8;padding: 10px;font-size: 14px;line-height: 22px;color: black;"><b>Note :</b> This device features an auto and manual mode function; users can enable auto speed control mode by inputting the desired temperature range(Min and Max Value) and clicking on the Enable Auto Mode button. If the slider is moved while auto mode is enabled the device automatically reverts back to manual speed control mode.</p>
    </div>
  </div>


<script>
function updateSliderPWM(element) {
  var sliderValue = document.getElementById("espRange").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;

  
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue, true);
  xhr.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 5000 ) ;


function submitMessage() {
      alert("Saved value to ESP SPIFFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }

</script>
</body>
</html>
)rawliteral";
