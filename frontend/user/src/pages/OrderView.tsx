import { Button, Typography } from "@mui/material";
import { useParams } from "react-router-dom";
import { Container } from "../components/Container";
import { useStore } from "../state/store";

export const OrderView = () => {
  const { id } = useParams<{ id: string }>();
  const store = useStore();

  const ID = parseInt(id || '-1');
  if (!ID) {
    return <Container title="Invalid" navigationPosition="home">Invalid ID</Container>;
  }
  const order = store.orders[ID];
  return (
    <Container title="Order" navigationPosition="home" >
      <Typography color={'text.primary'} variant="h4">Order {order.ticker}</Typography>
      <Typography color={'text.primary'} variant="h5">Price: {order.price}</Typography>
      <Typography color={'text.primary'} variant="h5">Volume: {order.amount}</Typography>
      <Typography color={'text.primary'} variant="h5">Total: {order.price * order.amount}</Typography>

      <Button onClick={() => store.cancelOrder(ID)}>Delete</Button>

    </Container>
  );
}
