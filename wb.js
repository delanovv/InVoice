const net = require('net');
const Speaker = require('speaker');
const options = {
  channels: 2,
  bitDepth: 16,
  sampleRate: 48000,
};

const speaker = new Speaker(options);

const users = [];

const server = net.createServer(socket => {
  console.log('Client connected:', socket.remoteAddress, socket.remotePort);
  console.log('сюда нахуй');
  socket.on('data', data => {
    try {
      const jsonData = JSON.parse(data.toString());
      if (jsonData.type === 'connect') {
        users.push({ socket: socket, nickname: jsonData.nickname });
        console.log(jsonData);
      }
      if (jsonData.type === 'voicechat') {
      }
      if (jsonData.type === 'kickuser') {
        const senderUser = jsonData.sender;
        const kickedUser = jsonData.kickedUser;
        for (let i = 0; i < users.length; i++) {
          if (kickedUser === users[i].nickname) {
            //users[i].disconnected = { state: true, disconnectedBy: senderUser};
            break;
          }
        }
      }
    } catch (error) {
      console.log(data);
      for (let i = 0; i < users.length; i++) {
        if (socket.remotePort == users[i].socket.remotePort) {
          if (users[i].disconnected) {
            console.log('Blocked User');
          } else {
            for (let i = 0; i < users.length; i++) {
              users[i].socket.write(data, err => {
                if (!err) {
                  console.log(
                    'Succesfull sended data to',
                    `${users[i].socket.remoteAddress}:${users[i].socket.remotePort}`,
                  );
                } else {
                  console.log(err);
                }
              });
            }
          }
        }
      }
    }
  });

  socket.on('close', () => {
    console.log(
      'Client disconnected:',
      socket.remoteAddress,
      socket.remotePort,
    );
    users.forEach((user, index) => {
      if (user.socket === socket) {
        users.splice(index, 1);
      }
    });
  });

  socket.on('error', err => {
    console.log('Socket error:', err);
  });
});

server.on('error', err => {
  console.log(`Server error:\n${err.stack}`);
});

server.listen(8080, () => {
  console.log('TCP server listening on port 8080');
});
