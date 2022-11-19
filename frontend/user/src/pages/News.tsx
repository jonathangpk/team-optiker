import { Dialog, DialogContent, DialogTitle, List, Typography } from "@mui/material"
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
      <DialogTitle>{store.newsPopup.title}</DialogTitle>
      <DialogContent>
        {store.newsPopup.description}
        {store.newsPopup.actions.map(action => (
          <div onClick={() => handleAction(action)}>
            <h3>{action.name}</h3>
            <p>{action.description}</p>
          </div>
        ))}
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
  <Container title="News" navigationPosition="news" backLocation=''>
    <NewsComponent />
  </Container>
)