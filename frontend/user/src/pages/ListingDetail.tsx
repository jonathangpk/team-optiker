import { Button, Typography } from "@mui/material";
import { useNavigate, useParams } from "react-router-dom";
import { Container } from "../components/Container";
import { PriceGraph } from "../components/NewChart";
import { OrderType } from "../generated/events";
import { useStore } from "../state/store";

export function ListingDetail() {
  let { id } = useParams<{ id: string }>();
  const navigate = useNavigate();
  const store = useStore();
  const listing = store.staticListings.find(lst => lst.ticker === id);

  if (!id || !listing) {
		// invalid stock
		navigate("/listings");
	}

  return (
    <Container title={id || ''} navigationPosition="listings">
    <PriceGraph />
    <Typography
        sx={{ display: 'inline', alignItems: "left" }}
        component="span"
        variant="h6"
        color="text.secondary"
      >
        {`${listing?.name} - ${listing?.description}`}
      </Typography>
      <br></br>
      <br></br>
      <Typography
        sx={{ display: 'inline', alignItems: "left" }}
        component="span"
        variant="h6"
        color="text.primary"
      >
        {`Price: 38.32$`}
      </Typography>
      <Button
          variant="outlined"
          style={{marginLeft: 40}}
          onClick={() => {
            navigate(`/listing/${id}/buy`);
          }}
        >
          BUY
        </Button>
      <Button
          variant="outlined"
          style={{marginLeft: 20}}
          onClick={() => {
            navigate(`/listing/${id}/sell`);
          }}
        >
          SELL
        </Button>
    </Container>
  )
}