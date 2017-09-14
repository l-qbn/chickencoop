//Converted with http://www.percederberg.net/tools/text_converter.html to C-String Text and saved as char

const char indexPage[] PROGMEM = {
  "<!DOCTYPE html>\n<html>\n<title>My Chicken Coop</title>\n<body>\n<p style=\"width:100%;height:80px;text-align:center;font-size:55px;\">My Chicken Coop</p>\n<input type=\"button\" onclick='location.href=\"/door\";' value=\"Door status\" style=\"width:100%; height:80px;font-size:55px;\"><br><br>\n<input type=\"button\" onclick='location.href=\"/open\";' value=\"OPEN door\" style=\"width:100%; height:80px;font-size:55px;\"><br><br>\n<input type=\"button\" onclick='location.href=\"/close\";' value=\"CLOSE door\" style=\"width:100%; height:80px;font-size:55px;\"><br><br>\n<input type=\"button\" onclick='location.href=\"/fixopen\";' value=\"Fix OPEN time\" style=\"width:100%; height:80px;font-size:55px;\"><br><br>\n<input type=\"button\" onclick='location.href=\"/temp\";' value=\"Temperature\" style=\"width:100%; height:80px;font-size:55px;\"><br><br>\n<input type=\"button\" onclick='location.href=\"/time\";' value=\"Time\"style=\"width:100%; height:80px;font-size:55px;\"><br><br>\n<input type=\"button\" onclick='location.href=\"/sun\";' value=\"Sunrise / Sunset\"style=\"width:100%; height:80px;font-size:55px;\"><br><br>\n<input type=\"button\" onclick='location.href=\"/battery\";' value=\"Battery\"style=\"width:100%; height:80px;font-size:55px;\"><br><br>\n</body>\n</html>\n\n\0"
  
};

const char timeSet[] PROGMEM = {
  "<!DOCTYPE html>\n<html>\n<body>\n<a href=\"/\">Home</a><br><form action=\"/timesetdone\">\n<input type=\"number\" name=\"year\" id=\"y\"> Year<br>\n<input type=\"number\" name=\"month\" id=\"m\"> Month<br>\n<input type=\"number\" name=\"day\" id=\"d\"> Day<br>\n<input type=\"number\" name=\"hour\" id=\"h\"> Hour<br>\n<input type=\"number\" name=\"minute\" id=\"min\"> Minute<br>\n<input type=\"submit\" value=\"SET\">\n</form>\n<script>\nvar d = new Date();\ndocument.getElementById(\"y\").value = d.getFullYear();\ndocument.getElementById(\"m\").value = d.getMonth()+1;\ndocument.getElementById(\"d\").value = d.getDate();\ndocument.getElementById(\"h\").value = d.getHours();\ndocument.getElementById(\"min\").value = d.getMinutes()+1;\n</script>\n</body>\n</html>\n\0"
};

const char fixOpenTimeSet[] PROGMEM = {
"<!DOCTYPE html>\r\n<html>\r\n<body>\r\n<h1><a href=\"/\">Home</a><br><form action=\"/fixosetdone\">\r\n<input type=\"number\" name=\"hour\" id=\"h\"> Hour<br>\r\n<input type=\"number\" name=\"minute\" id=\"min\"> Minute<br>\r\n<input type=\"checkbox\" name=\"fixopen\" id=\"fixo\"> Fix open ON<br>\r\n<input type=\"submit\" value=\"SET\">\r\n</form>\r\n</h1><script>\r\nvar d = new Date();\r\ndocument.getElementById(\"h\").value = d.getHours();\r\ndocument.getElementById(\"min\").value = d.getMinutes()+1;\r\n</script>\r\n</body>\r\n</html>\r\n\0"

};



