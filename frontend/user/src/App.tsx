import { useEffect } from 'react';
import './App.css';
import { handleServerMessage } from './state/client';
import { SnackbarProvider } from 'notistack';
import { useStore } from './state/store';
import { ServerMessage } from './generated/events';
import { useSnackbar } from 'notistack';
import { createBrowserRouter, RouterProvider } from 'react-router-dom';
import { Register } from './pages/Register';
import { Home } from './pages/Home';
import { Listings } from './pages/Listings';
import { News, NewsPopup } from './pages/News';
import { ListingDetail } from './pages/ListingDetail';
import { AuthState } from './state/authStore';
import { createTheme, ThemeProvider } from '@mui/material/styles';
import { OrderView } from './pages/OrderView';
import { BuyOrSell, ExchangeView } from './pages/ExchangeView';
import { AdminPage } from './pages/AdminPage';


function useInit() {
  const store = useStore();
  
  const { enqueueSnackbar } = useSnackbar()
  const setupEventListeners = () => {
    store.client.setNotificationHandler(enqueueSnackbar)
    
    store.client.setOnMessageHandler(message => {
      if (message.data instanceof ArrayBuffer) {
        // binary frame
        const data = new Uint8Array( message.data );
        const msg = ServerMessage.decode(data)
        handleServerMessage(store, msg);
      } else {
        // text frame
        console.log('wrong ws format')
        enqueueSnackbar('Wrong ws format', {variant: 'error'})
      }
    })
    
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
    path: '/listing/:id/buy',
    element: <ExchangeView type="buy" />
  },
  {
    path: '/listing/:id/sell',
    element: <ExchangeView type="sell" />
  },
  {
    path: '/order/:id',
    element: <OrderView />
  },
  {
    path: '/news',
    element: <News />
  },
  {
    path: '/admin',
    element: <AdminPage />
  },
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
      <NewsPopup />
    </div>
  );
}

const darkTheme = createTheme({
  palette: {
    mode: 'dark',
  },
  typography: {
   "fontFamily": `Roboto`,
   "fontSize": 14,
   "fontWeightLight": 50,
   "fontWeightRegular": 100,
   "fontWeightMedium": 300
  }
});

function ContextWrappers() {
  return (
    <SnackbarProvider maxSnack={3} >
      <ThemeProvider theme={darkTheme}>
        <App />
      </ThemeProvider>
    </SnackbarProvider>
  );
}

export default ContextWrappers;
