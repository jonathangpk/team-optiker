import { Divider } from "@mui/material";
import { Container } from "../components/Container";
import { ListingsComponent } from "../components/ListingComponent";
import { OrderOverview } from "../components/OrderOverview";



interface HomeProps {

}

export function Home(props: HomeProps) {
  return (
    <Container title="My Portfolio" navigationPosition="home">
      <ListingsComponent height="45vh"/>
      <Divider style={{padding: 10}} />
      <OrderOverview />
    </Container>
  )
}