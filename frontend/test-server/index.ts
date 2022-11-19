import WebSocket from 'ws';
import { ClientMessage, ServerMessage } from './generated/events';


const wss = new WebSocket.Server({ port: 8080, skipUTF8Validation: true });

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
                        amountShares: 1,
                    }
                ]
            }
        }
    }).finish()
    ws.send(msg)

    ws.on('message', (message, isBinary ) => {
        console.log(isBinary)
        // const metadata = clients.get(ws);
        if (message instanceof ArrayBuffer) {
            // binary frame
            const data = new Uint8Array( message );
            console.log('message', JSON.stringify(ClientMessage.decode(data)));
        } else if (message instanceof Buffer) {
            const data = new Uint8Array(message)
            const msg = ClientMessage.decode(data)
            console.log('message', JSON.stringify(msg));

        } else if (Array.isArray(message)) {
            console.log('array')
        } else {
            console.log(typeof message)
        }
        // message.sender = metadata.id;
        // message.color = metadata.color;
    })
})



console.log('started')