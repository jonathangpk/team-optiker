import { Container } from "../components/Container";
import { ListingsComponent } from "../components/ListingComponent";
import { OrderOverview } from "../components/OrderOverview";



interface HomeProps {

}

export function Home(props: HomeProps) {
  return (
    <Container title="Portfolio" navigationPosition="home">
      <ListingsComponent />
      <OrderOverview />
    </Container>
  )
}