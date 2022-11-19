import { Store } from "./store";
import { ClientMessage, ServerMessage } from "../generated/events";
import { Writer } from "protobufjs";
import { OptionsObject, SnackbarMessage } from "notistack";


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
    this.client.onopen = event => console.log('connected');
  }

  setNotificationHandler(handler: (message: SnackbarMessage, options?: OptionsObject | undefined) => void) {
    this.notificationHandler = handler
  }

  send(message: Writer) {
    if(this.client.readyState === WebSocket.OPEN) {
      this.client.send(message.finish());
    } else {
      console.log('not connected', this.client.readyState)
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

export function handleServerMessage(store: Store, message: ServerMessage) {
  if (!message.event) {
    console.log("Error, no event defined on message")
    return
  }
  const event = message.event
  switch(event.$case) {
    // Auth
    case 'authToken':
      return store.loggedIn(event.authToken.token)
    // Orders
    case 'orders':
      return store.setOrders(event.orders.orders)
    case 'orderCreated':
      return store.orderCreated(event.orderCreated)
    case 'orderPartiallyFullfilled':
      return store.orderPartiallyFullfilled(event.orderPartiallyFullfilled)
    case 'orderFullfilled':
      return store.orderFullfilled(event.orderFullfilled)
    case 'orderCanceld':
      return store.orderCanceld(event.orderCanceld.id)
      // Listings
    case 'listings':
      return store.setStaticListings(event.listings.listings)
    case 'listingUpdates':
      return store.setListingsWithPrice(event.listingUpdates.listings)
    // Positions
    case 'positions':
      return store.setPositions(event.positions.positions)
    // News
    case 'newNews':
      return
    case 'newsList':
      return
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
