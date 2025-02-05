import { useState, useEffect } from 'react';
import './App.css';
import { io } from 'socket.io-client';

// Establish a socket connection to the server
const socket = io("https://esp8266-led-backend.onrender.com");
// const socket = io("http://localhost:3000");

function App() {
    const [status, setStatus] = useState({ light: false, fan: false });

    useEffect(() => {
        // Listen for messages from the server
        socket.on('message', (message) => {
            console.log(`Received: ${message}`);
            const [device, action] = message.split('_');
            setStatus((prevState) => ({
                ...prevState,
                [device.toLowerCase()]: action === "ON",
            }));
        });

        return () => {
            // Clean up the socket listener when the component is unmounted
            socket.off('message');
        };
    }, []);

    const sendCommand = (device, action) => {
        const command = `${device}_${action}`;
        // Send the command to the server
        socket.emit('message', command);
        setStatus({ ...status, [device.toLowerCase()]: action === "ON" });
    };

    return (
        <div>
            <h1>Home Automation</h1>
            <button onClick={() => sendCommand("LIGHT", status.light ? "OFF" : "ON")}>
                {status.light ? "Turn Light OFF" : "Turn Light ON"}
            </button>
            {/* <button onClick={() => sendCommand("FAN", status.fan ? "OFF" : "ON")}>
                {status.fan ? "Turn Fan OFF" : "Turn Fan ON"}
            </button> */}
        </div>
    );
}

export default App;
