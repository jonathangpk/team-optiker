import { ClientMessage, InfoOrder, InfoPosition, ListingOrderBook, ListingPrice, ListingPrices, News, OrderCreated, OrderFullfilled, OrderPartiallyFullfilled, OrderType, PlaceOrder, ServerMessage, StaticListing } from "./generated/events";
import { Writer } from "protobufjs";
import { w3cwebsocket, IMessageEvent } from "websocket";



const HOST = 'ws://localhost:8080'
export type MI<T extends { encode: (...args: any) => any }> = T['encode'] extends (...args: infer A) => any ? A[0] : never;

function boxMullerTransform() {
  const u1 = Math.random();
  const u2 = Math.random();
  
  const z0 = Math.sqrt(-2.0 * Math.log(u1)) * Math.cos(2.0 * Math.PI * u2);
  const z1 = Math.sqrt(-2.0 * Math.log(u1)) * Math.sin(2.0 * Math.PI * u2);
  
  return { z0, z1 };
}

function normal(mean: number, stddev: number) {
  const { z0 } = boxMullerTransform();
  
  return z0 * stddev + mean;
}

export class Client {
  client: w3cwebsocket
  retryMessageBuffer: Writer[] = []
  constructor() {
    this.client = new w3cwebsocket(HOST);
    this.client.binaryType = "arraybuffer";

    this.client.onerror = (event) => {
      console.log('onerror', event)
    };
    this.client.onclose = (event) => {
      console.log('onclose', event)
    };
    this.client.onopen = () => {
      console.log('connected');
      this.retryMessageBuffer.forEach(message => this.send(message))
    }
  }

  send(message: Writer) {
    if (this.client.readyState === w3cwebsocket.OPEN) {
      this.client.send(message.finish());
    } else {
      console.log('not connected', this.client.readyState)
      this.retryMessageBuffer.push(message)
    }
  }

  setOnMessageHandler(handler: (message: IMessageEvent) => void) {
    this.client.onmessage = handler

  }
}

export type IStaticListing = MI<typeof StaticListing>
export type IListingPrice = MI<typeof ListingPrice>
export type IListingPrices = MI<typeof ListingPrices>
export type IListingOrderBook = MI<typeof ListingOrderBook>

export type IInfoPosition = MI<typeof InfoPosition>

type IInfoOrder = MI<typeof InfoOrder>
type IPlaceOrder = MI<typeof PlaceOrder>
type IOrderCreated = MI<typeof OrderCreated>
type IOrderPartiallyFullfilled = MI<typeof OrderPartiallyFullfilled>
type IOrderFullfilled = MI<typeof OrderFullfilled>

export type INews = MI<typeof News>

interface State {
  // Listing
  staticListings: IStaticListing[];
  listingsWithPrice: IListingPrices['listings'];
  listingOrderBook: Record<string, Omit<IListingOrderBook, 'ticker'>>

  positions: IInfoPosition[];

  // News
  news: INews[];

  // Orders
  orders: Record<number, IInfoOrder>;

}

class Bot {
  client: Client
  name: string
  token?: string
  state: State

  constructor(name: string) {
    this.client = new Client()
    this.name = name
    this.state = {
      staticListings: [

        {
          ticker: 'WGX',
          amountShares: 462378,
          description: 'Offical Government Bonds for the country of Wakanda',
          logo: '',
          name: 'Wakanda Government Bonds'
        },
        {
          ticker: 'AVX',
          amountShares: 143242,
          description: 'Avengers Inc. provides security all across the globe in all Metaverses',
          logo: '',
          name: 'Avengers Inc.'
        },
        {
          ticker: 'WPT',
          amountShares: 462378,
          description: 'Largest Newspaper and online media presence in Wakanda',
          logo: '',
          name: 'Wakanda Post'
        },
        {
          ticker: 'VIB',
          amountShares: 17564,
          description: 'Vibranium is noted for its extraordinary abilities to absorb, store, and release large amounts of kinetic energy',
          logo: '',
          name: 'Vibranium'
        },
        {
          ticker: 'TUM',
          amountShares: 16234,
          description: 'Most excellent stock on the market',
          logo: '',
          name: 'TUM'
        },
        {
          ticker: 'ETH',
          amountShares: 1,
          description: 'Among the highest Ranked University in Europe',
          logo: '',
          name: 'ETH'
        },
        {
          ticker: 'OPK',
          amountShares: 52389,
          description: 'Fictional financial scenario handling Simulation providing Services',
          logo: '',
          name: 'OptiKER'
        },
      ],
      listingOrderBook: {},
      listingsWithPrice: [],
      news: [],
      orders: {},
      positions: [],
    }
    this.client.setOnMessageHandler(this.onMessage.bind(this))
    setTimeout(() => {
      setInterval(() => {
        this.tradeLoop()
      }, 50)
    }, 1000)
  }

