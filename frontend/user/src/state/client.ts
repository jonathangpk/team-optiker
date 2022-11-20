import { OptionsObject, SnackbarMessage } from "notistack";
import { Writer } from "protobufjs";
import { ClientMessage, OrderType, ServerMessage } from "../generated/events";
import { Store } from "./store";


const HOST = 'ws://localhost:8080'

type ClientMessageKey = Exclude<ClientMessage['event'], undefined>['$case']
// type Event = {_: Exclude<ClientMessage['event'], undefined> }
// type EventByCase<K extends ClientMessageKey> = {
//   [KK in Event as KK['_']['$case']]:  Omit<KK['_'], '$case'>
// }
// type MessageForKey<K extends ClientMessageKey> = EventByCase<K>
// type B = EventByCase<'placeOrder'>

export class Client {
  client: WebSocket
  notificationHandler?: (message: SnackbarMessage, options?: OptionsObject | undefined) => void
  retryMessageBuffer: Writer[] = []
  constructor() {
    this.client = new WebSocket(HOST);
    this.client.binaryType = "arraybuffer"; 

    this.client.onerror = (event) => {
      if (this.notificationHandler)
      this.notificationHandler('Connection error', {variant: 'error'})
      console.log('onerror', event)
    };
    this.client.onclose = (event) => {
      if (this.notificationHandler)
        this.notificationHandler('Websocket Connection was closed', {variant: 'error'})
      console.log('onclose', event) 
    };
    this.client.onopen = event => {
      console.log('connected');
      this.retryMessageBuffer.forEach(message => this.send(message))
    }
    
  }

  setNotificationHandler(handler: (message: SnackbarMessage, options?: OptionsObject | undefined) => void) {
    this.notificationHandler = handler
  }

  send(message: Writer) {
    if(this.client.readyState === WebSocket.OPEN) {
      this.client.send(message.finish());
    } else {
      console.log('not connected', this.client.readyState)
      this.retryMessageBuffer.push(message)
    }
  }

  setOnMessageHandler(handler: (message: MessageEvent<any>) => void) {
    this.client.onmessage = handler

  }
}

// creates websocket connection
export function createClient() {
  
}

function exhausted(arg: never): never {
  throw new Error('exhausted')
  console.log('should not happen, switch should be exhausted')
}

export function handleServerMessage(store: Store, message: ServerMessage, snackbar: (message: SnackbarMessage, options?: OptionsObject | undefined) => void) {
  if (!message.event) {
    console.log("Error, no event defined on message")
    return
  }
  const event = message.event

  console.log(event.$case);
  console.log(message);

  switch(event.$case) {
    // Auth
    case 'authToken':
      return store.loggedIn(event.authToken.token)
    // Orders
    case 'orders':
      return store.setOrders(event.orders.orders)
    case 'orderCreated':
      snackbar(`Order received ${event.orderCreated.amount}x ${event.orderCreated.ticker} for ${event.orderCreated.price / 100}`, { variant: 'success' })
      return store.orderCreated(event.orderCreated)
    case 'orderPartiallyFullfilled': {
      const order = store.orders[event.orderPartiallyFullfilled.id]
      const amount = event.orderPartiallyFullfilled.amountLeft 
      const soldBought = order.type === OrderType.BID ? 'Bought' : 'Sold'
      snackbar(`${order.amount - amount} shares of ${order.ticker} were ${soldBought}`)
      return store.orderPartiallyFullfilled(event.orderPartiallyFullfilled)
    } 
    case 'orderFullfilled': {
      const order = store.orders[event.orderFullfilled.id]
      const soldBought = order.type === OrderType.BID ? 'Bought' : 'Sold'
      snackbar(`${order.amount} shares of ${order.ticker} were ${soldBought}`)
      return store.orderFullfilled(event.orderFullfilled) 
    }
    case 'orderCanceld': {
      const order = store.orders[event.orderCanceld.id]
      snackbar(`Order for ${order.amount} shares of ${order.ticker} were canceled`)
      return store.orderCanceld(event.orderCanceld.id)
    }
      // Listings
    case 'listings':
      return store.setStaticListings(event.listings.listings)
    case 'listingUpdates':
      return store.setListingsWithPrice(event.listingUpdates.listings)
    // Positions
    case 'positions':
      return store.setPositions(event.positions.positions)
    // News
    case 'createNews':
      return store.onNewNews(event.createNews)
    case 'newsList':
      return store.setNews(event.newsList.news)
    // Error
    case 'error':
      if (event.error.error === 'Error: Invalid Token.') {
        window.localStorage.removeItem('token')
        window.location.reload()
      }
      return
    // default:
    //   exhausted(event.$case)
      
  }

}
