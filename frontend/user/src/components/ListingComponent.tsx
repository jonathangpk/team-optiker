import * as React from 'react';
import List from '@mui/material/List';
import Divider from '@mui/material/Divider';
import ListItemText from '@mui/material/ListItemText';
import ListItemAvatar from '@mui/material/ListItemAvatar';
import Avatar from '@mui/material/Avatar';
import IconButton from '@mui/material/IconButton';
import Typography from '@mui/material/Typography';

import { useNavigate } from "react-router-dom";
import ListItemButton from '@mui/material/ListItemButton';
import { useStore } from '../state/store';


const dummy_data: IListing[] = [
  {
    name: "Tesla",
    abbr: "TSX",
    price: 30,
  },
  {
    name: "Apple",
    abbr: "APX",
    price: 230,
  },
  {
    name: "TUM",
    abbr: "TUM",
    price: 95234,
  },
  {
    name: "Wakanda",
    abbr: "WAK",
    price: 4324,
  },
  {
    name: "Narnia",
    abbr: "NAR",
    price: 72,
  }
]

interface IListing {
  name: string;
  abbr: string;
  price: number;

}

interface ListingsProps {

}

export function ListingsComponent(props: ListingsProps) {
  // const { listings } = props;
  const store = useStore();
  const listings = dummy_data;
  const navigate = useNavigate();

  return (
    <div>
      <List sx={{
        width: '100%',
        bgcolor: 'background.paper',
        maxHeight: "80vh",
        overflow: "auto",
      }}>
        {
          store.staticListings.map(lst =>
            <>
              <ListItemButton
                key={lst.ticker}
                alignItems="flex-start"
                onClick={() => {
                  navigate(`/listing/${lst.ticker}`);
                }}
              >

                {/* <ListItemAvatar>
                <Avatar alt="Remy Sharp" src="/static/images/avatar/1.jpg" />
              </ListItemAvatar> */}
                <ListItemText
                  primary={
                    <Typography
                      sx={{ display: 'inline' }}
                      variant="h6"
                      color="text.primary"
                    >
                      {lst.name}
                    </Typography >
                  }
                  secondary={lst.ticker + " — " + lst.description}
                />
                <div style={{ float: "right", top: "50px" }}>
                  <Typography
                    sx={{ display: 'inline' }}
                    variant="h6"
                    color="text.primary"
                    style={{ top: "20px", position: "relative" }}
                  >
                    {`${store.listingsWithPrice.find(l => l.symbol === lst.ticker)?.price} $`}
                  </Typography >
                </div>
              </ListItemButton>
              <Divider component="li" />  
            </>
          )
        }
      </List>
    </div>
  );
}