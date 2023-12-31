extern  WiFiManager wm;   

void handleRoot()
{
wm.server->send(200, "text/html", 
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"<meta charset='utf-8'>\n"
"<title>传图</title>\n"
"<link rel='stylesheet' href='styles.css'>\n"
"<script src='processingA.js'></script>\n"
"<script src='processingB.js'></script>\n"
"<script src='processingC.js'></script>\n"
"<script src='processingD.js'></script>\n"
"</head>\n"
"<body>\n"
"<div class='header_back'>\n"
"<div class='header_face' id='logTag' ></div>\n"
"</div>\n"
"<div class='content_back'>\n"
"<div class='content_face'>\n"
"<table style='width:100%; height:100%' border='0' cellspacing='0'>\n"

"<td class='menu_bar' rowspan='2'>\n"
"<div id='BT'></div>\n"
"<div id='RB'></div>\n"

"<table>\n"       
"<tr><td class='comment'>偏移:</td></tr>\n" 
"<tr id='X'></tr>\n"
"<tr id='Y'></tr>\n"
//"<tr id='WH'></tr>\n"
"</table>\n"
  
"<div id='BT1'></div>\n"     
"</td>\n"

"<td id='dstBox' class='content_body'></td>\n"  
  
"<tr>\n"
"<td class='content_body'>\n"
"<canvas id='source' class='hidden_input'></canvas>\n"        
"<div id='srcBox'>\n"
"<div class='imgBox'>\n"
"<div class='mesBox'>Drop image here</div>\n"
"</div>\n"
"</div>\n"            
"</td>\n"      
"</tr>\n"

"</table>\n"
"</div>\n"
"</div>\n"
"<div class='footer_back'>\n"
"<div class='footer_face'>\n"
"</div>\n"
"</div>\n"
"</body>\n"
"</html>\n");
}

void sendCSS()
{
wm.server->send(200, "text/css", 
".menu_button {\n"
"width: 100%;\n"
"height: 70px;\n"
"border-radius: 4px;\n"
"text-align: center;\n"
"cursor: pointer;\n"
"display: block;\n"
"margin-bottom:40px;\n"
"margin-top:40px;\n"
"font: 32px/70px Tahoma;\n"
"transition: all 0.18s ease-in-out;\n"
"border: 1px solid #4FD666;\n"
"background: linear-gradient(to top right, #3EC97A, #69EA49 20%, rgba(255, 255, 255, 0) 80%, rgba(255, 255, 255, 0)) top right/500% 500%;\n"
"color: green;\n"
"}\n"

".menu_button:hover {\n"
"color: white;\n"
"background-position: bottom left;\n"
"}\n"

".hidden_input {\n"
"width: 0.1px;\n"
"height: 0.1px;\n"
"opacity: 0;\n"
"overflow: hidden;\n"
"position: absolute;\n"
"z-index: -1;\n"
"}\n"

".imgBox\n"
"{\n"
"border: 2px dashed green;\n"
"border-radius: 8px;\n"
"background: lightyellow;\n"
"background-repeat: no-repeat;\n"
"text-align: center;\n"
"margin-top:5px;\n"
"height: 500px;\n"
"}\n"

".sourceImage\n"
"{\n"
"max-width: 960px;\n"
"}\n"

".mesBox {\n"
"margin: 50px 35px;\n"
"color: green;\n"
"font-size: 25px;\n"
"font-family: Verdana, Arial, sans-serif;\n"
"}\n"

".header_back\n"
"{\n"
"width:100%;\n"
"height:50px;\n"
"background:#EEEEEE;\n"
"}\n"

".content_back\n"
"{\n"
"width:100%;\n"
"background:#EEEEEE;\n"
"}\n"

".footer_back\n"
"{\n"
"width:100%;\n"
"height:50px;\n"
"background:#EEEEEE;\n"
"}\n"

".header_face\n"
"{\n"
"width:1150px;\n"
"height:100%;\n"
"margin:0 auto;\n"
"background:#CCFFCC;\n"
"}\n"

".content_face\n"
"{\n"
"width:1150px;\n"
"height:100%;\n"
"margin:0 auto;\n"
"background:#FFFFFF;\n"
"}\n"

".footer_face\n"
"{\n"
"width:1150px;\n"
"height:100%;\n"
"margin:0 auto;\n"
"background:#CCFFCC;\n"
"}\n"

".menu_bar\n"
"{\n"
"border-right:1px solid #4FD666;\n"
"background:#EEFFEE;\n"
"vertical-align:top;\n"
"padding:10px;\n"
"width:150px;\n"
"height:100%\n"
"}\n"

".content_body\n"
"{\n"
"vertical-align:top;\n"
"text-align:center;\n"
"height:100%;\n"
"padding:15px;\n"
"padding-left:5px;\n"
"}\n"

".title\n"
"{\n"
"color:green;\n"
"font-size:32px;\n"
"font-family: arial;\n"
"}\n"

".comment\n"
"{\n"
"width:100%;\n"
"color:green;\n"
"font-size:32px;\n"
"font-style:italic;\n"
"font-family: arial;\n"
"text-align:center;\n"
"}\n"

".nud\n"
"{\n"
"width:50px;\n"
"}\n"); 
}
