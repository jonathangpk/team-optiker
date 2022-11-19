import { Button, Typography } from "@mui/material";
import { useNavigate, useParams } from "react-router-dom";
import { Container } from "../components/Container";
import { PriceGraph } from "../components/NewChart";
import { OrderType } from "../generated/events";
import { useStore } from "../state/store";

export function ListingDetail() {
  let { id } = useParams<{ id: string }>();
  const navigate = useNavigate();

  return (
    <Container title={id || ''} navigationPosition="listings" backLocation='listings'>
    <PriceGraph />
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