  authenticate() {
    this.client.send(ClientMessage.encode({
      event: {
        $case: 'register',
        register: {
          name: this.name,
        }
      }
    }))
  }

  onMessage(message: IMessageEvent) {
    if (message.data instanceof ArrayBuffer) {
      // binary frame
      const data = new Uint8Array(message.data);
      const msg = ServerMessage.decode(data)
      const event = msg.event
      if (!event) return
      switch (event.$case) {
        case 'authToken':
          this.token = event.authToken.token
          return
        case 'error':
          return console.log('error', event.error)

        // Orders
        case 'orders':
          this.state.orders = Object.fromEntries(event.orders.orders.map(o => [o.id, o]))
          return
        // case 'orderCreated':
        //   this.state.orders[event.orderCreated.id] = event.orderCreated
        //   return
        // case 'orderCanceld':
        //   delete this.state.orders[event.orderCanceld.id]
        //   return
        // case 'orderPartiallyFullfilled':
        //   const order = this.state.orders[event.orderPartiallyFullfilled.id]
        //   this.state.orders[event.orderPartiallyFullfilled.id] = {
        //     ...order,
        //     amount: Math.min(order.amount, event.orderPartiallyFullfilled.amountLeft),
        //   }
        //   return
        // case 'orderFullfilled':
        //   delete this.state.orders[event.orderFullfilled.id]
        //   return

        // Listing
        case 'listings':
          this.state.staticListings = event.listings.listings
          return
        case 'listingUpdates':
          this.state.listingsWithPrice = event.listingUpdates.listings
          return

        // News
        case 'newsList':
          this.state.news = event.newsList.news
          return
        // Positions
        case 'positions':
          this.state.positions = event.positions.positions
          return

        // 
      }
    }
  }

  placeOrder(order: IPlaceOrder) {
    this.client.send(ClientMessage.encode({
      event: {
        $case: 'placeOrder',
        placeOrder: order,
      }
    }))
  }

  cancelOrder(id: number) {
    this.client.send(ClientMessage.encode({
      event: {
        $case: 'cancelOrder',
        cancelOrder: {
          id,
        }
      }
    }))
  }

  tradeLoop() {
    const cancelProb = 0.4 // Math.random() * 1
    const probPlaceOrder = 1// Math.random() * 1
     
    const orderSize = Math.random() * 1000

    const sigma = 10;
    const mu = 2;
    for (let key in this.state.orders) {
      if (Math.random() < cancelProb) {
        console.log('number?')
        console.log(key)
        // @ts-ignore
        this.cancelOrder(key)
      }
    }

    for (let listing of this.state.staticListings) {
      const currentPrice = this.state.listingsWithPrice.find(l => l.symbol === listing.ticker)?.price || 100
      if (!currentPrice) continue
      if (Math.random() < 1) {
        const type = Math.random() < 0.5 ? OrderType.BID : OrderType.ASK
        if (type == OrderType.ASK) {
          const orderAmount = Math.floor(Math.random() * orderSize)
          const price = normal(currentPrice, sigma)
          if (orderAmount > 0) {
            this.placeOrder({
              ticker: listing.ticker,
              amount: orderAmount,
              price,
              type,
            })
          }
        } else {
          const price = normal(currentPrice, sigma)
          this.placeOrder({
            ticker: listing.ticker,
            amount: Math.floor(Math.random() * orderSize),
            price,
            type,
          })
        }
      }
    }
  }
}

(new Bot('Jonathan')).authenticate()