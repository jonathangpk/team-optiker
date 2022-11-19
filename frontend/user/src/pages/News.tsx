import { Container } from "../components/Container"

interface NewsProps {

}

function NewsComponent(props: NewsProps) {
  return (
    <div>
      News
    </div>
  )
}

export const News = () => (
  <Container title="News" navigationPosition="news" backLocation=''>
    <NewsComponent />
  </Container>
)