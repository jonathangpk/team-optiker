import { ServerMessage } from "../generated/events";



const HOST = 'ws://localhost:8080'


function onOpen() {
  console.log('connected');
  // send auth message
}

function onMessage() {

}

// creates websocket connection
export function createClient() {
  const ws = new WebSocket(HOST);
  ws.binaryType = "arraybuffer";
  ws.onopen = event => {
    console.log('connected');
  };
  ws.onmessage = event => {
    if (event.data instanceof ArrayBuffer) {
      // binary frame
      const data = new Uint8Array( event.data );
      const message = ServerMessage.decode(data)
      console.log(message)
      console.log(message.event?.$case)

    } else {
      // text frame
      console.log('wrong ws format')
      console.log(event.data);
    }
  };
  ws.onerror = (event) => {
    console.log('onerror', event)
  };
  ws.onclose = (event) => {
    console.log('onclose', event) 
  };
  return ws
}