import React, { createContext, useEffect } from 'react';
import logo from './logo.svg';
import './App.css';
import { handleServerMessage } from './state/client';
import { PriceChart } from './components/PriceChart';
import { SnackbarProvider } from 'notistack';
import { AuthState, client, useStore } from './state/state';
import { ServerMessage } from './generated/events';
import { useSnackbar } from 'notistack';


function useInit() {
  const store = useStore();
  const { enqueueSnackbar } = useSnackbar()
  const setupEventListeners = () =>{
    client.onmessage = event => {
      if (event.data instanceof ArrayBuffer) {
        // binary frame
        const data = new Uint8Array( event.data );
        const message = ServerMessage.decode(data)
        handleServerMessage(store, message);
      } else {
        // text frame
        console.log('wrong ws format')
        enqueueSnackbar('Wrong ws format', {variant: 'error'})
      }
    };
    client.onerror = (event) => {
      enqueueSnackbar('Connection error', {variant: 'error'})
      console.log('onerror', event)
    };
    client.onclose = (event) => {
      enqueueSnackbar('Websocket Connection was closed', {variant: 'error'})
      console.log('onclose', event) 
    };
  }

  const authInit = () => {
    const token = window.localStorage.getItem('token');
    if (token && store.authenticated == AuthState.tokenAvailable) {
      store.tryLogin(token);
    }
  }
  useEffect(() => { 
    setupEventListeners()
    authInit()
    return () => client.close()
  }, [])
}

function App() {
  const store = useStore();
  useInit()
  return (
    <div className="App">
      <PriceChart priceHistory={[]} />
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.tsx</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
    </div>
  );
}

function ContextWrappers() {
  return (
    <SnackbarProvider maxSnack={3} >
      {/* <StoreContext.Provider value={store}> */}
      <App />
      {/* </StoreContext.Provider> */}
    </SnackbarProvider>
  );
}

export default ContextWrappers;
