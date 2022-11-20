import { List, ListItemButton, ListItemText, Typography } from "@mui/material";
import { useNavigate } from "react-router-dom";
import { useStore } from "../state/store";

// export interface InfoOrder {
//   id: string;
//   type: OrderType;
//   ticker: string;
//   price: number;
//   amount: number;
// }

const orderTypeToText = [
  "Sold", "Buy", "Error"
]

export function OrderOverview() {
  const store = useStore();
  const navigate = useNavigate();

  const orderList = Object.entries(store.orders)
    .map(([orderId, order]) => ({
      orderId,
      ...order,
    }));

  return (
		<List sx={{ width: '100%', bgcolor: 'background.paper' }}>
			{
				orderList.map(order =>
          <div>
            <ListItemButton
              key={order.id}
              alignItems="flex-start"
              onClick={() => {
                navigate(`/order/${order.id}`);
              }}
            >

              {/* <ListItemAvatar>
                <Avatar alt="Remy Sharp" src="/static/images/avatar/1.jpg" />
              </ListItemAvatar> */}
                <ListItemText
                  primary={
                    <Typography
                      sx={{ display: 'inline' }}
                      component="span"
                      variant="h6"
                      color="text.primary"
                    >
                      {`${orderTypeToText[order.type]} ${order.ticker}`}
                    </Typography>}
                  secondary={ 
                      <Typography
                        sx={{ display: 'inline' }}
                        component="span"
                        variant="subtitle1"
                        color="text.secondary"
                      >
                        {` - ${order.amount} shares at ${order.price / 100}`}
                      </Typography>
                  }
                  key={`it${order.id}`}
                />
            </ListItemButton>
          </div>
				)
			}
		</List>
  )
}