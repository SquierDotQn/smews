var ajaxTarget = 'joystick';
var x = 0;
var y = 0;
var clic = false;
function ajaxCallback(status,joystick){
	if(status == 200 && joystick != 0) {
		//joystick = parseInt(joystick);
		document.getElementById("message").innerHTML=joystick;
		var div = document.getElementById('joystick-table').getElementsByTagName('div');
		for(var i=0; i<div.length; i++){
			div[i].className = "";
		}
		clic = false;
		if(joystick == 0){
		}
		else if(joystick == 2){
			// BAS
			y -= 1;
		}
		else if(joystick == 4){
			// GAUCHE
			x -= 1;
		}
		else if(joystick == 6){
			// DROITE
			x += 1;
		}
		else if(joystick == 8){
			// HAUT
			y += 1;
		}
		else if(joystick == 5){
			// MILIEU
			clic = true;
		}
		doAjax(ajaxTarget+"?joystick="+joystick,ajaxCallback,0,0);
	}
}

function run(){
	document.getElementById("joystick").innerHTML = "";
	joystick = Number(document.getElementById("message").value);
	if(isNaN(joystick))
		joystick = -1;
	
	doAjax(ajaxTarget+"?joystick="+joystick,ajaxCallback,0,0);
}

function doAjax(ajaxTarget,ajaxCallBack,timeout,timeoutCallBack){
	var xhr;
	var xhrTimer;
	try{ xhr = new XMLHttpRequest(); }
	catch(e){ xhr = new ActiveXObject('Microsoft.XMLHTTP'); }
	xhr.onreadystatechange = function(){
		if(xhr.readyState == 4) {
			ajaxCallBack(xhr.status,xhr.responseText);
			if(timeout > 0)
				clearTimeout(xhrTimer);
		}
	};
	xhr.open("GET", ajaxTarget, true);
	if(timeout > 0)
		xhrTimer = setTimeout(function() { xhr.abort(); timeoutCallBack();}, timeout);
	xhr.send(null);
}

function ajaxGet(url) {
	var xhr;
	try{ xhr = new XMLHttpRequest(); }
	catch(e){ xhr = new ActiveXObject('Microsoft.XMLHTTP'); }

	xhr.onreadystatechange = function(){
		if(xhr.readyState  == 4){
			if(ajaxRet=xhr.status==200)
				ajaxCallback(xhr.responseText);
		}
	};
	xhr.open("GET", url, true);
	xhr.send(null);
}
