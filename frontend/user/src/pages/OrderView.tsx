import { useParams } from "react-router-dom";
import { Container } from "../components/Container";
import { useStore } from "../state/store";

export const OrderView = () => {
  const { id } = useParams<{ id: string }>();
  const store = useStore();

  const order = store.orders[id as string];


  return (
    <Container title="Order" navigationPosition="home" >
    </Container>
  );
}