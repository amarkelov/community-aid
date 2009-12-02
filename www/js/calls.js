// stores the reference to the XMLHttpRequest object 
var xmlHttp = createXmlHttpRequestObject();  
 
// retrieves the XMLHttpRequest object 
function createXmlHttpRequestObject()  
{  
	// will store the reference to the XMLHttpRequest object 
	var xmlHttp; 

	try { 
		// if running Mozilla or other browsers except IE6
		xmlHttp = new XMLHttpRequest(); 
	} 
	catch (e) {
		// if running Internet Explorer 
		if(window.ActiveXObject) { 
		    // assume IE6 or older 
		    var XmlHttpVersions = new Array('MSXML2.XMLHTTP.6.0', 
		                                    'MSXML2.XMLHTTP.5.0', 
		                                    'MSXML2.XMLHTTP.4.0', 
		                                    'MSXML2.XMLHTTP.3.0', 
		                                    'MSXML2.XMLHTTP', 
		                                    'Microsoft.XMLHTTP'); 
		    // try every prog id until one works 
			for (var i=0; i<XmlHttpVersions.length && !xmlHttp; i++) { 
				try {  
					// try to create XMLHttpRequest object 
					xmlHttp = new ActiveXObject(XmlHttpVersions[i]); 
				}  
				catch (e) {
					xmlHttp = false;
				}
			} 
		} 
	}

	// return the created object or display an error message 
	if (!xmlHttp) { 
		alert("Error creating the XMLHttpRequest object.");
	}
	else {  
		return xmlHttp; 
	}
} 
 
// make asynchronous HTTP request using the XMLHttpRequest object  
function process() { 
	if (xmlHttp) {
		// proceed only if the xmlHttp object isn't busy 
		if (xmlHttp.readyState == 4 || xmlHttp.readyState == 0) { 
			try {
				// get the value of 'floating' element
				floating = encodeURIComponent(document.getElementById("floating").value);
				// execute the calls.php page from the server 
				xmlHttp.open("GET", "/clients_list.php?floating=" + floating, true);
				// define the method to handle server responses 
				xmlHttp.onreadystatechange = handleServerResponse; 
				// make the server request 
				xmlHttp.send(null);
			}
			catch (e) {
				alert( "Can't connect to server:\n" + e.toString());
			}
		} 
		else {
		// if the connection is busy, try again after two seconds  
			setTimeout('process()', 2000);
		}
	}
} 
 
// executed automatically when a message is received from the server 
function handleServerResponse()  
{ 
	// move forward only if the transaction has completed 
	if (xmlHttp.readyState == 4) { 
		// status of 200 indicates the transaction completed successfully 
		if (xmlHttp.status == 200) { 
			try {
				/* 
				 * extract the text retrieved from the server
				 * we are expecting ready made HTML output
				 */
				htmlResponse = xmlHttp.responseText; 
				/*
				 * update the client display using the data received from the server
				 * server returns already built HTML table with clients and timestamps
				 */
				document.getElementById("ClientList").innerHTML = htmlResponse;
				
				// restart sequence in 2 seconds
				setTimeout('process()', 2000);
			}
			catch(e) {
				alert("Error reading the response: " + e.toString());
			}
		}  
		// a HTTP status different than 200 signals an error 
		else { 
			alert("There was a problem accessing the server: " + xmlHttp.statusText); 
		} 
	} 
} 
