import { Button, Typography } from "@mui/material";
import { useParams } from "react-router-dom";
import { Container } from "../components/Container";
import { PriceGraph } from "../components/NewChart";

export function ListingDetail() {
  let { id } = useParams<{ id: string }>();
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
          variant="text"
          style={{marginLeft: 20}}
          onClick={() => {
          }}
        >
          BUY
        </Button>
    </Container>
  )
}