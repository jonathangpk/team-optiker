import { Container } from "../components/Container";
import { ListingsComponent } from "../components/ListingComponent";
import { PriceChart } from "../components/PriceChart";


interface HomeProps {

}

export function Home(props: HomeProps) {
  return (
    <Container title="Portfolio" navigationPosition="home">
      <ListingsComponent />
    </Container>
  )
}