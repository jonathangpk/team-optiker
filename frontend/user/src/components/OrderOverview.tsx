import { useStore } from "../state/store";



export function OrderOverview() {
  const state = useStore()

  const orderList = Object.entries(state.orders)
    .map(([orderId, order]) => ({
      orderId,
      ...order,
    }))
  return (
    <div>
      { orderList.map(order => order.id) }
    </div>
  )
}