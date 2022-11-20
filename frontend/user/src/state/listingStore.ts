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
      ticker: 'WGX',
      amountShares: 462378,
      description: 'Offical Government Bonds for the country of Wakanda',
      logo: '',
      name: 'Wakanda Government Bonds'
    },
    {
      ticker: 'AVX',
      amountShares: 143242,
      description: 'Avengers Inc. provides security all across the globe in all Metaverses',
      logo: '',
      name: 'Avengers Inc.'
    },
    {
      ticker: 'WPT',
      amountShares: 462378,
      description: 'Largest Newspaper and online media presence in Wakanda',
      logo: '',
      name: 'Wakanda Post'
    },
    {
      ticker: 'VIB',
      amountShares: 17564,
      description: 'Vibranium is noted for its extraordinary abilities to absorb, store, and release large amounts of kinetic energy',
      logo: '',
      name: 'Vibranium'
    },
    {
      ticker: 'TUM',
      amountShares: 16234,
      description: 'Most excellent stock on the market',
      logo: '',
      name: 'TUM'
    },
    {
      ticker: 'ETH',
      amountShares: 1,
      description: 'Among the highest Ranked University in Europe',
      logo: '',
      name: 'ETH'
    },
    {
      ticker: 'OPK',
      amountShares: 52389,
      description: 'Fictional financial scenario handling Simulation providing Services',
      logo: '',
      name: 'OptiKER'
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