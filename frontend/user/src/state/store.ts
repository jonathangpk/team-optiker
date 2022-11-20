import create, { StateCreator } from 'zustand'

import { MI } from '../util';
import { ClientMessage, InfoPosition, News } from "./../generated/events";
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


export type INews = MI<typeof News>
export type INewsAction =  INews['actions'][number]
export interface NewsSlice {
  news: INews[];
  newsPopup: INews | undefined;
  setNews: (news: INews[]) => void;
  onNewNews: (news: INews) => void;
  hideNews: () => void;
  showNews: (news: INews) => void;
  createNews: (news: INews) => void;
}
 
export const newsSlice: StateCreator<Store, [], [], NewsSlice> = (set, get) => ({
  news: [],
  newsPopup: undefined,
  setNews: (news) => set({ news }),
  onNewNews: (news) => set({ newsPopup: news }),
  hideNews: () => set({ newsPopup: undefined }),
  createNews: (news) => get().client.send(ClientMessage.encode({
    event: {
      $case: 'createNews',
      createNews: news
    }
  })),
  showNews: news => set({ newsPopup: news }),
})

export type Store = AuthSlice & PositionSlice & ListingSlice & OrderSlice & NewsSlice & {
  client: Client;
}


export const useStore = create<Store>()((...a) => {
  return {
    client: new Client(),
    ...authSlice(...a),
    ...positionSlice(...a),
    ...listingSlice(...a),
    ...orderSlice(...a),
    ...newsSlice(...a),
  }
})
