import { Container } from "../components/Container";
import { ListingsComponent } from "../components/ListingComponent";

export const Listings = () => (
  <Container title="Listings" navigationPosition="listings" backLocation=''>
    <ListingsComponent />
  </Container>
)