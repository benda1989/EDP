#include "conf.h"

extern  WiFiManager wm;   

void sendJS_A()
{
wm.server->send(200, "text/javascript", 
"var srcBox,srcImg,dstImg;\n"
"var epdArr,epdInd,palArr,epdSize;\n"
"var curPal;\n"
"function getElm(n){return document.getElementById(n);}\n"
"function setInn(n,i){ document.getElementById(n).innerHTML=i;}\n"

"function processFiles(files){\n"
    "var file=files[0];\n"
    "var reader=new FileReader();\n"
    "srcImg=new Image();\n"
    "reader.onload=function(e){\n"
        "setInn('srcBox','<img id=\"imgView\" class=\"sourceImage\">');\n"
        "var img=getElm('imgView');\n"
        "img.src=e.target.result;\n"
        "srcImg.src=e.target.result;\n"
        "srcImg.onload = function () {\n"
          "let w = srcImg.width;\n"
          "let h = srcImg.height;\n"
          "let scale = dH / h;\n"
          "if (dW / dH > w / h) scale = dW / w;\n"
          "sW = Math.trunc(w * scale);\n"
          "sH = Math.trunc(h * scale);\n"
          "source = getElm('source');\n"
          "source.width = sW;\n"
          "source.height = sH;\n"
          "source.getContext('2d').drawImage(srcImg, 0, 0, sW, sH);\n"
        "}\n"
    "};\n"
    
    "reader.readAsDataURL(file);\n"
"}\n"
//-------------------------------------------
"function drop(e){\n"
    "e.stopPropagation();\n"
    "e.preventDefault();\n"
    "var files=e.dataTransfer.files;\n"
    "processFiles(files);\n"
"}\n"
//-------------------------------------------
"function ignoreDrag(e){\n"
    "e.stopPropagation();\n"
    "e.preventDefault();\n"
"}\n"
//-------------------------------------------
"function getNud(nm,vl){\n"
    "return '<td class=\"comment\">'+nm+':</td>'+\n"
    "'<td><input id=\"nud_'+nm+'\" class=\"nud\"type=\"number\" value=\"'+vl+'\"/></td>';\n"
"}\n"
//-------------------------------------------
"function Btn(nm,tx,fn){\n"
    "return '<div><label class=\"menu_button\" for=\"_'+nm+'\">'+tx+'</label>'+\n"
    "'<input class=\"hidden_input\" id=\"_'+nm+'\" type=\"'+\n"
    "(nm==0?'file\" onchange=\"':'button\" onclick=\"')+fn+'\"/></div>';\n"
"}\n"
//-------------------------------------------
"function RB(vl,ss, tx){\n"
    "return '<input type=\"radio\" style=\"zoom:2.5;\" name=\"kind\" value=\"m'+vl+\n" 
  "'\" onclick=\"rbClick('+vl+\",\"+ss+');\"'+(ss=="+String(D_EPD,DEC)+"?'checked=\"true\"':'')+'/>'+tx;\n"
"}\n"
//-------------------------------------------
"window.onload = function(){\n"
    "srcBox = getElm('srcBox');\n"
    "srcBox.ondragenter=ignoreDrag;\n"
    "srcBox.ondragover=ignoreDrag;\n"
    "srcBox.ondrop=drop;\n"
    "srcImg=0;\n"
    "epdInd=0;\n"
    "epdSize=0;\n"
"palArr=[[[0,0,0],[255,255,255]],\n"
"[[0,0,0],[255,255,255],[127,0,0]],\n"
"[[0,0,0],[255,255,255],[127,127,127]],\n"
"[[0,0,0],[255,255,255],[127,127,127],[127,0,0]],\n"
"[[0,0,0],[255,255,255]],\n"
"[[0,0,0],[255,255,255],[220,180,0]],\n"
"[[0,0,0]],\n"
"[[0,0,0],[255,255,255],[0,255,0],[0,0,255],[255,0,0],[255,255,0],[255,128,0]]];\n"

"setInn('BT',\n"
"Btn(0,'选择图片','processFiles(this.files);'));\n"

"setInn('BT1',\n"
"Btn(1,'处理','procImg(false,true);')+\n"
"Btn(2,'上传','uploadImage();'));\n"

"setInn('X',getNud('上下','0'));\n"
"setInn('Y',getNud('左右','0'));\n"
//"setInn('WH',getNud('w','200')+getNud('h','200'));\n"

"epdArr=[[128,296,5],[400,300,1],[640,384,1],[960,640,1]];\n"
  
//"setInn('RB',\n"
//"RB(0,29,'2.9寸')+\n"
//"RB(1,42,'4.2寸')+\n"
//"RB(2,75,'7.5寸')+\n"
//"RB(3,102,'10.2寸'));\n"

"rbClick("+String(D_EPDI,DEC)+','+String(D_EPD,DEC)+");\n"

"}\n"
//-------------------------------------------
"function rbClick(index, ss){\n"
//    "getElm('nud_w').value=""+epdArr[index][0];\n"
//    "getElm('nud_h').value=""+epdArr[index][1];\n"
    "dW = epdArr[index][0];\n"
    "dH = epdArr[index][1];\n"
    "epdInd=index;\n"
    "epdSize=ss;\n"
"}\n");
}

