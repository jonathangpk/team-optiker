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
                    },
                    {
                        ticker: 'AAPL1',
                        name: 'Apple',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'GOOG1',
                        name: 'Google',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'MSFT1',
                        name: 'Microsoft',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'AAPL2',
                        name: 'Apple',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'GOOG2',
                        name: 'Google',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'MSFT2',
                        name: 'Microsoft',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'AAPL3',
                        name: 'Apple',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'GOOG3',
                        name: 'Google',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'MSFT3',
                        name: 'Microsoft',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'AAPL4',
                        name: 'Apple',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'GOOG4',
                        name: 'Google',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
                    {
                        ticker: 'MSFT4',
                        name: 'Microsoft',
                        description: 'Tech company',
                        logo:'',
                        amountShares: 100000123,
                    },
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
    setInterval(() => {
        const val = Math.floor(Math.random() * 1000)
        ws.send(ServerMessage.encode({
            event: {
                $case: 'listingUpdates',
                listingUpdates: {
                    listings: {
                        'AAPL': {
                            askPrice: val-1,
                            price: val,
                            bidPrice: val + 1,
                            volume: 123
                        },
                        'GOOG': {
                            askPrice: val-1+100,
                            price: val+100,
                            bidPrice: val + 1+100,
                            volume: 123
                        },
                        'MSFT': {
                            askPrice: val-1+300,
                            price: val+300,
                            bidPrice: val + 1+300,
                            volume: 143
                        }
                    }
                }
            }
        }).finish())
    }, 1000)

    ws.on('message', (message, isBinary ) => {
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
            console.log(msg.event?.$case)

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