// Define the WebSocket gateway URL based on the current window's hostname
var gateway = `ws://${window.location.hostname}/ws`;

// Declare a variable to store the WebSocket connection
var websocket;

// Add an event listener for the 'load' event, which triggers the 'onload' function when the page loads
window.addEventListener('load', onload);

// Function to handle the 'load' event
function onload(event) {
    // Initialize the WebSocket connection when the page loads
    initWebSocket();
}

// Function to send a message "getValues" to the server via the WebSocket connection
function getValues(){
    websocket.send("getValues");
}

// Function to initialize the WebSocket connection
function initWebSocket() {
    // Log a message indicating an attempt to open a WebSocket connection
    console.log('Trying to open a WebSocket connection…');

    // Create a new WebSocket instance with the defined gateway URL
    websocket = new WebSocket(gateway);

    // Event handler for when the WebSocket connection is successfully opened
    websocket.onopen = onOpen;

    // Event handler for when the WebSocket connection is closed
    websocket.onclose = onClose;

    // Event handler for when a message is received through the WebSocket connection
    websocket.onmessage = onMessage;
}

// Function called when the WebSocket connection is opened successfully
function onOpen(event) {
    // Log a message indicating a successful connection
    console.log('Connection opened');

    // Send a message "getValues" to the server to retrieve initial values
    getValues();
}

// Function called when the WebSocket connection is closed
function onClose(event) {
    // Log a message indicating the closure of the connection
    console.log('Connection closed');

    // Attempt to reconnect after a delay of 2 seconds (2000 milliseconds)
    setTimeout(initWebSocket, 2000);
}

// Function to update a slider's value on the web page and send the updated value to the server
function updateSliderPWM(element) {
    // Extract the slider number from the element's ID
    var sliderNumber = element.id.charAt(element.id.length-1);
    
    // Get the current value of the slider
    var sliderValue = document.getElementById(element.id).value;
    
    // Update the corresponding HTML element to display the slider value
    document.getElementById("sliderValue"+sliderNumber).innerHTML = sliderValue;
    
    // Log the slider value to the console
    console.log(sliderValue);
    
    // Send a message to the server with the slider number and its updated value
    websocket.send(sliderNumber+"s"+sliderValue.toString());
}

// Function called when a message is received through the WebSocket connection
function onMessage(event) {
    // Log the received data to the console
    console.log(event.data);
    
    // Parse the received JSON data into a JavaScript object
    var myObj = JSON.parse(event.data);
    
    // Get the keys of the object (assuming the keys correspond to HTML element IDs)
    var keys = Object.keys(myObj);

    // Loop through the keys and update corresponding HTML elements and sliders with received values
    for (var i = 0; i < keys.length; i++){
        var key = keys[i];
        
        // Update the inner HTML of the corresponding element with the received value
        document.getElementById(key).innerHTML = myObj[key];
        
        // Set the slider value to the received value
        document.getElementById("slider"+ (i+1).toString()).value = myObj[key];
    }
}
