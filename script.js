let bluetoothDevice;
let characteristic;

// Bluetooth Connection
document.getElementById('connect-bluetooth').addEventListener('click', async () => {
  try {
    bluetoothDevice = await navigator.bluetooth.requestDevice({
      filters: [{ services: ['00001101-0000-1000-8000-00805f9b34fb'] }] // Standard Serial Port Service
    });

    const server = await bluetoothDevice.gatt.connect();
    const service = await server.getPrimaryService('00001101-0000-1000-8000-00805f9b34fb');
    characteristic = await service.getCharacteristic('00001101-0000-1000-8000-00805f9b34fb');

    console.log('Bluetooth Connected');
    alert('Bluetooth Connected!');
  } catch (error) {
    console.error('Bluetooth Connection Failed:', error);
    alert('Bluetooth Connection Failed. Please try again.');
  }
});

// Send Data to Bluetooth
function sendData(data) {
  if (characteristic) {
    const encoder = new TextEncoder();
    characteristic.writeValue(encoder.encode(data))
      .then(() => console.log('Data sent:', data))
      .catch((error) => console.error('Failed to send data:', error));
  } else {
    console.error('Bluetooth not connected');
  }
}

// Joystick Logic
const joystick = document.getElementById('joystick');
const handle = document.querySelector('.joystick-handle');
let isDragging = false;

joystick.addEventListener('mousedown', () => {
  isDragging = true;
});

window.addEventListener('mousemove', (e) => {
  if (isDragging) {
    const rect = joystick.getBoundingClientRect();
    const centerX = rect.left + rect.width / 2;
    const centerY = rect.top + rect.height / 2;
    const deltaX = e.clientX - centerX;
    const deltaY = e.clientY - centerY;
    const distance = Math.min(Math.hypot(deltaX, deltaY), 40); // Limit distance to 40px

    // Move joystick handle
    handle.style.transform = `translate(${deltaX}px, ${deltaY}px)`;

    // Send rover movement commands
    if (distance > 10) {
      if (Math.abs(deltaX) > Math.abs(deltaY)) {
        if (deltaX > 0) {
          sendData('RIGHT');
        } else {
          sendData('LEFT');
        }
      } else {
        if (deltaY > 0) {
          sendData('BACKWARD');
        } else {
          sendData('FORWARD');
        }
      }
    }
  }
});

window.addEventListener('mouseup', () => {
  isDragging = false;
  // Reset handle to center
  handle.style.transform = 'translate(-50%, -50%)';
  sendData('STOP'); // Stop the rover when joystick is released
});

// Slider Logic
const servo1 = document.getElementById('servo1');
const servo2 = document.getElementById('servo2');
const servo3 = document.getElementById('servo3');
const moistureSensor = document.getElementById('moisture-sensor');

servo1.addEventListener('input', () => {
  sendData(`SERVO1:${servo1.value}`);
});

servo2.addEventListener('input', () => {
  sendData(`SERVO2:${servo2.value}`);
});

servo3.addEventListener('input', () => {
  sendData(`SERVO3:${servo3.value}`);
});

moistureSensor.addEventListener('input', () => {
  sendData(`MOISTURE:${moistureSensor.value}`);
});