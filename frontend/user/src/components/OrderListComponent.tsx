import { AppBar, BottomNavigation, BottomNavigationAction, Box, IconButton, List, ListItem, ListItemButton, ListItemText, Toolbar, Typography } from "@mui/material";
import { Link, useNavigate } from "react-router-dom";
import ArrowBackIcon from '@mui/icons-material/ArrowBack';
import ShowChartIcon from '@mui/icons-material/ShowChart';
import FormatListBulletedIcon from '@mui/icons-material/FormatListBulleted';
import FeedIcon from '@mui/icons-material/Feed';
import { useState } from "react";

interface ContainerProps {
  children: React.ReactNode;
  title: string;
  navigationPosition: string,
}

type ListEntryRequest = {
  type: "bid" | "ask";
  user: string;
}

type ListEntryTransaction = {
  type: "tsx";
  from: string;
  to: string;
}

type ListEntry = (ListEntryRequest | ListEntryTransaction) & {
  listing: string,
  orderId: number,
  price: number,
  amount: number
}


function nextEntry () : ListEntry {
  const orderId = Math.round(Math.random() * 42398472);
  const amount = Math.round(Math.random() * 23423);
  const price = Math.random() * 2342553;
  const listing = "TSX";
  let type;

  if (Math.random() < 0.4) {
    type = "tsx"
    const from = `${Math.round(Math.random() * 14)}`;
    const to = `${Math.round(Math.random() * 14)}`;
    return ({
      type: "tsx" as "tsx", from, to, amount, price, listing, orderId
    });
  } else {
    const user = `${Math.round(Math.random() * 14)}`;
    if (Math.random() < 0.5) {
      return ({
        type: "bid" as "bid", user, amount, price, listing, orderId
      });
    } else {
      return({
        type: "ask" as "ask", user, amount, price, listing, orderId
      }); 
    }
  }
}

export function OrderListComponent() {
  const [list, setList] = useState<ListEntry[]>([]);

  setTimeout(() => {
    const newList = list.slice(0, 20);
    newList.unshift(nextEntry());
    setList(newList);
  }, 500);

  return (
    <Box sx={{
      width: "50%"
    }}>
      <List>
        {
          list.map(entry =>
            <>
              <ListItemButton
                key={entry.orderId}
                alignItems="flex-start"
              // onClick={() => {
              //   navigate(`/listing/${lst.ticker}`);
              // }}
              >
                <ListItemText
                  primary={
                    <span>
                      <Typography
                        sx={{ display: 'inline' }}
                        variant="body1"
                        color="text.primary"
                      >
                        {entry.type.toUpperCase()}
                      </Typography >
                      <Typography
                        sx={{ display: 'inline' }}
                        variant="body1"
                        color="text.secondary"
                        // style={{ top: "20px", position: "relative" }}
                      >
                        {` - ${entry.type === "tsx" ? `${entry.from} -> ${entry.to}` : entry.user} ${entry.listing} ${entry.amount.toFixed(2)} ${entry.price.toFixed(2)}`}
                      </Typography >
                    </span>

                  }
                />
              </ListItemButton>
            </>
          )
        }
      </List>
    </Box>
  );
}