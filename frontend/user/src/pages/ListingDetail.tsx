import { useParams } from "react-router-dom";
import { Container } from "../components/Container";

export function ListingDetail() {
  let { ticker } = useParams<{ticker: string}>();
  return (
    <Container title={ticker || ''}>
      Details
    </Container>
  )
}