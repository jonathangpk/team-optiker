import { AppBar, BottomNavigation, BottomNavigationAction, Box, IconButton, List, ListItem, ListItemButton, ListItemText, Paper, Table, TableBody, TableCell, TableContainer, TableHead, TableRow, Toolbar, Typography } from "@mui/material";
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

type ListEntryBase = {
  listing: string,
  orderId: number,
  price: number,
  volume: number
}

type ListEntryOrders = ListEntryBase & {
  type: "bid" | "ask";
  user?: string;
  from?: string;
  to?: string;
}

type ListEntryTransactions = ListEntryBase & {
  type: "tsx";
  user?: string;
  from?: string;
  to?: string;
}

type ListType = ListEntryOrders | ListEntryTransactions;


function nextEntry(selector: "orders" | "transactions") {
  const orderId = Math.round(Math.random() * 42398472);
  const volume = Math.round(Math.random() * 23423);
  const price = Math.random() * 2342553;
  const listing = "APX";
  let type;

  if (selector === "transactions") {
    type = "tsx"
    const from = `${Math.round(Math.random() * 14)}`;
    const to = `${Math.round(Math.random() * 14)}`;
    return ({
      type: "tsx" as "tsx", from, to, volume, price, listing, orderId
    });
  } else {
    const user = `${Math.round(Math.random() * 14)}`;
    if (Math.random() < 0.5) {
      return ({
        type: "bid" as "bid", user, volume, price, listing, orderId
      });
    } else {
      return ({
        type: "ask" as "ask", user, volume, price, listing, orderId
      });
    }
  }
}

export function OrderListComponent(props: { selector: "orders" | "transactions" }) {
  const [list, setList] = useState<ListType[]>([]);
  const { selector } = props;

  setTimeout(() => {
    setList([nextEntry(selector), ...list.slice(0, 9)]);
  }, 1000);


  return (
    <Box sx={{
      width: "45%",
      float: selector === "orders" ? "left" : "right",
      margin: "20px",
      height: 100
    }}>
      <Box sx={{height: 50, marginTop: -5}}>
        <Typography
          sx={{ display: 'inline' }}
          variant="h6"
          color="text.primary"
          style={{ top: "20px", position: "relative" }}
          marginBottom={50}
        >
          {selector === "orders" ? "Orders" : "Transactions"}
        </Typography >
      </Box>
      <Box sx={{height: 200}}>
        <TableContainer >
          <Table size="small" aria-label="a dense table" padding="none">
            <TableHead>
              {
                selector === "orders"
                  ?
                  <TableRow>
                    <TableCell><Typography variant="body1">Type</Typography></TableCell>
                    <TableCell><Typography variant="body1">User</Typography></TableCell>
                    <TableCell align="right"><Typography variant="body1">Listing</Typography></TableCell>
                    <TableCell align="right"><Typography variant="body1">Volume</Typography></TableCell>
                    <TableCell align="right"><Typography variant="body1">Price</Typography></TableCell>
                    <TableCell align="right"><Typography variant="body1">Total</Typography></TableCell>
                  </TableRow>
                  :
                  <TableRow>
                    <TableCell><Typography variant="body1">From</Typography></TableCell>
                    <TableCell><Typography variant="body1">To</Typography></TableCell>
                    <TableCell align="right"><Typography variant="body1">Listing</Typography></TableCell>
                    <TableCell align="right"><Typography variant="body1">Volume</Typography></TableCell>
                    <TableCell align="right"><Typography variant="body1">Price</Typography></TableCell>
                    <TableCell align="right"><Typography variant="body1">Total</Typography></TableCell>
                  </TableRow>
              }
            </TableHead>
            <TableBody>
              {list.map((row) => (
                <>
                  {
                    selector === "orders"
                      ?
                      <TableRow
                        key={row.orderId}
                      >
                        <TableCell>{row.type}</TableCell>
                        <TableCell>{row.user}</TableCell>
                        <TableCell align="right">{row.listing}</TableCell>
                        <TableCell align="right">{row.volume}</TableCell>
                        <TableCell align="right">{row.price.toFixed(2)}</TableCell>
                        <TableCell align="right">{(row.volume * row.price).toFixed(2)}</TableCell>
                      </TableRow>
                      :
                      <TableRow
                        key={row.orderId}
                      >
                        <TableCell>{row.from}</TableCell>
                        <TableCell>{row.to}</TableCell>
                        <TableCell align="right">{row.listing}</TableCell>
                        <TableCell align="right">{row.volume}</TableCell>
                        <TableCell align="right">{row.price.toFixed(2)}</TableCell>
                        <TableCell align="right">{(row.volume * row.price).toFixed(2)}</TableCell>
                      </TableRow>
                  }
                </>
              ))}
            </TableBody>
          </Table>
        </TableContainer>
      </Box>
    </Box>
  );
}