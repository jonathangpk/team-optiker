import { Server } from 'http'
import { Deserializer } from 'v8'
import create from 'zustand'
import { client } from '../App'
import { StaticListing, DynamicListing, ListingOrderBook, Login, Register, ServerMessage, ClientMessage } from '../generated/events'
import { createClient } from './client'


// type PM<T extends > = 


export enum AuthState {
  notRegistered, // Init when no token in local storage
  tokenAvailable, // Init when token in local storage
  loading, // When logging in or registering
  authenticated, // When logged in
}

export interface State {
  authenticated: AuthState
  tryLogin: (token: string) => void
  register: (name: string) => void
  loggedIn: (token: string) => void
}

export const useStore = create<State>()((set, get) => {
  return {
    authenticated: window.localStorage.getItem('token') ? AuthState.tokenAvailable : AuthState.notRegistered,
    tryLogin: token => {
      if (token) {
        client.send(ClientMessage.encode({ 
          event: {
            $case: 'login',
            login: {
              token,
            }
          },
        }).finish())
      }
    },
    register: (name: string) => {
      client.send(ClientMessage.encode({ 
        event: {
          $case: 'register',
          register: {
            name,
          }
        }
      }).finish())
    },
    loggedIn: (token: string) => {
      window.localStorage.setItem('token', token)
      set({ authenticated: AuthState.authenticated })
    }
  }
})
