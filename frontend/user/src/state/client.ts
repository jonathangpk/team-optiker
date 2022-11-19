import { Store } from "./store";
import { ServerMessage } from "../generated/events";



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
    case 'authToken':
      store.loggedIn(event.authToken.token)
      return
    case 'orderCreated':
      store.orderCreated(event.orderCreated)
      return
    case 'orderCanceld':
      store.orderCanceld(event.orderCanceld.id)

    // default:
    //   exhausted(event.$case)
      
  }

}
