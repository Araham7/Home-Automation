/* This is server code  */

import { createServer } from 'http'; 
import express from 'express';
import { Server } from 'socket.io';

const app = express();
const server = createServer(app);
const io = new Server(server, {
    cors: {
        origin: "*", // Change this to your frontend's domain for security
        methods: ["GET", "POST"],
        credentials: true, // Allow credentials (cookies, auth headers, etc.)
    },
});

io.on('connection', (socket) => {
    console.log('Client connected');

    socket.on('message', (message) => {
        console.log(`Received: ${message}`);
        // Broadcast the message to all clients
        io.emit('data', message);
        // io.emit('event_name', data);
    });

    socket.on('disconnect', () => {
        console.log('Client disconnected');
    });
});

const PORT = 3000;
server.listen(PORT, () => {
    console.log(`Socket.io server running on http://localhost:${PORT}`);
});


