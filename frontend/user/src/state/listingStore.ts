import { StateCreator } from "zustand";
import { ListingOrderBook, ListingPrice, ListingPrices, StaticListing } from "../generated/events";
import { MI } from "../util";
import { Store } from "./store";

export type IStaticListing = MI<typeof StaticListing>
export type IListingPrice = MI<typeof ListingPrice>
export type IListingPrices = MI<typeof ListingPrices>
export type IListingOrderBook = MI<typeof ListingOrderBook>
export interface ListingSlice {
  staticListings: IStaticListing[];
  setStaticListings: (staticListings: IStaticListing[]) => void;
  listingsWithPrice: IListingPrices['listings'];
  setListingsWithPrice: (listingsWithPrice: IListingPrices['listings']) => void;
  listingOrderBook: Record<string, Omit<IListingOrderBook, 'ticker'>>
  setListingOrderBook: (listingOrderBook: IListingOrderBook) => void;
}
export const listingSlice: StateCreator<Store, [], [], ListingSlice> = (set, get) => ({
  staticListings: [
    {
      ticker: 'Test Stock 0',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 0',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 1',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 2',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 3',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 4',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 5',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 6',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 7',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 8',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
    {
      ticker: 'Test Stock 9',
      amountShares: 1,
      description: '',
      logo: '',
      name: 'Test Stock 0'
    },
  ],
  setStaticListings: staticListings => {
    set({ staticListings })
  },
  listingsWithPrice: [],
  setListingsWithPrice: listingsWithPrice => set({ listingsWithPrice }),
  listingOrderBook: {},
  setListingOrderBook: listingOrderBook => set({ 
    listingOrderBook: {
      ...get().listingOrderBook,
      [listingOrderBook['ticker']]: listingOrderBook,
    }
   }),
})