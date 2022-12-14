import { StateCreator } from "zustand"
import { ClientMessage } from "../generated/events"
import { Store } from "./store"


export enum AuthState {
  notRegistered, // Init when no token in local storage
  tokenAvailable, // Init when token in local storage
  loading, // When logging in or registering
  authenticated, // When logged in
}

export interface AuthSlice {
  authenticated: AuthState
  tryLogin: (token: string) => void
  register: (name: string) => void
  loggedIn: (token: string) => void
}


export const authSlice: StateCreator<Store, [], [], AuthSlice> = (set, get) => ({
  authenticated: window.localStorage.getItem('token') ? AuthState.tokenAvailable : AuthState.notRegistered,
  tryLogin: token => {
    if (token) {
      get().client.send(ClientMessage.encode({ 
        event: {
          $case: 'login',
          login: {
            token,
          }
        },
      }))
    }
  },
  register: (name: string) => {
    get().client.send(ClientMessage.encode({ 
      event: {
        $case: 'register',
        register: {
          name,
        }
      }
    }))
  },
  loggedIn: (token: string) => {
    window.localStorage.setItem('token', token)
    set({ authenticated: AuthState.authenticated })
  }
})