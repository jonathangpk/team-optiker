import React from 'react';
import logo from './logo.svg';
import './App.css';
import { createClient } from './state/client';
import { PriceChart } from './components/PriceChart';

function App() {

  const [client, setClient] = React.useState(createClient());
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

export default App;
