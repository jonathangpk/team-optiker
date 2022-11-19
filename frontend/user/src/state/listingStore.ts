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
  staticListings: [],
  setStaticListings: staticListings => {
    set({ staticListings })
  },
  listingsWithPrice: {},
  setListingsWithPrice: listingsWithPrice => set({ listingsWithPrice }),
  listingOrderBook: {},
  setListingOrderBook: listingOrderBook => set({ 
    listingOrderBook: {
      ...get().listingOrderBook,
      [listingOrderBook['ticker']]: listingOrderBook,
    }
   }),
})