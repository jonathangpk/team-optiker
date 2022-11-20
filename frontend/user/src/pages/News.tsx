import { Button, ButtonGroup, Dialog, DialogActions, DialogContent, DialogTitle, List, Tooltip, Typography } from "@mui/material"
import { Container } from "../components/Container"
import { INewsAction, useStore } from "../state/store"


export function NewsPopup() {
  const store = useStore()

  const handleAction = (action: INewsAction) => {

  }
  const handleClose = () => {
    store.hideNews()
  }
  if (!store.newsPopup) return null
  return (
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
          <ButtonGroup style={{marginTop: 10}} variant="contained" aria-label="outlined primary button group">
              <Tooltip title={action.description}>
                <Button variant="outlined" onClick={() => handleAction(action)}>
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

  )
}

export function NewsComponent() {
  const store = useStore()
  return (
    <div>
      {store.news.map(news => (
        <div onClick={e => store.showNews(news)}>
          <Typography variant="h5" color={'primary'}>{news.title}</Typography>
          <Typography variant="body1" color={'primary'}>{news.description}</Typography>
        </div>
      ))}
    </div>
  )
}

export const News = () => (
  <Container title="News" navigationPosition="news">
    <NewsComponent />
  </Container>
)