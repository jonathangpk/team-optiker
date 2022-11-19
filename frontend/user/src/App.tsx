import React, { createContext, useEffect } from 'react';
import logo from './logo.svg';
import './App.css';
import { handleServerMessage } from './state/client';
import { PriceChart } from './components/PriceChart';
import { SnackbarProvider } from 'notistack';
import { useStore } from './state/store';
import { ServerMessage } from './generated/events';
import { useSnackbar } from 'notistack';
import { createBrowserRouter, RouterProvider } from 'react-router-dom';
import { Register } from './pages/Register';
import { Home } from './pages/Home';
import { Listings } from './pages/Listings';
import { News } from './pages/News';
import { ListingDetail } from './pages/ListingDetail';
import { AuthState } from './state/authStore';

const HOST = 'ws://localhost:8080'

export const client = new WebSocket(HOST);
client.binaryType = "arraybuffer";

function useInit() {
  const store = useStore();
  
  const { enqueueSnackbar } = useSnackbar()
  const setupEventListeners = () => {
    
    client.onopen = event => console.log('connected');
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
    // return () => client.close()
  }, [])
}

const router = createBrowserRouter([
  {
    path: "/",
    element: <Home />,
  },
  {
    path: '/listings',
    element: <Listings />,
  },
  {
    path: '/listing/:id',
    element: <ListingDetail />
  },
  {
    path: '/news',
    element: <News />
  }
]);
function App() {
  const store = useStore();
  useInit()
  return (
    <div className="App">
      {store.authenticated == AuthState.authenticated && <RouterProvider router={router} />}
      {store.authenticated == AuthState.notRegistered && <Register />}
      {store.authenticated == AuthState.loading && <div>Loading...</div>}
      {store.authenticated == AuthState.tokenAvailable && <div>Logging in again ...</div>}
    </div>
  );
}


function ContextWrappers() {
  return (
    <SnackbarProvider maxSnack={3} >
      <App />
    </SnackbarProvider>
  );
}

export default ContextWrappers;