void sendJS_B()
{
wm.server->send(200, "text/javascript", 
"var source;\n"
"var dX, dY, dW, dH, sW, sH;\n"
//-------------------------------------------
"function getVal(p, i){\n"
    "if((p.data[i]==0x00) && (p.data[i+1]==0x00))return 0;\n"
    "if((p.data[i]==0xFF) && (p.data[i+1]==0xFF))return 1;\n"
    "if((p.data[i]==0x7F) && (p.data[i+1]==0x7F))return 2;\n"
    "return 3;\n"
"}\n"
//-------------------------------------------
"function setVal(p,i,c){\n"
    "p.data[i]=curPal[c][0];\n"
    "p.data[i+1]=curPal[c][1];\n"
    "p.data[i+2]=curPal[c][2];\n"
    "p.data[i+3]=255;\n"
"}\n"

//-------------------------------------------
"function addVal(c,r,g,b,k){\n"
    "return[c[0]+(r*k)/32,c[1]+(g*k)/32,c[2]+(b*k)/32];\n"
"}\n"
//-------------------------------------------
"function getErr(r,g,b,stdCol){\n"
    "r-=stdCol[0];\n"
    "g-=stdCol[1];\n"
    "b-=stdCol[2];\n"
    "return r*r + g*g + b*b;\n"
"}\n"
//-------------------------------------------
"function getNear(r,g,b){\n"
    "var ind=0;\n"
    "var err=getErr(r,g,b,curPal[0]);\n"
    "for (var i=1;i<curPal.length;i++)\n"
    "{\n"
        "var cur=getErr(r,g,b,curPal[i]);\n"
        "if (cur<err){err=cur;ind=i;}\n"
    "}\n"
    "return ind;\n"
"}\n");
}

