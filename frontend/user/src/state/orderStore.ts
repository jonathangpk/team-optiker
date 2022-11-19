import { StateCreator } from "zustand";
import { ClientMessage, InfoOrder, OrderCreated, OrderFullfilled, OrderPartiallyFullfilled, PlaceOrder } from "../generated/events";
import { MI } from "../util";
import { Store } from "./store";


type IInfoOrder = MI<typeof InfoOrder>
type IPlaceOrder = MI<typeof PlaceOrder>
type IOrderCreated = MI<typeof OrderCreated>
type IOrderPartiallyFullfilled = MI<typeof OrderPartiallyFullfilled>
type IOrderFullfilled = MI<typeof OrderFullfilled>
export interface OrderSlice {
  orders: Record<string, IInfoOrder>;
  setOrders: (orders: IInfoOrder[]) => void;
  // User Actions
  placeOrder: (order: IPlaceOrder) => void;
  cancelOrder: (orderId: string) => void;
  // Update Patches
  orderCreated: (order: IOrderCreated) => void;
  orderPartiallyFullfilled: (order: IOrderPartiallyFullfilled) => void;
  orderFullfilled: (order: IOrderFullfilled) => void;
  orderCanceld: (orderId: string) => void;
}


export const orderSlice: StateCreator<Store, [], [], OrderSlice> = (set, get) => ({
  orders: {},
  setOrders: (orders) => set({ orders: Object.fromEntries(orders.map(o => [o.id, o])) }),
  placeOrder: (order) => {
    get().client.send(ClientMessage.encode({
      event: {
        $case: 'placeOrder',
        placeOrder: order,
      }
    }))
  },
  cancelOrder: (orderId) => {
    get().client.send(ClientMessage.encode({
      event: {
        $case: 'cancelOrder',
        cancelOrder: {
          id: orderId,
        }
      }
    }))
  },
  orderCreated: (order: IOrderCreated) => set({
    orders: {
      ...get().orders,
      [order.id]: order,
    }
  }),
  orderCanceld: (orderId: string) => {
    const orders = get().orders
    delete orders[orderId]
    return set({
      orders: { ...orders }
    })
  },
  orderPartiallyFullfilled: (partialFullfilled: IOrderPartiallyFullfilled) => {
    const orders = get().orders
    const order = orders[partialFullfilled.id]

    if (!order) console.log('Got Partially Fullfullid even though order does not exist anymore')
    return set({
      orders: {
        ...orders,
        [order.id]: {
          ...order,
          amount: Math.min(order.amount, partialFullfilled.amount),
        },
      }
    })
  },
  orderFullfilled: (order: IOrderFullfilled) => set({
  })
})
