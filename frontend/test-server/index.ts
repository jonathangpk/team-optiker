import WebSocket from 'ws';
import { ClientMessage, ServerMessage } from './generated/events';


const wss = new WebSocket.Server({ port: 8080 });

const clients = new Map();
let counter = 0;
wss.on('connection', (ws) => {
    const id = counter++;
    clients.set(id, ws);
    const msg = ServerMessage.encode({
        event: {
            $case: 'listings',
            listings: {
                listings: [
                    {
                        ticker: '',
                        name: '',
                        description: '',
                        logo:'',
                        amountShares: '',
                    }
                ]
            }
        }
    }).finish()
    ws.send(msg)
})


wss.on('message', (messageAsString) => {
    const message = JSON.parse(messageAsString);
    // const metadata = clients.get(ws);
    console.log('message', ClientMessage.decode(message));
    // message.sender = metadata.id;
    // message.color = metadata.color;
})

console.log('started')