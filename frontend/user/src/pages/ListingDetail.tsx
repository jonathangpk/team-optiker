import { useParams } from "react-router-dom";
import { Container } from "../components/Container";
import { PriceGraph } from "../components/NewChart";
import { PriceChart } from "../components/PriceChart";

export function ListingDetail() {
  let { id } = useParams<{ id: string }>();
  return (
    <Container title={id || ''} navigationPosition="listings" backLocation='listings'>
      Details for {id}
      <PriceGraph />
    </Container>
  )
}