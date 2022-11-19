import create, { StateCreator } from 'zustand'
import { MI } from '../util';
import { InfoPosition, ListingOrderBook, ListingPrice, ListingPrices, StaticListing } from "./../generated/events";
import { AuthSlice, authSlice } from './authStore';
import { Client } from './client';
import { ListingSlice, listingSlice } from './listingStore';
import { orderSlice, OrderSlice } from './orderStore';


export type IInfoPosition = MI<typeof InfoPosition>
export interface PositionSlice {
  positions: IInfoPosition[]
  setPositions: (positions: IInfoPosition[]) => void
}
const positionSlice: StateCreator<Store, [], [], PositionSlice> =(set, get) => ({
  positions: [],
  setPositions: (positions) => {
    set({ positions })
  }
})



export type Store = AuthSlice & PositionSlice & ListingSlice & OrderSlice & {
  client: Client;
}


export const useStore = create<Store>()((...a) => {
  return {
    client: new Client(),
    ...authSlice(...a),
    ...positionSlice(...a),
    ...listingSlice(...a),
    ...orderSlice(...a),
  }
})
