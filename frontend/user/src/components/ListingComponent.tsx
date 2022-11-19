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
  console.log(store)
  const listings = dummy_data;
  const navigate = useNavigate();

  return (
    <div>
      <List sx={{
        width: '100%',
        bgcolor: 'background.paper',
        maxHeight: 300,
        overflow: "auto",
      }}>
        {
          listings.map(lst =>
            <>
              <ListItemButton
                key={lst.abbr}
                alignItems="flex-start"
                onClick={() => {
                  navigate(`/listing/${lst.abbr}`);
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
                  secondary={lst.abbr + " — "}
                />
              </ListItemButton>
              <Divider component="li" />
            </>
          )
        }
      </List>
    </div>
  );
}