import { AppBar, BottomNavigation, BottomNavigationAction, Box, Button, ButtonGroup, Divider, IconButton, List, ListItemButton, ListItemText, Paper, Tooltip, Typography } from "@mui/material";
import { useState } from "react";
import HomeIcon from '@mui/icons-material/Home';
import ShowChartIcon from '@mui/icons-material/ShowChart';
import SubjectIcon from '@mui/icons-material/Subject';
import { OrderGraph } from "../components/OrderGraph";
import FeedIcon from '@mui/icons-material/Feed';
import { OrderListComponent } from "../components/OrderListComponent";
import { useStore } from "../state/store";

const newsChoices = [
  {
    title: 'Elon Musk buys Wakanda',
    description: 'In a completely unprecedented move the billionaire and richest person alive, Elon Musk has purchased all Wakanda Government State Bonds. Investments in Military are expected to increase. At the same time, as locals fear, the supply of independent media might suffer a great setback.',
    actions: [
      {
        name: 'Bail Out',
        description: 'Sell shares related to Wakanda.',
        suggestedTrades: [
          {
            type: "sell",
            symbol: "st3",
            amount: 150,
          }
        ]
      },
      {
        name: 'Invest in Military',
        description: 'Invest heavily into Military Listings.',
        suggestedTrades: [
          {
            type: "buy",
            symbol: "st4",
            amount: 250,
          }
        ]
      },
      {
        name: 'Invest in Media',
        description: 'Buy shares for print and online meda.',
        suggestedTrades: [
          {
            type: "buy",
            symbol: "st1",
            amount: 150,
          }
        ]
      },
      // {
      //     name: 'Long Vibranium',
      //     description: 'Long Vibranium',
      // }
    ]
  }
];

function renderContent(page: string) {
  switch (page) {
    case "home":
      return (
        <>
          <OrderGraph />
          <OrderListComponent selector="orders" />
          <OrderListComponent selector="transactions" />
        </>
      );
    case "news":
      return (
        <>
          <NewsChoice />
        </>
      );

  }
  return null;
}

function NewsChoice() {
  const store = useStore();
  const idx = store.newsIndex;

  return (
    <Box>
      <List>
        {store.news.map(news => (
          <>
            <ListItemButton
              key={news.title}
              alignItems="flex-start"
              onClick={() => {
                store.createNews(news);
              }}
            >
              <ListItemText
                primary={
                  <Typography
                    sx={{ display: 'inline' }}
                    variant="h5"
                    color="text.primary"
                  >
                    {news.title}
                  </Typography >
                }
                secondary={
                  <>
                    {news.description}
                    <br></br>
                    <Typography
                      sx={{ display: 'inline' }}
                      variant="body1"
                      color="text.primary"
                    >
                      Possible Responses
                    </Typography >
                    
                    <br></br>
                    {news.actions.map(action => (
                      <ButtonGroup style={{ marginTop: 10 }} variant="contained" aria-label="outlined primary button group">
                        {/* <Tooltip title={action.description}> */}
                          <Button variant="outlined" disabled={true}>
                            {action.name}
                          </Button>
                        {/* </Tooltip> */}
                      </ButtonGroup>
                    ))}
                  </>
                }
              />
            </ListItemButton>
            <Divider component="li" />
          </>
        ))}
      </List>
    </Box>
  )
}

export function AdminPage() {
  const [page, setPage] = useState("home");

  return (
    <Box>
      <AppBar component="nav">
        <Typography variant={"h3"}>Admin</Typography>
      </AppBar>
      <Box
        style={{
          marginTop: 80,
          width: '100%',
          position: "fixed",
          justifyContent: "center"
        }}>
        {
          renderContent(page)
        }
      </Box>
      <Paper sx={{ position: 'fixed', bottom: 0, left: 0, right: 0 }} elevation={3}>
        <BottomNavigation
          showLabels={true}
          value={page}
          onChange={(_, val) => setPage(val)}
        >
          <BottomNavigationAction icon={<ShowChartIcon fontSize="large" />} label="Home" value="home" />
          {/* <BottomNavigationAction icon={<ShowChartIcon fontSize="large" />} label="Graphs" value="graphs" /> */}
          <BottomNavigationAction icon={<FeedIcon fontSize="large" />} label="News" value="news" />
        </BottomNavigation>
      </Paper>
    </Box>
  );
}