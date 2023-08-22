// Simulated sensor data
const sensorData = [
    { id: "sensor1", x: 100, y: 150 },
    { id: "sensor2", x: 100, y: 150 },
    { id: "sensor3", x: 100, y: 150 },
    { id: "sensor4", x: 100, y: 150 },
    { id: "sensor5", x: 100, y: 150 },
    { id: "sensor6", x: 100, y: 150 },
    // Add more sensor data here
];

// Simulate updating sensor values
function updateSensorValues() {
    sensorData.forEach(sensor => {
        const sensorElement = document.getElementById(sensor.id);
        const randomValue = Math.floor(Math.random() * 100); // Replace with actual sensor data
        sensorElement.querySelector('.sensor-value').textContent = randomValue;
    });

    setTimeout(updateSensorValues, 500); // Update values every second
}

// Run initial sensor update
updateSensorValues();
