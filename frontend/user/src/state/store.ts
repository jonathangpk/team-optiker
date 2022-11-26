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
  newsIndex: number; // only admin choice relevance
  setNews: (news: INews[]) => void;
  onNewNews: (news: INews) => void;
  hideNews: () => void;
  showNews: (news: INews) => void;
  createNews: (news: INews) => void;
}
 
export const newsSlice: StateCreator<Store, [], [], NewsSlice> = (set, get) => ({
  news: [{
    title: 'Elon Musk buys Wakanda',
    description: 'In a completely unprecedented move the billionaire and richest person alive, Elon Musk has purchased all Wakanda Government State Bonds. Investments in Military are expected to increase. At the same time, as locals fear, the supply of independent media might suffer a great setback.',
    actions: [
      {
        name: 'Bail Out',
        description: 'Sell shares related to Wakanda.',
        suggestedTrades: [
          {
            type: "sell",
            symbol: "st3",
            amount: 150,
          }
        ]
      },
      {
        name: 'Invest in Military',
        description: 'Invest heavily into Military Listings.',
        suggestedTrades: [
          {
            type: "buy",
            symbol: "st4",
            amount: 250,
          }
        ]
      },
      {
        name: 'Invest in Media',
        description: 'Buy shares for print and online media.',
        suggestedTrades: [
          {
            type: "buy",
            symbol: "st1",
            amount: 150,
          }
        ]
      },
    ]
  }],
  newsPopup: undefined,
  newsIndex: 0,
  setNews: (news) => set({ news }),
  onNewNews: (news) => set({
    newsPopup: news,
    news: [news, ...get().news]
  }),
  hideNews: () => set({ newsPopup: undefined }),
  createNews: (news) => {
    get().client.send(ClientMessage.encode({
      event: {
        $case: 'createNews',
        createNews: news
      }
    }));
  },
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