void sendJS_C()
{
wm.server->send(200, "text/javascript",
"function procImg(isLvl,isRed){\n"
    "if (document.getElementsByClassName('sourceImage').length == 0){\n"
        "alert('First select image');\n"
        "return;\n"
    "}\n"
    
    "var palInd=epdArr[epdInd][2];\n"
    
    "if (isRed&&((palInd&1)==0)){\n"
        "alert('This white-black display');\n"
        "return;\n"
    "}\n"
    
    "if (!isRed)palInd=palInd&0xFE;\n"
    "curPal=palArr[palInd];\n"
    "getElm('dstBox').innerHTML=\n"
    "'<canvas id=\"canvas\"></canvas>';\n" 
    "var canvas=getElm('canvas');\n"
//    "sW=srcImg.width;\n"
//    "sH=srcImg.height;\n"
//    "source=getElm('source');\n"
//    "source.width=sW;\n"
//    "source.height=sH;\n"
//    "source.getContext('2d').drawImage(srcImg,0,0,sW,sH);\n"
//    "dW=parseInt(getElm('nud_w').value);\n"
//    "dH=parseInt(getElm('nud_h').value);\n"
    "dY=parseInt(getElm('nud_上下').value);\n"
    "dX=parseInt(getElm('nud_左右').value);\n"

    
    "if((dW<3)||(dH<3)){\n"
        "alert('Image is too small');\n"
        "return;\n"
    "}\n"
    
    "canvas.width=dW;\n"
    "canvas.height=dH;\n"
    "var index=0;\n"
    "var pSrc=source.getContext('2d').getImageData(0,0,sW,sH);\n"
    "var pDst=canvas.getContext('2d').getImageData(0,0,dW,dH);\n"
    
    "if(isLvl){\n"
        "for (var j=0;j<dH;j++){\n"
            "var y=dY+j;\n"
            "if ((y<0)||(y>=sH)){\n"
                "for (var i=0;i<dW;i++,index+=4) setVal(pDst,index,(i+j)%2==0?1:0);\n"
                "continue;\n"
            "}\n"
            
            "for (var i=0;i<dW;i++){\n"
                "var x=dX+i;\n"
                
                "if ((x<0)||(x>=sW)){\n"
                    "setVal(pDst,index,(i+j)%2==0?1:0);\n"
                    "index+=4;\n"
                     "continue;\n"
                 "}\n"
                 
                 "var pos=(y*sW+x)*4;\n"     
                 "setVal(pDst,index,getNear(pSrc.data[pos],pSrc.data[pos+1],pSrc.data[pos+2]));\n"
                 "index+=4;\n"
             "}\n"
         "}\n"
    "}else{\n"
        "var aInd=0;\n"
        "var bInd=1;\n"
        "var errArr=new Array(2);\n"
        "errArr[0]=new Array(dW);\n"
        "errArr[1]=new Array(dW);\n"
        
        "for (var i=0;i<dW;i++)\n"
            "errArr[bInd][i]=[0,0,0];\n"
            
        "for (var j=0;j<dH;j++){\n"
            "var y=dY+j;\n"
            
            "if ((y<0)||(y>=sH)){\n"
                "for (var i=0;i<dW;i++,index+=4)setVal(pDst,index,(i+j)%2==0?1:0);\n"  
                "continue;\n"
            "}\n"
            
            "aInd=((bInd=aInd)+1)&1;\n"
            "for (var i=0;i<dW;i++)errArr[bInd][i]=[0,0,0];\n"
            
            "for (var i=0;i<dW;i++){\n"
                "var x=dX+i;\n"
                
                "if ((x<0)||(x>=sW)){\n"
                    "setVal(pDst,index,(i+j)%2==0?1:0);\n"
                    "index+=4;\n"
                    "continue;\n"
                "}\n"
                
                "var pos=(y*sW+x)*4;\n"
                "var old=errArr[aInd][i];\n"
                "var r=pSrc.data[pos  ]+old[0];\n"
                "var g=pSrc.data[pos+1]+old[1];\n"
                "var b=pSrc.data[pos+2]+old[2];\n"
                "var colVal = curPal[getNear(r,g,b)];\n"
                "pDst.data[index++]=colVal[0];\n"
                "pDst.data[index++]=colVal[1];\n"
                "pDst.data[index++]=colVal[2];\n"
                "pDst.data[index++]=255;\n"
                "r=(r-colVal[0]);\n"
                "g=(g-colVal[1]);\n"
                "b=(b-colVal[2]);\n"
                
                "if (i==0){\n"
                    "errArr[bInd][i  ]=addVal(errArr[bInd][i  ],r,g,b,7.0);\n"
                    "errArr[bInd][i+1]=addVal(errArr[bInd][i+1],r,g,b,2.0);\n"
                    "errArr[aInd][i+1]=addVal(errArr[aInd][i+1],r,g,b,7.0);\n"
                "}else if (i==dW-1){\n"
                    "errArr[bInd][i-1]=addVal(errArr[bInd][i-1],r,g,b,7.0);\n"
                    "errArr[bInd][i  ]=addVal(errArr[bInd][i  ],r,g,b,9.0);\n"
                "}else{\n"
                    "errArr[bInd][i-1]=addVal(errArr[bInd][i-1],r,g,b,3.0);\n"
                    "errArr[bInd][i  ]=addVal(errArr[bInd][i  ],r,g,b,5.0);\n"
                    "errArr[bInd][i+1]=addVal(errArr[bInd][i+1],r,g,b,1.0);\n"
                    "errArr[aInd][i+1]=addVal(errArr[aInd][i+1],r,g,b,7.0);\n"
                "}\n"
            "}\n"
        "}\n"
    "}\n"
    
    "canvas.getContext('2d').putImageData(pDst,0,0);\n"
"}\n");
}

