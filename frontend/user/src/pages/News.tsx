import { Button, ButtonGroup, Dialog, DialogActions, DialogContent, DialogTitle, Divider, List, ListItemButton, ListItemText, Tooltip, Typography } from "@mui/material"
import { useState } from "react";
import { Container } from "../components/Container"
import { NewsAction, OrderType, SuggestedTrade } from "../generated/events";
import { INewsAction, Store, useStore } from "../state/store"

function handlePurchase(store: Store, trades: SuggestedTrade[]) {
  trades.forEach(trade => {
    const price = store.listingsWithPrice.find(lst => lst.symbol === trade.symbol);
    const price_val = price?.price;

    if(price_val) {
      store.placeOrder({
        amount: trade.amount,
        price: price_val,
        ticker: trade.symbol,
        type: trade.type === "buy" ? OrderType.BID : OrderType.ASK,
      });
    }
  });
}


export function NewsPopup() {
  const store = useStore();
  const [selectedAction, setSelectedAction] = useState<NewsAction>(null as unknown as NewsAction);


  console.log(selectedAction);
  const handleAction = (action: INewsAction) => {

  }
  const handleClose = () => {
    store.hideNews()
  }
  if (!store.newsPopup) return null
  return (
    <>
      <Dialog onClose={handleClose} open={!!store.newsPopup}>
        <DialogTitle>News Alert!</DialogTitle>
        <DialogContent>
          {store.newsPopup.title}
          <br></br>
          <Typography variant="body1" color="text.secondary">
            {store.newsPopup.description}
          </Typography>
          <br></br>
          Possible Responses
          <br></br>
          {store.newsPopup.actions.map(action => (
            <ButtonGroup style={{ marginTop: 10 }} variant="contained" aria-label="outlined primary button group">
              <Tooltip title={action.description}>
                <Button variant="outlined" onClick={() => setSelectedAction(action)}>
                  {action.name}
                </Button>
              </Tooltip>
            </ButtonGroup>
          ))}

          <DialogActions>
            <Button onClick={handleClose}>Close</Button>
          </DialogActions>
        </DialogContent>
      </Dialog>

      <Dialog open={selectedAction !== null}>
        <DialogTitle>{selectedAction?.name}</DialogTitle>
        <DialogContent>
          Responding to: {store.newsPopup?.title}
          <br></br>
          <Typography variant="body1" color="text.secondary">
            {selectedAction?.description}
          </Typography>
          <br></br>
          Associated Trades
          <br></br>
          {selectedAction?.suggestedTrades.map(trade => (
            <Typography variant="body1" color="text.secondary">
              {`${trade.type.toUpperCase()} ${trade.amount} shares of ${trade.symbol}`}
            </Typography>
          ))}

          <Button onClick={() => handlePurchase(store, selectedAction?.suggestedTrades)}>EXECUTE NOW</Button>

          <DialogActions>
            <Button onClick={() => setSelectedAction(null as unknown as NewsAction)}>Close</Button>
          </DialogActions>
        </DialogContent>
      </Dialog>
    </>
  );
}

export function NewsComponent() {
  const store = useStore()
  return (
    <List>
      {store.news.map(news => (
        <>
          <ListItemButton
            key={news.title}
            alignItems="flex-start"
            onClick={() => {
              store.showNews(news);
            }}
          >
            <ListItemText
              primary={
                <Typography
                  sx={{ display: 'inline' }}
                  variant="h6"
                  color="text.primary"
                >
                  {news.title}
                </Typography >
              }
              secondary={news.description}
            />
          </ListItemButton>
          <Divider component="li" />
        </>
      ))}
    </List>
  )
}

export const News = () => (
  <Container title="News" navigationPosition="news">
    <NewsComponent />
  </Container>
)