import WebSocket from 'ws';
import { AuthToken, ClientMessage, ServerMessage } from './generated/events';


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
                        ticker: 'AAPL',
                        name: 'Apple',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'GOOG',
                        name: 'Google',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'MSFT',
                        name: 'Microsoft',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    }
                ]
            }
        }
    }).finish()
    ws.send(msg)

    const news = {
        title: 'Elon Musk buys Wakanda',
        description: 'Wow! Elon Musk buys Wakanda',
        actions: [
            {
                name: 'Short Twitter',
                description: 'Short Twitter',
            },
            {
                name: 'Long Tesla',
                description: 'Long Tesla',
            },
            {
                name: 'Long Vibranium',
                description: 'Long Vibranium',
            }
        ]
    }

    ws.send(ServerMessage.encode({
        event: {
            $case: 'newsList',
            newsList: {
                news: [
                     news,
                ]
            }
        }
    }).finish())

    setTimeout(() => {
        ws.send(ServerMessage.encode({
            event: {
                $case: 'newNews',
                newNews: news,
            }
        }).finish())
    }, 2000)

    ws.on('message', (message, isBinary ) => {
        console.log(isBinary)
        // const metadata = clients.get(ws);
        if (message instanceof ArrayBuffer) {
            // binary frame
            console.log('array buffer')
            const data = new Uint8Array( message );
            console.log('message', JSON.stringify(ClientMessage.decode(data)));
        } else if (message instanceof Buffer) {
            const data = new Uint8Array(message)
            const msg = ClientMessage.decode(data)
            switch(msg.event?.$case) {
                case 'login': 
                case 'register': {
                    ws.send(ServerMessage.encode({
                        event: {
                            $case: 'authToken',
                            authToken: {
                                token: 'asdfalksdjfaskl',
                            }
                        }
                    }).finish())
                }
            }

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