void sendJS_D()
{
wm.server->send(200, "text/javascript",
"var pxInd,stInd;\n"
"var dispW,dispH;\n"
"var xhReq,dispX;\n"
"var rqMsg;\n"

"var prvPx,prvSt;\n"
"function ldPrv(){if(xhReq.status!=200){pxInd=prvPx;stInd=prvSt;}}\n"
"function svPrv(){prvPx=pxInd;prvSt=stInd;}\n"

"function byteToStr(v){return String.fromCharCode((v & 0xF) + 97, ((v >> 4) & 0xF) + 97);}\n"
"function wordToStr(v){return byteToStr(v&0xFF) + byteToStr((v>>8)&0xFF);}\n"
//-------------------------------------------
"function u_send(cmd,next){\n"
    "xhReq.open('POST',cmd, true);\n"
    "xhReq.send();\n"
    "if(next)stInd++;\n"
    "return 0;\n" 
"}\n"
//-------------------------------------------
"function u_next(){\n"
    "lnInd=0;\n"
    "pxInd=0;\n"
    "u_send('NEXT',true);\n"
"}\n"
//-------------------------------------------
"function u_done(){\n"
    "setInn('logTag','Complete!');\n"
    "return u_send('SHOW',true);\n"
"}\n"
//-------------------------------------------
"function u_load(a,k1,k2){\n"
    "var x=''+(k1+k2*pxInd/a.length);\n"
    "if(x.length>5)x=x.substring(0,5);\n"
    "setInn('logTag','Progress: '+x+'%');\n"
     "xhReq.open('POST','LOAD', true);\n"
  "xhReq.send(rqMsg+wordToStr(rqMsg.length)+'LOAD');\n"
  "if(pxInd>=a.length)stInd++;\n"
  "return 0;\n"
"}\n"

//-------------------------------------------
"function u_dataA(a,c,k1,k2)\n"
"{\n"
    "rqMsg='';\n"
    "svPrv();\n"

    "if(c==-1)\n"
    "{\n"
        "while((pxInd<a.length)&&(rqMsg.length<3000))\n"
        "{\n"
            "var v=0;\n"

            "for (var i=0;i<16;i+=2)\n"
            "{\n"
                "if(pxInd<a.length)v|=(a[pxInd]<<i);\n"
                "pxInd++;\n"
            "}\n"

            "rqMsg += wordToStr(v);\n"    
        "}\n"
    "}\n"
  "else{\n"  
        "let flage = false ;\n"
        "if (c == 1) {\n"
          "flage = true;\n"
          "c = 3;\n"
        " }\n"
        "while((pxInd<a.length)&&(rqMsg.length<3000))\n"
        "{\n"
           "var v=0;\n"

           "for (var i=0;i<8;i++)\n"
           "{\n"
                "if((pxInd<a.length)&&(a[pxInd]!=c))v|=(128>>i);\n"
                "pxInd++;\n"
            "}\n"
            "if (flage) v = ~v & 0xFF;\n"
            "rqMsg += byteToStr(v);\n"
        "}\n"
    "}\n"
  
    "return u_load(a,k1,k2);\n"
"}\n"
//-------------------------------------------
"function u_dataB(a,c,k1,k2){\n"
    "var x;\n"
    "rqMsg='';\n"
    "svPrv();\n"

    "while(rqMsg.length<3000)\n"
    "{\n"
        "x=0;\n"

        "while(x<122)\n"
        "{\n"
            "var v=0;\n"
            "for (var i=0;(i<8)&&(x<122);i++,x++)if(a[pxInd++]!=c)v|=(128>>i);\n"
            "rqMsg += byteToStr(v);\n" 
        "}\n"
    "}\n"
    
    "return u_load(a,k1,k2);\n" 
"}\n" 
//-------------------------------------------
"function imageRevert (source, dest) {\n"
  "for (var i = 0, h = source.height; i < h; i++) {\n"
    "for (var j = 0, w = source.width; j < w; j++) {\n"
      "dest.data[i * w * 4 + j * 4 + 0] =source.data[(h - i)  * w * 4 + j* 4 + 0];\n"
      "dest.data[i * w * 4 + j * 4 + 1] =source.data[(h - i)  * w * 4 + j* 4 + 1];\n"
      "dest.data[i * w * 4 + j * 4 + 2] =source.data[(h - i)  * w * 4 + j* 4 + 2];\n"
      "dest.data[i * w * 4 + j * 4 + 3] =source.data[(h - i)  * w * 4 + j* 4 + 3];\n"
    "}\n"
  "}\n"
  "return dest;\n"
"}\n"
//-------------------------------------------
"function uploadImage(){\n"
    "var c=getElm('canvas');\n"
    "var w=dispW=c.width;\n"
    "var h=dispH=c.height;\n"
    "var p=c.getContext('2d').getImageData(0,0,w,h);\n"
    "var p1=c.getContext('2d').getImageData(0,0,w,h);\n"
    "if(epdSize == 102) p = imageRevert(p,p1)\n"
    "var a=new Array(w*h);\n"
    "var i=0;\n"
    "for(var y=0;y<h;y++)for(var x=0;x<w;x++,i++) {\n"
      "a[i]=getVal(p,i<<2);\n"
    "}\n"
    "dispX=0;\n"
    "pxInd=0;\n"
    "stInd=0;\n"
    "xhReq=new XMLHttpRequest();\n"
    "var init='EPD';\n"
    
  "if (epdSize == 75){\n"           
        "xhReq.onload=xhReq.onerror=function(){\n"
            "ldPrv();\n"
            "if(stInd==0)return u_dataA(a,-1,0,100);\n"
            "if(stInd==1)return u_done();\n"
        "};\n"
        
        "xhReq.open('POST',init, true);\n"
        "xhReq.send(byteToStr(epdSize));\n"
        "return 0;\n"
    "}\n"

    "else{\n"
        "xhReq.onload=xhReq.onerror=function(){\n"
            "ldPrv();\n"
            "if(stInd==0)return u_dataA(a,0,0,50);\n"
            "if(stInd==1)return u_next();\n"
            "if(stInd==2)return u_dataA(a,(epdSize == 102) ? 1 : 3,50,50);\n" 
            "if(stInd==3)return u_done();\n"
        " };\n"
         "xhReq.open('POST',init, true);\n"
        "xhReq.send(byteToStr(epdSize));\n"
        "return 0;\n"
    "}\n"
"}\n");
}