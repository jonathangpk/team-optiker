import { Button, Typography } from "@mui/material";
import { useNavigate, useParams } from "react-router-dom";
import { Container } from "../components/Container";
import { PriceGraph } from "../components/NewChart";
import { useStore } from "../state/store";

export function ListingDetail() {
  let { id } = useParams<{ id: string }>();
  const navigate = useNavigate();
  const store = useStore();
  const listing = store.staticListings.find(lst => lst.ticker === id);
  const price = store.listingsWithPrice.find(lst => lst.symbol === id);

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
        {`Price: ${((price?.price || 0) / 100).toFixed(2)}$`}
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
        <div style={{marginTop: 20}}>
            <Typography variant="body2" color="text.primary">{`Ask Price: ${((price?.askPrice || 0) / 100).toFixed(2)}$`}</Typography>
            <Typography variant="body2" color="text.primary">{`Bid Price: ${((price?.bidPrice || 0) / 100).toFixed(2)}$`}</Typography>
        </div>
    </Container>
  )
}