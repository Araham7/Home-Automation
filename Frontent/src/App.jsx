import { useState, useEffect } from 'react';
import './App.css';
import { io } from 'socket.io-client';

// Establish a socket connection to the server
// const socket = io("http://localhost:3000");
const socket = io("https://esp8266-led-backend.onrender.com")

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
            <div id='upper-container'>
                <div id='logo'>
                    <img src="" alt="" />
                </div>
                <div id='menubar'>
                    <a href="">Documentation</a>
                    <a href="">Integrations</a>
                    <a href="">Blog</a>
                    <a href="">Need Help?</a>
                </div>
            </div>
            <div id='main-container'>
                <div id='main-container-left'>
                    <div id='main-container-left-text'>
                        <h1>Awaken</h1>
                        <h1><span>Your</span> Home</h1>
                        <h3>Open source home automation that puts local control and privacy first. Powered by a worldwide community of tinkerers and DIYers.</h3>
                    </div>
                   <div id='main-container-left-button'>
                   <button onClick={() => sendCommand("LIGHT", status.light ? "OFF" : "ON")}>
                        {status.light ? "Turn Light OFF" : "Turn Light ON"}
                    </button>
                   </div>
                   <h4>Many more features coming soon...</h4>
                </div>
                <div id='main-container-right'>
                    <img src="https://www.iconservices.biz/wp-content/uploads/2022/06/batch_Home-Automation-modified.png" alt="" />
                </div>
            </div>

            {/* <button onClick={() => sendCommand("FAN", status.fan ? "OFF" : "ON")}>
                {status.fan ? "Turn Fan OFF" : "Turn Fan ON"}
            </button> */}
        </div>
    );
}

export default App;
